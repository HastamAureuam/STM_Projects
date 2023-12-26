/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "disp1color.h"
#include "font.h"
#include <locale.h>
#include "dwt_stm32_delay.h"
#define f6x8_MONO_WIDTH 6

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint16_t xs = 15;
   float ys = 8;
   float xh = 38;
   float yh = 0;
   float xh1 = 32; // xh-6
   float yh1 = 0;
   uint8_t h_HEIGHT[16] = {5,2,7,8,4,3,7,5,2,7,4,8,7,3,4,3,2,5,2,7};
   uint8_t l_HEIGHT[16] = {12,9,14,15,11,10,14,12,9,14,11,15,14,10,11,10,9,12,9,14};
   int fillflag = 0;
   float yl, yl1;
   uint16_t xe, ye, isdead, Pisdead = 0;
   uint8_t jump_flag = 0;
   uint8_t jump_time = 0;
   uint8_t prop_count=0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile int16_t flag;
int16_t X = 32;
uint16_t Period = 70;
uint16_t TickNum = 1; //150
extern uint8_t data1[16], data2[16], data3[16], data4[16];
int button; // значение кнопки
int pbutton; // прошлое значение кнопки


void disp_row(int row){


	  if (row == 0){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data1, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	  }
	  if (row == 1){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data2, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	  }

	  if (row == 2){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data3, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	  }

	  if (row == 3){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data4, 16, 10);
		  }

		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	  }


	  HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_SET);
		  for(uint32_t x=0; x<=500; x++) {};
	 HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
  }

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t sensor_time;
uint16_t distance;

uint32_t Read_HCSR04()
{
	uint32_t local_time = 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);		// pull the trig pin high
	DWT_Delay_us(10);										// wait for 10 us
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);	// pull the trig pin low

	// wait for the echo pin to go high

	while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)));

	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))				// while the pin is high
	{
		local_time++;										// increment local time
		DWT_Delay_us(1);									// every 1 us
	}

	return local_time * 2;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  DWT_Delay_Init();
  HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
  char *pMyStr = "Game over";  //Строка на вывод
  uint16_t strSize = strlen(pMyStr);
  uint8_t symbolDelay = 2;
  //char *pMyStr2 = "Lose";
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // Алгоритм, чтобы избавиться от дребезга контактов кнопки
	  //=============================================================

	  	  if(distance <7) { // значение тек. больше предыдушего
	  		  jump_flag=1; // логическая метка прыжка, при нажатии на кнопку
	  	  }
	  	  //else jump_flag = 0;

	  //button = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
//	  if(button>pbutton) { // значение тек. больше предыдушего
//		  jump_flag = 1; // логическая метка прыжка, при нажатии на кнопку
//	  }
//	  pbutton = button; // новое становится старым
	  //=============================================================

	  sensor_time = Read_HCSR04();
	  distance = sensor_time * .034 / 2;
	  //distance = (distance -3);
	  if (distance >20) distance = 20;
	  if(distance <=0) distance = 0;


	  //=============================================================
	  // Основное условие игры
	  if ((flag == 1) &&(Pisdead == 0)){ // флаг переменная с таймера, которая срабатывает 60 раз в секунду
		  	  	xh = xh-0.3; // Движение препятствий
		  	  	if (xh<-6){ // Если препятствие ушло за конец экрана, прибавляется счётчик препятствий
		  	  		prop_count++; // счётчик
		  	  		if (prop_count>15){ // Ограничение счётчика (15 потому что массив из 16 элементов)
		  	  			prop_count=0; // Сброс счётчика препятствий ( Счётчик == индекс массива)
		  	  		}
		  	  		xh = 38; // Генерация нового препятствия с заданной координаты
		  	  	}
		  	  	xh1 = xh + 6; // Ширина препят
		  	  	yh = 0; //  Верхний край верхнего препят
		  	  	yl = 16; // Нижний край нижнего препят
		  	  	yh1 = h_HEIGHT[prop_count]; // Нижняя часть верхего препят
		  	  	yl1 = l_HEIGHT[prop_count]; // Вернхняя часть нижнего препят
	  			ys = ys + 0.33; // изменение координат птички, падение вниз
	  			// ======================================================
	  			// Обработка прыжка

		  	  	//ys = distance;

	  			if ((jump_flag == 1)&&(jump_time<4)){ // джамп тайм = кол-во кадров с начала прыжка
	  				jump_time++; // Cчётчик кадров в прыжке
	  				ys = ys-1.25; //изменение координаты птички вверх
	  			}
	  			else if ((jump_flag == 1) && (jump_time>=2)){// Проверка, если кадров в прыжке больше чем 3 прыжок заканичавется
	  				jump_time=0;
	  				jump_flag=0;
	  			}
	  			// =======================================================
	  			if (ys > 15) ys=15; // Ограничение координат птички снизу
	  			if (ys < 0) ys = 0; // Ограничение координат птички сверху
	  			if (((xs-1)>=xh) && (ys<=yh1)&&((xs-1)<=xh1)){ // Проверка столкновения птички с препят
	  				Pisdead++;
	  			}
			if ((xs>=xh) && (xs<=xh1) && ((ys+1) >= yl1)) // Как вверху, но для нижнего препятствия
			{
				Pisdead++; // игрок мёртв и игровой цикл не идёт
			}
	  			flag = 0;
	  		}
	  //================================================================
	  		if (Pisdead == 0){
	  		// Отчищение экрана
	  		disp1color_FillScreenbuff(0);
	  		// Отрисовка птички
    		disp1color_DrawLine(xs, (int)ys, xs-1, (int)ys);
    		disp1color_DrawLine(xs-1,(int)ys+1,xs+1,(int)ys+1);
    		// Отрисовка препятствий
    		disp1color_DrawRectangle(xh,yh,xh1,yh1);
    		disp1color_DrawRectangle(xh,yl,xh1,yl1);
	  		}



	  		if(Pisdead == 1)
	  		{
//	  			if(fillflag == 0)
//	  			{
	  				disp1color_FillScreenbuff(0);
	  				fillflag = 1;
//	  			}
	  			 for (int16_t x = 32; x > -((strSize + symbolDelay) * f6x8_MONO_WIDTH); x--){
	  			disp1color_printf(x, 4, FONTID_6X8M, pMyStr);
	  			disp1color_UpdateFromBuff(); //Перевод рассчитанных данных в массив
	  			prepare_data(); //Разбиение массива на массивы под каждую строку
		  		for (uint8_t i = 0; i < 20; i++) {
		  			disp_row(0);
		  			disp_row(1);
		  			disp_row(2);
		  			disp_row(3);
		  		}
	  			 }
	  		}
	  		disp1color_UpdateFromBuff(); //Перевод рассчитанных данных в массив
	  		prepare_data(); //Разбиение массива на массивы под каждую строку

	  		//Включение матрицы (построчно)
	  		for (uint8_t i = 0; i < 20; i++) {
	  			disp_row(0);
	  			disp_row(1);
	  			disp_row(2);
	  			disp_row(3);
	  		}
//	  	  disp1color_DrawRectangle(2,4,5,8);
//	  disp1color_UpdateFromBuff(); //Перевод рассчитанных данных в массив
//	  prepare_data(); //Разбиение массива на массивы под каждую строку
  }
  /* USER CODE END 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
