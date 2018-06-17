/*
  CY8MBR3116.cpp Library for controlling/handling the Cypress CY8MBR3116 touch controller
  Created by Christian Dürnberger, June 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include <Wire.h>
#include "CY8CMBR3116.h"


CY8CMBR3116::CY8CMBR3116(uint8_t _I2CAddress)
{
  this->I2CAddress              = _I2CAddress;
  this->longTouchThreshold      = 1250; // [milli]
  this->touchThreshold          = 250;  // [milli]
  this->taskTriggerGap          = 5000; // [micro]
  this->prevButtonStatus        = 0;
  this->prevProximityStatus     = 0;
}


CY8CMBR3116::~CY8CMBR3116()
{

}


// uint8_t _sensoryType, uint8_t _sensorId, bool _value
void CY8CMBR3116::setSensorStateCallback(std::function<void(uint8_t, uint8_t, bool)> _sensorStateChangedCallback)
{
  this->sensorStateChangedCallback = _sensorStateChangedCallback;
}

// uint8_t _sensorId, uint8_t _event, uint8_t _count
void CY8CMBR3116::setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)> _touchEventCallback)
{
  this->touchEventCallback = _touchEventCallback;
}

// uint8_t _sensorId, uint8_t _event
void CY8CMBR3116::setProximityEventCallback(std::function<void(uint8_t, uint8_t)> _proximityEventCallback)
{
  this->proximityEventCallback = _proximityEventCallback;
}

// uint8_t _gesture
void CY8CMBR3116::setGestureEventCallback(std::function<void(uint8_t)> _gestureEventCallback)
{
  this->gestureEventCallback = _gestureEventCallback;
}


void CY8CMBR3116::setThresholds(uint16_t _touchThreshold, uint16_t _longTouchThreshold, uint16_t _positioningTouchThreshold)
{
  this->longTouchThreshold        = _longTouchThreshold;
  this->touchThreshold            = _touchThreshold;
  this->positioningTouchThreshold = _positioningTouchThreshold;
}


void CY8CMBR3116::setup(uint8_t _setupConfig)
{
  // upload the selected configuration to the touch controller
  // the data is stored into the flash, so we should not call this method to extend the life of memory
  this->uploadConfiguration(_setupConfig);
}


bool CY8CMBR3116::uploadConfiguration(uint8_t _setupConfig)
{
  uint8_t               errorCode[1];
  std::array<char, 128> configurationData;

  // setup configuration for the testboard
  if(_setupConfig == 99)
  {
    configurationData = {
      0x7Bu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x80u, 0x80u, 0x7Fu, 0x80u,
      0x80u, 0x80u, 0x80u, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu,
      0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x03u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x83u,
      0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
      0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
      0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
      0xFFu, 0x00u, 0x00u, 0x00u, 0x40u, 0x03u, 0x01u, 0x08u,
      0x00u, 0x37u, 0x05u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x7Du, 0x0Bu
    };
  }
  // setup configuration for hardware rev 1.0
  // TODO: @@@
  else
  {
    configurationData = {
      0x7Bu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x80u, 0x80u, 0x7Fu, 0x80u,
      0x80u, 0x80u, 0x80u, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu,
      0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x03u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x83u,
      0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
      0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
      0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
      0xFFu, 0x00u, 0x00u, 0x00u, 0x40u, 0x03u, 0x01u, 0x08u,
      0x00u, 0x37u, 0x05u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
      0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x7Du, 0x0Bu
    };
  }


  // be sure device is out of standby mode when writing the configuration
  // for this we do a dummy read to bring it on (otherwise I2C may return NACK)
  this->readData(CY8_CTRL_CMD_ERR, 1, errorCode, 250);
  this->readData(CY8_CTRL_CMD_ERR, 1, errorCode, 250);

  Debug.println(F("TouchController pushing configuration to : %u"), this->I2CAddress);

  // set the register to the settings
  Wire.beginTransmission(this->I2CAddress);
  Wire.write(CY8_REGMAP_ORIGIN);
  Wire.endTransmission();
  delay(10);

  // we have to send the configuration data in chunks because arduino only can send 32 byte at a time
  uint16_t  dataToSend    = 128;
  uint16_t  dataIdx       = 0;
  uint8_t   chunkSize     = 31;
  uint8_t   chunkSizeSent;
  uint8_t   transferState;
  uint8_t   transferError = 0;

  while(dataToSend > 0)
  {
    chunkSizeSent = 0;
    // begin chunk send and jump to the offset register
    Wire.beginTransmission(this->I2CAddress);
    Wire.write(dataIdx);
    // now send the next bytes
    for(uint8_t i=0; (i<chunkSize && i<dataToSend); i++)
    {
      Wire.write(configurationData[dataIdx+i]);
      chunkSizeSent++;
    }
    transferState = Wire.endTransmission();
    if(transferState != 0)
    {
      transferError = 1;
      Debug.println(F("Error sending configuration: %u"), transferState);
    }
    dataIdx     += chunkSizeSent;
    dataToSend  -= chunkSizeSent;
    Debug.println(F("TouchController config loaded: %u/%u"), dataIdx, 128);
  }

  // acknowledge the sent configuration data
  if(!transferError)
  {
    Wire.beginTransmission(this->I2CAddress);
    Wire.write(CY8_CTRL_CMD);
    Wire.write(CY8_SAVE_CHECK_CRC);
    Wire.endTransmission();
    Debug.println(F("ACK config"));
    delay(300);

    // read the register that contains the status of the last executed command
    // if the ACK command failed the new configuration was not stored
    this->readData(CY8_CTRL_CMD_ERR, 1, errorCode, 300);
    if(errorCode[0] != 0x00)
    {
      transferError = errorCode[0];
      Debug.println(F("Error register: %u"), errorCode[0]);
    }
  }

  // reboot the touch controller so the setting will be used
  if(!transferError)
  {
    Wire.beginTransmission(this->I2CAddress);
    Wire.write(CY8_CTRL_CMD);
    Wire.write(CY8_SW_RESET);
    Wire.endTransmission();
    Debug.println(F("Touch Controller reset"));
    delay(200);
  }

  if(transferError)
    Debug.println(F("Error programming touch controller configuration: %u"), transferError);

  return transferError > 0 ? true : false;
}


void CY8CMBR3116::interrupt()
{
  if(this->active)
    this->process();
}


void CY8CMBR3116::reset()
{
  this->resetCacheData();

  Wire.beginTransmission(this->I2CAddress);
  Wire.write(CY8_CTRL_CMD);
  Wire.write(CY8_SW_RESET);
  Wire.endTransmission();
}


void CY8CMBR3116::setActive(bool _active)
{
  this->active = _active;
  if(!_active)
    this->resetCacheData();
}


void CY8CMBR3116::resetCacheData()
{
  for(uint8_t i=0; i<CY8_TOUCHSENSORCOUNT; i++)
  {
    touchStartTime[i] = 0;
    touchEndTime[i]   = 0;
    touchCounter[i]   = 0;
    taskProcess[i]    = false;
  }
}


void CY8CMBR3116::process()
{

  // when we got an interrupt we do have new touch data available (something has changed)
  // so we can read it from the approriate register value over I2C
  uint8_t buttonStatus[2], proximityStatus[1];

  // first read the button states. This is a 2 bytes value
  this->readData(CY8_BUTTON_STATUS, 2, buttonStatus);
  uint16_t curButtonStatus = ((buttonStatus[1]<<8) | buttonStatus[0]);
  for(uint8_t i=0; i<16; i++)
  {
    bool prevState  = bitRead(this->prevButtonStatus, i);
    bool curState   = bitRead(curButtonStatus, i);
    if(prevState != curState)
        this->sensorStateChanged(0, i, curState);
  }
  this->prevButtonStatus = curButtonStatus;

  // read the proximity states
  this->readData(CY8_PROX_STAT, 1, proximityStatus);
  uint8_t curProximityStatus = proximityStatus[0];
  for(uint8_t i=0; i<8; i++)
  {
    bool prevState  = bitRead(this->prevProximityStatus, i);
    bool curState   = bitRead(curProximityStatus, i);
    if(prevState != curState)
        this->sensorStateChanged(1, i, curState);
  }
  this->prevProximityStatus = curProximityStatus;
}


uint16_t CY8CMBR3116::readData(uint8_t _register, uint8_t _length, uint8_t* _data, uint16_t _waitTimeAfterWrite)
{
  uint16_t bytes_received = 0;
  uint16_t remaining = _length;
  uint8_t* next = _data;

  Wire.beginTransmission(this->I2CAddress);
  Wire.write(_register);
  Wire.endTransmission();

  if(_waitTimeAfterWrite > 0)
    delay(_waitTimeAfterWrite);

  Wire.requestFrom(this->I2CAddress, _length);
  while (Wire.available() && remaining--)
  {
    *next++ = Wire.read();
    ++bytes_received;
  }

  return bytes_received;
}


void CY8CMBR3116::sensorStateChanged(uint8_t _sensoryType, uint8_t _sensorId, bool _value)
{
  // call callback method if registered
  if(this->sensorStateChangedCallback)
    this->sensorStateChangedCallback(_sensoryType, _sensorId, _value);

  // on non proximity sensors we want to handle the touch, long touch and double touch event
  // for this we have to store some timing data, when touch appears and disappears
  if(_sensoryType == 0)
  {
    if(_value == 1)
    {
      // if we do touch we check the last touch end time. If it is in a certain amount of time and we already touched one
      uint64_t lastTouchEnd = this->touchEndTime[_sensorId];
      uint64_t touchStart   = millis();

      // when we did have a touch on the button before and we do touch again it may be a doublecklick or tripple....
      // to be aware of this we do add the touch counter, otherwise set the counter to 1
      if(lastTouchEnd > 0 && (touchStart - lastTouchEnd <= this->touchThreshold))
      {
        this->touchCounter[_sensorId] += 1;
        this->touchEndTime[_sensorId] = 0;
      }
      else
        this->touchCounter[_sensorId] = 1;

      // store the new touch start time
      this->touchStartTime[_sensorId] = touchStart;
      // if a button state has changed we have to process the button state in the task
      this->taskProcess[_sensorId] = true;
    }
    else
    {
        // if we are releasing a positioning touch we have to send the positioning stop event immediately
        // a positioning touch is identified by having the touchCounter set to 255 in the loop
        if(this->touchCounter[_sensorId] == 255)
        {
          if(this->touchEventCallback)
            this->touchEventCallback(_sensorId, 21, 1);
          this->touchCounter[_sensorId] = 0;
        }


        // store the end time of the touch, but only if it was no long touch before
        if(this->touchCounter[_sensorId] > 0)
        {
          uint64_t touchEnd   = millis();
          this->touchEndTime[_sensorId] = touchEnd;
          // if a button state has changed we have to process the button state in the task
          this->taskProcess[_sensorId] = true;
        }
    }
  }
  // for proximity sensors we can hand off the event as it is, only for gestures we have to store some data
  else
  {
    this->proximityEventCallback(_sensorId, _value);

    // TODO: gestures
    //  if Sensor A goes high, and then sensor c comes high and then sensor A is of and then sensor B, its a swipe down

  }
}


uint16_t CY8CMBR3116::calcDiff(uint64_t _stop, uint64_t _start)
{
  if(_stop < _start)
  {
    return (uint16_t)(_stop + (UINT64_MAX - _start));
  }
  else
  {
      return (uint16_t)(_stop - _start);
  }
}


void CY8CMBR3116::enableMultipleTouch(uint8_t _sensorId, bool _enable)
{
  this->multipleTouchEnabled[_sensorId] = _enable;
}


void CY8CMBR3116::enablePositioningTouch(uint8_t _sensorId, bool _enable)
{
  this->positioningTouchEnabled[_sensorId] = _enable;
}


// task entry for checking clicks
void CY8CMBR3116::task()
{

  //no need to do this every ms. It should be adequate ~ 10ms
  if(this->taskTriggerGap = 0 || (abs(micros() - this->taskLastRunTimeStop) > this->taskTriggerGap ))
  {

    this->taskLastRunTimeStart = micros();
    // we have to check "released" buttons with a touch counter
    for(uint8_t i=0; i<CY8_TOUCHSENSORCOUNT; i++)
    {
      if(this->taskProcess[i])
      {

        // if the button is currently touched and it's touched beyond the "positioning" touch limit, then its a positioning touch
        // in this case we have to send the positioning start event and disable the long touch event ( both are not valid! )
        // we also remove the button from the process list because we will handle it on touch end event in this case
        if(this->positioningTouchEnabled[i] && (millis() - this->touchStartTime[i]) >= this->positioningTouchThreshold)
        {
          // call callback method if registered
          if(this->touchEventCallback)
            this->touchEventCallback(i, 20, 1);

          this->touchStartTime[i] = 0;
          this->touchEndTime[i] = 0;
          this->touchCounter[i] = 255;
          this->taskProcess[i]  = false;
        }

        // if the button is currently touched and it's touched beyond the long touch limit, then its a long touch
        // is this the case we have to remove the button from the process task and do reset the touch counter
        if(this->taskProcess[i] && (millis() - this->touchStartTime[i]) >= this->longTouchThreshold)
        {
          // call callback method if registered
          if(this->touchEventCallback)
            this->touchEventCallback(i, 2, 1);

          this->touchStartTime[i] = 0;
          this->touchEndTime[i] = 0;
          this->touchCounter[i] = 0;
          this->taskProcess[i]  = false;
        }

        // if there is a touch end time for a sensor we may check if its a normal click (touchThreshold)
        // we have a special or clause that will be used if multiple clicks are disabled, so we do not ave to wait for the whole touchThreshold to be exceeded!
        if(this->taskProcess[i] && this->touchEndTime[i] > 0 && ((millis() - this->touchEndTime[i]) >= this->touchThreshold || !this->multipleTouchEnabled[i] ))
        {
          // call callback method if registered
          if(this->touchEventCallback)
            this->touchEventCallback(i, 1, this->touchCounter[i]);

          this->touchStartTime[i] = 0;
          this->touchEndTime[i] = 0;
          this->touchCounter[i] = 0;
          this->taskProcess[i]  = false;
        }
      }
    }
    this->taskLastRunTimeStop = micros();
  }
}