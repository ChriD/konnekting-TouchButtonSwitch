
/*
  Konnekting - TouchButtonSwitch (0xCDDC)
  Created by Christian Dürnberger (Chrid), Mai 2018
  https://github.com/ChriD
*/


/* TODO:
  # add temperature and humidity/temp sensor
  # sensor does not startup very well with testboard settings, dont know why
  # user software pwm https://github.com/bhagman/SoftPWM -> do not use digitalwrite.. use stm stuff (fast pinoutputs)
  # http://stm32duino.com/viewtopic.php?t=2305
*/


#include "Arduino.h"
#include "Wire.h"
#include "KonnektingDevice.h"
#include "TouchButtonSwitch.h"
#include "src/CY8TouchSwitch.h"

// we do use the same firmware for 4x or 6x touch switches
// so we have to define which type of sitch is being compiled
#define SWITCHTYPE        4

// define the sensorids for the given buttons (There are 16 sensors 0 - 15)
#define SENSORID_1        6
#define SENSORID_2        5
#define SENSORID_3        12
#define SENSORID_4        4

// define the led pins for the given buttons
#define SENSORLED_1_PIN        PA11
#define SENSORLED_2_PIN        PB9
#define SENSORLED_3_PIN        PA10
#define SENSORLED_4_PIN        PA4

// This is the time in miliseconds how long the button should wait after setup to allow touch_threshold
// There has to be a enough time for the user to put the frontplate on the switch before recalibration of
// the touch sensor has been done
#define STARTUP_IDLETIME       10000

// used for debugging
// should not be activated on productive environment
#define KDEBUG

// for testing purposes without having a bcu attached we have to skip
// the knx code to test the device. For this we define the NOBCU
//#define NOBCU


HardwareSerial SerialDBG(PA3, PA2);
HardwareSerial SerialBCU(PC5, PC4);
#define DEBUGSERIAL       SerialDBG
#define KNX_SERIAL        SerialBCU
#define PROG_LED_PIN      0
#define PROG_BUTTON_PIN   PB1
#define TC_INTERRUPTPIN   PC0

// the differnt type of switches we do have
// currently we do have only a 4x switch type
#define SWITCHTYPE_4X     4

// we do have some global vars
CY8TouchSwitch  *touchSwitch;
bool            startupRecalibrationNeeded = true;
uint64_t        mainLoopEndTime;


// TEST @@@
bool touchSetupOk = false;



void setup()
{
  #ifdef KDEBUG
    DEBUGSERIAL.begin(115200);   
    while (!DEBUGSERIAL) { }
    Debug.setPrintStream(&DEBUGSERIAL);
    Debug.println(F("KONNEKTING TouchButtonSwitch"));
  #endif

  touchSwitch = new CY8TouchSwitch();
  touchSwitch->setup();

  // add buttons for the given type of switch (4x or 6x)  
  touchSwitch->addButton(SENSORID_1, SENSORLED_1_PIN, true, false);
  touchSwitch->addButton(SENSORID_2, SENSORLED_2_PIN, true, false);
  touchSwitch->addButton(SENSORID_3, SENSORLED_3_PIN, true, false);
  touchSwitch->addButton(SENSORID_4, SENSORLED_4_PIN, true, false);

  touchSwitch->changeMode(TS_MODE_STARTUP1, true);
  delay(150);

  // startup I2C
  Wire.setSCL(PA8);
  Wire.setSDA(PC9);
  Wire.begin();

  touchSwitch->changeMode(TS_MODE_STARTUP2, true);
  delay(150);

  // after I2C is setup we may setup the touch controller
  // I'm not sure if its the best to update the config every time on start (due it should be persistent)
  touchSetupOk = touchSwitch->setupTouchController();

  touchSwitch->changeMode(TS_MODE_STARTUP3, true);
  delay(150);

  // after we have changed the mode of the switch to SETUP, we start setting up the
  // knx device stuff
  #ifndef NOBCU
    knxDeviceSetup();
  #endif

  touchSwitch->changeMode(TS_MODE_STARTUP4, true);
  delay(150);


  // setup the interrupt method for the touch controller
  pinMode(TC_INTERRUPTPIN, INPUT_PULLUP);
  attachInterrupt(TC_INTERRUPTPIN, touchControllerInterrupt, FALLING);

  // setup the prog button interrupt for rising edge
  pinMode(PROG_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PROG_BUTTON_PIN), progButtonPressed, FALLING);



  // set the callbacks we do like to send to the KNX-Bus
  touchSwitch->setSensorStateChangedEventCallback(sensorStateChangedEvent);
  touchSwitch->setTouchEventCallback(touchEvent);
  touchSwitch->setGestureEventCallback(gestureEvent);
  touchSwitch->setProximityEventCallback(proximityEvent);


  // after rebooting setup we have to wait a little bit for the user to put the frontboard
  // to the button, in this state we do stay "Setup" mode
  touchSwitch->changeMode(TS_MODE_SETUP, true);
  

}



