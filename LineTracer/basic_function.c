#include "msp.h"
#include "Clock.h"
#include "basic_function.h"

// LED
void Led_Init(void){
    // Setup P2 0-2 bit & P2 0 bit as GPIO
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;
    // Setup P2 0-2 bit & P2 0 bit as OUTPUT
    P2->DIR |= 0x07;
    // Turn off all the LEDs initially
    P2->OUT &= ~0x07;
}
void Led_On(void){
    // red
    P2->OUT |= 0X01;
    // green
    P2->OUT |= 0X02;
    // blue
    P2->OUT |= 0X04;
}

void Led_Off(void){
    // Turn off all the LEDs
    P2->OUT &= ~0x07;
}

// Switch Control
void Switch_Init(void){
    // Setup Switch as GPIO
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;
    // Setup Switch as Input
    P1->DIR &= ~0x12;
    // Enable pull up resistors
    P1->REN |= 0x12;
    // Now pull-up
    P1->OUT |= 0x12;
}

//void Switch_on_main(void){
//    int sw1;
//    int prev_state = 0;
//    sw1 = P1->IN & 0x02;
//    if (!sw1 && prev_state == 0) {
//        prev_state = 1;
//    }
//    else {
//        prev_state = 0;
//    }
//}

// QTRX Sensor
void Sensor_Init(void){
    // 0,2,4,6 IR Emitter
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;  // GPIO
    P5->DIR |= 0x08;    // OUTPUT
    P5->OUT &= ~0x08;   // turn off 4 even IR LEDs

    // 1,3,5,7 IR Emitter
    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;  // GPIO
    P9->DIR |= 0x04;    // OUTPUT
    P9->OUT &= ~0x04;   // turn off 4 odd IR LEDs

    // 0-7 IR Sensor
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;  // GPIO
    P7->DIR &= ~0xFF;   // INPUT
}

void Sensor_on(void){
    // Turn on IR LEDs
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;
    // Make P7.0-P7.7 as output
    P7->DIR = 0xFF;
    // Charges a capacitor
    P7->OUT = 0xFF;
    // Wait for fully charged
    Clock_Delay1us(10);
    // Make P7.0-P7.7 as input
    P7->DIR = 0x00;
    // Wait for a while
    Clock_Delay1us(1000);
//    SysTick_Wait1us(1000);
}

void Sensor_off(void){
        // Turn off IR LEDs
    P5->OUT &= ~0x08;
    P9->OUT &= ~0x04;
    Clock_Delay1ms(10);
}

//void Sensor_main(void){
//    while(1) {
//        Sensor_on();
//        int sensor;
//        // Read P7.0-P7.7 Input
//        // white : 0, black : 1
//        sensor = P7->IN & 0x10;
//        if (sensor) {
//            P2->OUT |= 0x01;
//        } else {
//            P2->OUT &= ~0x07;
//        }
//        Sensor_off();
//    }
//}

// SysTick Timer
void SysTick_Init(void){
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL = 0x00000005;
}

void SysTick_Wait1us(int delay){
    SysTick->LOAD = 48 * delay;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0){};
}

void SysTick_Wait1ms(){
    SysTick->LOAD = 48000;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0){};
}

void SysTick_Wait1s(void){
    int i, count = 1000;
    for(i = 0;i < count; i++){
        SysTick_Wait1ms();
    }
}

// MOTOR
void Motor_Init(void){
    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;  // 1) configure nSLPR & nSLPL as GPIO
    P3->DIR |= 0xC0;    // 2) make nSLPR & nSLPL as output
    P3->OUT &= ~0xC0;   // 3) output LOW

    P5->SEL0 &= ~0x30;
    P5->SEL1 &= ~0x30;  // 1) configure DIRR & DIRL as GPIO
    P5->DIR |= 0x30;    // 2) make DIRR & DIRL output
    P5->OUT &= ~0x30;   // 3) output LOW

    P2->SEL0 &= ~0xC0;
    P2->SEL1 &= ~0xC0;  // 1) configure PWMR & PWML as GPIO
    P2->DIR |= 0xC0;    // 2) make PWMR & PWML as output
    P2->OUT &= ~0xC0;   // 3) output LOW

    PWM_Init34(15000, 0, 0);
}

void Move(uint16_t leftDuty, uint16_t rightDuty) {
    P3->OUT |= 0xC0;
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}

void Left_Forward() {
    P5->OUT &= ~0x10;
}

void Left_Backward() {
    P5->OUT |= 0x10;
}

void Right_Forward() {
    P5->OUT &= ~0x20;
}

void Right_Backward() {
    P5->OUT |= 0x20;
}

//int Motor_main(void) {
//    Clock_Init48MHz();
//    Motor_Init();
//    while (1) {
//        // Move Forward
//        Left_Forward();
//        Right_Forward();
//        Move(2000, 2000);
//        Clock_Delay1ms(3000);
//
//        // Move Backward
//        Left_Backward();
//        Right_Backward();
//        Move(2000, 2000);
//        Clock_Delay1ms(3000);
//
//        // Rotate
//        Left_Forward();
//        Right_Backward();
//        Move(1500, 1500);
//        Clock_Delay1ms(3000);
//    }
//}

// Timer
void (*TimerA2Task)(void);
void TimerA2_Init(void(*task)(void), uint16_t period) {
    TimerA2Task = task;
    TIMER_A2->CTL = 0x0280;
    TIMER_A2->CCTL[0] = 0x0010;
    TIMER_A2->CCR[0] = (period - 1);
    TIMER_A2->EX0 = 0x0005;
    NVIC->IP[3] = (NVIC->IP[3] & 0xFFFFFF00) | 0x00000040;
    NVIC->ISER[0] = 0x00001000;
    TIMER_A2->CTL |= 0x0014;
}

void TA2_0_IRQHandler(void) {
    TIMER_A2->CCTL[0] &= ~0x0001;
    (*TimerA2Task)();
}

//int timer_main(void) {
//    Clock_Init48MHz();
//    TimerA2_Init(&task, 50000);
//    while(1) {};
//}

// PWM
void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
    P2->DIR |= 0xC0;
    P2->SEL0 |= 0xC0;
    P2->SEL1 &= ~0xC0;

    TIMER_A0->CCTL[0] = 0x800;
    TIMER_A0->CCR[0] = period;

    TIMER_A0->EX0 = 0x0000;

    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;

    TIMER_A0->CTL = 0x02F0;
}

void PWM_Duty3(uint16_t duty3) {
    TIMER_A0->CCR[3] = duty3;
}

void PWM_Duty4(uint16_t duty4) {
    TIMER_A0->CCR[4] = duty4;
}
