#include "motor_move.h"
#include "basic_function.h"
#include "Clock.h"
#include "graph_structure.h"

void right_rotate_time(int time){
    // 5000ms 180'
    // 2800ms 180
    Left_Forward();
    Right_Backward();
    Move(3000, 3000);
    Clock_Delay1ms(time);
    Move(0, 0);
}

void left_rotate_time(int time){
    Left_Backward();
    Right_Forward();
    Move(3000, 3000);
    Clock_Delay1ms(time);
    Move(0, 0);
}

void right_rotate_slight(void){
    Left_Forward();
    Right_Forward();
    Move(3000, 0);
    Clock_Delay1ms(10);
}

void left_rotate_slight(void){
    Left_Forward();
    Right_Forward();
    Move(0, 3000);
    Clock_Delay1ms(10);
}

void move_forward_slight(){
    Left_Forward();
    Right_Forward();
    Move(2000, 2000);
    Clock_Delay1ms(300);
}

void move_backward_slight(){
    Left_Backward();
    Right_Backward();
    Move(1600, 1600);
    Clock_Delay1ms(300);
}

void move_forward_time(int time){
    Left_Forward();
    Right_Forward();
    Move(4000, 4000);
    Clock_Delay1ms(time);
}

void move_backward_time(int time){
    Left_Backward();
    Right_Backward();
    Move(2500, 2500);
    Clock_Delay1ms(time);
//    Move(0, 0);
}

void correct_line(int sensor_straight){
    if(sensor_straight == 0x08){
        // 00001000
        Led_Off();
        right_rotate_slight();
        P2->OUT |= 0X04;
    }
    else if(sensor_straight == 0x10){
        // 00010000
        Led_Off();
        left_rotate_slight();
        P2->OUT |= 0X04;
    }
}

int read_sensor(void){
    Clock_Delay1us(10);
    Sensor_on();
    int sensor;
    // Read P7.0-P7.7 Input
    // white : 0, black : 1
    sensor = P7->IN & 0xFF;
    Sensor_off();
    return sensor;
}

void start_from_start_line(void){
    int sw1, sensor_start;
    while(1){
       sw1 = P1->IN & 0x02;
       if(sw1){
           sensor_start = read_sensor() & sensor_start_mask;
           if(sensor_start == sensor_start_mask){
               Led_Off();
               P2->OUT |= 0X02;
           }
           else if(sensor_start == 0x00){
               Led_Off();
               P2->OUT |= 0X01;
           }
           else{
               Led_Off();
           }
       }
       else break;
       Clock_Delay1ms(10);
    }
    Clock_Delay1ms(2000);
    move_forward_time(400);
    int sensor_straight, sensor_point, sensor;
    while(1){
        sensor = read_sensor();
        sensor_straight = sensor & sensor_straight_mask;
        sensor_point = sensor & sensor_point_mask;
        // there exist 11111111 black line
        if(sensor_point == sensor_point_mask){
            Led_Off();
            P2->OUT |= 0X02;
            break;
        }
        else if(sensor_straight == sensor_straight_mask){
            Led_Off();
            move_forward_time(50);
            Clock_Delay1ms(10);
        }
        else break;
    }
    move_forward_time(150);
    Move(0, 0);
}

void start_to_end_line(void){
    move_forward_time(200);
    int sensor_straight, sensor_start, sensor;
    while(1){
        sensor = read_sensor();
        sensor_straight = sensor & sensor_straight_mask;
        sensor_start = sensor & sensor_start_mask;
        if(sensor_start == sensor_start_mask){
            Led_Off();
            Move(0, 0);
            P2->OUT |= 0X02;
            break;
        }
        else if(sensor_straight == sensor_straight_mask){
            Led_Off();
            move_forward_time(50);
            Clock_Delay1ms(10);
        }
        else if(sensor_straight == 0x08 || sensor_straight == 0x10){
            correct_line(sensor_straight);
        }
        else if(sensor_straight == 0x00){
            // 00000000
            Led_Off();
            Move(0, 0);
            P2->OUT |= 0X01;
            break;
        }
        else break;
    }
}

void move_straight_to_point(void){
    move_forward_time(200);
    int sensor_straight, sensor, sensor_point;
    while(1){
        sensor = read_sensor();
        sensor_straight = sensor & sensor_straight_mask;
        // there exist 11111111 black line so cannot judge sensor_point_mask
        if(sensor_straight == sensor_straight_mask){
            Led_Off();
            move_forward_time(50);
            Clock_Delay1ms(10);
        }
        else if(sensor_straight == 0x08 || sensor_straight == 0x10){
            correct_line(sensor_straight);
        }
        else if(sensor_straight == 0x00){
            // 00000000
            Led_Off();
            Move(0, 0);
            P2->OUT |= 0X01;
            break;
        }
        else break;
    }
//    while(1){
//        sensor = read_sensor();
//        sensor_point = sensor & sensor_point_mask;
//        // there exist 11111111 black line
//        if(sensor_point == sensor_point_mask){
//            Led_Off();
//            Move(0, 0);
//            P2->OUT |= 0X02;
//            break;
//        }
//        else{
//            Led_Off();
//            move_backward_slight();
//            P2->OUT |= 0X01;
//        }
//    }
//    move_forward_slight();
//    Move(0, 0);
}

//void rotate_find_edges(int current_angle){
//    int i, sensor, sensor_left, sensor_right;
//    int edge_number = 0;
//    Point new_point;
//    for(i=0;i<8;i++){
//        sensor = read_sensor();
//        sensor_left = sensor & sensor_left_mask;
//        sensor_right = sensor & sensor_right_mask;
//        int left_egde = (current_angle+90)%360/45;
//        if(sensor_left != 0x00){
//            new_point.edge_absolute_angle[left_egde] = 1;
//            Led_Off();
//            P2->OUT |= 0X04;
//        }
//        else{
//            new_point.edge_absolute_angle[left_egde] = 0;
//            Led_Off();
//            P2->OUT |= 0X01;
//        }
//        left_rotate_time(1250);
//        Clock_Delay1ms(200);
//        current_angle = (current_angle + 45) % 360;
//    }
//}

void rotate_with_given_angle(int current_angle, int edge_angle){
    int angle_diff = (edge_angle - current_angle + 360) % 360;
    if (angle_diff <= 180) {
        left_rotate_time(angle_diff*2100/180);
    } else {
        right_rotate_time((360 - angle_diff)*2100/180);
    }
    int sensor, sensor_straight;
    while(1){
        sensor = read_sensor();
        sensor_straight = sensor & sensor_straight_mask;
        // there exist 11111111 black line so cannot judge sensor_point_mask
        if(sensor_straight != 0x00){
            Move(0, 0);
            Led_Off();
            break;
        }
        else{
            if (angle_diff <= 180) {
                left_rotate_time(10);
            } else {
                right_rotate_time(10);
            }
        }
    }
}


