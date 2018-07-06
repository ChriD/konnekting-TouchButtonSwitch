/*
  STM32SoftPWM.h
  Created by Christian Dürnberger, July 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "STM32SoftPWM.h"


STM32SoftPWM::STM32SoftPWM()
{

}


STM32SoftPWM::~STM32SoftPWM()
{
}


void STM32SoftPWM::setup()
{
    HAL_Init();
/*

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   // clr any pending irqs
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    // lets enable update events
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0b;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // ready to run
  NVIC_Init(&NVIC_InitStructure);
  */
/*
  // configure interrupt on a timer which will be our PWM generator
  this->NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  this->NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  this->NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  this->NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  uint16_t TimerPeriod = 0;

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);

  TimerPeriod = (uint16_t) (SystemCoreClock / 1000000) - 1;
  TIM_DeInit(TIM5);
  TIM_TimeBaseStructure.TIM_Period=1000;
  TIM_TimeBaseStructure.TIM_Prescaler= 0;
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM5, ENABLE);
  */
}