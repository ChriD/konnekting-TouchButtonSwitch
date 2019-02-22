
/*
  Konnekting - TouchButtonSwitch (0xCDDC)
  Created by Christian Dürnberger (ChriD), 2019
  https://github.com/ChriD
*/

/*
  Issues/Todos:   * lightning stuff
                  * proximity stuff
                  * 1,4,6 buttons defines
                  * Standard KNX Button com objects (switching, dimming,...)
                  * using external SPI flash instead of internal

*/

#include "Arduino.h"
#include "SetupFlashMemory.h"
#include "KonnektingDevice.h"
#include "TouchButtonSwitch.h"
#include "src/TouchSwitch_4X_V1.h"
//#include "src/TouchSwitch_1X_V1.h"


// simple version definition for the library
#define LIB_VERSION             "1.0"

// if this define is uncommented, debugging via serial is activated. This should not be active on productive environment!
#define KDEBUG

// for testing purposes without having an active bcu attached we have to skip
// the knx connection and task codes to test the device. This can be done by setting this define
#define BCUDISABLED

// This is the time in miliseconds how long the button should wait after startup/setup to allow touches
// There has to be a enough time for the user to put the frontplate on the switch before recalibration of
// the touch sensor is starting. The recalibration starts after the STARTUP_IDLETIME and may take up to 1 second
#define STARTUP_IDLETIME        10000

// define seriald for the Adafruit Itsy-Bitsy M0
// SERIAL_BCU is mandatory to work, the SERIAL_DBG will only be used in DEBUG Mode
#define SERIAL_DBG              SERIAL_PORT_USBVIRTUAL
#define SERIAL_BCU              Serial1

// so our touch switch device is the TouchSwitch_CODENAME_4X_V1 switch which has 4 touch buttons and 2 proximity
// sensors on top and on the button of the device. So we use the appropriate class for that switch
BaseSwitch    *baseSwitch = new TouchSwitch_4X_V1();
//BaseSwitch    *baseSwitch = new TouchSwitch_1X_V1();

// store the inital boot time (last line of setup method) and some other vars for some initial stuff
uint64_t  initialBootTime = 0;
boolean   initialSetupDone        = false;
boolean   initialCalibrationDone  = false;

// this one states the readiness of the konnekting library
// it does not reflect the whole readiness of the device!
boolean   isAppReady = false;

// TODO: do this one into the TouchSwitch lib?
#define PROG_BUTTON_PIN   10


// this method will be called whenever the mode on the switch changes
// for now it has only debug purposes
void onModeChange(SWITCH_MODE _fromMode, uint16_t _fromModeLevel, SWITCH_MODE _toMode, uint16_t _toModeLevel)
{
  Debug.println(F("Switch Mode changed from %u (%u) to %u (%u)"), _fromMode, _fromModeLevel, _toMode, _toModeLevel);
}

// this method will be called whenever a button action (tap, doubletap, longtap,..) was regognized by the
// switch library. We use this method to send the approriate data to the KNX Bus for each button action
void onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  uint16_t  idOffset = 7 * (_buttonId-1);
  uint16_t  idComObject = 0;

  // TODO: @@@ get the mode of the button from an array which is loaded on init KNX
  // currently only the "Standalone" mode is available
  // TODO: @@@ ID and DPT has to be gathered from the button class?!
  // due we know that the id gap between the com objects of the different button is 7 we can do a
  // nice generic generation of the comObject id
  if(_type == 1 && _value <= 1)
    idComObject = COMOBJ_button1;
  else if(_type == 1 && _value == 2)
    idComObject = COMOBJ_button1_double;
  else if(_type == 1)
    idComObject = COMOBJ_button1_multi;
  else if(_type == 2)
    idComObject = COMOBJ_button1_long;
  else if(_type == 20)
    idComObject = COMOBJ_button1_long_touchstart;
  else if(_type == 21)
    idComObject = COMOBJ_button1_long_touchend;
  else
    Debug.println(F("Unknown button event type %d"), _type);

  if(idComObject)
    idComObject += idOffset;

  Debug.println(F("Button: %d | Event: %d | Value: %d | ComObjId: %d"), _buttonId, _type, _value, idComObject);

  #ifdef BCUDISABLED
    return;
  #endif

  // write out the KNX Command if enabled
  // TODO: @@@ be sure we do not "overload the bus"?!?!
  if(KNXEnabled())
    Knx.write(idComObject, _value);
}


