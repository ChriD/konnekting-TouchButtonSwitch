/*
  ProximityTouchButton.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


/*
TODO: *
*/


#ifndef ProximityTouchButton_h
#define ProximityTouchButton_h

  #include "Arduino.h"
  #include "TouchButton.h"



  class ProximityTouchButton : public TouchButton
  {
    public:
      ProximityTouchButton(uint8_t _pin, uint16_t _id =0);
      ~ProximityTouchButton();

      void task();

    protected:

      boolean   isProximity;
      uint16_t  proximityLevel;

      boolean   isProximityCur;
      boolean   isProximityLast;
      boolean   isStableProximity;
      uint8_t   isStableProximityCount;

    };

#endif