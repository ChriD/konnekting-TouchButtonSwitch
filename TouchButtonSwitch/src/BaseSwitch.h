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
    boolean allowMultiTouch;
  };
  typedef struct BaseSwitchButtonParmsStruct BaseSwitchButtonParms;

  typedef Functor4<SWITCH_MODE, uint16_t, SWITCH_MODE, uint16_t> CallbackFunction_ModeChange;


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

      void attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &);
      void attachCallbackOnProximityAlert(const CallbackFunction_ProximityAlert &);
      void attachCallbackOnModeChange(const CallbackFunction_ModeChange &);

    protected:
      Button      *buttons[SWITCH_MAX_BUTTONCOUNT];

      // this one indicates the current mode of the switch
      // a switch may be in normal, programming, setup mode aso...
      SWITCH_MODE   mode;
      uint16_t      modeLevel;

      uint64_t      lastTaskRunTime ;

      CallbackFunction_ButtonAction     callback_onButtonAction;
      CallbackFunction_ProximityAlert   callback_onProximityAlert;
      CallbackFunction_ModeChange       callback_onModeChange;

      int8_t      maxButtonIdx;


      virtual void initButtons();
      virtual boolean setupButtons();
      virtual uint16_t addButton(Button*, uint16_t _buttonId = 0);

      void onButtonStateChanged(uint16_t, uint16_t);
      void onButtonAction(uint16_t, uint16_t, uint16_t);
      void onProximityAlert(uint16_t, boolean, uint16_t, uint16_t);

    private:

  };

#endif