#include "key.h"

void LINE_CONTROL(uint8_t line, bool state) {
  if (state) {
    switch (line) {
      case 1:
        LINE_1_SET;
        break;
      case 2:
        LINE_2_SET;
        break;
      case 3:
        LINE_3_SET;
        break;
      case 4:
        LINE_4_SET;
        break;
    }
  } else {
    switch (line) {
      case 1:
        LINE_1_RESET;
        break;
      case 2:
        LINE_2_RESET;
        break;
      case 3:
        LINE_3_RESET;
        break;
      case 4:
        LINE_4_RESET;
        break;
    }
  }
}

GPIO_PinState COL_READ(uint8_t col) {
  switch (col) {
    case 1:
      return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
    case 2:
      return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
    case 3:
      return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);
    case 4:
      return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
  }
}

void Key_save(XboxControllerData_t *data) {
  data->btnA_last = data->btnA;
  data->btnB_last = data->btnB;
  data->btnX_last = data->btnX;
  data->btnY_last = data->btnY;
  data->btnLB_last = data->btnLB;
  data->btnRB_last = data->btnRB;
  data->btnStart_last = data->btnStart;
  data->btnXbox_last = data->btnXbox;
  data->btnLS_last = data->btnLS;
  data->btnRS_last = data->btnRS;
  data->btnDirUp_last = data->btnDirUp;
  data->btnDirLeft_last = data->btnDirLeft;
  data->btnDirRight_last = data->btnDirRight;
  data->btnDirDown_last = data->btnDirDown;
}

void key_read(XboxControllerData_t *data) {
  bool key[4][4] = {0};
  uint8_t i, j;
  for (i = 1; i <= 4; i++) {
    LINE_CONTROL(i, low);
    for (j = 1; j <= 4; j++) {
      if (COL_READ(j) == GPIO_PIN_RESET) {
        key[i - 1][j - 1] = 1;
      }
    }
    LINE_CONTROL(i, high);
  }

  Key_save(data);

  data->btnA = key[0][0];
  data->btnB = key[0][1];
  data->btnX = key[0][2];
  data->btnY = key[0][3];
  data->btnDirUp = key[1][0];
  data->btnDirRight = key[1][1];
  data->btnDirDown = key[1][2];
  data->btnDirLeft = key[1][3];
  data->btnLS = key[2][0];
  data->btnRS = key[2][1];
  data->btnLB = key[2][2];
  data->btnRB = key[2][3];
  data->btnStart = key[3][0];
  data->SWA = key[3][2];
  data->SWB = key[3][3];
  if (HAL_GPIO_ReadPin(Xbox_GPIO_Port, Xbox_Pin) == GPIO_PIN_RESET)
    data->btnXbox = 1;
  else
    data->btnXbox = 0;
}