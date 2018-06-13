/*
  CY8TouchSwitch.h Library for controlling/handling a switch with several buttons
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

/* TODO:  * add temperature and humidity sensor
          * add TouchIC reboot (manually and automatic after )s ec beyond reset?)
          * auto reset proximity? after soem time
          * */

#ifndef CY8TouchSwitch_h
#define CY8TouchSwitch_h

    #include "CY8CMBR3116.h"
    #include "LEDWorker.h"
    #include "DebugUtil.h"


    #define TS_MODE_NORMAL    0
    #define TS_MODE_PROG      1
    #define TS_MODE_SETUP     2
    #define TS_MODE_STARTUP1  60
    #define TS_MODE_STARTUP2  61
    #define TS_MODE_STARTUP3  62
    #define TS_MODE_STARTUP4  63


    class CY8TouchSwitch
    {
      public:
        CY8TouchSwitch();
        ~CY8TouchSwitch();
        void setup();
        void task();
        void interrupt();
        void addButton(uint8_t _sensorId, uint8_t _ledPin, bool _enableMultipleTouch = false, bool _enablePositioningTouch = false);
        void changeMode(uint8_t _mode, bool _force = false);
        void resetTouchController();

        void setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)>);
        void setProximityEventCallback(std::function<void(uint8_t, uint8_t)>);
        void setGestureEventCallback(std::function<void(uint8_t)>);
      private:
        CY8CMBR3116 *touchController;
        LEDWorker   *ledWorkers[10];
        uint8_t     sendorIds[10];
        uint8_t     nextButtonIdx;
        uint8_t     mode;

        bool isSensorIdActive(uint8_t _sensorId);

        void sensorStateEvent(uint8_t _sensorType, uint8_t _sensorId, bool _value);
        void touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count);
        void proximityEvent(uint8_t _sensorId, uint8_t _event);
        void gestureEvent(uint8_t _event);

        void setMode_Normal();
        void setMode_Prog();
        void setMode_Setup();
        void setMode_Startup(uint8_t _startupLevel);

        // we do forward the evens from the touch controller
        // for now there is no need to do any bug change, the only thing which will be done
        // is filtereing the events for the given button id's
        std::function<void(uint8_t, uint8_t, uint8_t)> touchEventCallback;
        std::function<void(uint8_t, uint8_t)> proximityEventCallback;
        std::function<void(uint8_t)> gestureEventCallback;
    };

#endif