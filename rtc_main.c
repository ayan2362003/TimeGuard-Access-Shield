#include <lpc214x.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"
#include "types.h"
void Init_EINT(void);
char pass1[6] = "123456";
char pass2[6];
s32 hour,min,sec,date,month,year,day;
        s32 hr1 = 14, min1 = 0;  // Start time = 2:00 PM
        s32 hr2 = 17, min2 = 0;  // End time   = 5:00 PM


// Main function
void init_system()
{
        RTC_Init();     // Initialize RTC       
        Init_LCD();  // Initialize the LCD
        Init_KPM();  //Intilize the KPM
        Init_EINT(); //turn on the interuppt

  // Set the initial time (hours, minutes, seconds)
        SetRTCTimeInfo(14,20,0);
        SetRTCDateInfo(4,10,2025);
        SetRTCDay(4);
				
}


void display_RTC(){
    // Get and display the current time on LCD
                GetRTCTimeInfo(&hour,&min,&sec);
                DisplayRTCTime(hour,min,sec);
                GetRTCDateInfo(&date,&month,&year);
                DisplayRTCDate(date,month,year);
                GetRTCDay(&day);
                DisplayRTCDay(day);
}

u32 password()
{
    s32 i;
    s32 res = 1;
    char key;

    /* clear pass2 buffer so leftover bytes don't matter */
    for (i = 0; i < 6; i++) pass2[i] = 0;

    CmdLCD(CLEAR_LCD);
    StrLCD("Enter Password");
    CmdLCD(GOTO_LINE2_POS0);
    delay_ms(300);

    i = 0;
    while (i < 6)
    {
        key = KeyScan();          /* your existing KeyScan usage */

        /* if KeyScan can return 0 when no key pressed, uncomment next line:
           if (key == 0) continue;
           (If KeyScan already blocks until a key press, leaving as-is is fine.)
        */

        if (key == 'C')          /* backspace */
        {
            if (i > 0)
            {
                i--;
                pass2[i] = 0;
                CmdLCD(GOTO_LINE2_POS0 + i);
                CharLCD(' ');
            }
            /* don't decrement i twice; just continue */
            continue;
        }

        if (key == 'E')          /* finish early */
        {
            break;
        }

        /* accept printable keypad char */
        CmdLCD(GOTO_LINE2_POS0 + i);
        CharLCD(key);
        pass2[i] = key;
        i++;

        delay_ms(200);           /* keep existing debounce/delay behaviour */
    }

    /* pad remaining bytes with 0 for deterministic compare */
    for (; i < 6; i++) pass2[i] = 0;

    /* compare exactly 6 bytes (pass1 assumed to contain 6 valid bytes) */
    for (i = 0; i < 6; i++)
    {
        if (pass1[i] != pass2[i])
        {
            res = 0;
            break;
        }
    }

    CmdLCD(CLEAR_LCD);
    return (u32)res;
}
