
#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include <stdbool.h>
#include "data_pool.h"

#define LINE_1_SET GPIOA->BSRR =1<<7
#define LINE_2_SET GPIOC->BSRR =1<<9
#define LINE_3_SET GPIOC->BSRR =1<<8
#define LINE_4_SET GPIOC->BSRR =1<<7

#define LINE_1_RESET GPIOA->BSRR =(1<<7)<<16
#define LINE_2_RESET GPIOC->BSRR =(1<<9)<<16
#define LINE_3_RESET GPIOC->BSRR =(1<<8)<<16
#define LINE_4_RESET GPIOC->BSRR =(1<<7)<<16

// #define LINE_1_SET HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
// #define LINE_2_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
// #define LINE_3_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET)
// #define LINE_4_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET)

// #define LINE_1_RESET HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
// #define LINE_2_RESET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
// #define LINE_3_RESET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET)
// #define LINE_4_RESET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET)

#define high 1
#define low  0

typedef struct
{
    // 按键数据（bool类型）
    bool btnY;
    bool btnY_last;
    bool btnB;
    bool btnB_last;
    bool btnA;
    bool btnA_last;
    bool btnX;
    bool btnX_last;
    bool btnStart;
    bool btnStart_last;
    bool btnXbox;
    bool btnXbox_last;
    bool btnLB;
    bool btnLB_last;
    bool btnRB;
    bool btnRB_last;
    bool btnLS;
    bool btnLS_last;
    bool btnRS;
    bool btnRS_last;
    bool btnDirUp;
    bool btnDirUp_last;
    bool btnDirLeft;
    bool btnDirLeft_last;
    bool btnDirRight;
    bool btnDirRight_last;
    bool btnDirDown;
    bool btnDirDown_last;

    bool SWA;
    bool SWB;

    // 霍尔值（16位数值）
    uint16_t joyLHori;
    uint16_t joyLVert;
    uint16_t joyRHori;
    uint16_t joyRVert;
    uint16_t trigLT;
    uint16_t trigRT;

    float joyLHori_map;
    float joyLVert_map;
    float joyRHori_map;
    float joyRVert_map;
    float trigLT_map;
    float trigRT_map;
    
} XboxControllerData_t;

typedef struct
{
    // 按键数据（bool类型）
    bool btnY;
    bool btnB;
    bool btnA;
    bool btnX;
    bool btnStart;
    bool btnXbox;
    bool btnLB;
    bool btnRB;
    bool btnLS;
    bool btnRS;
    bool btnDirUp;
    bool btnDirLeft;
    bool btnDirRight;
    bool btnDirDown;

    // 霍尔值（16位数值）
    uint16_t joyLHori;
    uint16_t joyLVert;
    uint16_t joyRHori;
    uint16_t joyRVert;
    uint16_t trigLT;
    uint16_t trigRT;
}XboxData_t;



void key_read(XboxControllerData_t *data);
#endif