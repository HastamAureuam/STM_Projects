# STM_Projects
## P10_LedMatrix_Project
В данной работе представлен пример написания программы для светодиодной матрицы P10. Была реализована игра "Flappy Bird" с управлением на ультразвуковом датчике HC-SR04. 
Принцип работы состоит в том, что при расстоянии меньше 7 см птичка подлетает вверх на расстояние, при большем расстоянии птичка плавно опускается вниз.
Код для работы с матрицей написан с ипользованием библиотек disp1color, f6x8m, f10x16f, font. Назначение фукнций и их аргументы описаны в самих библиотеках.
Код для работы с ультразвуковым датчиком написан с ипользованием библиотеки dwt_stm32_delay. 
 Краткое описание библиотеки dwt_stm32_delay.
Перед началом работы, необходимо включить содержимое библиотеки в исходный файл: #include "dwt_stm32_delay.h"
Далее для активации работы библиотеки ее необходимо инициализировать строкой: DWT_Delay_Init();
После этого нужно написать данную функцию:
inline 'code'.
uint32_t **Read_HCSR04**()
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
При присвоении переменной значения возвращаемого функцией можно получать измеряемое датчиком расстояние в [см].
