#ifndef dyn_control_h
#define dyn_control_h

#include <DynamixelSerial.h>

#define DYMX_ID_LEFT 11
#define DYMX_ID_RIGHT 5
#define SCAN_MAX_ANGLE 900
#define SCAN_SPEED 10
#define SCAN_SIZE  20
#define UP_ANGLE 700
#define DOWN_ANGLE 500

void init_dyn();

void front_up();

void front_down();

#endif