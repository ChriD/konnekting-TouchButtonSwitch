/*
  TouchSwitch_4X_V1.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_4X_V1_h
#define TouchSwitch_4X_V1_h

  #include "Arduino.h"
  #include "TouchSwitch.h"

  #define TS_PIN_SENSOR_BUTTON_1      A1
  #define TS_PIN_SENSOR_BUTTON_2      A0
  #define TS_PIN_SENSOR_BUTTON_3      A5
  #define TS_PIN_SENSOR_BUTTON_4      A3
  #define TS_PIN_SENSOR_PROXIMITY_1   A2
  #define TS_PIN_SENSOR_PROXIMITY_2   A4

  class TouchSwitch_4X_V1 : public TouchSwitch
  {
    public:
      TouchSwitch_4X_V1();
      ~TouchSwitch_4X_V1();

      virtual void initButtons();

    protected:

    private:

  };

#endif