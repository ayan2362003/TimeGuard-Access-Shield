//eint_irq
#include <lpc21xx.h>
#include <string.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "lcd_defines.h"

extern char pass1[6];
extern s32 hr1, min1, hr2, min2;


void Edit_RTC_Info(void);  /*declaration for Edit RTC info*/
void Edit_HOUR(void);  /*declaration for Edit HOUR*/
void Edit_MIN(void);   /*declaration for Edit minute*/
void Edit_SEC(void);  /*declaration for Edit second*/
void Edit_DAY(void);  /*declaration for Edit day*/
void Edit_DATE(void); /*declaration for Edit date*/
void Edit_MONTH(void); /*declaration for Edit month*/
void Edit_YEAR(void);  /*declaration for Edit year*/


void SetRTCTimeInfo(u32,u32,u32);
void Edit_DEVICE_ALLOW_TIME(void);
void SetRTCDay(u32);
void SetRTCDateInfo(u32, u32, u32);
void CHANGE_PASSWORD(void);
void open_edit_menu(void);

u32 password(void);
#define EINT0_VIC_CHNO 14  
#define EINT0_STATUS_LED 30 //@p1.16
void einto_isr(void) __irq;

void Init_EINT()
{
        IODIR1 |= 1<<EINT0_STATUS_LED;  //cfg EINTO status led as gpio out;
        PINSEL0 |= 0x0000000C;   //cfg p0.1 pin as EINT0 input pin

        VICIntEnable = 1<<EINT0_VIC_CHNO;  //Enabling EINTO via VIC
        VICVectCntl0 = (1<<5) | EINT0_VIC_CHNO;  //cfg EINT0 as highest priority 0
        VICVectAddr0 = (u32)einto_isr;  //loading eint0 address

        EXTMODE = 1<<0; //cfg EINT0 for edge trigger
}

void einto_isr(void) __irq
{
    if (password())
    {
        /* check using minutes-since-midnight to handle hours+minutes cleanly */
        int now   = (int)HOUR * 60 + (int)MIN;
        int start = (int)hr1  * 60 + (int)min1;
        int end   = (int)hr2  * 60 + (int)min2;

        if (now >= start && now < end)
        {
            open_edit_menu();   // allowed
        }
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("Time Not Allowed");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("Allow: 2PM - 5PM"); /* or show dynamic times if you prefer */
            delay_ms(1500);
            CmdLCD(CLEAR_LCD);
        }
    }
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Wrong password");
        delay_ms(1000);
        CmdLCD(CLEAR_LCD);
    }

    EXTINT = 1 << 0;     /* clear EINT0 pending */
    VICVectAddr = 0;     /* end of ISR */
}


void open_edit_menu()
{
    CmdLCD(CLEAR_LCD);

    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("1 E.RTC");           // Edit RTC

    CmdLCD(GOTO_LINE1_POS0 + 8);
    StrLCD("2 ALW TM");          // Edit Allow Time

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("3 P CHG");           // Change Password

    CmdLCD(GOTO_LINE2_POS0 + 8);
    StrLCD("4 EXIT");

    switch (KeyScan())
    {
        case '1':
            Edit_RTC_Info();
            break;

        case '2':
            Edit_DEVICE_ALLOW_TIME();   
            break;

        case '3':
            CHANGE_PASSWORD();
            break;

        case '4':
            CmdLCD(CLEAR_LCD);
            return;

        default:
            CmdLCD(CLEAR_LCD);
            StrLCD("Invalid Input");
            delay_ms(1000);
            CmdLCD(CLEAR_LCD);
            return;
    }
}


void Edit_RTC_Info(void) /*function defination for Edit RTC_Info*/
{
        CmdLCD(CLEAR_LCD);
        StrLCD("1-H 2-MI 3-S 4-D");
        CmdLCD(0xc0);
        StrLCD("5-D 6-MO 7-Y 8-E");
        delay_ms(300);

        switch(KeyScan())
        {
                case '1': Edit_HOUR(); /*Function call for Edit Hour*/break;
                case '2': Edit_MIN(); /*Function call for Edit minute*/break;
                case '3': Edit_SEC(); /*Function call for Edit second*/break;
                case '4': Edit_DAY(); /*Function call for Edit day*/break;
                case '5': Edit_DATE(); /*Function call for Edit date*/break;
                case '6': Edit_MONTH(); /*Function call for Edit date*/break;
                case '7': Edit_YEAR(); /*Function call for Edit date*/break;
                case '8': CmdLCD(CLEAR_LCD); 
													return;
					
                default : CmdLCD(CLEAR_LCD); 
													StrLCD("Invalid Input");
                          delay_ms(1000);
                          CmdLCD(CLEAR_LCD);
                          return;
        }
}



