
#include "data_pool.h"
#include "UI.h"

QueueHandle_t ADC_RxPort;
QueueHandle_t Uart_TxPort;
QueueHandle_t KEY_TxPort;
QueueHandle_t IMU_RxPort;
QueueHandle_t UI_Port;
QueueHandle_t UI_init_Port;
QueueHandle_t UI_RosPORT;

uint16_t adc[6];
uint8_t uart[36];

void DataPool_Init(void)
{
    ADC_RxPort = xQueueCreate(1, sizeof(adc));
    KEY_TxPort = xQueueCreate(1, sizeof(XboxControllerData_t));
    Uart_TxPort = xQueueCreate(1, sizeof(uart));
    UI_Port = xQueueCreate(5, sizeof(UI_DATA_t));
    UI_init_Port = xQueueCreate(1, sizeof(UI_DATA_t));
    UI_RosPORT = xQueueCreate(1, sizeof(UI_ROS_DATA_t));
}


