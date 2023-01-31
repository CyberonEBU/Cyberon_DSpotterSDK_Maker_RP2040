#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(TARGET_NANO_RP2040_CONNECT)
#include "WiFiNINA.h" //for LED RGB
#endif

void LED_Init_All();

#if defined(TARGET_ARDUINO_NANO33BLE) || defined(TARGET_PORTENTA_H7) || defined(TARGET_NANO_RP2040_CONNECT) || defined(TARGET_NICLA_VISION)
void LED_RGB_Off();
void LED_RGB_Red();
void LED_RGB_Green();
void LED_RGB_Blue();
#endif

void LED_BUILTIN_On();
void LED_BUILTIN_Off();
