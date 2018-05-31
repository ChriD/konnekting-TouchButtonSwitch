

/* TODO:  * add KNX connection
          * get manufacturere ID
          * aso...*/

#include "Arduino.h"
#include "Wire.h"
#include "CY8TouchSwitch.h"
//#include "4xTouchButtonSwitch.h"

#define KDEBUG
#ifdef KDEBUG
  #include <DebugUtil.h>
  #ifdef __AVR_ATmega32U4__
    #define DEBUGSERIAL Serial
  #elif __SAMD21G18A__
    #define DEBUGSERIAL SerialUSB
  #elif ESP8266
    #define DEBUGSERIAL Serial1
  #elif ARDUINO_ARCH_STM32
    #define DEBUGSERIAL Serial
  #else
    #include <SoftwareSerial.h>
    SoftwareSerial softserial(11, 10); // RX, TX
    #define DEBUGSERIAL softserial
  #endif
#endif


#define TC_INTERRUPTPIN   PA8
CY8TouchSwitch *touchSwitch;


void setup()
{

  #ifdef KDEBUG
    // Start debug serial with 9600 bauds
    DEBUGSERIAL.begin(115200);
  #if defined(__AVR_ATmega32U4__) || defined(__SAMD21G18A__)
    // wait for serial port to connect. Needed for Leonardo/Micro/ProMicro/Zero only
    while (!DEBUGSERIAL)
    {
    }
  #endif
    // make debug serial port known to debug class
    // Means: KONNEKTING will use the same serial port for console debugging
    Debug.setPrintStream(&DEBUGSERIAL);
    Debug.println(F("KONNEKTING 4xTouchButtonSwitch"));
  #endif
  
  Debug.println(F("Setting up Device"));

  // startup I2C and serial interface
  Wire.begin();
  //Serial.begin(9600);

  touchSwitch = new CY8TouchSwitch();  
  touchSwitch->setup();  
  
  // setup the interrupt method for the touch controller
  pinMode(TC_INTERRUPTPIN,INPUT_PULLDOWN);
  attachInterrupt(TC_INTERRUPTPIN, TouchControllerInterrupt, FALLING);
}


void TouchControllerInterrupt() {
  Debug.println(F("Touch controller interrupt occured"));
  touchSwitch->interrupt(); 
}


void loop() {
  touchSwitch->loop();  
  delay(1);                                
}

