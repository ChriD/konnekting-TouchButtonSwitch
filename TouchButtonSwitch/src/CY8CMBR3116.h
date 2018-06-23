/*
  CY8MBR3116.h Library for controlling/handling the Cypress CY8MBR3116 touch controller
  Created by Christian Dürnberger, June 2018
  Released into the public domain.
  Register description:     http://www.cypress.com/file/127221/download
  Datasheet:                http://www.cypress.com/file/46236/download
  Device programming specs: http://www.cypress.com/file/133376/download
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

  // register offset mao of the CY8CMBR3116
  #define CY8_REGMAP_ORIGIN             0x00
  #define CY8_BUTTON_STATUS             0xAA
  #define CY8_LATCHED_BUTTON_STATUS     0xAC
  #define CY8_PROX_STAT                 0xAE
  #define CY8_CTRL_CMD                  0x86
  #define CY8_CTRL_CMD_ERR              0x89

  // sensitivity
  #define CY8_SENSITIVITY0              0x08
  #define CY8_SENSITIVITY1              0x09
  #define CY8_SENSITIVITY2              0x0A
  #define CY8_SENSITIVITY3              0x0B

  // thresholds
  #define CY8_BASE_THRESHOLD0           0x0C
  #define CY8_BASE_THRESHOLD1           0x0D
  #define CY8_FINGER_THRESHOLD2         0x0E
  #define CY8_FINGER_THRESHOLD3         0x0F
  #define CY8_FINGER_THRESHOLD4         0x10
  #define CY8_FINGER_THRESHOLD5         0x11
  #define CY8_FINGER_THRESHOLD6         0x12
  #define CY8_FINGER_THRESHOLD7         0x13
  #define CY8_FINGER_THRESHOLD8         0x14
  #define CY8_FINGER_THRESHOLD9         0x15
  #define CY8_FINGER_THRESHOLD10        0x16
  #define CY8_FINGER_THRESHOLD11        0x17
  #define CY8_FINGER_THRESHOLD12        0x18
  #define CY8_FINGER_THRESHOLD13        0x19
  #define CY8_FINGER_THRESHOLD14        0x1A
  #define CY8_FINGER_THRESHOLD15        0x1B

  // command codes for the CY8CMBR3116
  #define CY8_CMD_NULL                  0x00
  #define CY8_SAVE_CHECK_CRC            0x02
  #define CY8_CALC_CRC                  0x03
  #define CY8_LOAD_FACTORY              0x04
  #define CY8_LOAD_PRIMARY              0x05
  #define CY8_LOAD_SECONDARY            0x06
  #define CY8_SLEEP                     0x07
  #define CY8_CLEAR_LATCHED_STATUS      0x08
  #define CY8_CMD_RESET_PROX0_FILTER    0x09
  #define CY8_CMD_RESET_PROX1_FILTER    0x0A
  #define CY8_ENTER_CONFIG_MODE         0x0B
  #define CY8_EXIT_CONTROL_RUN          0xFE
  #define CY8_SW_RESET                  0xFF


  /*
    TODO: other codes!!!
  */

  class CY8CMBR3116
  {
    public:
      CY8CMBR3116(uint8_t _I2CAddress);
      ~CY8CMBR3116();
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

    };

#endif