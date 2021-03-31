#include "TIM.h"
#include "IO.h"



void ConfigureTIM16_CH1_AsPWM_EdgeAligned(void)//T16_CH1 PWM PD0 AF2 ���
{
  RCC->APBENR2 |= 1<<17;
  RCC->IOPENR |= 1<<3;
	GPIOD->MODER &=~(3<<0);
  GPIOD->MODER |=  2<<0; //���ù���
	GPIOD->AFR[0]&=0XFFFFFFF0;
  GPIOD->AFR[0] |= 0x02 << 0;
  
  TIM16->PSC = 159; //16M/(159+1)=0.1M, step=10us
  TIM16->ARR = 99;//10*��99+1��=1Ms ������
  TIM16->CCR1 = 1; // �ߵ�ƽʱ��
  TIM16->CCMR1 |=6<<4|1<<3;//CH_1 110ģʽ,Ԥװ��ʹ�ܣ�Ĭ��Ϊ���ģʽ��
  TIM16->CCER |= 1<<0;  //CH_1�������PWM
  TIM16->CR1 |= 1<<0; //ʹ�ܼ�����(���ض��� ����)
  TIM16->EGR |= 1<<0; //�����ж�
	TIM16->BDTR |=1<<14;//�Զ����ʹ��AOE
}
void TIM16_PWM_Stop(void)
{
	TIM16->CCER &=~(1<<0);//�ر����PWM
	TIM16->CR1 &=~(1<<0);	//��ֹ������
	TIM16->CNT =0;//��������
}
void TIM16_PWM_Start(void)
{
	TIM16->CCER |=1<<0;//�������PWM
	TIM16->CR1 |=1<<0;//ʹ�ܼ�����
}
void TIM16_PWM_Set(uint16_t CCR)
{
	TIM16->CCR1 = CCR;
}

void TIM6_Init(uint16_t arr,uint16_t psc)//TASK����
{
	RCC->APBENR1 |= 1<<4;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->DIER |= 1<<0;//ʹ�ܸ����ж�
	TIM6->CR1 |= 1<<0;//ʹ�ܼ�����
	NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 1); 
	NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
}
void TIM6_Start(void)
{
	TIM6->CR1 |=1<<0;//ʹ�ܼ�����
}
void TIM6_Stop(void)
{
	TIM6->CR1 &=~(1<<0);	//��ֹ������
	TIM6->CNT =0;//��������
}
uint8_t F_TASK_TIM6=0;
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	if ((TIM6->SR & 1) == 1) /* Check ARR match */ 
	{	
		TIM6->SR &= ~(1<<0);//����жϱ�־λ
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
	TIM7->DIER |= 1<<0;//ʹ�ܸ����ж�
	TIM7->CR1 |= 1<<0;//ʹ�ܼ�����
	NVIC_SetPriority(TIM7_LPTIM2_IRQn, 1); 
	NVIC_EnableIRQ(TIM7_LPTIM2_IRQn);
}
void TIM7_Start(void)
{
	TIM7->CR1 |=1<<0;//ʹ�ܼ�����
}
void TIM7_Stop(void)
{
	TIM7->CR1 &=~(1<<0);	//��ֹ������
	TIM7->CNT =0;//��������
}

void TIM7_LPTIM2_IRQHandler(void)
{
	if ((TIM7->SR & 1) == 1) /* Check ARR match */ 
	{			
		TIM7->SR &= ~(1<<0);//����жϱ�־λ
	}
}

void TIM_Init(void)
{	
	ConfigureTIM16_CH1_AsPWM_EdgeAligned();//
	TIM16_PWM_Set(0);
	TIM6_Init(1000,15999);
}




