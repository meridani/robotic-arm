#define __DELAY_BACKWARD_COMPATIBLE__
#include "movement.h"
#include <util/delay.h>

#include "positions.h"

void move_base(uint8_t dir)
{
    switch (dir) {
    case LEFT:
        PORTE &= ~(BASE_DIR);
        PORTD |= (BASE_EN);
        movement_directions.base = LEFT;
        break;

    case RIGHT:
        PORTE |= (BASE_DIR);
        PORTD |= (BASE_EN);
        movement_directions.base = RIGHT;
        break;

    case STOP:
        PORTD &= ~(BASE_EN);
        movement_directions.base = STOP;
        break;

    default:
        PORTD &= ~(BASE_EN);
        movement_directions.base = STOP;
        break;
    }
}

void move_1(uint8_t dir)
{
    switch (dir) {
    case LEFT:
        PORTE &= ~(AXIS_1_DIR);
        PORTD |= (AXIS_1_EN);
        movement_directions.axis_1 = LEFT;
        break;

    case RIGHT:
        PORTE |= (AXIS_1_DIR);
        PORTD |= (AXIS_1_EN);
        movement_directions.axis_1 = RIGHT;
        break;

    case STOP:
        PORTD &= ~(AXIS_1_EN);
        movement_directions.axis_1 = STOP;
        break;

    default:
        PORTD &= ~(AXIS_1_EN);
        movement_directions.axis_1 = STOP;
        break;
    }
}

void move_2(uint8_t dir)
{

    switch (dir) {
    case LEFT:
        PORTE &= ~(AXIS_2_DIR);
        PORTD |= (AXIS_2_EN);
        movement_directions.axis_2 = LEFT;
        break;

    case RIGHT:
        PORTE |= (AXIS_2_DIR);
        PORTD |= (AXIS_2_EN);
        movement_directions.axis_2 = RIGHT;
        break;

    case STOP:
        PORTD &= ~(AXIS_2_EN);
        movement_directions.axis_2 = STOP;
        break;

    default:
        PORTD &= ~(AXIS_2_EN);
        movement_directions.axis_2 = STOP;
        break;
    }
}

void move_3(uint8_t dir)
{

    switch (dir) {
    case LEFT:
        PORTE &= ~(AXIS_3_DIR);
        PORTD |= (AXIS_3_EN);
        movement_directions.axis_3 = LEFT;
        break;

    case RIGHT:
        PORTE |= (AXIS_3_DIR);
        PORTD |= (AXIS_3_EN);
        movement_directions.axis_3 = RIGHT;
        break;

    case STOP:
        PORTD &= ~(AXIS_3_EN);
        movement_directions.axis_3 = STOP;
        break;

    default:
        PORTD &= ~(AXIS_3_EN);
        movement_directions.axis_3 = STOP;
        break;
    }
}

void move_jaw(uint8_t dir)
{

    switch (dir) {
    case LEFT:
        PORTE &= ~(JAW_DIR);
        PORTD |= (JAW_EN);
        movement_directions.jaw = LEFT;
        break;

    case RIGHT:
        PORTE |= (JAW_DIR);
        PORTD |= (JAW_EN);
        movement_directions.jaw = RIGHT;
        break;

    case STOP:
        PORTD &= ~(JAW_EN);
        movement_directions.jaw = STOP;
        break;

    default:
        PORTD &= ~(JAW_EN);
        movement_directions.jaw = STOP;
        break;
    }
}
void stop_all()
{
    PORTD &= ~(JAW_EN | BASE_EN | AXIS_1_EN | AXIS_2_EN | AXIS_3_EN);
}
void e_stop()
{
    stop_all();
    while (1) {
        display_segment(LED_SEGMENT_E);
        _delay_ms(250);
        display_segment(LED_EMPTY);
        _delay_ms(250);
    }
}

void home_base()
{
    move_base(LEFT);
    while ((PINF & (BASE_SW)) == BASE_SW) {}
    move_base(STOP);
    current_positions.base = 0;
}
void home_1()
{
    move_1(RIGHT);
    while ((PINF & (AXIS_1_SW)) == AXIS_1_SW) {}
    move_1(STOP);
    current_positions.joint_1 = 0;
}
void home_2()
{
    move_2(RIGHT);
    while ((PINF & (AXIS_2_SW)) == AXIS_2_SW) {}
    move_2(STOP);
    current_positions.joint_2 = 0;
}
void home_3()
{
    move_3(LEFT);
    while ((PINF & (AXIS_3_SW)) == AXIS_3_SW) {}
    move_3(STOP);
    current_positions.joint_3 = 0;
}

