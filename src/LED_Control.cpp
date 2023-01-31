#include "LED_Control.h"

void LED_Init_All()
{
  // intitialize the digital Pin and LED_BUILTIN as an output
  #if defined(TARGET_PORTENTA_H7) || defined(TARGET_NANO_RP2040_CONNECT) || defined(TARGET_NICLA_VISION) || defined(TARGET_ARDUINO_NANO33BLE)
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  #endif
  pinMode(LED_BUILTIN, OUTPUT);
  LED_RGB_Off();
  LED_BUILTIN_Off();
}

#if defined(TARGET_PORTENTA_H7) || defined(TARGET_NICLA_VISION) || defined(TARGET_ARDUINO_NANO33BLE)
void LED_RGB_On()
{
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void LED_RGB_Off()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void LED_RGB_Red()
{
  LED_RGB_Off();
  digitalWrite(LEDR, LOW);
}

void LED_RGB_Green()
{
  LED_RGB_Off();
  digitalWrite(LEDG, LOW);
}

void LED_RGB_Blue()
{
  LED_RGB_Off();
  digitalWrite(LEDB, LOW);
}
#elif defined(TARGET_NANO_RP2040_CONNECT)
void LED_RGB_On()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void LED_RGB_Off()
{
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void LED_RGB_Red()
{
  LED_RGB_Off();
  digitalWrite(LEDR, HIGH);
}

void LED_RGB_Green()
{
  LED_RGB_Off();
  digitalWrite(LEDG, HIGH);
}

void LED_RGB_Blue()
{
  LED_RGB_Off();
  digitalWrite(LEDB, HIGH);
}
#endif

void LED_BUILTIN_On()
{
#if defined(TARGET_PORTENTA_H7) || defined(TARGET_NICLA_VISION)
  LED_RGB_On();
#else
  digitalWrite(LED_BUILTIN, HIGH);
#endif
}

void LED_BUILTIN_Off()
{
#if defined(TARGET_PORTENTA_H7) || defined(TARGET_NICLA_VISION)
  LED_RGB_Off();
#else
  digitalWrite(LED_BUILTIN, LOW);
  #endif
}
