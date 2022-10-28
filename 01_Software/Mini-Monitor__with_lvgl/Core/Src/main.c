/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "gpio.h"
#include "oled.h"
#include "bmp.h"
#include "INA226.h"
#include "tim.h"
#include "bsp_internalFlash.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"


uint8_t printbuf[100] = {0};
uint8_t oledbuf[60] = {0};
uint8_t ina226_refresh = 0;
uint8_t tmin, tsec, tmsec100;
uint8_t RST_Flag;
uint8_t LongPress_Flag;
uint8_t longtime_100ms;
uint8_t disp_pos=1;
uint32_t test_cnt=0;


void Logo(void)
{
	uint8_t i;
	uint8_t line_len = 20;
	char ChenJH[] = "Designed by ChenJH";
	uint8_t buf_len = strlen(ChenJH);
	
	OLED_ShowPicture(0,0,128,64,BMP2,1);
	OLED_Refresh();
	
	for(i=0; i<128; i++)
	{
		OLED_DrawPoint(0+i,36,1);		//left
		OLED_DrawPoint(127-i,36,1);	//right
		if(i>=line_len && i<=128/2+line_len/2-1)
		{
			OLED_DrawPoint(i-line_len,36,0);			//left
			OLED_DrawPoint(127+line_len-i,36,0);	//right
		}
		OLED_Refresh();
		HAL_Delay(2);
	}
	
	for(i=0; i<buf_len; i++)
	{
		OLED_ShowChar(9+6*i, 42, (uint8_t)ChenJH[i], 8, 1);
		OLED_Refresh();
		HAL_Delay(15);
	}
	HAL_Delay(1500);
	for(i=0; i<buf_len; i++)
	{
		OLED_ShowChar_clean(9+6*i, 42, (uint8_t)ChenJH[i], 8);
		OLED_Refresh();
		HAL_Delay(15);
	}
	
	for(i=0; i<64; i++)
	{
		OLED_DrawPoint(0+i,36,0);		//left
		OLED_DrawPoint(127-i,36,0);	//right
		OLED_Refresh();
		HAL_Delay(2);
	}
	HAL_Delay(100);
	OLED_Clear();
}

void Rst_Test_OLED(void)
{
	uint8_t i;
	
	OLED_ShowString(5,0,  "V: 0.0000 V ", 20,1);
	OLED_ShowString(5,20, "C: 0.00 mA  ", 20,1);
	OLED_ShowString(6+5, 43, "       ", 20,1);
	OLED_ShowString(78+5,44, "mA -avg",8,1);
	OLED_ShowString(78+5,55, "00:00:0 ", 8,1);
	OLED_DrawRectangle(5,42,72,63, 1);
	OLED_Refresh();
	
	for(i=0; i<3; i++)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		OLED_ShowString(10+5, 43, "Reset ", 20,1);
		OLED_DrawRectangle(5,42,72,63, 1);
		OLED_Refresh();
		HAL_Delay(200);
		
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		OLED_ShowString(6+5, 43, "       ", 20,1);
		OLED_DrawRectangle(5,42,72,63, 1);
		OLED_Refresh();
		HAL_Delay(200);
	}
}



void lv_example_label_1(void)
{
//    lv_obj_t * label1 = lv_label_create(lv_scr_act());
//    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
//    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
//    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
//                              "and wrap long text automatically.");
//    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
//    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
//    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 100);
    lv_label_set_text(label2, "Hello World !");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);
}




/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
	EXTI_Key_Config();
	
  INA226_Init();
  OLED_Init();
	
	lv_init();
	lv_port_disp_init();
	
//	OLED_DrawPoint(5, 5, 1);
//	OLED_Refresh();

	lv_example_label_1();
	
	
//	disp_pos = (uint8_t)InterFlash_Read_Word(ADDR_FLASH_SECTOR_3); //Load disp_pos from flash
//	if(disp_pos!=0 && disp_pos!=1)	disp_pos=1;
//	OLED_DisplayTurn(disp_pos);
//	Logo();
	
//	CDC_Check_Open();	//CDC function
	
	HAL_NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	

  /* Infinite loop */
  while (1)
  {	
		
		lv_task_handler();

		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		HAL_Delay(200);
		
#if 0
		CDC_Check_Open();	//CDC function
	
		if(RST_Flag)	//Reset running
		{
			RST_Flag = 0;
			Rst_Test_OLED();
		}
		if(LongPress_Flag == LONGPRESS_MORE_3S)	//Long press 3s to switch OLED display position
		{
			LongPress_Flag = LONGPRESS_0S;
			disp_pos = !disp_pos;
			OLED_DisplayTurn(disp_pos);
			InterFlash_Write_Word((uint32_t)disp_pos); //Store disp_pos to flash
		}
		
    if(ina226_refresh)	//Measure & Refresh display
		{
			ina226_refresh = 0;
			
/*===================================[ Get V&C ]===================================*/
			GetVoltage(&ina226_data.voltageVal);
			Get_Shunt_Current(&ina226_data.Shunt_Current);
			
/**************************************[ V ]****************************************/			
			sprintf((char*)oledbuf, "V: %2.4f V ", ina226_data.voltageVal/1000);
			OLED_ShowString(5,0, oledbuf,20,1);
/**************************************[ C ]****************************************/	
			sprintf((char*)oledbuf, "C: %3.2f mA  ", ina226_data.Shunt_Current);
			OLED_ShowString(5,20, oledbuf,20,1);

/************************************[ Avg_C ]**************************************/	
			ina226_data.Shunt_Current_Avg = calc_average(ina226_data.Shunt_Current_Avg, ina226_data.Shunt_Current, test_cnt + 1); //caculate average current
			sprintf((char*)oledbuf, "%3.2f ", ina226_data.Shunt_Current_Avg);
			OLED_ShowString(6+5, 43, oledbuf, 20,1);
			OLED_ShowString(78+5,44, (unsigned char*)"mA -avg",8,1);
/*************************************[ Time ]**************************************/	
			sprintf((char*)oledbuf, "%02d:%02d:%d ", tmin, tsec, tmsec100);
			OLED_ShowString(78+5,55, oledbuf, 8,1);
/**********************************[ Rectangle ]************************************/	
			OLED_DrawRectangle(5,42,72,63, 1);

/*================================[ OLED_Refresh ]=================================*/
			OLED_Refresh();
			test_cnt++;
		}
#endif
		
//    CDC_Transmit_FS(printbuf, sprintf((char*)printbuf,"ID: 0x%x\r\n", ina226_data.ina226_id));
//		HAL_Delay(1);
//    CDC_Transmit_FS(printbuf, sprintf((char*)printbuf,"voltageVal: %.2f mV\r\n", ina226_data.voltageVal));
//		HAL_Delay(1);
//    CDC_Transmit_FS(printbuf, sprintf((char*)printbuf,"Shunt_voltage: %.2f uV\r\n", ina226_data.Shunt_voltage));
//		HAL_Delay(1);
//    CDC_Transmit_FS(printbuf, sprintf((char*)printbuf,"Shunt_Current: %.2f mA\r\n\n", ina226_data.Shunt_Current));
//		HAL_Delay(100);
		
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

