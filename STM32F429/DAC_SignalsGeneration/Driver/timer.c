#include "timer.h"
#include "dac.h"
#include "videodata.h"

uint16_t FrameDataCount;
uint16_t FrameCount1=0;
uint16_t FrameCount2=0;
uint16_t FrameDataTemp;
float FrameDataFloatTemp;
uint16_t FrameDMACount;
uint16_t FrameDMAOKFlag=0;
uint16_t FrameDataLineFinFlag=0;
uint16_t FrameDataLine;
uint16_t FrameDataMatrixCount=0;


//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timerx_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
			
			FrameDataMatrixCount=0;
			FrameDataLineFinFlag=1;
			for(FrameDataCount=0;FrameDataCount<3000;FrameDataCount++)
			{
				FrameDataFloatTemp=AllVideoData[FrameCount1][FrameCount2];
				if(FrameDataFloatTemp==255)
					FrameDataTemp=4095;
				else if((FrameDataFloatTemp>96)&&(FrameDataLineFinFlag==0))
				{
					FrameDataTemp=FrameDataFloatTemp-96;
					FrameDataFloatTemp-=96;
					FrameDataTemp=(uint16_t)(FrameDataFloatTemp*32/4096*4095);
					
					if(!OutputMatrixPPFlag)
					{
						OutputMatrixYaxis1[FrameDataMatrixCount]=FrameDataLine;
						OutputMatrixXaxis1[FrameDataMatrixCount]=FrameDataTemp;
					}
					else
					{
						OutputMatrixYaxis2[FrameDataMatrixCount]=FrameDataLine;
						OutputMatrixXaxis2[FrameDataMatrixCount]=FrameDataTemp;
					}
					FrameDataMatrixCount++;
					
					FrameDataLineFinFlag=1;
				}
				else if((FrameDataFloatTemp<=96)&&(FrameDataLineFinFlag==0))
				{
					FrameDataTemp=(uint16_t)(FrameDataFloatTemp*32/4096*4095);
					if(!OutputMatrixPPFlag)
					{
						OutputMatrixYaxis1[FrameDataMatrixCount]=FrameDataLine;
						OutputMatrixXaxis1[FrameDataMatrixCount]=FrameDataTemp;
					}
					else
					{
						OutputMatrixYaxis2[FrameDataMatrixCount]=FrameDataLine;
						OutputMatrixXaxis2[FrameDataMatrixCount]=FrameDataTemp;
					}
					FrameDataMatrixCount++;
				}
				else if(FrameDataLineFinFlag==1)
				{
					FrameDataLineFinFlag=0;
					FrameDataTemp=(uint16_t)(FrameDataFloatTemp*32/4096*4095);
					FrameDataLine=FrameDataTemp;
					
				}
				else if(FrameDataFloatTemp==0)
					FrameDataTemp=0;
				
				FrameCount2++;
				if(FrameCount2>=20000)
				{
					FrameCount2=0;
					FrameCount1++;
				}
				
				if(FrameDataTemp==4095)
					break;
				else if(FrameDataTemp==0)
				{
					FrameCount2=0;
					FrameCount1=0;
					break;
				}
				/*else 
				{
					if(!OutputMatrixPPFlag)
					{
							if(FrameDataLineFinFlag)
							{
								
								OutputMatrixYaxis1[FrameDataMatrixCount]=FrameDataLine;
							}
							else
							{
								OutputMatrixYaxis1[FrameDataMatrixCount]=FrameDataLine;
								OutputMatrixXaxis1[FrameDataMatrixCount]=FrameDataTemp;
							
								FrameDataMatrixCount++;
							}
					}
					else 
					{

							OutputMatrixYaxis2[FrameDataMatrixCount]=FrameDataLine;

							OutputMatrixXaxis2[FrameDataMatrixCount]=FrameDataTemp;
						
							FrameDataMatrixCount++;
					}
				
				}*/
			}
			
			if(FrameDataMatrixCount>=1)
				FrameDataMatrixCount--;
			
			FrameDMACount=FrameDataMatrixCount;
			
			FrameDMAOKFlag=1;
			
			STM_EVAL_LEDToggle(LED4);
					
		}
}

/**
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on CPU @180MHz and APB1 @45MHz
  * @note   TIM6 Update event occurs each TIM6CLK(MHz)/256    
  * @param  None
  * @retval None
  */
void TIM6_Config(void)
{
  /* TIM6CLK = HCLK / 4 = SystemCoreClock /4 */
  
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0xFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}
