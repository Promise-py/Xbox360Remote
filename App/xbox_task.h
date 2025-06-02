#pragma once

#include <math.h>

#include "FreeRTOS.h"
#include "UI.h"
#include "cmsis_os.h"
#include "data_pool.h"
#include "key.h"
#include "main.h"
#include "task.h"
#include "usart.h"
/*变量定义区*/
XboxControllerData_t XboxControllerData;
int a;
uint8_t send_msgs[50];
UI_DATA_t UI_Data;
/*任务节点创建区*/
osThreadId uartTxTaskHandle;
osThreadId StateGetTaskHandle;
osThreadId INSTaskHandle;
osThreadId UITaskHandle;

/*任务函数声明*/
void UartSendTask(void const *argument);
void StateGetTask(void const *argument);
void INSTask(void const *argument);
void UITask(void const *argument);

/**
 * @name    oSTASKInit
 * @brief   任务初始化，所有运行的任务都在此初始化
 */
void OSTASKInit(void) {
  // osThreadDef(uartTx, UartSendTask, osPriorityNormal, 0, 256);
  // uartTxTaskHandle = osThreadCreate(osThread(uartTx), NULL);

  osThreadDef(state_get, StateGetTask, osPriorityNormal, 0, 256);
  StateGetTaskHandle = osThreadCreate(osThread(state_get), NULL);

#ifdef USE_IMU
  osThreadDef(ins, INSTask, osPriorityNormal, 0, 512);
  INSTaskHandle = osThreadCreate(osThread(ins), NULL);
#endif

  osThreadDef(lcd, UITask, osPriorityNormal, 0, 512);
  UITaskHandle = osThreadCreate(osThread(lcd), NULL);
}

void UartSendTask(void const *argument) {
  for (;;) {
    if (xQueueReceive(Uart_TxPort, send_msgs, 0) == pdPASS) {
      HAL_UART_Transmit(&huart1, send_msgs, sizeof(send_msgs), 1000);
    }
    // 10Hz
    osDelay(5);
  }
}

void StateGetTask(void const *argument) {
  for (;;) {
    key_read(&XboxControllerData);
    UART_SendData(&XboxControllerData);
    vTaskDelay(10);
  }
}

void INSTask(void const *argument) {
  INS_Init();
  for (;;) {
    INS_Task();
    vTaskDelay(1);
  }
}

void UITask(void const *argument) {
  LCD_Clear(WHITE);  // 清全屏白色
  UI_Show(&UI_Data);
  for (;;) {
    if (xQueueReceive(UI_Port, &UI_Data, 0) == pdPASS) {
      // LCD_Clear(WHITE);  // 清全屏白色
      // LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
      // LCD_SetWindows(0,0,159,127);
      UI_Show(&UI_Data);
    }
    vTaskDelay(1);
  }
}