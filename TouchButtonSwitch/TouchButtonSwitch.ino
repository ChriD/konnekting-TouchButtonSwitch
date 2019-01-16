
/*
  Konnekting - TouchButtonSwitch (0xCDDC)
  Created by Christian Dürnberger (ChriD), 2019
  https://github.com/ChriD
*/


#include "Arduino.h"
#include <FlashAsEEPROM.h>
#include "KonnektingDevice.h"
#include "TouchButtonSwitch.h"
#include "src/TouchSwitch_4X_V1.h"


// simple version definition for the library
#define LIB_VERSION             "1.0"

// if this define is uncommented, debugging via serial is activated. This should not be active on productive environment!
#define KDEBUG

// for testing purposes without having an active bcu attached we have to skip
// the knx connection and task codes to test the device. This can be done by setting this define
//#define BCUDISABLED

// This is the time in miliseconds how long the button should wait after startup/setup to allow touches
// There has to be a enough time for the user to put the frontplate on the switch before recalibration of
// the touch sensor has been done
#define STARTUP_IDLETIME        10000

// define seriald for the Adafruit Itsy-Bitsy M0
// SERIAL_BCU is mandatory to work, the SERIAL_DBG will onlky be used in DEBUG Mode
#define SERIAL_DBG              SERIAL_PORT_USBVIRTUAL
#define SERIAL_BCU              Serial1

// so our touch swutch device is the TouchSwitch_CODENAME_4X_V1 switch wich has 4 touch buttons and 2 proximity
// sensors on top and on the button of the device. So we use the approiate class for that switch
BaseSwitch   *baseSwitch = new TouchSwitch_4X_V1();

// TODO: do this one into the TouchSwitch lib!
#define PROG_BUTTON_PIN   10


uint16_t counter = 0;


// TODO: IFDEF NOTEEPROM
//FlashStorage
byte readMemory(int index) {
    Debug.println(F("FLASH read on index %d"),index);
    return EEPROM.read(index);
}

void writeMemory(int index, byte val) {
    Debug.println(F("FLASH write value %d on index %d"),val, index);
    EEPROM.write(index, val);
}

void updateMemory(int index, byte val) {
    Debug.println(F("FLASH update"));
    if (EEPROM.read(index) != val) {
        EEPROM.write(index, val);
    }
}

void commitMemory() {
    Debug.println(F("FLASH commit"));
    EEPROM.commit();
}



void onButtonStateChanged(uint16_t _buttonId, uint16_t _state)
{
  SERIAL_DBG.print(_buttonId);
  SERIAL_DBG.print(" : ");
  SERIAL_DBG.print(_state);
  SERIAL_DBG.print("\n");
}

void onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  SERIAL_DBG.print(_buttonId);
  SERIAL_DBG.print(" : ");
  SERIAL_DBG.print(_type);
  SERIAL_DBG.print(" | ");
  SERIAL_DBG.print(_value);
  SERIAL_DBG.print("\n");

  #ifdef BCUDISABLED
    return;
  #endif

  // TEST1
  SERIAL_DBG.print("\nOutput");
  Knx.write(COMOBJ_button1, DPT1_001_on);

  uint16_t idOffset = 0;
  if(_buttonId == 1)
  {
    if(_type == 1 && _value <= 1)
      Knx.write(COMOBJ_button1 + idOffset, DPT1_001_on);
    else if(_type == 1)
       Knx.write(COMOBJ_button1_multi + idOffset, _value);
    else if(_type == 2)
      Knx.write(COMOBJ_button1_long + idOffset, DPT1_001_on);
    else if(_type == 20)
      Knx.write(COMOBJ_button1_position_touchstart + idOffset, DPT1_001_on);
    else if(_type == 21)
      Knx.write(COMOBJ_button1_position_touchend + idOffset, DPT1_001_on);
  }
}


void onProximityAlert(uint16_t _buttonId, boolean _isProximity, uint16_t _proximityValue, uint16_t _proximityLevel)
{
  /*
  counter++;
  SERIAL_DBG.print(counter);
  SERIAL_DBG.print(" # ");
  SERIAL_DBG.print(_buttonId);
  SERIAL_DBG.print(" : ");
  SERIAL_DBG.print(_isProximity);
  SERIAL_DBG.print(" | ");
  SERIAL_DBG.print(_proximityValue);
  SERIAL_DBG.print(" | ");
  SERIAL_DBG.print(_proximityLevel);
  SERIAL_DBG.print("\n");
  */
}



