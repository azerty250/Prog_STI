#ifndef motor_control_h
#define motor_control_h

#include "Encoder.h" 

#define E1  5    //M1 Speed Control
#define E2  6    //M2 Speed Control
#define M1  4    //M1 Direction Control
#define M2  7    //M1 Direction Control

void set_speed(int left, int right);

void init_motor();

//returns the current count on the right encoder
float get_diff_enc_right();

void reset_enc_right();

//returns the current count on the left encoer
float get_diff_enc_left();

void reset_enc_left();

//Blocks the maximum speed at 200 (because we have a voltage of 15V)
void max_min_speed(int* speed_left, int* speed_right);

void motor_control_main(void);

//Shakes the robot to make the bottles fall
void shake();


#endif
