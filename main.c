#include <avr/io.h>
#include <util/delay.h>

#include "main.h"
#include "movement.h"
#include "positions.h"

void InitPorts(void)
{
    // Init 8 segment led outputs
    DDRC |= (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G);
    PORTC = (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G);

    // Init Axis enable pins
    DDRD |= (AXIS_1_EN | AXIS_2_EN | AXIS_3_EN | JAW_EN | BASE_EN);
    PORTD &= ~(AXIS_1_EN | AXIS_2_EN | AXIS_3_EN | JAW_EN | BASE_EN);

    // Init Axis direction pins
    DDRE |= (AXIS_1_DIR | AXIS_2_DIR | AXIS_3_DIR | JAW_DIR | BASE_DIR);
    PORTE &= ~(AXIS_1_DIR | AXIS_2_DIR | AXIS_3_DIR | JAW_DIR | BASE_DIR);

    // DDRG |= (ROBOT_READY_BIT);
    PORTF |= (COMMAND_BIT_0 | COMMAND_BIT_1 | COMMAND_START_BIT);
}

void display_segment(int display)
{
    PORTC |= LED_EMPTY;
    PORTC &= display;
}

uint8_t read_command()
{
    uint8_t temp;
    temp = ((PINF & (COMMAND_BIT_0 | COMMAND_BIT_1)) >> 5);
    return temp;
}

void our_delay(int delay)
{
    for (int i = 0; i < delay; i++) {
        if (!((PINF & (BASE_SW)) == BASE_SW) && movement_directions.base == LEFT)
            move_base(STOP);
        if (!((PINF & (AXIS_1_SW)) == AXIS_1_SW) && movement_directions.axis_1 == RIGHT)
            move_1(STOP);
        if (!((PINF & (AXIS_2_SW)) == AXIS_2_SW) && movement_directions.axis_2 == RIGHT)
            move_2(STOP);

        if (!((PINF & (AXIS_3_SW)) == AXIS_3_SW) && movement_directions.axis_3 == LEFT)
            move_3(STOP);

        if (!((PING & (JAW_SW_CLOSE)) == JAW_SW_CLOSE) && movement_directions.jaw == RIGHT) {
            our_delay(150);
            move_jaw(STOP);
        }
        if (!((PINF & (JAW_SW)) == JAW_SW) && movement_directions.jaw == LEFT)
            move_jaw(STOP);

        _delay_us(1000);
    }
}

int main(void)
{
    uint8_t m_command;

    InitPorts();

    home_axes();

    while (1) {
        if (is_robot_ready) {
            // Wait for start signal
            while ((PINF & (COMMAND_START_BIT)) == COMMAND_START_BIT) {
                display_segment(LED_SEGMENT_C);
            }
            m_command      = read_command();
            is_robot_ready = false;
            switch (m_command) {
            case 0:
                display_segment(LED_SEGMENT_0);
                move_to_position(INIT_POSITION_BASE_TIME);
                is_robot_ready = true;
                break;

            case 1:
                display_segment(LED_SEGMENT_1);
                home_base();
                grab();
                is_robot_ready = true;
                break;

            case 2:
                display_segment(LED_SEGMENT_2);
                drop();
                is_robot_ready = true;
                break;

            case 3:
                display_segment(LED_SEGMENT_3);
                end_base();
                drop();
                is_robot_ready = true;
                break;

            default:
                display_segment(LED_SEGMENT_E);
                break;
            }
        }
    }
}