/*
  CY8MBR3116.h Library for controlling/handling the Cypress CY8MBR3116 touch controller
  Created by Christian Dürnberger, June 2018
  Released into the public domain.
  http://www.cypress.com/file/127221/download
*/


/*
TODO:
* remove auto reset on touch controller
* add configuration upload on reset
* add gesture
*/


#ifndef CY8CMBR3116_h
#define CY8CMBR3116_h

  #include "Arduino.h"
  #include "DebugUtil.h"

  #define CY8_TOUCHSENSORCOUNT          16

  #define CY8_BUTTON_STATUS             0xAA
  #define CY8_LATCHED_BUTTON_STATUS     0xAC
  #define CY8_PROX_STAT                 0xAE

  #define CY8_SW_RESET                  0xFF
  #define CY8_CTRL_CMD                  0x86

  class CY8CMBR3116
  {
    public:
      CY8CMBR3116(uint8_t _I2CAddress);
      ~CY8CMBR3116();
      void setup();
      void interrupt();
      void process();
      void task();

      void setSensorStateCallback(std::function<void(uint8_t, uint8_t, bool)>);
      void setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)>);
      void setProximityEventCallback(std::function<void(uint8_t, uint8_t)>);
      void setGestureEventCallback(std::function<void(uint8_t)>);

      void setThresholds(uint16_t _touchThreshold, uint16_t _longTouchThreshold, uint16_t _positioningTouchThreshold);
      void enableMultipleTouch(uint8_t sensorId, bool _enable = true);
      void enablePositioningTouch(uint8_t _sensorId, bool _enable = true);
      void reset();
    private:
      std::function<void(uint8_t, uint8_t, bool)> sensorStateChangedCallback;
      std::function<void(uint8_t, uint8_t, uint8_t)> touchEventCallback;
      std::function<void(uint8_t, uint8_t)> proximityEventCallback;
      std::function<void(uint8_t)> gestureEventCallback;

      uint16_t readData(uint8_t _register, uint8_t _length, uint8_t* _data);
      void sensorStateChanged(uint8_t _sensoryType, uint8_t _sensorId, bool _value);
      uint16_t calcDiff(uint64_t _stop, uint64_t _start);

      uint8_t   I2CAddress;
      uint16_t  prevButtonStatus;
      uint16_t  prevProximityStatus;
      uint64_t  touchStartTime[CY8_TOUCHSENSORCOUNT];
      uint64_t  touchEndTime[CY8_TOUCHSENSORCOUNT];
      uint8_t   touchCounter[CY8_TOUCHSENSORCOUNT];
      bool      positioningTouchEnabled[CY8_TOUCHSENSORCOUNT];
      bool      taskProcess[CY8_TOUCHSENSORCOUNT];
      bool      multipleTouchEnabled[CY8_TOUCHSENSORCOUNT];
      uint64_t  taskLastRunTimeStart;
      uint64_t  taskLastRunTimeStop;

      uint16_t  longTouchThreshold;
      uint16_t  touchThreshold;
      uint16_t  positioningTouchThreshold;
      uint8_t   taskTriggerGap;

    };

#endif