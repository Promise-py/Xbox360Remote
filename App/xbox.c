
#include "xbox.h"
#include "xbox_task.h"

void XboxInit(void)
{
    __disable_irq();
    DataPool_Init();
    JOY_Init();
    HAL_TIM_Base_Start_IT(&htim2);
    OSTASKInit();
    __enable_irq();
}