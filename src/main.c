/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComPolling/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F4xx UART HAL API to transmit 
  *          and receive a data buffer with a communication process based on
  *          polling transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @ingroup STM32F4xx_FreeRTOS_Counting_Tasks_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct package_type {
	TaskHandle_t task_handle;
	uint8_t task_name[16];
	uint32_t count;
} pkg_t;

/* Private define ------------------------------------------------------------*/
#define MANAGER_STACK_DEPTH	(1024U)
#define WORKER_STACK_DEPTH	(1024U)

/* Private macro -------------------------------------------------------------*/
#define ASSERT(exp) if (exp) Error_Handler()

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void count_tasks_start(void * parameter);
static void manager_task(void * parameter);
static void work_task(void * parameter);
static void uart_send(UART_HandleTypeDef *huart, uint8_t *buff, uint32_t len);
static void Error_Handler(void);
static void itoa(int32_t num, uint8_t *str);
static void reverse(uint8_t *begin, uint8_t *end);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  BSP_LED_Toggle(LED3);

  count_tasks_start(NULL);

  vTaskStartScheduler();

  /* Infinite loop */
  while (1) ;

}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED4 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED4); 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief  create manager task before starting scheduling
  * @param  void *: parameter
  * @retval None
  */
static void count_tasks_start(void * parameter)
{
	TaskHandle_t manager_handle;
	BaseType_t result = pdTRUE;

	result = xTaskCreate(manager_task, "manager", MANAGER_STACK_DEPTH, (void *)NULL, 1, &manager_handle);
	
	ASSERT(result == pdFALSE);
}

/**
  * @brief  manager task code
  * @param  void *: parameter
  * @retval None
  */
static void manager_task(void * parameter)
{
	/* eliminate compile error */
	(void)parameter;

	UART_HandleTypeDef *huart;
	pkg_t *pkg;
	BaseType_t result;
	uint32_t i, len;
	uint8_t buff[32];

	/* create hal uart handle */
	huart = (UART_HandleTypeDef *)pvPortMalloc(sizeof(UART_HandleTypeDef));
	ASSERT(huart == NULL);

	/* create arrays of packages */
	pkg = (pkg_t *)pvPortMalloc(sizeof(pkg_t) * 4);
	ASSERT(pkg == NULL);

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = 9600 baud
		- Hardware flow control disabled (RTS and CTS signals) */
	huart->Instance          = USARTx;

	huart->Init.BaudRate     = 9600;
	huart->Init.WordLength   = UART_WORDLENGTH_8B;
	huart->Init.StopBits     = UART_STOPBITS_1;
	huart->Init.Parity       = UART_PARITY_NONE;
	huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart->Init.Mode         = UART_MODE_TX_RX;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;

	ASSERT(HAL_UART_Init(huart) != HAL_OK);

	for (i = 0;i < 4;++i) {
		
		pkg[i].count = 0;

		memset(pkg[i].task_name, '\0', 16);

		memcpy(pkg[i].task_name, "worker_", 7);

		pkg[i].task_name[7] = '0' + i;

		result = xTaskCreate(
				work_task,
				(char *)pkg[i].task_name,
				WORKER_STACK_DEPTH,
				(void *)&pkg[i].count,
				0,
				&pkg[i].task_handle);
		ASSERT(result == pdFALSE);
	}

	while (1) {

		// sleep 4 ms to yeild for work tasks
		vTaskDelay(40);

		for (i = 0;i < 4;++i) {
			
			memset(buff, '\0', 32);

			len = strlen((char *)pkg[i].task_name);
			memcpy(buff, pkg[i].task_name, len);
			
			buff[len++] = ':';
			
			itoa(pkg[i].count, buff+len);
			len += strlen((char *)buff+len);
			
			memcpy(buff+len, "\r\n", 2);
			len += 2;

			uart_send(huart, buff, len);
		}
	}
}

/**
  * @brief  work task code
  * @param  void * : parameter
  * @retval None
  */
static void work_task(void * parameter)
{
	uint32_t old_tick = 0, new_tick = 0;
	uint32_t *count_ptr;

	count_ptr = (uint32_t *)parameter;

	old_tick = new_tick = HAL_GetTick();

	while (1) {
		new_tick = HAL_GetTick();

		if (old_tick != new_tick) {

			old_tick = new_tick;

			++(*count_ptr);
		}

		HAL_Delay(1);
	}
}

/**
  * @brief  access uart transmit and protect the critical section via mutex
  * @param  UART_HandleTypeDef * : huart
  * @param  uint8_t *    : buff
  * @param  uint32_t     : len
  * @retval None
  */
static void uart_send(UART_HandleTypeDef *huart, uint8_t *buff, uint32_t len)
{
	ASSERT(HAL_UART_Transmit(huart, buff, len, HAL_MAX_DELAY)!= HAL_OK);
}

/**
  * @brief  integer convert to a string
  * @param  int32_t   : num
  * @param  uint8_t * : str
  * @retval None
  */
static void itoa(int32_t num, uint8_t *str)
{
	uint8_t *tmp = str,
		sign = 0;

	if (num < 0) {
		sign = 1;
		num = ~num + 1;
	}

	do {
		*tmp = '0' + num % 10;
		++tmp;
		num /= 10;
	} while (num) ;

	if (sign) {
		*tmp = '-';
		++tmp;
	}

	reverse(str, tmp-1);
}

/**
  * @brief  reverse string
  * @param  uint8_t * : begin
  * @param  uint8_t * : end
  * @retval None
  */
static void reverse(uint8_t *begin, uint8_t *end)
{
	uint8_t tmp;

	while (begin < end) {
		tmp = *begin;
		*begin = *end;
		*end = tmp;
		++begin;
		--end;
	}
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
