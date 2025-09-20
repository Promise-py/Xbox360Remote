
#include "UI.h"

UI_DATA_t ui_data_;
float pixel_pos[2];
bool UI_Flag = false;
uint8_t last_ros_flag = 10;
uint8_t last_lock_finish = 10;
uint8_t last_camera_flag = 10;

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

  UI_ROS_DATA_t ui_ros_data;
  if (xQueueReceive(UI_RosPORT, &ui_ros_data, 0) == pdPASS) {
    ui_data_.yolo_data_last = ui_data_.yolo_data;
    ui_data_.lock_finish_flag = ui_ros_data.lock_finish_flag;
    ui_data_.camera_flag = ui_ros_data.camera_flag;
    ui_data_.yolo_data = ui_ros_data.yolo_data;
  }

  if (ui_ros_data.lock_mode_flag == 1)
    ui_data_.UI_Flag = manual_mode;
  else if (ui_ros_data.lock_mode_flag == 0)
    ui_data_.UI_Flag = init_mode;
  else if (ui_ros_data.lock_mode_flag == 2)
    ui_data_.UI_Flag = lock_mode;
  else if (ui_ros_data.lock_mode_flag == 3)
    ui_data_.UI_Flag = pass_mode;
  else
    ui_data_.UI_Flag = Finsih_Init;

  /* 摇杆控制屏幕中的ui小车 */

  // if (ui_data_.UI_Flag = Finsih_Init) {
  //   ui_data_.UI_pos_last[0] = ui_data_.UI_pos[0];
  //   ui_data_.UI_pos_last[1] = ui_data_.UI_pos[1];
  //   if (command->joyRHori > 32768)
  //     ui_data_.UI_pos[0]++;
  //   else if (command->joyRHori < 32768)
  //     ui_data_.UI_pos[0]--;
  //   if (command->joyRVert > 32768)
  //     ui_data_.UI_pos[1]--;
  //   else if (command->joyRVert < 32768)
  //     ui_data_.UI_pos[1]++;

  // // 出界限制
  // if (ui_data_.UI_pos[0] > 154)
  //   ui_data_.UI_pos[0] = 154;
  // else if (ui_data_.UI_pos[0] < 6)
  //   ui_data_.UI_pos[0] = 6;

  // if (ui_data_.UI_pos[1] > 122)
  //   ui_data_.UI_pos[1] = 122;
  // else if (ui_data_.UI_pos[1] < 6)
  //   ui_data_.UI_pos[1] = 6;

  // ui_data_.world_pos_last[0] = ui_data_.world_pos[0];
  // ui_data_.world_pos_last[1] = ui_data_.world_pos[1];

  // ui_data_.world_pos[0] = ui_data_.UI_pos[0] * LCD_COEFF;
  // ui_data_.world_pos[1] = ui_data_.UI_pos[1] * LCD_COEFF;

  // ui_data_.UI_Tensile_last = ui_data_.UI_Tensile;
  // if (command->btnDirRight && !command->btnDirRight_last)
  //   ui_data_.UI_Tensile += 1;
  // else if (command->btnDirUp && !command->btnDirUp_last)
  //   ui_data_.UI_Tensile -= 1;

  // if (ui_data_.UI_Tensile > Tensile_MAX)
  //   ui_data_.UI_Tensile = 30;
  // else if (ui_data_.UI_Tensile < 0)
  //   ui_data_.UI_Tensile = 0;
  // }

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

void UI_Carmera(UI_DATA_t* ui_data) {
  static uint8_t lcd_clear = 0;
  static uint8_t motor_flag = 0;
  static uint8_t draw_flag = 0;
  static uint8_t camera_flag_last = 0;
  if (!lcd_clear) {
    LCD_Clear(WHITE);
    lcd_clear = 1;
  }

  if (ui_data->camera_flag) {
    draw_flag = 0;
    if (ui_data->camera_flag != camera_flag_last) {
      LCD_Clear(WHITE);
      camera_flag_last = ui_data->camera_flag;
    }

    if (ui_data->lock_finish_flag) {
      if (!motor_flag) {
        Show_Str(16, 20, RED, WHITE, "Finish locking", 20, 1);

        // 电击震动
        HAL_GPIO_WritePin(MOTOR1_GPIO_Port, MOTOR1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR2_GPIO_Port, MOTOR2_Pin, GPIO_PIN_SET);
        HAL_Delay(1000);  // 等待1秒
        HAL_GPIO_WritePin(MOTOR1_GPIO_Port, MOTOR1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR2_GPIO_Port, MOTOR2_Pin, GPIO_PIN_RESET);
        motor_flag = 1;
      }
    } else {
      LCD_Fill(16, 20, 35, 50, WHITE);  // 清除显示finish区域
      motor_flag = 0;
    }

    // if (ui_data->yolo_data_last != ui_data->yolo_data) {
    //   LCD_Fill(16, 60, 160, 85, WHITE);  // 清除显示yolo区域
    //   Show_Str(16, 60, BLACK, WHITE, "Yolo error:", 20, 1);
    //   if (ui_data->yolo_data < 10)
    //     LCD_ShowNum(100, 60, (int)ui_data->yolo_data, 1, 20);
    //   else if (ui_data->yolo_data < 100 && ui_data->yolo_data > 9)
    //     LCD_ShowNum(100, 60, (int)ui_data->yolo_data, 2, 20);
    //   else if (ui_data->yolo_data < 1000 && ui_data->yolo_data > 99)
    //     LCD_ShowNum(100, 60, (int)ui_data->yolo_data, 3, 20);
    // }
  } else  // 没看到篮筐
  {
    if (ui_data->camera_flag != camera_flag_last) {
      LCD_Clear(WHITE);
      camera_flag_last = ui_data->camera_flag;
    }
    if (!draw_flag) {
      Gui_Drawbmp16(60, 50, 1);
      draw_flag = 1;
    }
  }
}

