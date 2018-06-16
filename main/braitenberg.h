#include "hcsr04.h"

#define SENSOR_NUMBER 6
#define ENABLE_US     50

void init_us_sensors();

void get_brait_speed(int* speed_right, int* speed_left, int sign, int* brait_active);

