/*
  CY8TouchSwitch.h Library for controlling/handling a switch with several buttons
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

/* TODO:  * add temperature and humidity sensor
          * */

#ifndef CY8TouchSwitch_h
#define CY8TouchSwitch_h

    #include "CY8CMBR3116.h"
    #include "LEDWorker.h"
    #include "DebugUtil.h"

    class CY8TouchSwitch
    {
      public:
        CY8TouchSwitch();
        ~CY8TouchSwitch();
        void setup();
        void loop();
        void interrupt();
      private:
        CY8CMBR3116 *touchController;
        LEDWorker   *ledWorker1;

        void sensorStateEvent(uint8_t sensorType, uint8_t _sensorId, bool _value);
        void touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count);
        void proximityEvent(uint8_t _sensorId, uint8_t _event);
        void gestureEvent(uint8_t _event);
    };

#endif