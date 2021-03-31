#ifndef __IO_H
#define __IO_H 		
#include "MY_ST_config.h"

//	 	PC10
#define PC10_OUT {RCC->IOPENR|=1<<2;GPIOC->MODER&=~(3<<20);GPIOC->MODER|=1<<20;} 
#define PC10_SET GPIOC->ODR|=1<<10
#define PC10_RESET  GPIOC->ODR&=~(1<<10)
#define PC10_TOG GPIOC->ODR^=1<<10

//	 	PC11
#define PC11_OUT {RCC->IOPENR|=1<<2;GPIOC->MODER&=~(3<<22);GPIOC->MODER|=1<<22;} 
#define PC11_SET GPIOC->ODR|=1<<11
#define PC11_RESET  GPIOC->ODR&=~(1<<11)
#define PC11_TOG GPIOC->ODR^=1<<11



#define Move_Forward {PC10_SET;PC11_RESET;}
#define Move_Backward {PC10_RESET;PC11_SET;}
#define Move_Stop  {PC10_RESET;PC11_RESET;}

//PC13
#define KEY_IN  {RCC->IOPENR|=1<<2;GPIOC->MODER&=~(3<<26);GPIOC->MODER|=0<<26;} 
#define KEY_State ((GPIOC->IDR & 1<<13) == 1<<13)

//LED_CTRL4	 	PA5
#define LED_4_OUT {RCC->IOPENR|=1<<0;GPIOA->MODER&=~(3<<10);GPIOA->MODER|=1<<10;} 
#define LED_4_SET GPIOA->ODR|=1<<5
#define LED_4_RESET  GPIOA->ODR&=~(1<<5)
#define LED_4_TOG GPIOA->ODR^=1<<5

void IO_Init(void);
	
#endif

