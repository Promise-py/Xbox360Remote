
#include "data_pool.h"
#include "UI.h"

QueueHandle_t ADC_RxPort;
QueueHandle_t Uart_TxPort;
QueueHandle_t KEY_TxPort;
QueueHandle_t IMU_RxPort;
QueueHandle_t UI_Port;

uint16_t adc[6];
uint8_t uart[36];

void DataPool_Init(void)
{
    ADC_RxPort = xQueueCreate(1, sizeof(adc));
    KEY_TxPort = xQueueCreate(1, sizeof(XboxControllerData_t));
    Uart_TxPort = xQueueCreate(1, sizeof(uart));
    UI_Port = xQueueCreate(1, sizeof(UI_DATA_t));
}