void UI_Init(UI_DATA_t* ui_data) {
  if (ui_data->UI_Flag == SW_Init) {
    GUI_DrawFont16(16, 47, BLACK, WHITE, "??", 1);
    GUI_DrawFont16(56, 63, BLACK, WHITE, "?", 1);
  } else if (ui_data->UI_Flag == Joy_Init) {
    GUI_DrawFont16(32, 47, BLACK, WHITE, "3", 1);
    GUI_DrawFont16(24, 63, BLACK, WHITE, "4", 1);
  }
}

void UI_Show(UI_DATA_t* ui_data) {
  if (!UI_Flag) {
    if (ui_data->UI_Flag == SW_Init) {
      GUI_DrawFont16(16, 47, BLACK, WHITE, "??", 1);
      GUI_DrawFont16(56, 63, BLACK, WHITE, "?", 1);
      ui_data->UI_Flag_last = SW_Init;
    } else if (ui_data->UI_Flag == Joy_Init) {
      LCD_Clear(WHITE);
      GUI_DrawFont16(32, 47, BLACK, WHITE, "3", 1);
      GUI_DrawFont16(24, 63, BLACK, WHITE, "4", 1);
    } else if (ui_data->UI_Flag == Finsih_Init &&
               ui_data->UI_Flag_last == SW_Init) {
      LCD_Clear(WHITE);
      UI_Flag = true;
      ui_data->UI_Flag_last = Finsih_Init;
    }
  } else {
    if (ui_data->UI_Flag == Finsih_Init);
      // LCD_Clear(WHITE);
    // UI_Path(ui_data)
    // else if (ui_data->UI_Flag == Lock_Basket)
    //   UI_Carmera(ui_data);
  }
}

void UI_show_state(UI_ROS_DATA_t *ui_ros)
{

  if (ui_ros->lock_mode_flag == init_mode && last_ros_flag!= init_mode) 
  {
    LCD_Clear(WHITE);
    GUI_DrawFont16(24, 63, BLACK, WHITE, "5", 1);//舵轮校准中
    last_ros_flag= init_mode;
  }
  else if(ui_ros->lock_mode_flag == manual_mode && last_ros_flag!= manual_mode) 
  {
    LCD_Clear(WHITE);
    GUI_DrawFont16(24, 63, BLACK, WHITE, "6", 1);//手动模式
    last_ros_flag= manual_mode;
  }
  else if(ui_ros->lock_mode_flag == lock_mode && last_ros_flag!= lock_mode) 
  {
    LCD_Clear(WHITE);
    GUI_DrawFont16(24, 63, BLACK, WHITE, "7", 1);//自动锁框
    last_ros_flag= lock_mode;
  }
  else if(ui_ros->lock_mode_flag == pass_mode && last_ros_flag!= pass_mode) 
  {
    LCD_Clear(WHITE);
    GUI_DrawFont16(24, 63, BLACK, WHITE, "8", 1);//传球模式
    last_ros_flag= pass_mode;
  }
  
  if(ui_ros->lock_finish_flag==1 && last_lock_finish!= ui_ros->lock_finish_flag)
  {
    LCD_Clear(WHITE);
    Show_Str(16, 20, RED, WHITE, "Finish locking", 20, 1);
    // 电击震动
    HAL_GPIO_WritePin(MOTOR1_GPIO_Port, MOTOR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR2_GPIO_Port, MOTOR2_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);  // 等待1秒
    HAL_GPIO_WritePin(MOTOR1_GPIO_Port, MOTOR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR2_GPIO_Port, MOTOR2_Pin, GPIO_PIN_RESET);
    last_lock_finish = ui_ros->lock_finish_flag;
  }
  else if(ui_ros->lock_finish_flag==0 && last_lock_finish!= ui_ros->lock_finish_flag)
  {
    LCD_Fill(16, 20, 35, 50, WHITE);  // 清除显示finish区域
    last_lock_finish = ui_ros->lock_finish_flag;
  }
  
  if(ui_ros->camera_flag)
  {
    if(!last_camera_flag)
    {
      LCD_Fill(120, 1, 160, 41, WHITE); // 清除显示区域
      last_camera_flag = ui_ros->camera_flag;
    }
    Gui_Drawbmp16(120,1,1);//显示相机
  }
  else
  {
    if(last_camera_flag)
    {
      LCD_Fill(120, 1, 160, 41, WHITE); // 清除显示区域
      last_camera_flag = ui_ros->camera_flag;
    }
    Gui_Drawbmp16(120,1,2);//显示禁止
  }
}
