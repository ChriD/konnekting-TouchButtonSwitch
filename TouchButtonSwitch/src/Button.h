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

  #define DEBOUNCE_PERIOD 40
  #define TASK_RUNPERIOD 10

  #define CALLBACK_ONBUTTON void (*callback_onButton)(uint8_t, uint8_t)
  #define CALLBACK_ONBUTTONSTATECHANGED void (*callback_onButtonStateChanged)(uint8_t)
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



      boolean positioningModeEnabled;
      boolean multipleStateEnabled;

    protected:
      // callback event for the touch button events (tap, double-tap, multiple-tap, long-press, positioning-down, positioning-up)
      CALLBACK_ONBUTTON;
      // calback event which will be called whenever the internal state of the button was changed
      CALLBACK_ONBUTTONSTATECHANGED;

      // time when the 'external' state of the button has changed, this time is needed for software debouncing
      // and is in interaction to the DEBOUNCE_PERIOD macro
      uint64_t lastDebounceTime;
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

      boolean statePollingEnabled;

      virtual void buttonStateChanged(uint8_t _state);
      virtual int8_t calcButtonState();
      virtual uint16_t getPeriod(uint64_t _lastCallTime, bool _useMicros = false);

      virtual void confirmButtonAction();

    };

#endif