void setup()
{
  // when starting we set the SETUP MODE on the switch with the first _proximityLevel
  // so in most cases some led will be set to a specific color or one of four led is illuminating
  // this is a nice to have to go into troublehooting without having a serial debugger online
  // SETUP MODE 1 is at the very beginning and indicates as power on LED
  baseSwitch->setMode(SWITCH_MODE::SETUP, 1);

  // start the debugging serial and wait for it to be present before doing anything
  // of course we do only activate the serial if we have enabled debugging
  #ifdef KDEBUG
    SERIAL_DBG.begin(115200);
    while (!SERIAL_DBG) {}
    Debug.setPrintStream(&SERIAL_DBG);
    Debug.println(F("KONNEKTING TouchButtonSwitch Library %u"), LIB_VERSION);
  #endif

  baseSwitch->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0,&onButtonAction));
  baseSwitch->attachCallbackOnProximityAlert(makeFunctor((CallbackFunction_ProximityAlert*)0,&onProximityAlert));

  // setup the connection to the KNX-BUS
  #ifndef BCUDISABLED
    initKNX();
    // if we have already an application programm (!isFactorySetting), get the values from the EEPROM
    // and apply them to the switch
    if (!Konnekting.isFactorySetting())
      baseSwitch->initParameters(); // TODO: @@@ How to use Konnekting var? Parm via address?
  #endif

  // SETUP MODE 2 is after connection with the BCU is established
  baseSwitch->setMode(SWITCH_MODE::SETUP, 2);

  // after we have a connection to the bus, we do setup the touch switch and start the calibration
  // of all the touch areas and proximity sensors
  if(!baseSwitch->setup())
  {
    // TODO: add some error code?!
    Debug.println(F("Error initializing Touch Switch"));
  }

  // SETUP MODE 3 is after the setup of the switch has been done
  baseSwitch->setMode(SWITCH_MODE::SETUP, 3);

  // TODO: @@@ calibration should be done after about 10 secods!!!!
  baseSwitch->startCalibration();

  // SETUP MODE 4 is after setup is done and calibration is STARTUP_IDLETIME
  // this mode will stay till the STARTUP_IDLETIME is reached
  baseSwitch->setMode(SWITCH_MODE::SETUP, 4);

  // setup the prog button interrupt for rising edge
  pinMode(PROG_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PROG_BUTTON_PIN), progButtonPressed, FALLING);
}


// this method does the initialization of the KONNEKTING Library
// if there is a problem connecting to the bus, the method will not return!
void initKNX()
{

  Konnekting.setMemoryReadFunc(&readMemory);
  Konnekting.setMemoryWriteFunc(&writeMemory);
  Konnekting.setMemoryUpdateFunc(&updateMemory);
  Konnekting.setMemoryCommitFunc(&commitMemory);

  Konnekting.init(SERIAL_BCU, &progLed, MANUFACTURER_ID, DEVICE_ID, REVISION);
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
}

void progLed (bool _isPrg){
  if(_isPrg)
  {
    //touchSwitch->changeMode(TS_MODE_PROG, false);
  }
  else
  {
    //touchSwitch->changeMode(TS_MODE_NORMAL, false);
  }
}

// this method will be called when a com object is updated
void knxEvents(byte _index)
{
}


uint64_t loopSum    = 0;
uint64_t loopCount  = 0;
boolean isAppReady = false;


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

  // let the button do its application loop tasks
  baseSwitch->task();

  // TODO: after about  10 seconds start re-calibration
  // TODO: after about  12 seconds start Normal mode

  //  TODO: @@@
  if(isAppReady != Konnekting.isReadyForApplication())
  {
    isAppReady = Konnekting.isReadyForApplication();
    Debug.println(F("APP READY: %u"), isAppReady);
  }


  // give some info about the average duration of the loop
  #ifdef KDEBUG
    endLoop = micros();
    loopCount++;
    loopSum += endLoop - startLoop;
    if(loopSum > 1000*1000)
    {
      //Debug.println(F("Main Loop timing: %uus"), (loopSum/loopCount));
      loopCount = 0;
      loopSum   = 0;
    }

  // @@@ proximity debugging:
    //TouchButton *t1 = (TouchButton*)baseSwitch->getButtonByIndex(4);
    //TouchButton *t2 = (TouchButton*)baseSwitch->getButtonByIndex(5);
    //Debug.println(F("PROXA: %uus         PROXB: %uss"), t1->getLastSampleValue(), t2->getLastSampleValue());

    //if(endLoop - startLoop > 400)
    //  Debug.println(F("ATTENTION: Main Loop exceeds KNX timing requirement! %uus"), (endLoop - startLoop));
  #endif
}
