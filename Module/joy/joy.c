
#include "joy.h"

uint16_t adc_value[6];
uint16_t adc_value_aver[6];
uint16_t adc_init[6];
uint8_t aver_count;
uint8_t adc_check;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  aver_count++;
  int i;
  for (i = 0; i < 6; i++) {
    adc_value_aver[i] += adc_value[i];
  }

  if (aver_count == 12) {
    aver_count = 0;

    if (!adc_check) {
      for (i = 0; i < 6; i++) {
        adc_value_aver[i] /= 12;
      }
      memcpy(adc_init, adc_value_aver, 12);
      adc_check = 1;
    } else {
      for (i = 0; i < 6; i++) {
        adc_value_aver[i] /= 13;
      }
      xQueueSendFromISR(ADC_RxPort, &adc_value_aver, 0);
    }
  }
}

void JOY_Init(void) { HAL_ADC_Start_DMA(&hadc1, adc_value, 6); }

void JOY_MID(uint16_t* joy_mid) { memcpy(joy_mid, adc_init, 12); }