/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define clo1_Pin GPIO_PIN_0
#define clo1_GPIO_Port GPIOC
#define clo2_Pin GPIO_PIN_1
#define clo2_GPIO_Port GPIOC
#define clo3_Pin GPIO_PIN_2
#define clo3_GPIO_Port GPIOC
#define clo4_Pin GPIO_PIN_3
#define clo4_GPIO_Port GPIOC
#define lin1_Pin GPIO_PIN_7
#define lin1_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_1
#define LED4_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_11
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_14
#define LCD_CS_GPIO_Port GPIOB
#define lin4_Pin GPIO_PIN_7
#define lin4_GPIO_Port GPIOC
#define lin3_Pin GPIO_PIN_8
#define lin3_GPIO_Port GPIOC
#define lin2_Pin GPIO_PIN_9
#define lin2_GPIO_Port GPIOC
#define Xbox_Pin GPIO_PIN_8
#define Xbox_GPIO_Port GPIOA
#define CS2_Pin GPIO_PIN_12
#define CS2_GPIO_Port GPIOC
#define CS1_Pin GPIO_PIN_2
#define CS1_GPIO_Port GPIOD
#define MOTOR2_Pin GPIO_PIN_8
#define MOTOR2_GPIO_Port GPIOB
#define MOTOR1_Pin GPIO_PIN_9
#define MOTOR1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
