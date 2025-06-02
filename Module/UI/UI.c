
#include "UI.h"

UI_DATA_t ui_data_;
float pixel_pos[2];

void UI_FSM(XboxControllerData_t* XboxData) {
  if (XboxData->btnY && !XboxData->btnY_last) {
    ui_data_.Cursor++;
    if (ui_data_.Cursor == 3) ui_data_.Cursor = 0;
    xQueueSendFromISR(UI_Port, &ui_data_, 0);
  }

  else if (XboxData->btnA && !XboxData->btnA_last) {
    ui_data_.Cursor--;
    if (ui_data_.Cursor == -1) ui_data_.Cursor = 2;
    xQueueSendFromISR(UI_Port, &ui_data_, 0);
  }

  if (XboxData->btnX && !XboxData->btnX_last) {
    switch (ui_data_.Cursor) {
      case 0:
        ui_data_.UpWheel_RPM += 500;
        if (ui_data_.UpWheel_RPM > 10000) ui_data_.UpWheel_RPM = 10000;
        break;
      case 1:
        ui_data_.LeftWheel_RPM += 500;
        if (ui_data_.LeftWheel_RPM > 10000) ui_data_.LeftWheel_RPM = 10000;
        break;
      case 2:
        ui_data_.RightWheel_RPM += 500;
        if (ui_data_.RightWheel_RPM > 10000) ui_data_.RightWheel_RPM = 10000;
        break;
    }
    xQueueSendFromISR(UI_Port, &ui_data_, 0);
  } else if (XboxData->btnB && !XboxData->btnB_last) {
    switch (ui_data_.Cursor) {
      case 0:
        ui_data_.UpWheel_RPM -= 500;
        if (ui_data_.UpWheel_RPM < 0) ui_data_.UpWheel_RPM = 0;
        break;
      case 1:
        ui_data_.LeftWheel_RPM -= 500;
        if (ui_data_.LeftWheel_RPM < 0) ui_data_.LeftWheel_RPM = 0;
        break;
      case 2:
        ui_data_.RightWheel_RPM -= 500;
        if (ui_data_.RightWheel_RPM < 0) ui_data_.RightWheel_RPM = 0;
        break;
    }
    xQueueSendFromISR(UI_Port, &ui_data_, 0);
  }
}

void UI_Set_Friction(int* UpWheel_RPM, int* leftWheel_RPM,
                     int* rightWheel_RPM) {
  // *UpWheel_RPM = ui_data_.UpWheel_RPM;
  // *leftWheel_RPM = ui_data_.LeftWheel_RPM;
  // *rightWheel_RPM = ui_data_.RightWheel_RPM;
}

UI_DATA_t UI_Set_POS(XboxControllerData_t* command) {
  //   float vx = (int)((command->joyRHori - 32768) * SPEED_MAX / 32767 *
  //   LCD_COEFF * 0.01); float vy = (int)((command->joyRVert - 32768) *
  //   SPEED_MAX / 32767 * LCD_COEFF * 0.01);
  ui_data_.UI_pos_last[0] = ui_data_.UI_pos[0];
  ui_data_.UI_pos_last[1] = ui_data_.UI_pos[1];
  if (command->joyRHori > 32768)
    ui_data_.UI_pos[0]++;
  else if (command->joyRHori < 32768)
    ui_data_.UI_pos[0]--;
  if (command->joyRVert > 32768)
    ui_data_.UI_pos[1]--;
  else if (command->joyRVert < 32768)
    ui_data_.UI_pos[1]++;

  // 出界限制
  if (ui_data_.UI_pos[0] > 154)
    ui_data_.UI_pos[0] = 154;
  else if (ui_data_.UI_pos[0] < 6)
    ui_data_.UI_pos[0] = 6;

  if (ui_data_.UI_pos[1] > 122)
    ui_data_.UI_pos[1] = 122;
  else if (ui_data_.UI_pos[1] < 6)
    ui_data_.UI_pos[1] = 6;

  ui_data_.world_pos_last[0] = ui_data_.world_pos[0];
  ui_data_.world_pos_last[1] = ui_data_.world_pos[1];

  ui_data_.world_pos[0] = ui_data_.UI_pos[0] * LCD_COEFF;
  ui_data_.world_pos[1] = ui_data_.UI_pos[1] * LCD_COEFF;

  ui_data_.UI_Tensile_last = ui_data_.UI_Tensile;
  if (command->btnDirRight && !command->btnDirRight_last)
    ui_data_.UI_Tensile += 1;
  else if (command->btnDirUp && !command->btnDirUp_last)
    ui_data_.UI_Tensile -= 1;

  if (ui_data_.UI_Tensile > Tensile_MAX)
    ui_data_.UI_Tensile = 30;
  else if (ui_data_.UI_Tensile < 0)
    ui_data_.UI_Tensile = 0;
  xQueueSendFromISR(UI_Port, &ui_data_, 0);
  return ui_data_;
}