// if the prog button is pressed we have to switch the device into the prog mode
// this happens if the user is pressing the prog button
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



void progLed (bool state){
  if(state)
  {
    touchSwitch->changeMode(TS_MODE_PROG, false);
  }
  else
  {
    touchSwitch->changeMode(TS_MODE_NORMAL, false);
  }
}



void knxDeviceSetup()
{
  Konnekting.init(KNX_SERIAL, &progLed, MANUFACTURER_ID, DEVICE_ID, REVISION);
  if (!Konnekting.isFactorySetting())
  {

    //
    //temperature send
    //temperature alarm resend
    //temperature min alarm
    //temperature max alarm
    //humidity send
    //humidity alarm resend
    //humidity min alarm
    //humidity max alarm
    
   //assert(1==2);



    // thresholds
    int16_t touch_threshold           = Konnekting.getUINT16Param(PARAM_touch_threshold);
    uint16_t mode_longtouch_threshold = Konnekting.getUINT16Param(PARAM_mode_longtouch_threshold);
    uint16_t mode_position_threshold  = Konnekting.getUINT16Param(PARAM_mode_position_threshold);
    touchSwitch->setThresholds(touch_threshold, mode_longtouch_threshold, mode_position_threshold);
    Debug.println(F("Thresholds: Touch=%u, Longtouch=%u, Position=%u"), touch_threshold, mode_longtouch_threshold, mode_position_threshold);

    // backlights
    uint8_t valueStandby      = Konnekting.getUINT8Param(PARAM_light_intensity_standy);
    uint8_t valueProximity    = Konnekting.getUINT8Param(PARAM_light_intensity_proximity);
    touchSwitch->setBacklightParameters(valueStandby, valueProximity);
    Debug.println(F("Backlight: Standby=%u, Proximity=%u"), valueStandby, valueProximity);

    // touch ic settings
    uint8_t tsic_sensitivity = Konnekting.getUINT8Param(PARAM_tsic_sensitivity);
    touchSwitch->getTouchControllerObject()->setSensorSensitivity(tsic_sensitivity);
    uint8_t tsic_fingerThreshold = Konnekting.getUINT8Param(PARAM_tsic_fingerThreshold);
    touchSwitch->getTouchControllerObject()->setSensorFingerThreshold(tsic_fingerThreshold);
    Debug.println(F("TouchIC ButtonSensitivity: %u, ButtonFingerThreshold=%u"), tsic_sensitivity, tsic_fingerThreshold);

    // setup settings for sensor 1
    bool    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button1_enableMultiTouch);
    uint8_t mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button1_mode);
    touchSwitch->setButtonParameters(SENSORID_1, enableMultiTouch, mode);
    Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_1, enableMultiTouch, mode);

     // setup settings for sensor 2
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button2_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button2_mode);
    touchSwitch->setButtonParameters(SENSORID_2, enableMultiTouch, mode);
    Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_2, enableMultiTouch, mode);

     // setup settings for sensor 3
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button3_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button3_mode);
    touchSwitch->setButtonParameters(SENSORID_3, enableMultiTouch, mode);
    Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_3, enableMultiTouch, mode);

    // setup settings for sensor 4
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button4_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button4_mode);
    touchSwitch->setButtonParameters(SENSORID_4, enableMultiTouch, mode);
    Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_4, enableMultiTouch, mode);


    //typeTemp = (int) Konnekting.getUINT8Param(PARAM_tempSendUpdate);
    //temperature polling interval (ms)
    //intervalTempUser = (long) Konnekting.getUINT32Param(PARAM_tempPollingTime)*1000;
    //minimal difference between previous and current temperature [°C]
    //diffTempUser = (float) Konnekting.getUINT8Param(PARAM_tempDiff)*0.1;
    //intervalHumdUser = (long) Konnekting.getUINT32Param(PARAM_rhPollingTime)*1000; //humidity polling interval (ms)
    Debug.println(F("User settings loaded"));
  }
  else
  {
    Debug.println(F("Factory settings loaded"));
  }

}



// the touch controller will trigger an interrupt when the state of a sensor changes
// we have to reroute the interrupt to the touchSwitch class for further processing
void touchControllerInterrupt()
{
  //Debug.println(F("Touch controller fired interrupt"));
  touchSwitch->interrupt();
}



