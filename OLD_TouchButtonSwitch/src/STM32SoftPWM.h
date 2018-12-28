/*
  STM32SoftPWM.h
  Created by Christian Dürnberger, July 2018
  Released into the public domain.


  https://visualgdb.com/tutorials/arm/stm32/timers/hal/

  https://www.mikrocontroller.net/topic/396957
  https://www.mikrocontroller.net/topic/396957


  https://www.mikrocontroller.net/topic/371504

*/

#ifndef STM32SoftPWM_h
#define STM32SoftPWM_h

    #include "Arduino.h"
    #include "DebugUtil.h"

    //we do need the HAL libraries for setting up the soft pwm timers
    //#include "stm32/HAL/stm32yyxx_ll.h"
    #include "stm32_def.h"
    //#include "stm32/timer.h"
    //#include "stm32l4xx.h"

    class STM32SoftPWM
    {
      public:
        STM32SoftPWM();
        ~STM32SoftPWM();
        void setup();
      protected:
        //TIM_TypeDef *timer;
        //NVIC_InitTypeDef NVIC_InitStructure;
      private:
    };

#endif