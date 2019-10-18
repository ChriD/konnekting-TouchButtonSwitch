/*
  TouchSwitch_5X_V1.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.

  This is a 5 button touch switch with one main button in the middle and 4 sub buttons each 2 left and right
  It does have one single RGB Led in the middle of the main button for siganlizing the button modes and user
  states. It also does have an optional speaker for click response and a optional BME280 for temperature and humidity
*/

#include "Arduino.h"
#include "TouchSwitch_5X_V1.h"


TouchSwitch_5X_V1::TouchSwitch_5X_V1() : TouchSwitch()
{
  this->lastPatternRunTime = 0;
  this->rgbLed = NULL;

  this->lastEnvSenorsRunTime  = 0,

  // activate ebvironmental sensors
  this->envSensorsSettings.temperature  = true;
  this->envSensorsSettings.humidity     = true;
  this->envSensorsSettings.pressure     = true;


  this->ledPattern_Default[0] = LedPattern::CMD_SET;
  this->ledPattern_Default[1] = 255;
  this->ledPattern_Default[2] = 0;
  this->ledPattern_Default[3] = 255;
  this->ledPattern_Default[4] = LedPattern::CMD_FINISHED;
}


TouchSwitch_5X_V1::~TouchSwitch_5X_V1()
{
}


void TouchSwitch_5X_V1::initButtons()
{
  // we have to set up ours switch by defining the touch buttons it does have
  this->addButton(new TouchButton(A2, 1, 0)); // main pad
  this->addButton(new TouchButton(A1, 2));
  this->addButton(new TouchButton(A3, 3));
  this->addButton(new TouchButton(A4, 4));
  this->addButton(new TouchButton(A5, 5));

  // TODO: Add PRG Button which is a normal "PinButton"
  //this->addButton(new PinButton(TS_4X_V1_BTN1_PIN, TS_4X_V1_BTN1_ID, FALLING));

  this->rgbLed = new LedPattern_RGB(11,12,13);

  // on setup be sure that there is some lightning.
  // these setting will be overwritten when the memory with the knx settings was loaded!
  this->rgbLed->setBrightness(25);
}


// TODO: Add general init()?
void TouchSwitch_5X_V1::setMode(SWITCH_MODE _mode, uint16_t _modeLevel)
{
  TouchSwitch::setMode(_mode, _modeLevel);

  // when changeing the mode we have to stop the current pattern which is running
  this->rgbLed->stop();

  if(_mode == SWITCH_MODE::NORMAL)
    this->rgbLed->start(this->ledPattern_Default);
  if(_mode == SWITCH_MODE::PROG)
    this->rgbLed->start(ledPattern_Prog);
  if(_mode == SWITCH_MODE::CALIBRATION)
    this->rgbLed->start(ledPattern_Calibration);
  if(_mode == SWITCH_MODE::SETUP)
    this->rgbLed->start(ledPattern_Setup);
}


void TouchSwitch_5X_V1::onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  TouchSwitch::onButtonAction(_buttonId, _type, _value);

  // if we do have a touch on a button we do signalize it by a short pattern and a beep
  if(this->mode == SWITCH_MODE::NORMAL)
  {
    if(this->speakerSettings.clickFeedbackEnabled)
      tone(A0, this->speakerSettings.clickFeedbackFrequency, this->speakerSettings.clickFeedbackDuration);

    this->rgbLed->stop();
    this->rgbLed->start(ledPattern_Touch);
  }
}


void TouchSwitch_5X_V1::parmLightningSettings(BaseSwitchLightning _lightningSettings)
{
  TouchSwitch::parmLightningSettings(_lightningSettings);
  this->ledPattern_Default[1] = _lightningSettings.stdR;
  this->ledPattern_Default[2] = _lightningSettings.stdG;
  this->ledPattern_Default[3] = _lightningSettings.stdB;
  if(this->rgbLed)
    this->rgbLed->setBrightness(_lightningSettings.stdBrightness);
}


boolean TouchSwitch_5X_V1::setup()
{
  boolean ret = TouchSwitch::setup();

  // if the env sensor is enabled we have to set up the I2C and do ind the sensor on the standard
  // I2C Address forthe BME280/BMP280
  if( this->envSensorsSettings.temperature  ||
      this->envSensorsSettings.humidity     ||
      this->envSensorsSettings.pressure)
  {
    Wire.begin();

    // wait for the temp sensor to be found
    while(!bme.begin() && millis() < 10000)
    {
      // TODO: show error (Blinking red LED)
      SERIAL_PORT_USBVIRTUAL.println("Could not find BME280 sensor!");
      ret = false;
      delay(1000);
    }

    switch(bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        SERIAL_PORT_USBVIRTUAL.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        SERIAL_PORT_USBVIRTUAL.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        SERIAL_PORT_USBVIRTUAL.println("Found UNKNOWN sensor! Error!");
        ret = false;
    }
  }

  return ret;
}


void TouchSwitch_5X_V1::requestEnvironmentData()
{
  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  bme.read(pres, temp, hum, tempUnit, presUnit);

  this->curEnvData.temperature  = temp  + this->envSensorsSettings.temperatureAdj;
  this->curEnvData.humidity     = hum   + this->envSensorsSettings.humidityAdj;
  this->curEnvData.pressure     = pres;

  this->callback_onEnvDataUpdated(this->curEnvData);
}


void TouchSwitch_5X_V1::task()
{
  TouchSwitch::task();

  // period for led will be fixed on 10ms
  if(this->getPeriod(this->lastPatternRunTime) > 10)
  {
    this->rgbLed->update();
    // everytime a pattern is finished, we have to load the "standard" pattern
    if(this->rgbLed->finished())
      this->rgbLed->start(this->ledPattern_Default);
    this->lastPatternRunTime = millis();
  }

  // periodically get the temperature and humidity if period time is enabled
  // (period time may be auto enabled if there are any environmental warnings are active)
  // this may lead to problems if a button is clicked meanwhile?!
  if( (this->envSensorsSettings.temperature || this->envSensorsSettings.humidity || this->envSensorsSettings.pressure) &&
      (this->getPeriod(this->lastEnvSenorsRunTime) > this->envSensorsSettings.temperaturePeriod || !this->lastEnvSenorsRunTime) &&
      this->envSensorsSettings.temperaturePeriod)
  {
    this->requestEnvironmentData();
    this->lastEnvSenorsRunTime = millis();
  }
}



// TODO: specify the ID's of the button so we can identify them
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!






