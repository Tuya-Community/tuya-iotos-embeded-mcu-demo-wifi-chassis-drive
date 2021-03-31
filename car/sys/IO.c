#include "IO.h"
#include "delay.h"
#include "TIM.h"
#include "USART.h"




void IO_Init(void)
{
	PC10_OUT;
	PC10_RESET;
	PC11_OUT;
	PC11_RESET;	
	KEY_IN;
	LED_4_OUT;
	LED_4_RESET;
}
