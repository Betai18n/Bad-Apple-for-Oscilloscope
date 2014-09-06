#ifndef __DAC_H
#define __DAC_H

#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "stm32f429i_discovery.h"


#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R1_ADDRESS     0x40007410
#define DAC_DHR12R1_ADDRESS		 0x40007408


void DAC_GPIOInit(void);

void DAC_Ch1_VideoXaxisConfig(uint32_t OutputData,uint16_t Frame);
void DAC_Ch2_VideoYaxisConfig(uint32_t OutputData,uint16_t Frame);


void DAC_Ch1_EscalatorConfig(void);
void DAC_Ch1_CosineConfig(void);
void DAC_Ch2_SineWaveConfig(void);

void DAC_Ch1_NoiseConfig(void);
void DAC_Ch2_TriangleConfig(void);

extern uint16_t OutputMatrixPPFlag;

extern uint16_t OutputMatrix[1500];
extern uint16_t OutputMatrixXaxis1[2000];
extern uint16_t OutputMatrixYaxis1[2000];
extern uint16_t OutputMatrixXaxis2[2000];
extern uint16_t OutputMatrixYaxis2[2000];

#endif
