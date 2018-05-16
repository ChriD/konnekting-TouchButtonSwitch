/*
  TouchButton.h - Library for Touch(or any other) button handling
  Created by Christian D�rnberger, April 2018
  Released into the public domain.
  Wrapped Code from 'salsaman' http://forum.arduino.cc/index.php?topic=14479.0 into a class and did some re-factoring
*/

#ifndef TouchButton_h
#define TouchButton_h

    #include "Arduino.h"

    class TouchButton
    {
      public:
        TouchButton(unsigned int _pin, unsigned int _id, bool _buttonTouchState = HIGH, unsigned int _debouncePeriod = 20, unsigned int _doubleClickPeriod = 250, unsigned int _longHoldPeriod = 1000);
        ~TouchButton();
        void init();
        unsigned int touchButtonLoop();
        unsigned int getButtonId();
      private:
        unsigned int buttonPin;
        unsigned int buttonId;
        bool buttonTouchState;
        bool buttonValue;
        bool buttonValuePrev;

        // debounce period to prevent flickering when pressing or releasing the button in ms
        unsigned int debouncePeriod;
        // max time between clicks for a double click event in ms
        // be aware if you raise this var the "click" event will come always after this amount of time after a keypress/touch
        unsigned int doubleClickPeriod;
        // time gap for identifiying "long" clicks (pess+hold)
        unsigned int longHoldPeriod;

        // some internals
        bool waitingForDoubleClick;
        bool ignoreRelease;
        bool longHoldAlreadyTriggered;
        bool isSingleClick;
        bool isDoubleClick;
        long buttonPressedTime;
        long buttonReleasedTime;
    };


#endif