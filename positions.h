#ifndef _POSITIONS_H_
#define _POSITIONS_H_

#include <avr/io.h>

// Alap beálláshoz használt időzítő állandók
#define INIT_POSITION_BASE_TIME 4500
#define INIT_POSITION_1_TIME 1500U
#define INIT_POSITION_2_TIME 300U
#define INIT_POSITION_3_TIME 1000U
#define INIT_POSITION_JAW_TIME 400U

struct position {
    uint16_t base;
    uint16_t joint_1;
    uint16_t joint_2;
    uint16_t joint_3;
    uint16_t jaw;
} current_positions;

#endif