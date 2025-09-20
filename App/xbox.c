
#include "xbox.h"

#include "xbox_task.h"

void XboxInit(void) {
  __disable_irq();
  DataPool_Init();
  HAL_TIM_Base_Start_IT(&htim2);
  UART_ReceiveInit();
  OSTASKInit();
  __enable_irq();
}
