/*
  TouchButton.cpp - Library for Touch(or any other) button handling
  Created by Christian D�rnberger, April 2018
  Released into the public domain.
  Wrapped Code from 'salsaman' http://forum.arduino.cc/index.php?topic=14479.0 into a class and did some re-factoring
*/

#include "Arduino.h"
#include "TouchButton.h"

TouchButton::TouchButton(unsigned int _pin,  unsigned int _id, bool _buttonTouchState, unsigned int _debouncePeriod, unsigned int _doubleClickPeriod, unsigned int _longHoldPeriod)
{
  this->buttonPin         = _pin;
  this->buttonId          = _id;
  this->buttonTouchState  = _buttonTouchState;
  this->buttonValue       = !this->buttonTouchState;
  this->buttonValuePrev   = this->buttonValue;
  this->debouncePeriod    = _debouncePeriod;
  this->doubleClickPeriod = _doubleClickPeriod;
  this->longHoldPeriod    = _longHoldPeriod;
  this->waitingForDoubleClick   = false;
  this->buttonPressedTime       = -1;
  this->buttonReleasedTime      = -1;
  this->init();
}


TouchButton::~TouchButton()
{

}


void TouchButton::init()
{
  pinMode(this->buttonPin, INPUT);
  digitalWrite(this->buttonPin, !this->buttonTouchState);
}


unsigned int TouchButton::getButtonId()
{
  return this->buttonId;
}


unsigned int TouchButton::touchButtonLoop()
{
   unsigned int buttonEvent = 0;

   // get the current vakue of the digital input and check wether the button is pressed or not
   // we do have an option to set wether HIGH or LOW ist the "touched, clicked" state
   this->buttonValue = digitalRead(this->buttonPin);

   // Button pressed down
   if (this->buttonValue == this->buttonTouchState && this->buttonValuePrev == !this->buttonTouchState && (millis() - this->buttonReleasedTime) > this->debouncePeriod)
   {
      this->buttonPressedTime = millis();
      this->ignoreRelease = false;
      // if the button is pressed we do asume it will be a single click, we may change this decision on release of the button
      // because there we do have th info of the timing. Now we cant say if its realy a single, double click or a long press
      this->isSingleClick = true;

      // now check if this button press may be a second click within the double click period, then its likely to be a doubleclick
      if ((millis()-this->buttonReleasedTime) < this->doubleClickPeriod && this->isDoubleClick == false && this->waitingForDoubleClick == true)
        this->isDoubleClick = true;
      else
        this->isDoubleClick = false;

      // on a new click we have to reset some variables, like the long hold triggered marker option
      // the marker that we are waiting for a second click (doubleclick)
      this->longHoldAlreadyTriggered  = false;
      this->waitingForDoubleClick     = false;
   }

   // Button released
   else if (this->buttonValue == !this->buttonTouchState && this->buttonValuePrev == this->buttonTouchState && (millis() - this->buttonPressedTime) > this->debouncePeriod)
   {
      if (!this->ignoreRelease)
      {
        this->buttonReleasedTime = millis();
        if (this->isDoubleClick == false)
          this->waitingForDoubleClick = true;
        else
        {
            buttonEvent = 2;
            // when we know we do have a doublecclick, then reset some vars
            this->isDoubleClick = false;
            this->waitingForDoubleClick = false;
            this->isSingleClick = false;
        }
      }
   }

   // Test for normal click event: this happens when the double glick gap period is expired and the button was released
   if (this->buttonValue == !this->buttonTouchState && (millis()-this->buttonReleasedTime) >= this->doubleClickPeriod && this->waitingForDoubleClick == true && this->isDoubleClick == false && this->isSingleClick == true && buttonEvent != 2)
   {
       buttonEvent = 1;
       this->waitingForDoubleClick = false;
   }

   // Test for hold but only trigger if we did not trigger it already
   if (this->buttonValue == this->buttonTouchState && (millis() - this->buttonPressedTime) >= this->longHoldPeriod && !this->longHoldAlreadyTriggered)
   {
      buttonEvent = 3;
      // be sure we do ignore the release of the button because we triggered a "long" hold
      // and long holds not not have to trigger the "released" event!
      this->ignoreRelease = true;
      // alsow be sure that we do not wait for a doubleclick anymore
      this->waitingForDoubleClick = false;
      // and it is no doubleclick
      this->isDoubleClick =  false;

      this->longHoldAlreadyTriggered = true;
   }

  // store old value of the button for the next loop
  this->buttonValuePrev = this->buttonValue;

   return buttonEvent;
}


