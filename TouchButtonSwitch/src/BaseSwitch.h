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

      void attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &);


    protected:
      Button      *buttons[SWITCH_MAX_BUTTONCOUNT];

      CallbackFunction_ButtonAction callback_onButtonAction;

      int8_t      maxButtonIdx;


      virtual void initButtons();
      virtual boolean setupButtons();
      virtual Button* addButton(Button*, uint16_t _buttonId = 0);

      void onButtonAction(uint16_t, uint16_t, uint16_t);
      void onButtonStateChanged(uint16_t, uint16_t);

    private:

  };

#endif