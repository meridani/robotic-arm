#include <avr/io.h>
#include <util/delay.h>

#include "main.h"
#include "movement.h"
#include "positions.h"

// Ki és bemenetek konfigurálása
void InitPorts(void)
{
    // 8 szegmens pinek
    DDRC |= (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G | LED_DP);
    PORTC = (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G | LED_DP);

    // Robot vezérlő pinek
    DDRD |= (AXIS_1_EN | AXIS_2_EN | AXIS_3_EN | JAW_EN | BASE_EN);
    PORTD &= ~(AXIS_1_EN | AXIS_2_EN | AXIS_3_EN | JAW_EN | BASE_EN);

    // Robot kar irány pinek
    DDRE |= (AXIS_1_DIR | AXIS_2_DIR | AXIS_3_DIR | JAW_DIR | BASE_DIR);
    PORTE &= ~(AXIS_1_DIR | AXIS_2_DIR | AXIS_3_DIR | JAW_DIR | BASE_DIR);

    // Parancs bemenetek
    PORTF |= (COMMAND_BIT_0 | COMMAND_BIT_1 | COMMAND_START_BIT);

    // Robot ready kimenet
    DDRB |= (READY_STATE);
    PORTB &= ~(READY_STATE);
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
    // Csak akkor figyeljük a végállás kapcsolót ha annak az irányába megyünk
    // Így ha a végállásban vagyunk el tudunk indulni a másik irányba
    for (int i = 0; i < delay; i++) {
        if (!((PINF & (BASE_SW)) == BASE_SW) && movement_directions.base == LEFT)
            move_base(STOP);
        if (!((PINF & (BASE_SW_END)) == BASE_SW_END) && movement_directions.base == RIGHT)
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

    is_robot_ready = false;
    PORTC |= LED_DP;
    PORTB &= ~READY_STATE;

    // Minden pont alaphelyzetbe állítása
    home_axes();

    while (1) {
        if (is_robot_ready) {
            // Start jelre várakozás
            while ((PINF & (COMMAND_START_BIT)) == COMMAND_START_BIT) {
                display_segment(LED_SEGMENT_C);
                PORTC &= ~LED_DP;
                PORTB |= (READY_STATE);
            }

            // Parancs beolvasása
            m_command      = read_command();
            is_robot_ready = false;
            PORTC |= LED_DP;
            PORTB &= ~READY_STATE;

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
                move_base(RIGHT);
                our_delay(INIT_POSITION_BASE_TIME);
                move_base(STOP);
                is_robot_ready = true;
                break;

            case 2:
                display_segment(LED_SEGMENT_2);
                home_base();
                move_base(RIGHT);
                our_delay(INIT_POSITION_BASE_TIME);
                move_base(STOP);
                drop2();
                is_robot_ready = true;
                break;

            case 3:
                display_segment(LED_SEGMENT_3);
                end_base();
                drop();
                move_base(LEFT);
                our_delay(INIT_POSITION_BASE_TIME);
                move_base(STOP);
                is_robot_ready = true;
                break;

            default:
                display_segment(LED_SEGMENT_E);
                break;
            }
        }
    }
}