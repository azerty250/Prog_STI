#ifndef dyn_control_h
#define dyn_control_h

#include "DynamixelSerial.h"
#include <Arduino.h>

#define DYMX_ID_LEFT 11
#define DYMX_ID_RIGHT 5
#define DYMX_ID_DOOR 7
#define SCAN_MAX_ANGLE 900
#define SCAN_SPEED 10
#define SCAN_SIZE  20
#define UP_ANGLE 1000
#define DOWN_ANGLE 50

void init_dyn();

void front_up();

void front_down();

void open_gate();

void close_gate();

#endif
