#ifndef __DATA_POOL_H
#define __DATA_POOL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include <stddef.h>
#include <stdint.h>
#include "main.h"
#include "key.h"
#include "ins_task.h"


void DataPool_Init(void);

extern QueueHandle_t ADC_RxPort;
extern QueueHandle_t KEY_TxPort;
extern QueueHandle_t IMU_RxPort;
extern QueueHandle_t Uart_TxPort;
extern QueueHandle_t UI_Port;

#endif