void onProximityAlert(uint16_t _buttonId, boolean _isProximity, uint16_t _proximityValue, uint16_t _proximityLevel)
{
  // TODO: @@@
  //Debug.println(F("Proximity %u"));
}


// this method only should return if we are allowed to send KNX commands to the bus
// as long as the device is not properly programmed we are not allowed to do anything on the bus
boolean KNXEnabled()
{
  return Konnekting.isReadyForApplication();
}



void setup()
{
  // start the debugging serial and wait for it to be present before doing anything
  // of course we do only activate the serial if we have enabled debugging
  #ifdef KDEBUG
    SERIAL_DBG.begin(115200);
    while (!SERIAL_DBG) {}
    Debug.setPrintStream(&SERIAL_DBG);
    Debug.println(F("KONNEKTING TouchButtonSwitch Library %s"), LIB_VERSION);
  #endif

  // attach callbacks to the switch
  baseSwitch->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  baseSwitch->attachCallbackOnProximityAlert(makeFunctor((CallbackFunction_ProximityAlert*)0,&onProximityAlert));
  baseSwitch->attachCallbackOnModeChange(makeFunctor((CallbackFunction_ModeChange*)0,&onModeChange));

  // do the setup of the switch which will add all the buttons and leds the switch
  // may work with. TODO: add some setup error code?!
  if(!baseSwitch->setup())
    Debug.println(F("Error initializing Touch Switch"));

  baseSwitch->setMode(SWITCH_MODE::SETUP, 1);

  // setup the persistent "flash memory"
  // this method is outsourced to "SetupFlashMemory.h" file for easy changing
  setupFlashMemory();

  // setup the connection to the KNX-BUS
  #ifndef BCUDISABLED
    initKNX();
    initKNXParameters();
  #endif

  baseSwitch->setMode(SWITCH_MODE::SETUP, 2);

  // setup the prog button interrupt for rising edge
  pinMode(PROG_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PROG_BUTTON_PIN), progButtonPressed, FALLING);

  initialBootTime = millis();
  baseSwitch->setMode(SWITCH_MODE::SETUP, 3);
}


// this method does the initialization of the KONNEKTING Library
// if there is a problem connecting to the bus, the method will not return!
void initKNX()
{
  Konnekting.init(SERIAL_BCU, &progLed, MANUFACTURER_ID, DEVICE_ID, REVISION);
}


void initKNXParameters()
{
  if (Konnekting.isFactorySetting())
    return;

  // TODO: how to get proper button id?
  BaseSwitchButtonParms button1Parms;
  button1Parms.allowMultiTouch = (bool) Konnekting.getUINT8Param(PARAM_button1_multiTouchEnabled);
  baseSwitch->setButtonParameters(1, button1Parms);
  Debug.println(F("Sensor %u: Mode=%u, LongTouchMode=%u, MultiTouch=%u, "), 1, 0, 0, button1Parms.allowMultiTouch);

  BaseSwitchButtonParms button2Parms;
  button1Parms.allowMultiTouch = (bool) Konnekting.getUINT8Param(PARAM_button1_multiTouchEnabled);
  baseSwitch->setButtonParameters(2, button2Parms);
  Debug.println(F("Sensor %u: Mode=%u, LongTouchMode=%u, MultiTouch=%u, "), 2, 0, 0, button2Parms.allowMultiTouch);

  BaseSwitchButtonParms button3Parms;
  button1Parms.allowMultiTouch = (bool) Konnekting.getUINT8Param(PARAM_button1_multiTouchEnabled);
  baseSwitch->setButtonParameters(3, button3Parms);
  Debug.println(F("Sensor %u: Mode=%u, LongTouchMode=%u, MultiTouch=%u, "), 3, 0, 0, button3Parms.allowMultiTouch);

  BaseSwitchButtonParms button4Parms;
  button1Parms.allowMultiTouch = (bool) Konnekting.getUINT8Param(PARAM_button1_multiTouchEnabled);
  baseSwitch->setButtonParameters(4, button4Parms);
  Debug.println(F("Sensor %u: Mode=%u, LongTouchMode=%u, MultiTouch=%u, "), 4, 0, 0, button4Parms.allowMultiTouch);

  Debug.println(F("User settings loaded"));
}


