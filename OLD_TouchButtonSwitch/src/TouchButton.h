/*
  TouchButton.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchButton_h
#define TouchButton_h

  #include "Arduino.h"
  #include "DebugUtil.h"


  class TouchButton
  {
    public:
      TouchButton(uint8_t _pin);
      ~TouchButton();

      void setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)>);
      void setProximityEventCallback(std::function<void(uint8_t, uint8_t)>);
      void setGestureEventCallback(std::function<void(uint8_t)>);


    private:
      // callback events for the touch button
      std::function<void(uint8_t, uint8_t, uint8_t)> touchEventCallback;
      std::function<void(uint8_t, uint8_t)> proximityEventCallback;
      std::function<void(uint8_t)> gestureEventCallback;

      // the pin where the touch button is beeing set up
      uint8_t pin;

      /*
      bool setup(uint8_t _setupConfig = 0);
      void interrupt();
      void process();
      void task();

      void setSensorStateCallback(std::function<void(uint8_t, uint8_t, bool)>);
      void setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)>);
      void setProximityEventCallback(std::function<void(uint8_t, uint8_t)>);
      void setGestureEventCallback(std::function<void(uint8_t)>);

      void setSensorSensitivity(uint8_t _sensitivity);
      void setSensorFingerThreshold(uint8_t _threshold);

      void setActive(bool _active = true);
      void setThresholds(uint16_t _touchThreshold, uint16_t _longTouchThreshold, uint16_t _positioningTouchThreshold);
      void enableMultipleTouch(uint8_t _sensorId, bool _enable = true);
      void enablePositioningTouch(uint8_t _sensorId, bool _enable = true);
      void reset();
      void resetCacheData();
    private:
      std::function<void(uint8_t, uint8_t, bool)> sensorStateChangedCallback;
      std::function<void(uint8_t, uint8_t, uint8_t)> touchEventCallback;
      std::function<void(uint8_t, uint8_t)> proximityEventCallback;
      std::function<void(uint8_t)> gestureEventCallback;

      uint16_t readData(uint8_t _register, uint8_t _length, uint8_t* _data ,uint16_t _waitTimeAfterWrite = 0);
      void sensorStateChanged(uint8_t _sensoryType, uint8_t _sensorId, bool _value);
      uint16_t calcDiff(uint64_t _stop, uint64_t _start);
      bool uploadConfiguration(uint8_t _setupConfig = 0);

      bool      active;
      uint8_t   I2CAddress;
      uint16_t  prevButtonStatus;
      uint16_t  prevProximityStatus;
      uint64_t  touchStartTime[CY8_TOUCHSENSORCOUNT];
      uint64_t  touchEndTime[CY8_TOUCHSENSORCOUNT];
      uint8_t   touchCounter[CY8_TOUCHSENSORCOUNT];
      bool      taskProcess[CY8_TOUCHSENSORCOUNT];
      bool      positioningTouchEnabled[CY8_TOUCHSENSORCOUNT];
      bool      multipleTouchEnabled[CY8_TOUCHSENSORCOUNT];
      uint64_t  taskLastRunTimeStart;
      uint64_t  taskLastRunTimeStop;

      uint16_t  longTouchThreshold;
      uint16_t  touchThreshold;
      uint16_t  positioningTouchThreshold;
      uint8_t   taskTriggerGap;
      */

    };

#endif