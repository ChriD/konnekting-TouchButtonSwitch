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

      void attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &);
      void attachCallbackOnProximityAlert(const CallbackFunction_ProximityAlert &);


    protected:
      Button      *buttons[SWITCH_MAX_BUTTONCOUNT];

      CallbackFunction_ButtonAction   callback_onButtonAction;
      CallbackFunction_ProximityAlert callback_onProximityAlert;

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