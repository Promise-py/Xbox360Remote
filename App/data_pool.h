#ifndef __DATA_POOL_H
#define __DATA_POOL_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "ins_task.h"
#include "key.h"
#include "main.h"
#include "queue.h"

void DataPool_Init(void);

extern QueueHandle_t ADC_RxPort;
extern QueueHandle_t KEY_TxPort;
extern QueueHandle_t IMU_RxPort;
extern QueueHandle_t Uart_TxPort;
extern QueueHandle_t UI_Port;
extern QueueHandle_t UI_RosPORT;

typedef struct {
  uint8_t
      lock_mode_flag;  // 当前模式：0-初始化，1-手动模式，2-自动锁框，3-传球模式
  uint8_t lock_finish_flag;  // 是否锁定篮筐
  uint8_t camera_flag;       // 视野中是否有篮筐:0-无，1-有
  float yolo_data;
} UI_ROS_DATA_t;

#endif
