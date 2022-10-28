#include "main.h"
#include "bsp_exti.h"

 /**
  * @brief  ���� PB13 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /*��������GPIO�ڵ�ʱ��*/
    KEY0_INT_GPIO_CLK_ENABLE();

    /* ѡ�񰴼�0������ */ 
    GPIO_InitStructure.Pin = KEY0_INT_GPIO_PIN;
    /* ��������Ϊ����ģʽ */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;	    		
    /* ������������ */
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    /* ʹ������Ľṹ���ʼ������ */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* ���� EXTI �ж�Դ ��key0 ���š������ж����ȼ�*/
    HAL_NVIC_SetPriority(KEY0_INT_EXTI_IRQ, 0, 0);
    /* ʹ���ж� */
//    HAL_NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);
	
}

/*********************************************END OF FILE**********************/
