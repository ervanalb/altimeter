#include "hal.h"

enum {ON, OFF} state;

static void charge_leds()
{
    leds(charging() ? 0xFF : 0xAA);
}

static void turn_on()
{
    if(init_barometer()) for(;;);
    leds(0x55);
    on();
    state = ON;
}

static void turn_off()
{
    tone(0);
    off();
    state = OFF;
}

int main()
{
    init();
    state = OFF;

    putchar('$');
    putchar('P');
    putchar('M');
    putchar('T');
    putchar('K');
    putchar('1');
    putchar('6');
    putchar('1');
    putchar(',');
    putchar('0');
    putchar('*');
    putchar('2');
    putchar('8');
    putchar('\r');
    putchar('\n');

    for(;;)
    {
        switch(state)
        {
        case OFF:
            charge_leds();
            if(button())
            {
                turn_on();
                while(button());
            }
            break;
        case ON:
            if(button())
            {
                turn_off();
                while(button());
            }
            break;
        }
    }
}

