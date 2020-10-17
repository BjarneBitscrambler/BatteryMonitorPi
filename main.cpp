// Reads Battery status messages, parses them, and sends NMEA 2000 frames with battery data.
// Assumes Battery is connected to /dev/can1, N2K network connected to /dev/can0
// Linux Socket-CAN kernel drivers are set by default to echo frames written by one socket
//to all other sockets connected to the same CAN port. So, if SignalK is set to monitor
//traffic on can0, then it will also receive the battery data.

#include <iostream>
#include <sys/types.h>  
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>    
#include <linux/can.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "TrilliumBatteryInterface.h"

#include "NMEA2000_CAN.h"  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include "N2kMessages.h"

#define DEFAULT_SENDER_ID           42
#define BATT_DATA_RATE_MS           400
#define PRODUCT_INFO_RATE_MS        2000
#define BATTERY_NO_MSG_TIMEOUT      5000
#define BATT_CAPACITY_C             (93 * 60 * 60)  //Battery Capacity in Coulombs (AHr * 3600)

int OpenInputSocket( void ) {
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    //open a socket for raw CAN traffic, non-blocking on read/write
    int socket_in = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW);

    if( socket_in == -1 ) {
       fprintf(stderr, "couldn't open socket: error %d\n", errno );
       return socket_in;
    }
    
    //bind socket to port /dev/can1
    strcpy(ifr.ifr_name, "can1" );
    ioctl(socket_in, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(socket_in, (struct sockaddr *)&addr, sizeof(addr));

    return socket_in;
}//end   OpenInputSocket()  
    
    
//Check for a data frame from the CAN socket
//Return the data in struct pframe
bool ReadCANFrame( int socket_in, struct can_frame *pframe ) {
    int nbytes;

    nbytes = read(socket_in, pframe, sizeof(struct can_frame));

    if( nbytes < 0 ) {
        if( errno == 11 ) {
            //there is no data ready to read - no action needed
        } else {
            //we had unexpected error
            fprintf(stderr, "error reading input socket: nbytes=%d, errno=%d", nbytes, errno );
        }
        return false;
    }else if( nbytes == 0 ) {
        //no CAN frames received. May not ever get nbytes==0, as -1 is the value when no data is avail.
        return false;
    }else if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "read: incomplete CAN frame\n");
        return false;
    }else {
        //successful read of 1 frame
        return true;
    }
}//end ReadCANFrame()


void Setup( int *socket_in ) {
  //setup the data output: NMEA2000 frames on /dev/can0
    // Reserve enough buffer for sending all messages. This does not work on small memory devices like Uno or Mega
    NMEA2000.SetN2kCANSendFrameBufSize(250);
    // Set Product information
    NMEA2000.SetProductInformation("00000001", // Manufacturer's serial code
                                   100, // Manufacturer's product code
                                   "House Battery Monitor",  // Manufacturer's Model ID
                                   "1.0.0 (2020-10-15)",  // Manufacturer's Software version code
                                   "Trojan Trillium", // Manufacturer's Hardware version
                                   0, //LEN
                                   2101,  //NMEA version number
                                   1, //certification level
                                   0  //device ID
                                   );
    // Set device information
    NMEA2000.SetDeviceInformation(1, // Unique number. Use e.g. Serial number.
                                  170, // Device function=Battery. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                  35, // Device class=Electrical Generation See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                  2046 //Manufacturer ID (none assigned officially)                               
                                 );
    // Uncomment 3 rows below to see, what device will send to bus                           
    NMEA2000.SetForwardStream(&serStream);
    // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.
    // NMEA2000.SetForwardOwnMessages();

    // To see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
    NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,DEFAULT_SENDER_ID);
    NMEA2000.EnableForward(false); // Disable all msg forwarding to Serial
    NMEA2000.Open();
  
  //Setup the incoming data source: battery connected to /dev/can1
    *socket_in = OpenInputSocket();
}//end Setup()


int main() {

    int socketToBattery;
    Setup( &socketToBattery );
    
    struct can_frame canFrame;
    Trojan_Trillium_Battery myBattery;
    unsigned long tsLastSentBattMsg = millis();
    unsigned long tsLastSentPIDMsg = millis();
    unsigned long tsLastRxBattMsg = millis();
    timespec napTime;   //how long we can sleep - the min period between frames
    napTime.tv_sec = 0;
    napTime.tv_nsec = 100000; //108 bits / 1e6 * 1e9 for CAN2.0 Standard Frame at 1MHz bit rate
    tN2kMsg N2kMsg;
    unsigned char sequenceID = 0;
    const unsigned char batteryInstance = 1;
    
    while( 1 ) {
        if( ReadCANFrame( socketToBattery, &canFrame ) ) {
            switch (myBattery.parseRxMsg( &canFrame )) {
                //insert here any particular actions on receiving battery msgs
                case BatteryMessageType::Heartbeat:
                    tsLastRxBattMsg = millis();
                    break;
                case BatteryMessageType::DataMessage1:
                    tsLastRxBattMsg = millis();
                    break;
                case BatteryMessageType::DataMessage2:
                    tsLastRxBattMsg = millis();
                    break;
                case BatteryMessageType::DataMessage3:
                    tsLastRxBattMsg = millis();
                    break;
                default:
                    break;
            }
        }else if((millis() - tsLastSentBattMsg > BATT_DATA_RATE_MS) &&
                 (myBattery.getBatteryStatus() != BatteryStatus::Unknown)) {
             //periodically send out NMEA2000 frames reporting battery status
            //this is rollover-safe interval comparison
            tsLastSentBattMsg = millis();
            SetN2kDCBatStatus(N2kMsg, batteryInstance,
                                myBattery.getVoltage(), 
                                N2kDoubleNA, 
                                myBattery.getTemperature(), 
                                sequenceID);
            NMEA2000.SendMsg(N2kMsg);
            SetN2kDCStatus(N2kMsg,sequenceID, batteryInstance, 
                                N2kDCt_Battery,
                                myBattery.getChargePercent(), 
                                N2kUInt8NA, 
                                N2kDoubleNA, 
                                N2kDoubleNA, 
                                BATT_CAPACITY_C );
            NMEA2000.SendMsg(N2kMsg);
            sequenceID++;
        }else if( (millis() - tsLastSentPIDMsg) > PRODUCT_INFO_RATE_MS ) {
             //periodically send out NMEA2000 frames reporting product ID
            //this is rollover-safe interval comparison
            tsLastSentPIDMsg = millis();
            NMEA2000.SendProductInformation();
            NMEA2000.SendIsoAddressClaim(); //unsure if this is needed
        }else {
            //no incoming or outgoing frames - may as well sleep ~1 frame period
            nanosleep( &napTime, NULL );
            if( (millis() - tsLastRxBattMsg > BATTERY_NO_MSG_TIMEOUT) &&
              ( myBattery.getBatteryStatus() != BatteryStatus::Unknown )) {
                fprintf( stderr, "Nothing heard from battery in %d ms\n", BATTERY_NO_MSG_TIMEOUT);
                myBattery.setBatteryStatus( (uint8_t)BatteryStatus::Unknown );
            }
        }

        NMEA2000.ParseMessages();   //device heartbeat, check for incoming msgs, address negotiation
    }
    return 0;
}//end main()

