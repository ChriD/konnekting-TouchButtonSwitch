/*
  XButtonSwitch.h
  Created by Christian D�rnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "XButtonSwitch.h"

XButtonSwitch::XButtonSwitch(void (*_buttonEventCallback)(unsigned int _buttonId, unsigned int _buttonEvent))
{
  this->buttonEventCallback   = _buttonEventCallback;
  this->buttonCount           = 0;
}


XButtonSwitch::~XButtonSwitch()
{

}


void XButtonSwitch::addButton(unsigned int _pin, unsigned int _buttonId)
{
  this->touchButton[this->buttonCount] = new TouchButton(_pin, _buttonId);
  this->buttonCount++;
}


void XButtonSwitch::xButtonSwitchLoop()
{
  unsigned int i;
  unsigned int buttonEvent;

  for(i=0; i<this->buttonCount; i++)
  {
    buttonEvent = this->touchButton[i]->touchButtonLoop();
    if(buttonEvent > 0)
    {
      this->buttonEventCallback(this->touchButton[i]->getButtonId(), buttonEvent);
    }
  }

  // INFO: We may add multitouch ability here, but i do not have need for this right now
  // so you may do this now if you like to
}


