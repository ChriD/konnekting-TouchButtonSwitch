/*
  Button.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "Button.h"


Button::Button()
{
  this->lastButtonState = 0;
  this->lastDebounceTime = 0;
  this->lastTaskRunTime = 0;

  this->statePollingEnabled = true;

  this->pressStartTime = 0;
  this->pressEndTime = 0;
  this->confirmTapThreshold = 250;
  this->confirmLongPressThreshold = 1250;
  this->tapCounter = 0;
  this->runConfirmButtonAction = false;

  this->positioningModeEnabled = false;
  this->multipleStateEnabled = false;
}


Button::~Button()
{
}

uint16_t Button::getPeriod(uint64_t _lastCallTime, bool _useMicros)
{
  uint64_t cur;

  if(_useMicros)
    cur = micros();
  else
    cur = millis();
  // if we had an overflow we have to calculate the period in a special way
  // in fact we have to add "(max)uint16_t - LastCallTime"  to  "cur"
  // but ofr our purpose its okay to only return "cur".
  if(cur < _lastCallTime)
  {
    return (uint16_t)(cur + (UINT64_MAX - _lastCallTime));
  }
  else
  {
    return (uint16_t)(cur - _lastCallTime);
  }
}


// uint8_t _sensorId, uint8_t _event, uint8_t _count
void Button::setCallbackOnButton(CALLBACK_ONBUTTON)
{
  this->callback_onButton = callback_onButton;
}

void Button::setCallbackOnButtonStateChanged(CALLBACK_ONBUTTONSTATECHANGED)
{
  this->callback_onButtonStateChanged = callback_onButtonStateChanged;
}



int8_t Button::calcButtonState()
{
  return 0;
}

boolean Button::setup()
{
  return true;
}


// Be aware, this method does need a stable state! (debounced state)
void Button::buttonStateChanged(uint8_t _state)
{
  this->curButtonState = _state;

  // call callback method if registered
  if(this->callback_onButtonStateChanged)
    this->callback_onButtonStateChanged(_state);

  if(_state == 1)
  {
    uint64_t lastStateEnd = this->pressEndTime;
    uint64_t stateStart   = millis();

    // when we did have a touch on the button before and we do touch again it may be a doublecklick or tripple....
    // to be aware of this we do add the touch counter, otherwise set the counter to 1
    if(lastStateEnd > 0 && (stateStart - lastStateEnd <= this->confirmTapThreshold))
    {
      this->tapCounter += 1;
      this->pressEndTime = 0;
    }
    else
      this->tapCounter = 1;

    // store the new start time of the satte change
    this->pressStartTime = stateStart;
    // if a button state has changed we have to process the button state in the task
    this->runConfirmButtonAction = true;
  }
  else
  {
    // if we are releasing a positioning state we have to send the positioning stop event immediately
    // a positioning state is identified by having the touchCounter set to 255 in the loop
    if(this->tapCounter == 255)
    {
      if(this->callback_onButton)
        this->callback_onButton(21, 1);
      this->tapCounter = 0;
    }


    // store the end time of the state, but only if it was no long state before
    if(this->tapCounter > 0)
    {
      uint64_t stateEnd = millis();
      this->pressEndTime = stateEnd;
      // if a button state has changed we have to process the button state in the task
      this->runConfirmButtonAction = true;
    }
  }
}


void Button::confirmButtonAction()
{
  // if there is nothing to to, we skip the whole process of approving an event
  if(!this->runConfirmButtonAction)
    return;

  // if the button is currently touched and it's touched beyond the "positioning" touch limit, then its a positioning touch
  // in this case we have to send the positioning start event and disable the long touch event ( both are not valid! )
  // we also remove the button from the process list because we will handle it on touch end event in this case
  if(this->positioningModeEnabled && this->getPeriod(this->pressStartTime) >= this->confirmLongPressThreshold)
  {
    // call callback method if registered
    if(this->callback_onButton)
      this->callback_onButton(20, 1);

    this->pressStartTime = 0;
    this->pressEndTime = 0;
    this->tapCounter = 255;
    this->runConfirmButtonAction = false;
  }

  // if the button is currently touched and it's touched beyond the long touch limit, then its a long touch
  // is this the case we have to remove the button from the process task and do reset the touch counter
  if(this->runConfirmButtonAction && !this->positioningModeEnabled && this->getPeriod(this->pressStartTime) >= this->confirmLongPressThreshold)
  {
    // call callback method if registered
    if(this->callback_onButton)
      this->callback_onButton(2, 1);

    this->pressStartTime = 0;
    this->pressEndTime = 0;
    this->tapCounter = 0;
    this->runConfirmButtonAction  = false;
  }

  // if there is a touch end time for a sensor we may check if its a normal click (touchThreshold)
  // we have a special or clause that will be used if multiple clicks are disabled, so we do not ave to wait for the whole touchThreshold to be exceeded!
  if(this->runConfirmButtonAction && this->pressEndTime > 0 && (this->getPeriod(this->pressEndTime) >= this->confirmTapThreshold || !this->multipleStateEnabled ))
  {
    // call callback method if registered
    if(this->callback_onButton)
      this->callback_onButton(1, this->tapCounter);

    this->pressStartTime = 0;
    this->pressEndTime = 0;
    this->tapCounter = 0;
    this->runConfirmButtonAction  = false;
  }

}


//
void Button::task()
{
  // we do not need to check the button state every cycle of the main task loop
  // it is okay to do it every bunch of ms. (10 to 50 seem appropriate)
  if(this->getPeriod(this->lastTaskRunTime) >= TASK_RUNPERIOD)
  {
    uint8_t buttonState = this->calcButtonState();
    this->lastTaskRunTime = millis();

    // only poll the state of the button if its enbaled
    // if polling is diabled the state has to be provided from an external source. For this the '@@@' has to be used
    if(this->statePollingEnabled)
    {
      // we do a very simple but for our case effective software debounce, this will be okay for normal buttons and touch as well
      // if software debouncing is not neede you may use the 'buttonStateChanged' method on its own
      if(buttonState != this->lastButtonState)
        this->lastDebounceTime = millis();
      if(this->getPeriod(this->lastDebounceTime) > DEBOUNCE_PERIOD)
      {
        if (buttonState != this->curButtonState)
        {
          // if we do use this library with an interrupt we do not need to have the 'task' method callback_onButtonStateChanged
          // instead we onlyhave to call the 'buttonStateChanged' method within the interrupt
          this->buttonStateChanged(buttonState);
        }
      }
      this->lastButtonState = buttonState;
    }

    //
    this->confirmButtonAction();
  }
}