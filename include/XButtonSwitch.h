/*
  XButtonSwitch.h Library for controlling/handling a switch with several buttons
  Created by Christian D�rnberger, Mai 2018
  Released into the public domain.
*/

/* TODO:  * add temperature sensor
          * add beeper/buzzer?
          * add low current leds
          * add multitouch support
          * add settings for release time aso...
          * */

#ifndef XButtonSwitch_h
#define XButtonSwitch_h

    #define XBUTTONSWITCH_MAXBUTTONCOUNT  8

    #include "TouchButton.h"

    class XButtonSwitch
    {
      public:
        XButtonSwitch(void (*_buttonEventCallback)(unsigned int _buttonId, unsigned int _buttonEvent));
        ~XButtonSwitch();
        void addButton(unsigned int _pin, unsigned int _buttonId);
        void xButtonSwitchLoop();
      private:
        void (*buttonEventCallback)(unsigned int _buttonId, unsigned int _buttonEvent);
        unsigned int buttonCount;
        // allow a maximum of 8 buttons
        TouchButton *touchButton[(XBUTTONSWITCH_MAXBUTTONCOUNT-1)];
    };

#endif