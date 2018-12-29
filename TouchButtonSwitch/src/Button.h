/*
  Button.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

/*
TODO: * methods for settings
      * add settings to constructor
      * add and update docu
      * add method for interrupt
      * enable/disable polling
*/


#ifndef Button_h
#define Button_h

  #include "Arduino.h"

  #define BTN_STD_DEBOUNCE_PERIOD               40
  #define BTN_STD_CONFIRM_TAP_THRESHOLD         250
  #define BTN_STD_CONFIRM_LONGPRESS_THRESHOLD   1250
  #define BTN_STD_DEBOUNCE_PERIOD               40
  #define BTN_STD_TASK_RUNPERIOD                10

  #define CALLBACK_ONBUTTON void (*callback_onButton)(uint16_t, uint8_t, uint8_t)
  #define CALLBACK_ONBUTTONSTATECHANGED void (*callback_onButtonStateChanged)(uint16_t, uint8_t)
  // https://github.com/knolleary/pubsubclient/issues/115

  //
  //mqtt.set_callback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); }

  class Button
  {
    public:
      Button();
      ~Button();
      virtual boolean setup();
      virtual void task();

      void setCallbackOnButton(CALLBACK_ONBUTTON);
      void setCallbackOnButtonStateChanged(CALLBACK_ONBUTTONSTATECHANGED);

      // id parm methods
      uint16_t parmId();
      void parmId(uint16_t);
      // statePollingEnabled parm methods
      boolean parmStatePollingEnabled();
      void parmStatePollingEnabled(boolean);
      // positioningModeEnabled parm methods
      boolean parmPositioningModeEnabled();
      void parmPositioningModeEnabled(boolean);
      // multipleTapsEnabled parm methods
      boolean parmMultipleTapsEnabled();
      void parmMultipleTapsEnabled(boolean);
      // confirmTapThreshold parm methods
      uint16_t parmConfirmTapThreshold();
      void parmConfirmTapThreshold(uint16_t);
      // confirmLongPressThreshold parm methods
      uint16_t parmConfirmLongPressThreshold();
      void parmConfirmLongPressThreshold(uint16_t);
      // debouncePeriod parm methods
      uint16_t parmDebouncePeriod();
      void parmDebouncePeriod(uint16_t);

    protected:
      // callback event for the touch button events (tap, double-tap, multiple-tap, long-press, positioning-down, positioning-up)
      CALLBACK_ONBUTTON;
      // calback event which will be called whenever the internal state of the button was changed
      CALLBACK_ONBUTTONSTATECHANGED;

      // a identifier for the button. Its good if we have some buttons on a switch for example
      // this id should be provided by external code and will be present in the callbacks so we may use one
      // callback for all buttons
      uint16_t id;
      // time when the 'external' state of the button has changed, this time is needed for software debouncing
      // and is in interaction to the DEBOUNCE_PERIOD macro
      uint64_t lastDebounceTime;
      uint16_t debouncePeriod;
      // the time when the task method was last executed.
      // this time marker is needed to prevent keeping the button task method to fill up cpu usage
      // its in interaction with the TASK_RUNPERIOD macro
      uint64_t lastTaskRunTime;

      uint8_t lastButtonState;
      uint8_t curButtonState;

      // time markers when the button went last down and up
      uint64_t pressEndTime;
      uint64_t pressStartTime;
      // this threshold is the period of time which is needed to confirm the button hit as a button event (click, doubleclick,...)
      // this is needed when double or multiple taps are enabled and have to be recognized, otherwise a one
      // click can be recognized immediatelly without the need of a threshold
      uint16_t confirmTapThreshold;
      // this threshold is the one which indicates that ist a long press. It should be between 1 to 3 seconds for good user experience
      // if the button has the "positioning mode" active, it will use this threshold for the indication when the positioning mode is entered!
      // LongPress and Positioning touch can not be active at the same time
      uint16_t confirmLongPressThreshold;
      // a tap counter which counts the taps when multiple taps are enabled (double-hit, multiple-hit)
      uint16_t tapCounter;
      // this is a internal marker if we have to process the 'confirmButtonAction' method which will fire our onButton events due the
      // info gathered in the 'buttenStateChanged' method
      boolean runConfirmButtonAction;
      // if the lib is used with interrupts you do not need polling and therefore you can disable the internal state polling
      // with this boolean
      boolean statePollingEnabled;

      boolean positioningModeEnabled;
      boolean multipleTapsEnabled;


      virtual void buttonStateChanged(uint8_t _state);
      virtual int8_t calcButtonState();
      virtual uint16_t getPeriod(uint64_t _lastCallTime, bool _useMicros = false);

      virtual void confirmButtonAction();

    };

#endif