void UI_Show_Location(float pos_x, float pos_y) {
  int integer = (int)pos_x;
  int decimals = (int)((pos_x - integer) * 100);  // 两位小数
  uint8_t integer_len;
  if (integer < 10)
    integer_len = 1;
  else if (integer < 100 && integer > 9)
    integer_len = 2;

  LCD_Fill(122, 5, 160, 40, WHITE);  // 清除显示区域

  Show_Str(105, 5, BLACK, WHITE, "x:", 20, 1);

  switch (integer_len) {
    case 1:
      LCD_ShowNum(122, 5, integer, integer_len, 20);  // 显示整数部分
      Show_Str(130, 5, BLACK, WHITE, ".", 20, 1);

      if (decimals < 10) LCD_ShowNum(135, 5, 0, 1, 20);
      LCD_ShowNum(135, 5, decimals, 2, 20);  // 显示小数部分
      break;

    case 2:
      LCD_ShowNum(122, 5, integer, integer_len, 15);  // 显示整数部分
      Show_Str(135, 5, BLACK, WHITE, ".", 20, 1);
      break;
  }

  integer = (int)pos_y;
  decimals = (int)((pos_y - integer) * 100);  // 两位小数

  if (integer < 10)
    integer_len = 1;
  else if (integer < 100 && integer > 9)
    integer_len = 2;

  Show_Str(105, 25, BLACK, WHITE, "y:", 20, 1);

  switch (integer_len) {
    case 1:
      LCD_ShowNum(122, 25, integer, integer_len, 20);  // 显示整数部分
      Show_Str(130, 25, BLACK, WHITE, ".", 20, 1);

      if (decimals < 10) LCD_ShowNum(135, 25, 0, 1, 20);
      LCD_ShowNum(135, 25, decimals, 2, 20);  // 显示小数部分
      break;

    case 2:
      LCD_ShowNum(122, 25, integer, integer_len, 15);  // 显示整数部分
      Show_Str(135, 25, BLACK, WHITE, ".", 20, 1);
      break;
  }
}

void UI_Show_Tensile(double distance) {
  uint8_t integer = (int)distance;
  uint8_t decimals = (int)((distance - integer) * 100);  // 两位小数
  uint8_t integer_len;
  if (integer < 10)
    integer_len = 1;
  else if (integer < 100 && integer > 9)
    integer_len = 2;

  LCD_Fill(122, 50, 160, 85, WHITE);  // 清除显示区域
  Show_Str(105, 50, BLACK, WHITE, "d:", 20, 1);

  switch (integer_len) {
    case 1:
      LCD_ShowNum(122, 50, integer, integer_len, 20);  // 显示整数部分
      Show_Str(130, 50, BLACK, WHITE, ".", 20, 1);
      if (decimals != 0)
        LCD_ShowNum(135, 50, decimals, 1, 20);  // 显示小数部分
      else
        LCD_ShowNum(135, 50, 0, 1, 20);
      break;

    case 2:
      LCD_ShowNum(122, 50, integer, integer_len, 15);  // 显示整数部分
      Show_Str(135, 50, BLACK, WHITE, ".", 20, 1);
      if (decimals != 0)
        LCD_ShowNum(145, 50, decimals, 1, 20);  // 显示小数部分
      else
        LCD_ShowNum(145, 50, 0, 1, 20);
      break;
  }

  // LCD_ShowNum(135, 50, 0, 1, 20);
}

void UI_Place(void) {
  gui_circle(16, 64, RED, 4, 0);  // 篮筐
  LCD_DrawLine(10, 50, 10, 78);
  LCD_DrawLine(0, 44, 40, 44);
  LCD_DrawLine(40, 44, 40, 84);
  LCD_DrawLine(0, 84, 40, 84);
  gui_circle(16, 64, BLACK, 50, 0);  // 3
  LCD_DrawLine(0, 14, 16, 14);
  LCD_DrawLine(0, 114, 16, 114);
  LCD_DrawLine(120, 0, 120, 128);
}

void UI_Friction_Wheel(UI_DATA_t* ui_data) {
  // gui_circle();
  gui_circle(80, 28, RED, 20, 0);
  gui_circle(40, 65, RED, 20, 0);
  gui_circle(120, 65, RED, 20, 0);

  if (ui_data->Cursor == 0) {
    LCD_ShowNum(66, 20, ui_data_.UpWheel_RPM, 4, 20);
    LCD_ShowNum(26, 56, ui_data_.LeftWheel_RPM, 4, 14);
    LCD_ShowNum(106, 56, ui_data_.RightWheel_RPM, 4, 14);
  } else {
    LCD_ShowNum(66, 20, ui_data_.UpWheel_RPM, 4, 14);
    LCD_ShowNum(26, 56, ui_data_.LeftWheel_RPM, 4, 20);
    LCD_ShowNum(106, 56, ui_data_.RightWheel_RPM, 4, 20);
  }
}

void UI_Path(UI_DATA_t* ui_data) {
  UI_Place();
  //   ui_data_.UI_pos[0] = (int)ui_data_.world_pos[0] / LCD_COEFF;
  //   ui_data_.UI_pos[1] = (int)ui_data_.world_pos[1] / LCD_COEFF;
  if (ui_data_.UI_pos[0] != ui_data_.UI_pos_last[0] ||
      ui_data_.UI_pos[1] != ui_data_.UI_pos_last[1]) {
    gui_circle(ui_data_.UI_pos_last[0], ui_data_.UI_pos_last[1], WHITE, 14,
               1);  // 跟车覆盖轨迹
    gui_circle(ui_data_.UI_pos[0], ui_data_.UI_pos[1], RED, 6, 0);  // 车
  }

  if (ui_data_.world_pos[0] != ui_data_.world_pos_last[0] ||
      ui_data_.world_pos[1] != ui_data_.world_pos_last[1]) {
    UI_Show_Location(ui_data_.world_pos[0], ui_data_.world_pos[1]);
  }

  if (ui_data_.UI_Tensile != ui_data_.UI_Tensile_last)
    UI_Show_Tensile(ui_data_.UI_Tensile);
}

void UI_Show(UI_DATA_t* ui_data) { UI_Path(ui_data); }







