
#include "code.h"

uint16_t adcValue[6];
uint8_t xbox_msg[42];
uint16_t joy_mid[6];
uint16_t joyL_Vradio = 65535 / 3400;
uint16_t trig_radioR = 1024 / 4095;
uint16_t trig_radioL = 1024 / 4095;
XboxControllerData_t *command;
UI_DATA_t ui_data;

uint16_t CRC16_Table(uint8_t *p, uint8_t counter) {
  uint16_t crc = 0;
  for (uint8_t i = 0; i < counter; i++) {
    crc = CRC16Table[((crc >> 8) ^ p[i]) & 0xFF] ^ (crc << 8);
  }
  return crc;
}

void Send_Frame(uint8_t frame_id, uint8_t data_length, uint8_t *data) {
  uint8_t buff_msg[data_length + 8];

  buff_msg[0] = FRAME_HEAD_0;
  buff_msg[1] = FRAME_HEAD_1;
  buff_msg[2] = frame_id;
  buff_msg[3] = data_length;

  for (int q = 0; q < data_length; q++) {
    buff_msg[4 + q] = data[q];  // 直接装填
  }

  // check_code.crc_code = CRC16_Table(data, data_length);
  buff_msg[4 + data_length] = check_code.crc_buff[0];
  buff_msg[5 + data_length] = check_code.crc_buff[1];
  buff_msg[6 + data_length] = FRAME_END_0;
  buff_msg[7 + data_length] = FRAME_END_1;

  // xQueueSendFromISR(Uart_TxPort, buff_msg, 0);
  // 使用UART发送数据
  HAL_UART_Transmit(&huart1, buff_msg, data_length + 8, HAL_MAX_DELAY);
}

void UART_SendData(XboxControllerData_t *command) {
  if (xQueueReceive(ADC_RxPort, adcValue, 0) == pdPASS) {
    JOY_MID(joy_mid);
    // UI_FSM(command);
    // UI_Set_Friction(&ui_data.UpWheel_RPM,&ui_data.LeftWheel_RPM,&ui_data.RightWheel_RPM);

    if (adcValue[0] - joy_mid[0] > JOY_Death_Zone ||
        adcValue[0] - joy_mid[0] < -JOY_Death_Zone)
      command->joyLVert = (uint16_t)(65535 / 4095 * adcValue[0]);
    else
      command->joyLVert = 32768;

    if (adcValue[1] - joy_mid[1] > JOY_Death_Zone ||
        adcValue[1] - joy_mid[1] < -JOY_Death_Zone)
      command->joyLHori = 65535 - (uint16_t)(65535 / 4095 * adcValue[1]);
    else
      command->joyLHori = 32768;

    if (adcValue[5] - joy_mid[5] > JOY_Death_Zone ||
        adcValue[5] - joy_mid[5] < -JOY_Death_Zone)
      command->joyRVert = (uint16_t)(65535 / 4095 * adcValue[5]);
    else
      command->joyRVert = 32768;

    if (adcValue[4] - joy_mid[4] > JOY_Death_Zone ||
        adcValue[4] - joy_mid[4] < -JOY_Death_Zone)
      command->joyRHori = 65535 - (uint16_t)(65535 / 4095 * adcValue[4]);
    else
      command->joyRHori = 32768;

    UI_DATA_t ui_data = UI_Set_POS(command);
    ui_world_x.world_x = ui_data.world_pos[0];
    ui_world_y.world_y = ui_data.world_pos[1];
    ui_data_tensile.tensile = ui_data.UI_Tensile;

    if (adcValue[2] - joy_mid[2] > 100 || adcValue[2] - joy_mid[2] < -100) {
      if (adcValue[2] > 2800)
        command->trigLT = 1024;
      else
        command->trigLT = (uint16_t)(adcValue[2] * 1024 / 3000);
    } else
      command->trigLT = 0;

    if (adcValue[3] - joy_mid[3] > 100 || adcValue[3] - joy_mid[3] < -100) {
      if (adcValue[3] > 3000)
        command->trigRT = 1024;
      else
        command->trigRT = (uint16_t)(adcValue[3] * 1024 / 3100);
    } else
      command->trigRT = 0;

    xbox_msg[0] = (uint8_t)command->btnY;
    xbox_msg[1] = (uint8_t)command->btnB;
    xbox_msg[2] = (uint8_t)command->btnA;
    xbox_msg[3] = (uint8_t)command->btnX;
    xbox_msg[5] = (uint8_t)command->btnStart;
    xbox_msg[7] = (uint8_t)command->btnXbox;
    xbox_msg[8] = (uint8_t)command->btnLB;
    xbox_msg[9] = (uint8_t)command->btnRB;
    xbox_msg[10] = (uint8_t)command->btnLS;
    xbox_msg[11] = (uint8_t)command->btnRS;
    xbox_msg[12] = (uint8_t)command->btnDirUp;
    xbox_msg[13] = (uint8_t)command->btnDirLeft;
    xbox_msg[14] = (uint8_t)command->btnDirRight;
    xbox_msg[15] = (uint8_t)command->btnDirDown;

    xbox_msg[16] = (uint8_t)command->SWA;
    xbox_msg[17] = (uint8_t)command->SWB;

    xbox_msg[18] = (uint8_t)(command->joyLHori >> 8);
    xbox_msg[19] = (uint8_t)(command->joyLHori & 0xff);
    xbox_msg[20] = (uint8_t)(command->joyLVert >> 8);
    xbox_msg[21] = (uint8_t)(command->joyLVert & 0xff);
    xbox_msg[22] = (uint8_t)(command->joyRHori >> 8);
    xbox_msg[23] = (uint8_t)(command->joyRHori & 0xff);
    xbox_msg[24] = (uint8_t)(command->joyRVert >> 8);
    xbox_msg[25] = (uint8_t)(command->joyRVert & 0xff);
    xbox_msg[26] = (uint8_t)(command->trigLT >> 8);
    xbox_msg[27] = (uint8_t)(command->trigLT & 0xff);
    xbox_msg[28] = (uint8_t)(command->trigRT >> 8);
    xbox_msg[29] = (uint8_t)(command->trigRT & 0xff);
    memcpy(xbox_msg + 30, ui_world_x.data, 4);
    memcpy(xbox_msg + 34, ui_world_y.data, 4);
    memcpy(xbox_msg + 38, ui_data_tensile.data, 4);
    Send_Frame(0x01, 42, xbox_msg);
  }
}
