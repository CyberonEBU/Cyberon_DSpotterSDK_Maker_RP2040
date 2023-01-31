/*
    GetSerialNumber.ino
        To get the serial number of the board and print it on the console output.

    This sketch supports the following board types:
        Arduino Nano RP2040 Connect
            https://store-usa.arduino.cc/products/arduino-nano-rp2040-connect?selectedStore=us
        Arduino Nano 33 BLE Sense
            https://store-usa.arduino.cc/products/arduino-nano-33-ble-sense
        Arduino Portenta H7
            https://store-usa.arduino.cc/products/portenta-h7?selectedStore=us

    18 NOV 2022 by Cyberon Corporation.
    https://www.cyberon.com.tw/index.php?lang=en
*/
#include <Arduino.h>
#include <DSpotterSDK_MakerHL.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.print("The serial number of your device is ");
  Serial.println(DSpotterSDKHL::GetSerialNumber());
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
  return;
}
