#ifndef __VIDEODATA_H
#define __VIDEODATA_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_dac.h"
#include "stm32f429i_discovery.h"

extern const uint16_t Sine12bit[367];
extern const uint16_t VideoXaxis12bit[1088];
extern const uint16_t Cosine12bit[367];
extern const uint16_t VideoYaxis12bit[1088];
extern const uint8_t Escalator8bit[6];
extern const uint8_t AllVideoData[85][20000];

#endif

