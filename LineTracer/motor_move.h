#ifndef MOTOR_MOVE_H
#define MOTOR_MOVE_H

void right_rotate_time(int time);
void left_rotate_time(int time);
void right_rotate_slight(void);
void left_rotate_slight(void);
void move_forward_slight(void);
void move_backward_slight(void);
void move_forward_time(int time);
void move_backward_time(int time);
void correct_line(int sensor_straight);
int read_sensor(void);
void start_from_start_line(void);
void start_to_end_line(void);
void move_straight_to_point(void);
void rotate_find_edges(int current_angle);
void rotate_with_given_angle(int current_angle, int edge_angle);

#endif // MOTOR_MOVE_H
