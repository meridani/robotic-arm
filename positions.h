#ifndef _POSITIONS_H_
#define _POSITIONS_H_

#include <avr/io.h>

// Position b (default position)
#define INIT_POSITION_BASE_TIME 6850U
#define INIT_POSITION_1_TIME 2000U
#define INIT_POSITION_2_TIME 000U
#define INIT_POSITION_3_TIME 1000U
#define INIT_POSITION_JAW_TIME 350U

struct position {
    uint16_t base;
    uint16_t joint_1;
    uint16_t joint_2;
    uint16_t joint_3;
    uint16_t jaw;
} target_positions, current_positions;

static struct position robot_positions[] = {
    { INIT_POSITION_BASE_TIME, INIT_POSITION_1_TIME, INIT_POSITION_2_TIME, INIT_POSITION_3_TIME, INIT_POSITION_JAW_TIME },
    { 2000, 4500, 2400, 0, 0 },
    { 9000, 0, 0, 0, 0 },
    { 7000, 0, 0, 0, 0 },
};

#endif