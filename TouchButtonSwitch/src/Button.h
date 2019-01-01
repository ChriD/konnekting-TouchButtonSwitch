/*
  Button.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef Button_h
#define Button_h

  #include "Arduino.h"
  #include "functor/functor.h"

  #define BTN_STD_DEBOUNCE_PERIOD               40
  #define BTN_STD_CONFIRM_TAP_THRESHOLD         250
  #define BTN_STD_CONFIRM_LONGPRESS_THRESHOLD   1250
  #define BTN_STD_TASK_RUNPERIOD                10

  typedef Functor3<uint16_t, uint16_t, uint16_t> CallbackFunction_ButtonAction;
  typedef Functor2<uint16_t, uint16_t> CallbackFunction_ButtonStateChanged;
  typedef Functor4<uint16_t, boolean, uint16_t, uint16_t> CallbackFunction_ProximityAlert;

  //enum class BUTTON_TYPE { MECHANIC = 0, TOUCH = 10};

  class Button
  {
    public:
      Button();
      ~Button();
      virtual boolean setup();
      // task method has to be called in the application loop (no matter if interrupt task is used or not!)
      virtual void task();
      // the interruptTask method can be called when statePollingEnabled is set to false to provide the state
      // from an external source like a touch controller or similar
      virtual void interruptTask(uint8_t);
      // a button, no matter of which type, may have the ability to calibrate
      virtual void startCalibration();

      void attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &);
      void attachCallbackOnButtonStateChanged(const CallbackFunction_ButtonStateChanged &);
      void attachCallbackOnProximityAlert(const CallbackFunction_ProximityAlert &);

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
      // allowProximityLevels parm methods
      boolean parmAllowProximityLevels();
      void parmAllowProximityLevels(boolean);

    protected:

      // callback event for the touch button events (tap, double-tap, multiple-tap, long-press, positioning-down, positioning-up)
      CallbackFunction_ButtonAction callback_onButtonAction;
      // calback event which will be called whenever the internal state of the button was changed
      CallbackFunction_ButtonStateChanged  callback_onButtonStateChanged;
      // callback event which will be called when there is proximity
      CallbackFunction_ProximityAlert callback_onProximityAlert;
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
      // the last button state represents the last external state of the button which was polled
      uint8_t lastButtonState;
      // the current button state represents the current internal state of the button (down or up)
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
      // the 'positioning mode' is a mode where the press and release events are fired in the 'onButton' event.
      // it can not co-exist with the long press event. Therfore the long press is deactivated when the position mode is enabled
      boolean positioningModeEnabled;
      // the button can handle multi taps (doubletap, ...) and here you can enable or disable this
      // the advantage of diabled multi tap is that the tap event itself can be recognized immediatelly without having to wait for
      // the confirm threshold
      boolean multipleTapsEnabled;
      // indicates if proximity recognize can output levels of proximity (distance)
      boolean allowProximityLevels;

      virtual uint16_t getPeriod(uint64_t _lastCallTime, bool _useMicros = false);
      virtual int8_t calcButtonState();
      virtual void buttonStateChanged(uint8_t _state);
      virtual void confirmButtonAction();

    };

#endif