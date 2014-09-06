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


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			
			
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