void close_jaw()
{
    move_jaw(RIGHT);
    while ((PING & (JAW_SW_CLOSE)) == JAW_SW_CLOSE) {}
    our_delay(150);
    move_jaw(STOP);

    current_positions.jaw = 9999;
}

void open_jaw()
{
    move_jaw(LEFT);
    while ((PINF & (JAW_SW)) == JAW_SW) {}
    move_jaw(STOP);
    // move_jaw(RIGHT);
    // _delay_ms(INIT_POSITION_JAW_TIME);
    // move_jaw(STOP);
    current_positions.jaw = 0;
}

void retract_arm()
{
}

void extend_arm()
{
    int time = 3000;

    // Open Jaw
    open_jaw();

    // if (pos.joint_1 > current_positions.joint_1) {
    //     time = pos.joint_1 - current_positions.joint_1;
        move_2(LEFT);
        our_delay(200);
        move_1(LEFT);
        our_delay(time);
    // } else if (pos.joint_1 < current_positions.joint_1) {
    //     time = current_positions.joint_1 - pos.joint_1;
    //     move_2(RIGHT);
    //     our_delay(100);
    //     move_1(RIGHT);
    //     our_delay(time);
    //     // our_delay(350);
    // } else {
    //     move_1(STOP);
    //     move_2(STOP);
    // }

    move_1(STOP);
    move_2(STOP);
    our_delay(200);

    close_jaw();

    our_delay(200);

    move_1(RIGHT);
    our_delay(300);
    move_2(RIGHT);
    our_delay(time);
    move_1(STOP);

    while ((PINF & (AXIS_2_SW)) == AXIS_2_SW) {}
    move_2(STOP);
}

void move_to_position(struct position pos)
{
    int time = 0;
    if (pos.base > current_positions.base) {
        time = pos.base - current_positions.base;
        move_base(RIGHT);
        our_delay(time);
    } else if (pos.base < current_positions.base) {
        time = current_positions.base - pos.base;
        move_base(LEFT);
        our_delay(time);
        our_delay(350);
    } else {
        move_base(STOP);
    }
    current_positions.base = pos.base;
    move_base(STOP);
}

bool home_axes(void)
{
    display_segment(LED_SEGMENT_A);
    _delay_ms(1000);

    // Home base axis
    display_segment(LED_SEGMENT_0);
    // move_1(RIGHT);
    // while ((PINF & (AXIS_1_SW)) == AXIS_1_SW) {}
    // move_1(STOP);
    home_1();
    move_1(LEFT);
    _delay_ms(INIT_POSITION_1_TIME);
    move_1(STOP);
    current_positions.joint_1 = INIT_POSITION_1_TIME;

    // Home axis 1
    display_segment(LED_SEGMENT_1);
    // move_2(RIGHT);
    // while ((PINF & (AXIS_2_SW)) == AXIS_2_SW) {}
    // move_2(STOP);
    home_2();
    move_2(LEFT);
    _delay_ms(INIT_POSITION_2_TIME);
    move_2(STOP);
    current_positions.joint_2 = INIT_POSITION_2_TIME;

    // Home axis 2
    display_segment(LED_SEGMENT_2);
    // move_3(LEFT);
    // while ((PINF & (AXIS_3_SW)) == AXIS_3_SW) {}
    // move_3(STOP);
    home_3();
    move_3(RIGHT);
    _delay_ms(INIT_POSITION_3_TIME);
    move_3(STOP);
    current_positions.joint_3 = INIT_POSITION_3_TIME;

    // Home axis 3
    display_segment(LED_SEGMENT_3);
    // move_base(LEFT);
    // while ((PINF & (BASE_SW)) == BASE_SW) {}
    // move_base(STOP);
    home_base();
    move_base(RIGHT);
    our_delay(INIT_POSITION_BASE_TIME);
    move_base(STOP);
    current_positions.base = INIT_POSITION_BASE_TIME;

    // Home Jaw
    display_segment(LED_SEGMENT_4);
    // move_jaw(LEFT);
    // while ((PINF & (JAW_SW)) == JAW_SW) {}
    // move_jaw(STOP);
    open_jaw();
    move_jaw(RIGHT);
    _delay_ms(INIT_POSITION_JAW_TIME);
    move_jaw(STOP);
    current_positions.jaw = INIT_POSITION_JAW_TIME;

    stop_all();
    display_segment(LED_SEGMENT_B);
    is_robot_ready = true;
    return true;
}
