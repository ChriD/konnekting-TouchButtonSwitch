/* TODO:  * add KNX connection
          * get manufacturere ID
          * aso...*/

#include "Arduino.h"
#include "Wire.h"
#include "KonnektingDevice.h"
#include "TouchButtonSwitch.h"
#include "src/CY8TouchSwitch.h"


// used for debugging
// should not be activated on protductive environment
#define KDEBUG

// for testing purposes without having a bcu attached we have to skip
// the knx code to test the device. For this we define the NOBCU
#define NOBCU

#define DEBUGSERIAL       Serial
#define KNX_SERIAL        Serial1
#define PROG_LED_PIN      6
#define PROG_BUTTON_PIN   A5
#define TC_INTERRUPTPIN   PA8


CY8TouchSwitch  *touchSwitch;
bool            startupRecalibrationNeeded = true;
uint64_t        mainLoopEndTime;


void setup()
{

  #ifdef KDEBUG
    DEBUGSERIAL.begin(115200);
    while (!DEBUGSERIAL)
    {
    }
    Debug.setPrintStream(&DEBUGSERIAL);
    Debug.println(F("KONNEKTING TouchButtonSwitch"));
  #endif

  touchSwitch = new CY8TouchSwitch();
  touchSwitch->setup();

  // add buttons for the given type of switch (4x or 6x)
  // this has to be done after we do have loaded the KNX settings
  touchSwitch->addButton(3, D3, true);
  touchSwitch->addButton(4, D4, true);
  touchSwitch->addButton(5, D11, true);
  touchSwitch->addButton(6, D9, true);

  touchSwitch->changeMode(TS_MODE_STARTUP1, true);
  delay(150);

  // startup I2C
  Wire.begin();

  touchSwitch->changeMode(TS_MODE_STARTUP2, true);
  delay(150);

  // after we have changed the mode of the switch to SETUP, we start setting up the
  // knx device stuff
  #ifndef NOBCU
    knxDeviceSetup();
  #endif

  touchSwitch->changeMode(TS_MODE_STARTUP3, true);
  delay(150);

  // setup the interrupt method for the touch controller
  pinMode(TC_INTERRUPTPIN, INPUT_PULLDOWN);
  attachInterrupt(TC_INTERRUPTPIN, touchControllerInterrupt, FALLING);

  // setup the prog button
  pinMode(PROG_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PROG_BUTTON_PIN), progButtonPressed, RISING);

  touchSwitch->changeMode(TS_MODE_STARTUP4, true);
  delay(150);

  // be sure we do set back to the normal state after rebooting
  touchSwitch->changeMode(TS_MODE_NORMAL, true);
}


void progButtonPressed() {
  Debug.println(F("Prog-Button pressed!"));
  Konnekting.toggleProgState();
}


void progLed (bool state){
  touchSwitch->changeMode(TS_MODE_PROG, false);
}


void knxDeviceSetup()
{
  //Konnekting.init(KNX_SERIAL, PROG_BUTTON_PIN, PROG_LED_PIN, MANUFACTURER_ID, DEVICE_ID, REVISION);
  Konnekting.init(KNX_SERIAL, &progLed, MANUFACTURER_ID, DEVICE_ID, REVISION);
  if (!Konnekting.isFactorySetting())
  {
    //typeTemp = (int) Konnekting.getUINT8Param(PARAM_tempSendUpdate);
    //temperature polling interval (ms)
    //intervalTempUser = (long) Konnekting.getUINT32Param(PARAM_tempPollingTime)*1000;
    //minimal difference between previous and current temperature [°C]
    //diffTempUser = (float) Konnekting.getUINT8Param(PARAM_tempDiff)*0.1;
    //intervalHumdUser = (long) Konnekting.getUINT32Param(PARAM_rhPollingTime)*1000; //humidity polling interval (ms)
  }
  Debug.println(F("KNX-Device settings loaded"));
}


// the touch controller will trigger an interrupt when the state of a sensor changes
// we have to reroute the interrupt to the touchSwitch class for further processing
void touchControllerInterrupt() {
  touchSwitch->interrupt();
}


void knxEvents(byte _index) {
    // nothing to do in this sketch
}


void loop()
{
  // we have to call the knx.task form the konnekting library faster then ~ 400us to not miss any telegram
  // in next versions this should be obsolete but for now we have to stay with itself
  // ATTENTION: Currently we do have some loops which will go above 400us due I2C handling, we have to ceck in
  // production mode if this will lead to problems
  #ifndef NOBCU
    Knx.task();
  #endif

  #ifdef KDEBUG
    uint64_t now = micros();
    if(mainLoopEndTime && now > mainLoopEndTime && now - mainLoopEndTime > 400)
      Debug.println(F("ATTENTION: Main Loop exceeds KNX timing requirement! %uus"), (now - mainLoopEndTime));
    //Debug.println(F("MainLoop %u"), (now - mainLoopEndTime));
  #endif

  // the switch itself must loop eyerytime, no matter of KNX is active or not
  // while KNX connector is not active, the switch is in a special mode and will not raise any touch events
  touchSwitch->task();

  // be sure to reset the touch controller after resetting the main controller and be sure
  // that there is time to "set up" the frontboard finish (reset for recalibration)
  if (startupRecalibrationNeeded &&  millis() > 10000)
  {
    startupRecalibrationNeeded = false;
    touchSwitch->resetTouchController();
  }

  if (Konnekting.isReadyForApplication())
  {
    // TODO: switch to normal mode
  }
  else
  {
    // TODO: switch to PRG mode
  }

  // only for test!!!!
  if ( millis() > 5000)
    touchSwitch->changeMode(TS_MODE_NORMAL, false);


  mainLoopEndTime = micros();
}

