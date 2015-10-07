#include "hal.h"

enum {ON, OFF} state;

static void charge_leds()
{
    leds(charging() ? 0xFF : 0xAA);
}

static int a;

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
                state = ON;
                leds(0x55);
                on();
                while(button());
            }
            break;
        case ON:
            if(button())
            {
                state = OFF;
                tone(0);
                off();
                while(button());
            }
            break;
        }
    }
}
