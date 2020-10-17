#include "TrilliumBatteryInterface.h"

#include <cstring>
#include <cstdio>

//following values are same as in N2kMsg.h - flags for invalid/unknown values
#define N2K_INVALID_FLOAT -1e9
#define N2K_INVALID_UINT8 0xff

/*Constructor for class Trojan_Trillium_Battery
  which parses the CAN-bus messages sent by battery. 
  Tracks parameters: voltage, temperature, state of charge.
*/
Trojan_Trillium_Battery::Trojan_Trillium_Battery() {
  batteryStatus = BatteryStatus::Unknown;
  canStatus = CANStatus::Disconnected;
  voltage = N2K_INVALID_FLOAT;
  temperatureKelvin = N2K_INVALID_FLOAT;
  chargePercent = N2K_INVALID_UINT8;
  
} //end Trojan_Trillium_Battery()


/*  Parses the canFrame struct and places contents into member variables
    Returns enum value indicating type of message received.
*/
BatteryMessageType Trojan_Trillium_Battery::parseRxMsg(struct can_frame *canFrame) {
    
    if( canFrame == NULL) {
        return BatteryMessageType::Unknown;
    }    

    BatteryMessageType msgType = (BatteryMessageType)(canFrame->can_id & CAN_SFF_MASK); 
    //Battery sends standard frames with 11-bit ID, which indicates msg type.
    //important to avoid assigning invalid value to msgType, so in following 
    //switch() ensure that default case assigns msgType = Unknown
    switch (msgType) {
    case BatteryMessageType::Heartbeat:
      canStatus = (CANStatus)canFrame->data[0];
      break;
    case BatteryMessageType::DataMessage1:  // Temperature and Batt Status
      setBatteryStatus(canFrame->data[2]);
      setTemperature(canFrame->data[0], canFrame->data[1]);
      //fprintf(stderr,"Converting %x %x to T and %x to status\n", canFrame->data[1], canFrame->data[0], canFrame->data[2]);
      break;
    case BatteryMessageType::DataMessage2:  // Temperature, Batt Status, Voltage
      setBatteryStatus(canFrame->data[2]);
      setTemperature(canFrame->data[0], canFrame->data[1]);
      setVoltage(canFrame->data[3], canFrame->data[4],canFrame->data[5], canFrame->data[6]);
      break;
    case BatteryMessageType::DataMessage3:  // State of Charge
      setChargePercent(canFrame->data[2]);
      break;
    default:
      msgType = BatteryMessageType::Unknown;
      break;
  }
  return msgType;

}  // end parseRxMsg()

void Trojan_Trillium_Battery::setBatteryStatus(uint8_t msgByte) {
      switch( msgByte ) {
        case 0x00:
          batteryStatus = BatteryStatus::Disconnected;
          break;
        case 0x01:
          batteryStatus = BatteryStatus::Connected;
          break;
        default:
          batteryStatus = BatteryStatus::Unknown;
      }
}//end setBatteryStatus()
  
void Trojan_Trillium_Battery::setTemperature(uint8_t ByteLS,uint8_t ByteMS) {
    uint16_t    data;
    int16_t     temperature8ths;
    data = ((uint16_t)ByteMS << 8) + ByteLS;
    std::memcpy(&temperature8ths, &data, sizeof(int16_t));//reinterpret as signed value
    temperatureKelvin = (float)temperature8ths / 8.0 + CELSIUS_TO_KELVIN;
}//end setTemperature()

void Trojan_Trillium_Battery::setVoltage(   uint8_t ByteLS, 
                                            uint8_t Byte01, 
                                            uint8_t Byte02, 
                                            uint8_t ByteMS ) {
    voltage =
        ((float)  ((uint32_t)ByteLS + 
                  ((uint32_t)Byte01 << 8) +
                  ((uint32_t)Byte02 << 16) + 
                  ((uint32_t)ByteMS << 24))) /
                  1024.0;

}//end setVoltage()

void Trojan_Trillium_Battery::setChargePercent(uint8_t msgByte) {
    chargePercent = msgByte;
}//end setChargePercent()

// The following get___() methods return the specified parameter
//of the available parameters.
//Per SignalK convention, units are SI:  Volts, Kelvin, Percent
//
BatteryStatus Trojan_Trillium_Battery::getBatteryStatus(void) {
  return batteryStatus;
}  // end getBatteryStatus()

CANStatus Trojan_Trillium_Battery::getCANStatus(void) {
  return canStatus;
}  // end getCANStatus()

float Trojan_Trillium_Battery::getVoltage(void) {
  return voltage;
}  // end getPitchRadians()

float Trojan_Trillium_Battery::getTemperature(void) {
  return temperatureKelvin;
}  // end getTemperature()

uint8_t Trojan_Trillium_Battery::getChargePercent(void) {
  return chargePercent;
}  // end getCharge()
