#ifndef BASIC_FUNCTION_H
#define BASIC_FUNCTION_H

#include "msp.h"

// sensor mask
#define sensor_start_mask 0xFF // 11111111
#define sensor_straight_mask 0x18 // 00011000
#define sensor_point_mask 0x3C // 00111100
#define sensor_left_mask 0xC0 // 11000000
#define sensor_right_mask 0x03 // 00000011

// LED Functions
void Led_Init(void);
void Led_On(void);
void Led_Off(void);

// Switch Control Functions
void Switch_Init(void);

// QTRX Sensor Functions
void Sensor_Init(void);
void Sensor_on(void);
void Sensor_off(void);

// SysTick Timer Functions
void SysTick_Init(void);
void SysTick_Wait1us(int delay);
void SysTick_Wait1ms(void);
void SysTick_Wait1s(void);

// MOTOR Functions
void Motor_Init(void);
void Move(uint16_t leftDuty, uint16_t rightDuty);
void Left_Forward(void);
void Left_Backward(void);
void Right_Forward(void);
void Right_Backward(void);

// Timer Function
void TimerA2_Init(void(*task)(void), uint16_t period);
//void TA2_0_IRQHandler(void);

// PWM Functions
void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4);
void PWM_Duty3(uint16_t duty3);
void PWM_Duty4(uint16_t duty4);

#endif // BASIC_FUNCTION_H
