# BatteryMonitorPi
This program reads CAN-bus data broadcast by a Trojan Trillium battery, converts it to NMEA2000 format, and re-broadcasts it on a second CAN port connected to a NMEA2000 network.  It runs on a Raspberry Pi, but can be adapted for other processors equipped with 2 CAN ports, such as an Espressif ESP32.

## Hardware
Works with Trojan Trillium Lithium batteries (https://www.trojanbattery.com/trillium/) with integrated battery management and CAN port. I am using a model TR 12.8-92 on our boat (and we love it!). Interface details are in Trojan's Technical Document TRJN0531 (https://www.trojanbattery.com/pdf/TRJN0531_TrilliumTechDoc_011419.pdf) 

The Raspberry Pi is a model 2B. Others should be fine.

The CAN interface is provided by a Waveshare 2-port Isolated CAN Hat for the Pi (https://www.waveshare.com/product/raspberry-pi/hats/interface-power/2-ch-can-hat.htm)  No hardware modifications were needed, and after following Waveshare's instructions the CAN ports appear as regular network interfaces named can0 and can1.

## Libraries
- Timo Lappalainen's NMEA2000 library https://github.com/ttlappalainen/NMEA2000 
- Al Thomason's NMEA2000_socketCAN library https://github.com/thomasonw/NMEA2000_socketCAN

Theoretically, the two libraries should be able to link into the project as pre-compiled libraries. However I was unable to configure my development environment to work properly that way, so instead the library source files are copied into the project folder and compiled together with the main source files. This is not a problem with the libraries themselves - merely an IDE configuration issue I haven't worked around yet.

## Development Environment
Apache NetBeans v11.3 running on laptop with Ubuntu 20.04. Compiling is done on the RPi, via SSH connection directly from Netbeans. I wasn't able to edit the environment to use pre-compiled libraries, so had to copy the library's source files into the main project.

## Files
In addition to the library files, there are:
- main.cpp
- TrilliumBatteryInterface.h  
- TrilliumBatteryInterface.cpp

## Additional Notes
The Pi is also running SignalK server. When configured to monitor the same CAN port that the BatteryMonitorPi outputs data on, the SignalK software is able to interpret and display the battery data.


