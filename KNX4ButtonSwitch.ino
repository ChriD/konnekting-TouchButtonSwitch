

/* TODO:  * add KNX connection
          * get manufacturere ID
          * */

#include "Arduino.h"
#include "XButtonSwitch.h"
#include "KonnektingDevice.h"
#include "KNX4ButtonSwitch.h"


//#define KDEBUG
#ifdef KDEBUG
  #include <DebugUtil.h>
  #ifdef __AVR_ATmega32U4__
    #define DEBUGSERIAL Serial
  #elif __SAMD21G18A__
    #define DEBUGSERIAL SerialUSB
  #elif ESP8266
    #define DEBUGSERIAL Serial1
  #else
    #include <SoftwareSerial.h>
    SoftwareSerial softserial(11, 10); // RX, TX
    #define DEBUGSERIAL softserial
  #endif
#endif


// ################################################
// ### KONNEKTING Configuration
// ################################################
#ifdef __AVR_ATmega328P__
  #define KNX_SERIAL Serial // Nano/ProMini etc. use Serial
#elif ESP8266
  #define KNX_SERIAL Serial // ESP8266 use Serial
#else
  #define KNX_SERIAL Serial1
#endif

// ################################################
// ### IO Configuration
// ################################################
#define PROG_LED_PIN 2
#define PROG_BUTTON_PIN 3 // TODO: @@@


XButtonSwitch *xButtonSwitch;

void setup()
{
  Serial.begin(9600);
  xButtonSwitch = new XButtonSwitch(buttonEvent);
  xButtonSwitch->addButton(7, 1);
  xButtonSwitch->addButton(8, 2);
  xButtonSwitch->addButton(9, 3);
  xButtonSwitch->addButton(10, 4);
  //DEBUG_PRINT("Setting up Touch buttons on: ")

  // Initialize KNX enabled Arduino Board
  /*
  Konnekting.init(KNX_SERIAL,
          PROG_BUTTON_PIN,
          PROG_LED_PIN,
          MANUFACTURER_ID,
          DEVICE_ID,
          REVISION);

  // If device has been parametrized with KONNEKTING Suite, read params from EEPROM
  // Otherwise continue with global default values from sketch
  if (!Konnekting.isFactorySetting()) {
      //blinkDelay = (int) Konnekting.getUINT16Param(PARAM_blinkDelay); //blink every xxxx ms
  }
  */


}


void loop()
{
    // do KNX related stuff (like sending/receiving KNX telegrams)
    // This is required in every KONNEKTING aplication sketch
    //Knx.task();

    /*
    // be sure we do all "firmware" stuff only if the device is not in programming mode
    if (Konnekting.isReadyForApplication())
    {
      // let the buttonSwitch class let's do its work
      // it will tell us if a button has changed with the buttonEvent callback
      xButtonSwitch->xButtonSwitchLoop();
    }
    */

    xButtonSwitch->xButtonSwitchLoop();
}



// ################################################
// ### KNX EVENT CALLBACK
// ################################################

// we may handle leds or beeper/buzzer here?!
void knxEvents(byte index) {
  /*
    // nothing to do in this sketch
    switch (index) {

        case COMOBJ_ledOnOff: // object index has been updated

            if (Knx.read(COMOBJ_ledOnOff)) {
                digitalWrite(TEST_LED, HIGH);
                Debug.println(F("Toggle LED: on"));
            } else {
                digitalWrite(TEST_LED, LOW);
                Debug.println(F("Toggle LED: off"));
            }
            break;

        default:
            break;
    }*/
};



void buttonEvent(unsigned int buttonId, unsigned int _buttonEvent)
{
  Serial.print(buttonId);
  Serial.print(":");
  Serial.println(_buttonEvent);
  // here we have to send the knx telegramms onto the bus
  //knx.write(COMOBJ_trigger, laststate);
}

