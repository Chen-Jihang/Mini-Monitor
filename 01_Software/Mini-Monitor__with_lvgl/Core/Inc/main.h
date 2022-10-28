/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_exti.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int


extern uint8_t ina226_refresh;
extern uint8_t tmin, tsec, tmsec100;
extern uint8_t RST_Flag;
extern uint8_t LongPress_Flag;
extern uint8_t longtime_100ms;
extern uint8_t disp_pos;
extern uint32_t test_cnt;


typedef enum{
	LONGPRESS_0S,				//release
	LONGPRESS_0_3S,			//0S < PressTime <3S
	LONGPRESS_MORE_3S,	//PressTime >=3S
	
}PressTime;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_CS_Pin					GPIO_PIN_4
#define OLED_CS_GPIO_Port		GPIOA
#define OLED_RST_Pin				GPIO_PIN_6
#define OLED_RST_GPIO_Port	GPIOA
#define OLED_DC_Pin					GPIO_PIN_0
#define OLED_DC_GPIO_Port		GPIOB

#if 0
	#define LED_Pin							GPIO_PIN_12
	#define LED_GPIO_Port				GPIOB
#else	//CoreBoard
	#define LED_Pin							GPIO_PIN_13
	#define LED_GPIO_Port				GPIOC
#endif
	
#define KEY_Pin							GPIO_PIN_13
#define KEY_GPIO_Port				GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
