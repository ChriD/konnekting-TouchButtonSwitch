/*
  BaseSwitch.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef BaseSwitch_h
#define BaseSwitch_h

  #include "Arduino.h"
  #include "Button.h"

  #define SWITCH_MAX_BUTTONCOUNT       10

  // a switch does have some standrt modes
  // TODO: @@@
  enum class SWITCH_MODE { NORMAL = 0, PROG = 10, SETUP = 20, CALIBRATION = 30, UNDEFINED = 99};


  struct BaseSwitchButtonParmsStruct
  {
    uint8_t mode;
    uint8_t longTouchMode;
    boolean allowMultiTouch;
  };
  typedef struct BaseSwitchButtonParmsStruct BaseSwitchButtonParms;



  // some structures and typedefs for the evironmentla sensors a button may have
  struct BaseSwitchEnvSensorStruct
  {
    boolean   temperature       = false;
    float     temperatureAdj    = 0.0;
    uint64_t  temperaturePeriod = 1000*60*3;
    boolean   humidity          = false;
    float     humidityAdj       = 0;
    uint64_t  humidityPeriod    = 1000*60*3;
    boolean   pressure          = false;
  };
  typedef struct BaseSwitchEnvSensorStruct BaseSwitchEnvSensors;

  struct BaseSwitchEnvDataStruct
  {
    float temperature = 0;
    float humidity    = 0;
    float pressure    = 0;
  };
  typedef struct BaseSwitchEnvDataStruct BaseSwitchEnvData;



  typedef Functor4<SWITCH_MODE, uint16_t, SWITCH_MODE, uint16_t> CallbackFunction_ModeChange;
  typedef Functor1<BaseSwitchEnvData> CallbackFunction_EnvDataUpdated;


  class BaseSwitch
  {
    public:
      BaseSwitch();
      ~BaseSwitch();

      virtual boolean setup();
      virtual void task();
      virtual void startCalibration();
      virtual Button* getButtonById(uint16_t);
      virtual Button* getButtonByIndex(uint16_t);
      virtual void setMode(SWITCH_MODE, uint16_t _modeLevel = 0);
      virtual void setButtonParameters(uint16_t, BaseSwitchButtonParms _parameters);
      virtual void requestEnvironmentData();

      void attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &);
      void attachCallbackOnProximityAlert(const CallbackFunction_ProximityAlert &);
      void attachCallbackOnModeChange(const CallbackFunction_ModeChange &);
      void attachCallbackOnEnvDataUpdated(const CallbackFunction_EnvDataUpdated &);

      BaseSwitchEnvSensors parmEnvSensorsSettings();
      void parmEnvSensorsSettings(BaseSwitchEnvSensors);

      boolean parmSpeakerEnabled();
      void parmSpeakerEnabled(boolean);

    protected:
      Button      *buttons[SWITCH_MAX_BUTTONCOUNT];

      // this one indicates the current mode of the switch
      // a switch may be in normal, programming, setup mode aso...
      SWITCH_MODE   mode;
      uint16_t      modeLevel;
      uint64_t      lastTaskRunTime ;

      // holds the last data read by the environmental sensors
      BaseSwitchEnvData     curEnvData;

      boolean               speakerEnabled;
      BaseSwitchEnvSensors  envSensorsSettings;

      CallbackFunction_ButtonAction       callback_onButtonAction;
      CallbackFunction_ProximityAlert     callback_onProximityAlert;
      CallbackFunction_ModeChange         callback_onModeChange;
      CallbackFunction_EnvDataUpdated     callback_onEnvDataUpdated;

      int8_t      maxButtonIdx;


      virtual void initButtons();
      virtual boolean setupButtons();
      virtual uint16_t addButton(Button*, uint16_t _buttonId = 0);

      virtual void onButtonStateChanged(uint16_t, uint16_t);
      virtual void onButtonAction(uint16_t, uint16_t, uint16_t);
      virtual void onProximityAlert(uint16_t, boolean, uint16_t, uint16_t);

      virtual uint64_t getPeriod(uint64_t _lastCallTime, bool _useMicros = false);

    private:

  };

#endif