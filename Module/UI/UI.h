#ifndef __UI_H
#define __UI_H

#include <stdint.h>

#include "GUI.h"
#include "data_pool.h"
#include "lcd.h"

#define LCD_COEFF 0.0625  // 0.0625m等于一个像素点
#define SPEED_MAX 4.0     // m/s
#define Tensile_MAX 30    // 最大拉伸长度m
// // enum UI_SELECT { UP_WHEEL, LEFT_WHEEL, RIGHT_WHEEL };

typedef struct {
  int UpWheel_RPM;
  int LeftWheel_RPM;
  int RightWheel_RPM;
  uint8_t Cursor;

  float world_pos[2];
  float world_pos_last[2];
  int UI_pos[2];
  int UI_pos_last[2];
  float UI_Tensile;
  float UI_Tensile_last;

  uint8_t UI_Flag;
  uint8_t UI_Flag_last;

  uint8_t lock_mode_flag;
  uint8_t lock_finish_flag;  // 是否锁定篮筐
  uint8_t camera_flag;       // 视野中是否有篮筐
  float yolo_data;
  float yolo_data_last;

} UI_DATA_t;

enum UI_INIT_STATE {
  SW_Init,
  Joy_Init,
  Finsih_Init,
};

enum ROS_STATE { init_mode, manual_mode, lock_mode, pass_mode };

void UI_FSM(XboxControllerData_t* XboxData);

void UI_Set_Friction(int* UpWheel_RPM, int* leftWheel_RPM, int* rightWheel_RPM);
UI_DATA_t UI_Set_POS(XboxControllerData_t* command);
void UI_Place(void);
void UI_Friction_Wheel(UI_DATA_t* ui_data);
void UI_Path(UI_DATA_t* ui_data);
void UI_Show(UI_DATA_t* ui_data);

#endif  // UI_H
