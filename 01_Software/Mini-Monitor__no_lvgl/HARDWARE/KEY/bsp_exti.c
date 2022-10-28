#include "main.h"
#include "bsp_exti.h"

 /**
  * @brief  配置 PB13 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /*开启按键GPIO口的时钟*/
    KEY0_INT_GPIO_CLK_ENABLE();

    /* 选择按键0的引脚 */ 
    GPIO_InitStructure.Pin = KEY0_INT_GPIO_PIN;
    /* 设置引脚为输入模式 */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;	    		
    /* 设置引脚上拉 */
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    /* 使用上面的结构体初始化按键 */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* 配置 EXTI 中断源 到key0 引脚、配置中断优先级*/
    HAL_NVIC_SetPriority(KEY0_INT_EXTI_IRQ, 0, 0);
    /* 使能中断 */
//    HAL_NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);
	
}

/*********************************************END OF FILE**********************/
