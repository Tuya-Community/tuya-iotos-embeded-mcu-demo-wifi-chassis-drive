#include "TIM.h"
#include "IO.h"



void ConfigureTIM16_CH1_AsPWM_EdgeAligned(void)//T16_CH1 PWM PD0 AF2 舵机
{
  RCC->APBENR2 |= 1<<17;
  RCC->IOPENR |= 1<<3;
	GPIOD->MODER &=~(3<<0);
  GPIOD->MODER |=  2<<0; //复用功能
	GPIOD->AFR[0]&=0XFFFFFFF0;
  GPIOD->AFR[0] |= 0x02 << 0;
  
  TIM16->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM16->ARR = 99;//10*（99+1）=1Ms 总周期
  TIM16->CCR1 = 1; // 高电平时间
  TIM16->CCMR1 |=6<<4|1<<3;//CH_1 110模式,预装载使能（默认为输出模式）
  TIM16->CCER |= 1<<0;  //CH_1开启输出PWM
  TIM16->CR1 |= 1<<0; //使能计数器(边沿对齐 递增)
  TIM16->EGR |= 1<<0; //更新中断
	TIM16->BDTR |=1<<14;//自动输出使能AOE
}
void TIM16_PWM_Stop(void)
{
	TIM16->CCER &=~(1<<0);//关闭输出PWM
	TIM16->CR1 &=~(1<<0);	//禁止计数器
	TIM16->CNT =0;//计数清零
}
void TIM16_PWM_Start(void)
{
	TIM16->CCER |=1<<0;//开启输出PWM
	TIM16->CR1 |=1<<0;//使能计数器
}
void TIM16_PWM_Set(uint16_t CCR)
{
	TIM16->CCR1 = CCR;
}

void TIM6_Init(uint16_t arr,uint16_t psc)//TASK分配
{
	RCC->APBENR1 |= 1<<4;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->DIER |= 1<<0;//使能更新中断
	TIM6->CR1 |= 1<<0;//使能计数器
	NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 1); 
	NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
}
void TIM6_Start(void)
{
	TIM6->CR1 |=1<<0;//使能计数器
}
void TIM6_Stop(void)
{
	TIM6->CR1 &=~(1<<0);	//禁止计数器
	TIM6->CNT =0;//计数清零
}
uint8_t F_TASK_TIM6=0;
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	if ((TIM6->SR & 1) == 1) /* Check ARR match */ 
	{	
		TIM6->SR &= ~(1<<0);//清除中断标志位
		switch(mcu_get_wifi_work_state())
		{
			case SMART_CONFIG_STATE:
			case AP_STATE:LED_4_SET;break;
			case WIFI_NOT_CONNECTED:
			case WIFI_CONNECTED:LED_4_TOG;break;
			case WIFI_CONN_CLOUD:LED_4_RESET;break;
			case WIFI_LOW_POWER:		LED_4_RESET;break;
			case SMART_AND_AP_STATE:LED_4_RESET;break;
			default :LED_4_RESET;break;
		}
	}
}

	
void TIM7_Init(uint16_t arr,uint16_t psc)
{
	RCC->APBENR1 |= 1<<5;
	TIM7->ARR = arr;
	TIM7->PSC = psc;
	TIM7->DIER |= 1<<0;//使能更新中断
	TIM7->CR1 |= 1<<0;//使能计数器
	NVIC_SetPriority(TIM7_LPTIM2_IRQn, 1); 
	NVIC_EnableIRQ(TIM7_LPTIM2_IRQn);
}
void TIM7_Start(void)
{
	TIM7->CR1 |=1<<0;//使能计数器
}
void TIM7_Stop(void)
{
	TIM7->CR1 &=~(1<<0);	//禁止计数器
	TIM7->CNT =0;//计数清零
}

void TIM7_LPTIM2_IRQHandler(void)
{
	if ((TIM7->SR & 1) == 1) /* Check ARR match */ 
	{			
		TIM7->SR &= ~(1<<0);//清除中断标志位
	}
}

void TIM_Init(void)
{	
	ConfigureTIM16_CH1_AsPWM_EdgeAligned();//
	TIM16_PWM_Set(0);
	TIM6_Init(1000,15999);
}