/* read up to maxd digits, show at pos (column offset from GOTO_LINE2_POS0).
   returns -1 if no digit entered, otherwise numeric value */
/* compact helper (fixed: no unused variable) */
static int read_num(int maxd, int pos)
{
    int val = 0, d = 0;
    char k;

    for (;;) {
        delay_ms(200);
        k = KeyScan();
        if (k == 0) continue;
        if (k == 'C') {              /* backspace */
            if (d > 0) { d--; val /= 10;
                CmdLCD(GOTO_LINE2_POS0 + pos + d); CharLCD(' '); }
            continue;
        }
        if (k == 'E') break;         /* finish */
        if (k >= '0' && k <= '9' && d < maxd) {
            CmdLCD(GOTO_LINE2_POS0 + pos + d); CharLCD(k);
            val = val * 10 + (k - '0'); d++;
            if (d == maxd) break;    /* auto finish when max digits read */
        }
    }
    return (d == 0) ? -1 : val;
}

/* Edit only hour (0..23) */
void Edit_HOUR(void)
{
    int v;
    CmdLCD(CLEAR_LCD); 
		StrLCD("Enter HOUR"); 
		CmdLCD(GOTO_LINE2_POS0 + 7);
    v = read_num(2, 0);            /* two digits shown at col 7/8 */
    if (v >= 0 && v <= 23) 
{ 
			SetRTCTimeInfo(v, MIN, SEC);	
			CmdLCD(CLEAR_LCD); 	
			StrLCD("Hour Updated"); 
			delay_ms(400);
			CmdLCD(CLEAR_LCD); 
}
    else if (v >= 0)	
 { 
				CmdLCD(CLEAR_LCD); 
				U32LCD(v); 
				CmdLCD(GOTO_LINE2_POS0); 	
				StrLCD("Invalid HOURS"); 
				delay_ms(400); 
				CmdLCD(CLEAR_LCD); }
}

/* Edit only minute (0..59) */
void Edit_MIN(void)
{
    int v;

    CmdLCD(CLEAR_LCD);
    StrLCD("Enter MIN");
    CmdLCD(GOTO_LINE2_POS0 + 7);

    v = read_num(2, 0);

    if (v >= 0 && v <= 59)
    {
        SetRTCTimeInfo(HOUR, v, SEC);
        CmdLCD(CLEAR_LCD);
        StrLCD("Min Updated");
		delay_ms(400); 
		CmdLCD(CLEAR_LCD);
    }
    else if (v >= 0)
    {
        CmdLCD(CLEAR_LCD);
        U32LCD(v);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Invalid minutes");
        delay_ms(400);
        CmdLCD(CLEAR_LCD);
    }
}

/* Edit only second (0..59) */
void Edit_SEC(void)
{
    int v;

    CmdLCD(CLEAR_LCD);
    StrLCD("Enter SEC");
    CmdLCD(GOTO_LINE2_POS0 + 7);

    v = read_num(2, 0);

    if (v >= 0 && v <= 59)
    {
        SetRTCTimeInfo(HOUR, MIN, v);
        CmdLCD(CLEAR_LCD);
        StrLCD("Sec Updated");
		delay_ms(400); 
		CmdLCD(CLEAR_LCD);
    }
    else if (v >= 0)
    {
        CmdLCD(CLEAR_LCD);
        U32LCD(v);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Invalid seconds");
        delay_ms(400);
        CmdLCD(CLEAR_LCD);
    }
}


