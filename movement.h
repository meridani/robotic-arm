#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "main.h"
#include "positions.h"
#include <avr/io.h>
#include <stdbool.h>

// Robot motor irányok
#define LEFT 1
#define RIGHT 2
#define STOP 0

// -------------------------------
// Tengelyek:   engedélyező pin
//              Végállás(ok) pin
//              Irány pin
#define AXIS_2_EN (1 << PD0)
#define AXIS_2_SW (1 << PF2)
#define AXIS_2_DIR (1 << PE2)

#define AXIS_3_EN (1 << PD1)
#define AXIS_3_SW (1 << PF3)
#define AXIS_3_DIR (1 << PE3)

#define JAW_EN (1 << PD2)
#define JAW_SW (1 << PF4)
#define JAW_SW_CLOSE (1 << PG0)
#define JAW_DIR (1 << PE4)

#define BASE_EN (1 << PD3)
#define BASE_SW (1 << PF0)
#define BASE_SW_END (1 << PG1)
#define BASE_DIR (1 << PE5)

#define AXIS_1_EN (1 << PD4)
#define AXIS_1_SW (1 << PF1)
#define AXIS_1_DIR (1 << PE6)
// -------------------------------

#define DEFAULT_MOVE_TIME 1500

// A tengelyek jelenlegi mozgásirányát tárolja
static struct movement_direction {
    uint8_t base;
    uint8_t axis_1;
    uint8_t axis_2;
    uint8_t axis_3;
    uint8_t jaw;
} movement_directions = {
    STOP, STOP, STOP, STOP, STOP
};

// A különböző tengelyek mozgását vezérlő függvények
extern void move_base(uint8_t dir);
extern void move_1(uint8_t dir);
extern void move_2(uint8_t dir);
extern void move_3(uint8_t dir);
extern void move_jaw(uint8_t dir);
extern void move_to_position(uint16_t pos);

// Elkapás vezérlő függvények
extern void close_jaw(void);
extern void open_jaw(void);

// A kar kiengedő és behúzó függvények
extern void extend_arm(void);
extern void retract_arm(void);

// Komplex mozgás
extern void grab(void);
extern void drop(void);
extern void drop2(void);

// Alaphelyzetekbe beállító függvények
extern void end_base(void);
extern void home_base(void);
extern void home_1(void);
extern void home_2(void);
extern void home_3(void);
extern bool home_axes(void);

// Minden leállítása
extern void stop_all(void);
// Vészleállás, minden leállít és E betű villog a kijelzőn. Csak resettel lehet kijönni belőle
extern void e_stop(void);

#endif