#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f4xx.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define KEY0_INT_GPIO_PORT                GPIOB
#define KEY0_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE();
#define KEY0_INT_GPIO_PIN                 GPIO_PIN_13
#define KEY0_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define KEY0_IRQHandler                   EXTI15_10_IRQHandler

/*******************************************************/


void EXTI_Key_Config(void);

#endif /* __EXTI_H */
