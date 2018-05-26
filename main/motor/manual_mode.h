#ifndef manual_mode_h
#define manual_mode_h

#include "motor_control.h"

#define PIN_RX_1 32 // pin for input num * from receiver
#define PIN_RX_2 30 // (*=1,2,3,4)
#define PIN_RX_3 28
#define PIN_RX_4 26

#define RX_1 0 // index for memory for input num * from receiver
#define RX_2 1 // (*=1,2,3,4)
#define RX_3 2
#define RX_4 3 

void manual_mode_main(int verbose);
void init_manual_mode();


#endif
