
/*
  Konnekting - TouchButtonSwitch (0xCDDC)
  Created by Christian Dürnberger (ChriD), 2019
  https://github.com/ChriD
*/


#include "Arduino.h"
#include "KonnektingDevice.h"
#include "TouchButtonSwitch.h"
#include "src/TouchSwitch_4X_V1.h"


// simple version definition for the library
#define LIB_VERSION             "1.0"

// if this define is uncommented, debugging via serial is activated. This should not be active on productive environment!
#define KDEBUG

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


uint16_t counter = 0;




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
}


void onProximityAlert(uint16_t _buttonId, boolean _isProximity, uint16_t _proximityValue, uint16_t _proximityLevel)
{
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
}



void setup()
{

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

  if(!baseSwitch->setup())
    SerialUSB.print("Error initializing Touch Switch");
  baseSwitch->startCalibration();
}


// this method does the initialization of the KONNEKTING Library
// if there is a problem connecting to the bus, the method will not return!
void initKNX()
{
  Konnekting.init(SERIAL_BCU, &progLed, MANUFACTURER_ID, DEVICE_ID, REVISION);
}


void initKNXDeviceParameters()
{
  // thresholds
    int16_t touch_threshold           = Konnekting.getUINT16Param(PARAM_touch_threshold);
    uint16_t mode_longtouch_threshold = Konnekting.getUINT16Param(PARAM_mode_longtouch_threshold);
    //uint16_t mode_position_threshold  = Konnekting.getUINT16Param(PARAM_mode_position_threshold);
    //touchSwitch->setThresholds(touch_threshold, mode_longtouch_threshold, mode_position_threshold);
    //Debug.println(F("Thresholds: Touch=%u, Longtouch=%u, Position=%u"), touch_threshold, mode_longtouch_threshold, mode_position_threshold);

    // backlights
    uint8_t valueStandby      = Konnekting.getUINT8Param(PARAM_light_intensity_standy);
    uint8_t valueProximity    = Konnekting.getUINT8Param(PARAM_light_intensity_proximity);
    //touchSwitch->setBacklightParameters(valueStandby, valueProximity);
    //Debug.println(F("Backlight: Standby=%u, Proximity=%u"), valueStandby, valueProximity);

    // touch ic settings
    uint8_t tsic_sensitivity = Konnekting.getUINT8Param(PARAM_tsic_sensitivity);
    //touchSwitch->getTouchControllerObject()->setSensorSensitivity(tsic_sensitivity);
    uint8_t tsic_fingerThreshold = Konnekting.getUINT8Param(PARAM_tsic_fingerThreshold);
    //touchSwitch->getTouchControllerObject()->setSensorFingerThreshold(tsic_fingerThreshold);
    //Debug.println(F("TouchIC ButtonSensitivity: %u, ButtonFingerThreshold=%u"), tsic_sensitivity, tsic_fingerThreshold);

    // setup settings for sensor 1
    bool    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button1_enableMultiTouch);
    uint8_t mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button1_mode);
    //touchSwitch->setButtonParameters(SENSORID_1, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_1, enableMultiTouch, mode);

     // setup settings for sensor 2
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button2_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button2_mode);
    //touchSwitch->setButtonParameters(SENSORID_2, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_2, enableMultiTouch, mode);

     // setup settings for sensor 3
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button3_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button3_mode);
    //touchSwitch->setButtonParameters(SENSORID_3, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_3, enableMultiTouch, mode);

    // setup settings for sensor 4
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button4_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button4_mode);
    //touchSwitch->setButtonParameters(SENSORID_4, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_4, enableMultiTouch, mode);
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




void loop()
{
  // we have to call the knx.task form the konnekting library faster then ~ 400us to not miss any telegram
  // in next versions this should be obsolete but for now we have to stay with this pitfall
  // ATTENTION: Currently we do have some loops which will go above 400us due I2C handling, we have to ceck in
  // production mode if this will create any problems
  baseSwitch->task();

}
