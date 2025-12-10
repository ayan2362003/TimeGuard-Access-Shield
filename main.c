#include <LPC21xx.h>
#include "kpm.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"

void init_system(void);
void display_RTC(void);
u32 password(void);
void einto_isr(void) __irq;

char key;

int main()
{
    init_system();

    while(1)
    {
        do {
            display_RTC();
        } while(ColScan());

        if(KeyScan()=='E')
        {
            if(password())
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("Valid password");
                delay_ms(1000);
                CmdLCD(CLEAR_LCD);
            }
            else
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("Wrong Password");
                delay_ms(1000);
                CmdLCD(CLEAR_LCD);
            }
        }
    }
}