// if the prog button is pressed we have to switch the device into the prog mode
// we have to debounce the button because we do not have hardware debounce
volatile unsigned long progButtonDebounceTime = 0;
void progButtonPressed()
{
  unsigned long tempTime = millis();
  if(tempTime >= progButtonDebounceTime)
  {
    if(tempTime - progButtonDebounceTime < 200)
      return;
  }
  progButtonDebounceTime = tempTime;
  Debug.println(F("PROG BTN"));
  Konnekting.toggleProgState();

  baseSwitch->setMode(SWITCH_MODE::PROG);
}


// the 'progLed' is here for visualization of the programming mode
void progLed (bool _isPrg){
  if(_isPrg)
    baseSwitch->setMode(SWITCH_MODE::PROG);
  else
    baseSwitch->setMode(SWITCH_MODE::NORMAL);
}


// this method will be called when a com object is updated
void knxEvents(byte _index)
{
}


// this is a method which is beeing called by the loop and should handle the setuo stuff
// that has to be done after booting up (like caliration aso...)
void loopTaskSetup()
{
  #ifndef BCUDISABLED
    if(isAppReady != Konnekting.isReadyForApplication())
    {
      isAppReady = Konnekting.isReadyForApplication();
      Debug.println(F("Konnekting application ready: %u"), isAppReady);
    }
  #endif

  // after the STARTUP_IDLETIME (which should allow us to put on the frontplate on the switch)
  // we do a calibration of all the buttons (this is necessray for touch buttons, but its good for
  // mechanical buttons too)
  if(!initialSetupDone && (millis() - initialBootTime) > STARTUP_IDLETIME)
  {
    initialSetupDone = true;
    Debug.println(F("Initial setup done! Starting button calibration..."));
    baseSwitch->setMode(SWITCH_MODE::CALIBRATION, 0);
    baseSwitch->startCalibration();
  }

  if(!initialCalibrationDone && (millis() - initialBootTime) > (STARTUP_IDLETIME + 2000))
  {
    initialCalibrationDone = true;
    Debug.println(F("Calibration done!"));
    baseSwitch->setMode(SWITCH_MODE::NORMAL);
  }
}


uint64_t  lastDebugOutput = 0;


void loop()
{
  #ifdef KDEBUG
    uint64_t startLoop  = micros();
    uint64_t endLoop    = 0;
  #endif

  // we have to call the knx.task form the konnekting library faster then ~ 400us to not miss any telegram
  // in next versions this should be obsolete but for now we have to stay with this pitfall
  #ifndef BCUDISABLED
    Knx.task();
  #endif

  //if(Konnekting.isFactorySetting())
  //  return

  // let the button do its application loop tasks, the first few seconds it will not do anything because of
  // the switch mode not set to NORMAL. It will go through some setup and calibration modes until it reaches
  // the "NORMAL" mode
  baseSwitch->task();

  // we do have some setup tasks like calibration which have to be done in the loop
  loopTaskSetup();

  // give some info about the average duration of the loop
  #ifdef KDEBUG
    endLoop = micros();
    // @@@ proximity debugging:
    if((millis() - lastDebugOutput) > 10)
    {
      /*
      TouchButton *t1 = (TouchButton*)baseSwitch->getButtonByIndex(0);
      TouchButton *t2 = (TouchButton*)baseSwitch->getButtonByIndex(1);
      TouchButton *tp1 = (TouchButton*)baseSwitch->getButtonByIndex(4);
      TouchButton *tp2 = (TouchButton*)baseSwitch->getButtonByIndex(5);
      Debug.println(F("Proximity: B1: %u        B2: %u        P1: %u        P2: %u"), t1->getLastSampleValue(), t2->getLastSampleValue(), tp1->getLastSampleValue(), tp2->getLastSampleValue());
      lastDebugOutput = millis();
      */
    }
    //if(endLoop - startLoop > 400)
    //  Debug.println(F("ATTENTION: Main Loop exceeds KNX timing requirement! %uus"), (endLoop - startLoop));
  #endif
}
