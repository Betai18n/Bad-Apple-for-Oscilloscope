#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "stm32f429i_discovery.h"


void TIM6_Config(void);
void Timerx_Init(u16 arr,u16 psc);


extern uint16_t FrameDataCount;
extern uint16_t FrameCount1;
extern uint16_t FrameCount2;
extern uint16_t FrameDataTemp;
extern float FrameDataFloatTemp;
extern uint16_t FrameDMACount;
extern uint16_t FrameDMAOKFlag;

#endif
