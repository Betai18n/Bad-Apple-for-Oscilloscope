/**
  ******************************************************************************
  * @file    DAC_SignalsGeneration/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example provides a short description of how to use the DAC 
  *          peripheral to generate several signals using DMA controller.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "timer.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup DAC_SignalsGeneration
  * @{
  */

/** @addtogroup DAC_SignalsGeneration
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/




__IO uint8_t SelectedWavesForm = 0;
__IO uint8_t KeyPressed = SET; 

uint16_t uwPrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f429_439xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */    


	DAC_GPIOInit();

  /* TIM6 Configuration ------------------------------------------------------*/
  TIM6_Config();  
	
  uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	Timerx_Init(212-1,uwPrescalerValue);
  /* Configures User Button */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	
	STM_EVAL_LEDInit(LED3);
	
	STM_EVAL_LEDInit(LED4);
	
  
  while (1)
  {
    /* If the User Button is pressed */
    if (FrameDMAOKFlag)
    {            
      if(!OutputMatrixPPFlag)
			{
					if(FrameDataCount!=0)
					{
					
						DAC_DeInit(); 
						DAC_Ch1_VideoXaxisConfig((uint32_t)&OutputMatrixXaxis1,FrameDMACount);
						DAC_Ch2_VideoYaxisConfig((uint32_t)&OutputMatrixYaxis1,FrameDMACount);
						
					}
					else 
					{
						OutputMatrixXaxis1[0]=0;
						OutputMatrixYaxis1[0]=0;
						DAC_DeInit(); 
						DAC_Ch1_VideoXaxisConfig((uint32_t)&OutputMatrixXaxis1,1);
						DAC_Ch2_VideoYaxisConfig((uint32_t)&OutputMatrixYaxis1,1);
					}
			}
			else
			{
					if(FrameDataCount!=0)
					{
						DAC_DeInit(); 
						DAC_Ch1_VideoXaxisConfig((uint32_t)&OutputMatrixXaxis2,FrameDMACount);
						DAC_Ch2_VideoYaxisConfig((uint32_t)&OutputMatrixYaxis2,FrameDMACount);
						
					}
					else 
					{
						OutputMatrixXaxis2[0]=0;
						OutputMatrixYaxis2[0]=0;
						DAC_DeInit(); 
						DAC_Ch1_VideoXaxisConfig((uint32_t)&OutputMatrixXaxis2,1);
						DAC_Ch2_VideoYaxisConfig((uint32_t)&OutputMatrixYaxis2,1);
					}
			}
			if(OutputMatrixPPFlag)
				OutputMatrixPPFlag=0;
			else 
				OutputMatrixPPFlag=1;
			
			FrameDMAOKFlag=0;
    }
  }
}






#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
