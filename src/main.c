/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "stm32f7xx_hal.h"

/* USER CODE BEGIN Includes */
#include <..\system\include\retarget\retarget.h>
#include "u_tty.h"
#include "diag/Trace.h"
#include "string.h"
#include "stdlib.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
char readBuf[1];
__IO ITStatus newuartRXmsg = SET;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/*void EXTI15_10_IRQHandler(void);*/
void printWelcomeMessage(void);
uint8_t processUserInput();
/*uint8_t readUserInput(void);*/
size_t str_len(const char *str);
void PerformCriticalTasks(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
    {

    /* USER CODE BEGIN 1 */
    char buf[20];
    /*uint8_t opt = 0;*/

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART3_UART_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();

    /* USER CODE BEGIN 2 */
    RetargetInit(&huart3); /*retarget STDIN/STDOUT to UART3*/
    /*HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);*//*enable external interrupt 15_10IRQ*/

    printWelcomeMessage();

    printf("What is your name: ");
    scanf("%s", buf);
    printf("\r\nHello %s!\r\n", buf);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while (1)
	{

	if (newuartRXmsg == SET)
	    {
	    newuartRXmsg = RESET;
	    processUserInput();
	    }
	HAL_UART_Receive_IT(&huart3, (uint8_t*) readBuf, 1);
	PerformCriticalTasks();
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	HAL_Delay(500);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* USER CODE END 3 */

    }

void PerformCriticalTasks(void)
    {
    /* HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);
     HAL_Delay(500);*/
    }

uint8_t processUserInput()
    {

    char msg[30];
    int userinput = atoi(readBuf);

    sprintf(msg, "%d.", userinput);
    tx(msg);
    switch (userinput)
	{
    case 1:
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	sprintf(msg, "LED1: %s",
		HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin) == GPIO_PIN_SET ?
			"ON  " : "OFF ");
	ts(0x03240, msg);
	break;
    case 2:
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	sprintf(msg, "LED2: %s",
		HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin) == GPIO_PIN_SET ?
			"ON  " : "OFF ");
	ts(0x03440, msg);
	break;
    case 3:
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	sprintf(msg, "LED3: %s",
		HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin) == GPIO_PIN_SET ?
			"ON  " : "OFF ");
	ts(0x03140, msg);
	break;

    case 4:
	sprintf(msg, "USER BUTTON status: %s",
		HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)
			== GPIO_PIN_SET ? "PRESSED" : "RELEASED");
	ts(0x03440, msg);
	break;
    case 5:
	printWelcomeMessage();
	break;
    default:
	tx(" Wrong key");
	break;
	};
    tx(PROMPT);

    return (0);
    }

/** System Clock Configuration
 */
void SystemClock_Config(void)
    {

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
     */
    __HAL_RCC_PWR_CLK_ENABLE()
    ;

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
	Error_Handler();
	}

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

    /**Configure the Systick interrupt time 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 7, 0);
    }

/** NVIC Configuration
 */
static void MX_NVIC_Init(void)
    {
    /* USART3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART3_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    /* EXTI15_10_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

/* USART3 init function */
static void MX_USART3_UART_Init(void)
    {

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
	{
	Error_Handler();
	}

    }

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 PC1   ------> ETH_MDC
 PA1   ------> ETH_REF_CLK
 PA2   ------> ETH_MDIO
 PA7   ------> ETH_CRS_DV
 PC4   ------> ETH_RXD0
 PC5   ------> ETH_RXD1
 PB13   ------> ETH_TXD1
 PA8   ------> USB_OTG_FS_SOF
 PA9   ------> USB_OTG_FS_VBUS
 PA10   ------> USB_OTG_FS_ID
 PA11   ------> USB_OTG_FS_DM
 PA12   ------> USB_OTG_FS_DP
 PG11   ------> ETH_TX_EN
 PG13   ------> ETH_TXD0
 */
static void MX_GPIO_Init(void)
    {

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOH_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOD_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOG_CLK_ENABLE()
    ;

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin,
	    GPIO_PIN_RESET);

    /*Configure GPIO pin : USER_Btn_Pin */
    GPIO_InitStruct.Pin = USER_Btn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
    GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
    GPIO_InitStruct.Pin = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
    GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : RMII_TXD1_Pin */
    GPIO_InitStruct.Pin = RMII_TXD1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_OverCurrent_Pin */
    GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
    GPIO_InitStruct.Pin = USB_SOF_Pin | USB_ID_Pin | USB_DM_Pin | USB_DP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_VBUS_Pin */
    GPIO_InitStruct.Pin = USB_VBUS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
    GPIO_InitStruct.Pin = RMII_TX_EN_Pin | RMII_TXD0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    }

/* USER CODE BEGIN 4 */

void printWelcomeMessage(void)
    {
    tx(T_CLSA);
    ts(0x14941, ASCART01rn);/*STM32f476ZG*/
    txtest();
    /*ts(WELCOME_STYLE, WELCOME_MESSAGE);*/
    ts(MENU_STYLE, MAIN_MENU);
    }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
    if (GPIO_Pin == GPIO_PIN_13)
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
    {
    /* Set transmission flag: transfer complete*/
    newuartRXmsg = SET;

    }
/*void HAL_SYSTICK_Callback(void)
 {

 }*/

size_t str_len(const char *str)
    {
    return ((*str) ? str_len(++str) + 1 : 0);
    }
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */

void Error_Handler(void)
    {
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
	{
	}
    /* USER CODE END Error_Handler */
    }

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
    {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

    }

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
