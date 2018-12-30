/*
  TouchSwitch.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_h
#define TouchSwitch_h

  #include "Arduino.h"
  #include "BaseSwitch.h"
  #include "TouchButton.h"
  #include "ProximityTouchButton.h"

  class TouchSwitch : public BaseSwitch
  {
    public:
      TouchSwitch();
      ~TouchSwitch();

    protected:

    private:

  };

#endif