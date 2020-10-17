/* 
 * File:   TrilliumBatteryInterface.h
 * Author: Bjarne Hansen
 *
 * Created 2020-10-16
 * 
 * Interface file for Trojan Trillium Batteries.
 *
 * These LiFePO4-chemistry boat/RV/home batteries have a CAN-bus
 *  interface that reports various battery parameters at 
 *  regular intervals. See Trojan's TRJN0531_TrilliumTechDoc.pdf
 *  on their website for details.
 *
 * Using CAN frames from supported battery, reports the following parameters:
 *  - CAN Status
 *  - Battery Status
 *  - Temperature
 *  - Voltage
 *  - State of Charge
 * The CAN interface documentation states there are some messages
 *  that control the battery. These are not implemented in this file.
 *
 * Relies on a physical CAN interface to the battery.
 */

#ifndef TRILLIUMBATTERYINTERFACE_H
#define TRILLIUMBATTERYINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <linux/can.h>  //if this file is not avail, then define struct as:
    /*struct can_frame {
    //	canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    //	__u8    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
    //	__u8    __pad;   /* padding */
    //	__u8    __res0;  /* reserved / padding */
    //	__u8    __res1;  /* reserved / padding */
    //	__u8    data[CAN_MAX_DLEN] __attribute__((aligned(8)));
    //};*/


#define CELSIUS_TO_KELVIN (273.15)

enum class BatteryMessageType:int32_t {
  Unknown = -1,
  //actual message IDs are used for enum values
  Heartbeat    = 0x071d,  //sent every 200ms by battery
  DataMessage1 = 0x019d,  //sent every 200ms by battery
  DataMessage2 = 0x029d,  //sent every 200ms by battery
  DataMessage3 = 0x039d   //sent every 200ms by battery
};

enum class BatteryStatus {
  //following two states are as defined in the Trojan interface spec
  Disconnected = 0, //means that the contactor is open-circuit
  Connected = 1,     //battery is able to supply current
  Unknown = 0xff
};

enum class CANStatus {
  Disconnected = 0,       //Not receiving any valid CAN frames
  //following states are as defined in the Trojan interface spec
  Stopped = 0x04,         //battery is broadcasting only heartbeat
  PreOperational = 0x7F,  //not sure how differs from Stopped
  Operational = 0x05      //broadcasting all messages
};

class Trojan_Trillium_Battery {
 public:
                        Trojan_Trillium_Battery();
  BatteryMessageType    parseRxMsg(struct can_frame *canFrame);
  BatteryStatus         getBatteryStatus(void);
  CANStatus             getCANStatus(void);
  float                 getVoltage(void);
  float                 getTemperature(void);
  uint8_t               getChargePercent(void);
  void                  setBatteryStatus(uint8_t msgByte);

 private:
  void setTemperature(uint8_t ByteLS, uint8_t ByteMS);
  void setVoltage(uint8_t ByteLS,uint8_t Byte01,uint8_t Byte02,uint8_t ByteMS );
  void setChargePercent(uint8_t msgByte);

  
  BatteryStatus         batteryStatus;
  CANStatus             canStatus;
  float                 voltage;
  float                 temperatureKelvin;
  uint8_t               chargePercent;

};

#ifdef __cplusplus
}
#endif

#endif /* TRILLIUMBATTERYINTERFACE_H */