void Edit_DAY(void) /*Function defination for Edit day*/
{
        s32 day;
        CmdLCD(CLEAR_LCD);
        StrLCD("Chose Choice");
        delay_ms(500);
        CmdLCD(CLEAR_LCD);
        StrLCD("0SU 1MO 2TU 3WED");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("4THU 5FRI 6SAT");
        day = KeyScan()-48;
        if(day>=0 && day<=6)
        {
                SetRTCDay(day);
                CmdLCD(CLEAR_LCD);
        }
        else
        {
                CmdLCD(CLEAR_LCD);
                U32LCD(day);
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Invalid day");
                delay_ms(500);
                CmdLCD(CLEAR_LCD);
        }
}

/* Edit only date (day 1..31) */
void Edit_DATE(void)
{
    int v;
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter DATE");
    CmdLCD(GOTO_LINE2_POS0 + 7);
    v = read_num(2, 0);  /* up to 2 digits shown at col 7/8 */

    if (v == -1) { CmdLCD(CLEAR_LCD); return; } /* nothing entered */
    if (v >= 1 && v <= 31) {
        SetRTCDateInfo(v, MONTH, YEAR); /* preserve MONTH and YEAR */
        CmdLCD(CLEAR_LCD);
        StrLCD("Date Updated");
		delay_ms(400); 
		CmdLCD(CLEAR_LCD);

    } else {
        CmdLCD(CLEAR_LCD);
        U32LCD(v);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Invalid date");
        delay_ms(400);
        CmdLCD(CLEAR_LCD);
    }
}

/* Edit only month (1..12) */
void Edit_MONTH(void)
{
    int v;
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter MONTH");
    CmdLCD(GOTO_LINE2_POS0 + 7);
    v = read_num(2, 0);

    if (v == -1) { CmdLCD(CLEAR_LCD); return; }
    if (v >= 1 && v <= 12) {
        SetRTCDateInfo(DOM, v, YEAR); /* preserve DOM and YEAR */
        CmdLCD(CLEAR_LCD);
        StrLCD("Month Updated");
		delay_ms(400);
		CmdLCD(CLEAR_LCD); 
    } else {
        CmdLCD(CLEAR_LCD);
        U32LCD(v);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Invalid MONTH");
        delay_ms(400);
        CmdLCD(CLEAR_LCD);
    }
}

/* Edit only year (allow up to 4 digits) */
void Edit_YEAR(void)
{
    int v;
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter YEAR");
    CmdLCD(GOTO_LINE2_POS0 + 7);
    v = read_num(4, 0);  /* up to 4 digits shown at cols 7..10 */

    if (v == -1) { CmdLCD(CLEAR_LCD); return; }
    if (v >= 1 && v <= 4095) {
        SetRTCDateInfo(DOM, MONTH, v); /* preserve DOM and MONTH */
        CmdLCD(CLEAR_LCD);
        StrLCD("Year Updated");
		delay_ms(400); 
		CmdLCD(CLEAR_LCD);
    } else {
        CmdLCD(CLEAR_LCD);
        U32LCD(v);
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Invalid YEAR");
        delay_ms(400);
        CmdLCD(CLEAR_LCD);
    }
}


/* helper: read up to maxlen chars into buf, returns length (0..maxlen) */
static int read_pass(char *buf, int maxlen)
{
    int len = 0;
    char k;
    int pos;

    /* init */
    for (pos = 0; pos < maxlen+1; pos++) buf[pos] = 0;

    while (len < maxlen) {
        delay_ms(300);
        k = KeyScan();
        if (k == 0) continue;

        if (k == 'C') {             /* backspace */
            if (len > 0) {
                len--;
                buf[len] = 0;
                CmdLCD(GOTO_LINE2_POS0 + len);
                CharLCD(' ');
            }
            continue;
        }
        if (k == 'E') break;        /* finish early */

        /* accept any keypad character (digits usually) */
        buf[len] = k;
        CmdLCD(GOTO_LINE2_POS0 + len);
        CharLCD(k);
        len++;
    }
    buf[len] = 0; /* null-terminate for safe display/compare */
    return len;
}

