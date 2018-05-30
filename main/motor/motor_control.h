#ifndef motor_control_h
#define motor_control_h

#include "Encoder.h"

#define E1  5    //M1 Speed Control
#define E2  6    //M2 Speed Control
#define M1  4    //M1 Direction Control
#define M2  7    //M1 Direction Control

void set_speed(char left, char right);

void init_motor();

long get_diff_enc_right();

void reset_enc_right();

long get_diff_enc_left();

void reset_enc_left();	

void motor_control_main(void);



#endif