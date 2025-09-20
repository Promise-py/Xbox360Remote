
#include "code.h"

uint16_t adcValue[6];
uint8_t xbox_msg[43];
uint16_t joy_mid[6];
uint16_t joyL_Vradio = 65535 / 3400;
uint16_t trig_radioR = 1024 / 4095;
uint16_t trig_radioL = 1024 / 4095;
XboxControllerData_t *command;
UI_ROS_DATA_t ui_ros_data;
uint8_t usart1_rx_buf[100];

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
  // HAL_UART_Transmit_DMA(&huart1, buff_msg, data_length + 8);
}

void Receive_Frame(uint8_t *data, uint8_t data_length) {
  // 检查数据长度是否正确
  uint8_t i;
  for (i = 0; i < data_length; i++) {
    if (data[i] == FRAME_HEAD_0 && data[i + 1] == FRAME_HEAD_1) {
      break;  // 找到帧头
    }
  }
  // 检查帧头和帧尾
  if (data[i] != FRAME_HEAD_0 || data[i + 1] != FRAME_HEAD_1 ||
      data[i + 13] != FRAME_END_0 || data[i + 14] != FRAME_END_1) {
    return;  // 帧头或帧尾不匹配
  }

  // 提取帧ID和数据长度
  uint8_t frame_id = data[i + 2];
  uint8_t length = data[i + 3];

  // 检查数据长度是否与实际长度匹配
  // if (length != data_length - 8) {
  //   return;  // 数据长度不匹配
  // }

  // 提取数据内容
  ui_ros_data.lock_mode_flag = data[i + 4];
  ui_ros_data.lock_finish_flag = data[i + 5];
  memcpy(ros_data.data, data + i + 6, 4);
  ui_ros_data.yolo_data = ros_data.Data;
  ui_ros_data.camera_flag = data[i + 10];

  xQueueSendFromISR(UI_RosPORT, &ui_ros_data, 0);
  // 跳过校验CRC
  return;
}

void UART_SendData(XboxControllerData_t *command) {
  if (xQueueReceive(ADC_RxPort, adcValue, 0) == pdPASS) {
    JOY_MID(joy_mid);
    float joy;
    // UI_FSM(command);
    // UI_Set_Friction(&ui_data.UpWheel_RPM,&ui_data.LeftWheel_RPM,&ui_data.RightWheel_RPM);

    if (adcValue[0] - joy_mid[0] > JOY_Death_Zone ||
        adcValue[0] - joy_mid[0] < -JOY_Death_Zone) {
      joy = (int32_t)(65535.0 / (JOY_LVERT_MAX - JOY_LVERT_MIN) *
                      (adcValue[0] - joy_mid[0]));
      if (joy > 32767)
        joy = 32767;
      else if (joy < -32768)
        joy = -32768;
      else
        command->joyLVert = (uint16_t)(32768 + joy);
    } else
      command->joyLVert = 32768;

    if (adcValue[1] - joy_mid[1] > JOY_Death_Zone ||
        adcValue[1] - joy_mid[1] < -JOY_Death_Zone) {
      joy = (int32_t)(65535.0 / (JOY_LHORI_MAX - JOY_LHORI_MIN) *
                      (adcValue[1] - joy_mid[1]));
      if (joy > 32767)
        joy = 32767;
      else if (joy < -32768)
        joy = -32768;
      else
        command->joyLHori = (uint16_t)(32768 + joy);
    } else
      command->joyLHori = 32768;

    if (adcValue[5] - joy_mid[5] > JOY_Death_Zone ||
        adcValue[5] - joy_mid[5] < -JOY_Death_Zone) {
      joy = (int32_t)(65535.0 / (JOY_RVERT_MAX - JOY_RVERT_MIN) *
                      (adcValue[5] - joy_mid[5]));
      if (joy > 32767)
        joy = 32767;
      else if (joy < -32768)
        joy = -32768;
      else
        command->joyRVert = (uint16_t)(32768 + joy);

    } else
      command->joyRVert = 32768;

    if (adcValue[4] - joy_mid[4] > JOY_Death_Zone ||
        adcValue[4] - joy_mid[4] < -JOY_Death_Zone) {
      joy = (int32_t)(65535.0 / (JOY_RHORI_MAX - JOY_RHORI_MIN) *
                      (adcValue[4] - joy_mid[4]));
      if (joy > 32767)
        joy = 32767;
      else if (joy < -32768)
        joy = -32768;
      else
        command->joyRHori = (uint16_t)(32768 + joy);
    } else
      command->joyRHori = 32768;

    // if (!command->SWB) {
    //   UI_DATA_t ui_data = UI_Set_POS(command);
    //   ui_world_x.world_x = ui_data.world_pos[0];
    //   ui_world_y.world_y = ui_data.world_pos[1];
    //   ui_data_tensile.tensile = ui_data.UI_Tensile;
    // }

    if (adcValue[2] - joy_mid[2] > 100 || adcValue[2] - joy_mid[2] < -100) {
      if (adcValue[2] > 3400)
        command->trigLT = 1024;
      else
        command->trigLT = (uint16_t)((adcValue[2] - joy_mid[2]) * 1024 / 2535);
    } else
      command->trigLT = 0;

    if (adcValue[3] - joy_mid[3] > 100 || adcValue[3] - joy_mid[3] < -100) {
      if (adcValue[3] > 3500)
        command->trigRT = 1024;
      else
        command->trigRT = (uint16_t)((adcValue[2] - joy_mid[2]) * 1024 / 2550);
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
    xbox_msg[18] = (uint8_t)command->SWC;

    xbox_msg[19] = (uint8_t)(command->joyLHori >> 8);
    xbox_msg[20] = (uint8_t)(command->joyLHori & 0xff);
    xbox_msg[21] = (uint8_t)(command->joyLVert >> 8);
    xbox_msg[22] = (uint8_t)(command->joyLVert & 0xff);
    xbox_msg[23] = (uint8_t)(command->joyRHori >> 8);
    xbox_msg[24] = (uint8_t)(command->joyRHori & 0xff);
    xbox_msg[25] = (uint8_t)(command->joyRVert >> 8);
    xbox_msg[26] = (uint8_t)(command->joyRVert & 0xff);
    xbox_msg[27] = (uint8_t)(command->trigLT >> 8);
    xbox_msg[28] = (uint8_t)(command->trigLT & 0xff);
    xbox_msg[29] = (uint8_t)(command->trigRT >> 8);
    xbox_msg[30] = (uint8_t)(command->trigRT & 0xff);
    memcpy(xbox_msg + 31, ui_world_x.data, 4);
    memcpy(xbox_msg + 35, ui_world_y.data, 4);
    memcpy(xbox_msg + 39, ui_data_tensile.data, 4);
    Send_Frame(0x01, 43, xbox_msg);
  }
}

void UART_ReceiveInit(void) {
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usart1_rx_buf, 100);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart == &huart1) {
    Receive_Frame(usart1_rx_buf, sizeof(usart1_rx_buf));

    UART_ReceiveInit();
  }
}