/* safer CHANGE_PASSWORD */
void CHANGE_PASSWORD(void)
{
    char Npass[7], Cpass[7];
    int nlen, clen, i;
    int res = 0;

    /* NEW PASSWORD */
    CmdLCD(CLEAR_LCD);
    StrLCD("NEW PASSWORD");
    CmdLCD(GOTO_LINE2_POS0);
    nlen = read_pass(Npass, 6);

    /* CONFIRM PASSWORD */
    CmdLCD(CLEAR_LCD);
    StrLCD("CFORM PASSWORD");
    CmdLCD(GOTO_LINE2_POS0);
    clen = read_pass(Cpass, 6);

    /* compare lengths first, then bytes */
    if (nlen == clen && nlen > 0) {
        res = 1;
        for (i = 0; i < nlen; i++) {
            if (Npass[i] != Cpass[i]) { res = 0; break; }
        }
    } else {
        res = 0;
    }

    if (res) {
        /* copy into external pass1 (size should be >=6). store exactly 6 bytes or less */
        for (i = 0; i < 6; i++) {
            if (i < nlen) pass1[i] = Npass[i];
            else pass1[i] = 0;
        }
        CmdLCD(CLEAR_LCD);
        StrLCD("Updated Password");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD(Npass);   /* safe because Npass is null-terminated */
        delay_ms(500);
        CmdLCD(CLEAR_LCD);
    } else {
        CmdLCD(CLEAR_LCD);
        StrLCD("NPass and Cpass");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Not Matched");
        delay_ms(500);
        CmdLCD(CLEAR_LCD);
    }
}
void Edit_DEVICE_ALLOW_TIME(void)
{
    int sh, sm, eh, em;

    /* ---------------------- START TIME ---------------------- */
    CmdLCD(CLEAR_LCD);
    StrLCD("Set Start Time");
    
    CmdLCD(GOTO_LINE2_POS0);      // Line 2
    StrLCD("HH:MM");

    /* Hour input */
    CmdLCD(GOTO_LINE2_POS0);      // position HH
    sh = read_num(2, 0);

    if (sh < 0 || sh > 23) {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Hour");
        delay_ms(800);
        CmdLCD(CLEAR_LCD);
        return;
    }

    /* Minute input */
    CmdLCD(GOTO_LINE2_POS0 + 3);  // position MM
    sm = read_num(2, 3);

    if (sm < 0 || sm > 59) {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Minute");
        delay_ms(800);
        CmdLCD(CLEAR_LCD);
        return;
    }

    /* ---------------------- END TIME ---------------------- */
    CmdLCD(CLEAR_LCD);
    StrLCD("Set End Time");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("HH:MM");

    /* Hour input */
    CmdLCD(GOTO_LINE2_POS0);
    eh = read_num(2, 0);

    if (eh < 0 || eh > 23) {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Hour");
        delay_ms(800);
        CmdLCD(CLEAR_LCD);
        return;
    }

    /* Minute input */
    CmdLCD(GOTO_LINE2_POS0 + 3);
    em = read_num(2, 3);

    if (em < 0 || em > 59) {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Minute");
        delay_ms(800);
        CmdLCD(CLEAR_LCD);
        return;
    }

    /* ---------------------- VALIDATION ---------------------- */
    if ( (eh < sh) || (eh == sh && em <= sm) ) {
        CmdLCD(CLEAR_LCD);
        StrLCD("End <= Start");
        delay_ms(900);
        CmdLCD(CLEAR_LCD);
        return;
    }

    /* ---------------------- SAVE VALUES ---------------------- */
    hr1 = sh;  
    min1 = sm;
    hr2 = eh;
    min2 = em;

    /* ---------------------- SUCCESS MSG ---------------------- */
    CmdLCD(CLEAR_LCD);
    StrLCD("Time Updated");

    CmdLCD(GOTO_LINE2_POS0);
    CharLCD((hr1/10)+'0'); CharLCD((hr1%10)+'0');
    CharLCD(':');
    CharLCD((min1/10)+'0'); CharLCD((min1%10)+'0');

    StrLCD(" - ");

    CharLCD((hr2/10)+'0'); CharLCD((hr2%10)+'0');
    CharLCD(':');
    CharLCD((min2/10)+'0'); CharLCD((min2%10)+'0');

    delay_ms(1200);
    CmdLCD(CLEAR_LCD);
}
