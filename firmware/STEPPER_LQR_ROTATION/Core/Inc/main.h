/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define ICM_SPI_CS_Pin GPIO_PIN_1
#define ICM_SPI_CS_GPIO_Port GPIOC
#define ICM_SPI_MISO_Pin GPIO_PIN_2
#define ICM_SPI_MISO_GPIO_Port GPIOC
#define ISM_SPI_MOSI_Pin GPIO_PIN_3
#define ISM_SPI_MOSI_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define STEPPER_SPI_CS_Pin GPIO_PIN_4
#define STEPPER_SPI_CS_GPIO_Port GPIOA
#define STEPPER_SPI_CLK_Pin GPIO_PIN_5
#define STEPPER_SPI_CLK_GPIO_Port GPIOA
#define STEPPER_SPI_MISO_Pin GPIO_PIN_6
#define STEPPER_SPI_MISO_GPIO_Port GPIOA
#define STEPPER_SPI_MOSI_Pin GPIO_PIN_7
#define STEPPER_SPI_MOSI_GPIO_Port GPIOA
#define ISM_SPI_SCK_Pin GPIO_PIN_10
#define ISM_SPI_SCK_GPIO_Port GPIOB
#define RC_CH2_Pin GPIO_PIN_14
#define RC_CH2_GPIO_Port GPIOB
#define RC_CH1_Pin GPIO_PIN_6
#define RC_CH1_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define STEPPER_RST_Pin GPIO_PIN_5
#define STEPPER_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