void sensorStateChangedEvent(uint8_t sensorType, uint8_t _sensorId, bool _value)
{
  // TEST
  #ifndef NOBCU
    if(_sensorId > 1)
    {
      Knx.write(COMOBJ_button1, _sensorId);
      Knx.write(COMOBJ_button1_multi, _sensorId);
      Knx.write(COMOBJ_button1_double, _value);
    }
  #endif
}


void touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count)
{
  // calculate the offset for the KNX-Comobject id given on the sensor id
  // sensor ids are from 1 to the count of the buttons
  uint8_t idOffset = 0;
  //uint8_t idOffset = (_sensorId - 1) * 4;

  // send the touch event to the knx bus
  // we can use the base index for the object and add the ID-1 for correct index????   

  #ifndef NOBCU
  
    if(_event == 1)
    {
      if(_count == 1)
        Knx.write(COMOBJ_button1 + idOffset, DPT1_001_on);
      else if(_count == 2)
        Knx.write(COMOBJ_button1_double + idOffset, DPT1_001_on);
      else
        Knx.write(COMOBJ_button1_multi + idOffset, _count);
    }
    else if (_event == 2)
    {
      Knx.write(COMOBJ_button1_long + idOffset, DPT1_001_on);
    }
    else if (_event == 20)
    {
      Knx.write(COMOBJ_button1_position_touchstart + idOffset, DPT1_001_on);
    }
    else if (_event == 21)
    {
      Knx.write(COMOBJ_button1_position_touchend + idOffset, DPT1_001_on);
    }

  #endif

}


void proximityEvent(uint8_t _sensorId, uint8_t _event)
{
  //Knx.write(COMOBJ_button1_double, _sensorId);
  //Knx.write(COMOBJ_button1_multi, _event);
}


void gestureEvent(uint8_t _event)
{

}


// this method will be called when a com object is updated
void knxEvents(byte _index)
{
  // TODO: Temp/humidity asking

  
  switch (_index)
  {
     // case COMOBJ_btnShortInput:
     //       boolShortClick = Knx.read(COMOBJ_btnShortInput);
     //       Debug.println(F("KNX event: boolShortClick: %d"),boolShortClick);
     //       break;
     //   case COMOBJ_btnLongInput:
     //       boolLongClick = Knx.read(COMOBJ_btnLongInput);
     //       Debug.println(F("KNX event: boolLongClick: %d"),boolLongClick);
     //       break;
     //   case COMOBJ_binD0ValueInput:
     //       inputStateD0 = Knx.read(COMOBJ_binD0ValueInput);
     //       Debug.println(F("KNX event: inputStateD0: %d"),inputStateD0);
     //       break;
     //   case COMOBJ_binD1ValueInput:
     //       inputStateD1 = Knx.read(COMOBJ_binD1ValueInput);
     //       Debug.println(F("KNX event: inputStateD1: %d"),inputStateD1);
     //       break;
     //   case COMOBJ_nightMode:
     //       nightMode = (bool) Knx.read(COMOBJ_nightMode);
     //       Debug.println(F("KNX event: nightMode: %d"),nightMode);
     //       break;
        default:
          break;
  }
  
}



void loop()
{  
  // we have to call the knx.task form the konnekting library faster then ~ 400us to not miss any telegram
  // in next versions this should be obsolete but for now we have to stay with itself
  // ATTENTION: Currently we do have some loops which will go above 400us due I2C handling, we have to ceck in
  // production mode if this will create any problems
  #ifndef NOBCU
    Knx.task();
  #endif

  #ifdef KDEBUG
    uint64_t now = micros();
    if(mainLoopEndTime && now > mainLoopEndTime && now - mainLoopEndTime > 400)
      Debug.println(F("ATTENTION: Main Loop exceeds KNX timing requirement! %uus"), (now - mainLoopEndTime));
  #endif


  // the switch itself must loop eyerytime, no matter of KNX is active or not
  // while KNX connector is not active, the switch is in a special mode and will not raise any touch events
  touchSwitch->task();

  // be sure to reset the touch controller after resetting the main controller and be sure
  // that there is time to "set up" the frontboard finish (reset for recalibration)

  if (startupRecalibrationNeeded &&  millis() > STARTUP_IDLETIME)
  {
    startupRecalibrationNeeded = false;
    touchSwitch->resetTouchController();
    touchSwitch->changeMode(TS_MODE_NORMAL, true);
    Debug.println(F("Switch is ready for action!"));
    Knx.write(COMOBJ_button1_multi, 254);
  }


  if (Konnekting.isReadyForApplication())
  {
  }
  else
  {
  }


  mainLoopEndTime = micros();

  
}


