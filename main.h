#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdbool.h>

#define BUILTIN_SW1 PB1
#define BUILTIN_SW2 PB2
#define BUILTIN_SW3 PB3
#define BUILTIN_SW4 PB4

#define ROBOT_READY_BIT (1 << PG1)
#define COMMAND_BIT_0 (1 << PF5)
#define COMMAND_BIT_1 (1 << PF6)
#define COMMAND_START_BIT (1 << PF7)

#define LED_G (1 << PC0)
#define LED_A (1 << PC1)
#define LED_F (1 << PC2)
#define LED_B (1 << PC3)
#define LED_E (1 << PC4)
#define LED_C (1 << PC5)
#define LED_D (1 << PC6)

#define LED_EMPTY 0x7F
#define LED_SEGMENT_0 ~(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F)
#define LED_SEGMENT_1 ~(LED_B | LED_C)
#define LED_SEGMENT_2 ~(LED_A | LED_B | LED_D | LED_E | LED_G)
#define LED_SEGMENT_3 ~(LED_A | LED_B | LED_C | LED_D | LED_G)
#define LED_SEGMENT_4 ~(LED_B | LED_C | LED_F | LED_G)
#define LED_SEGMENT_5 ~(LED_A | LED_C | LED_D | LED_F | LED_G)
#define LED_SEGMENT_6 ~(LED_A | LED_C | LED_D | LED_E | LED_F | LED_G)
#define LED_SEGMENT_7 ~(LED_A | LED_B | LED_C)
#define LED_SEGMENT_8 ~(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G)
#define LED_SEGMENT_9 ~(LED_A | LED_B | LED_C | LED_D | LED_F | LED_G)
#define LED_SEGMENT_A ~(LED_A | LED_B | LED_C | LED_E | LED_F | LED_G)
#define LED_SEGMENT_B ~(LED_C | LED_D | LED_E | LED_F | LED_G)
#define LED_SEGMENT_C ~(LED_A | LED_D | LED_E | LED_F)
#define LED_SEGMENT_D ~(LED_B | LED_C | LED_D | LED_E | LED_G)
#define LED_SEGMENT_E ~(LED_A | LED_D | LED_E | LED_F | LED_G)
#define LED_SEGMENT_F ~(LED_A | LED_E | LED_F | LED_G)

extern void display_segment(int display);

extern void our_delay(int delay);

uint8_t read_command(void);

volatile bool is_robot_ready;

#endif