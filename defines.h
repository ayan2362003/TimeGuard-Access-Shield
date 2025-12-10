#ifndef DEFINES_H
#define DEFINES_H

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef double f64;

#define CLEAR_LCD 0x01
#define RET_CUR_HOME 0x02
#define SHIFT_CUR_RIGHT 0X06
//#define SHIFT_CUR_LEFT 0x07

//#define DSP_OFF 0x08
#define DSP_ON_CUR_OFF 0x0c
#define DSP_ON_CUR_ON 0x0e
#define DSP_ON_CUR_BLINK 0x0f

//#define SHIFT_DISP_LEFT 0x10
#define SHIFT_DISP_RIGHT 0x14

#define MODE_8BIT_1LINE 0x30
#define MODE_8BIT_2LINE 0x38

#define GOTO_LINE1_POS0 0x80
#define GOTO_LINE2_POS0 0xc0
#define GOTO_CGRAM_START 0x40

#define LCD_DATA 16 //@p1.16 - p1.23
#define LCD_RS 25 //p1.24
#define LCD_RW 24 //p1.26
#define LCD_EN 26 //p1.27


//kpm_defines.h

#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19

#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

//rtc defines
#include <LPC21xx.h>
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)

//RTC MACROS	
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

#define RTC_ENABLE  1<<0
#define RTC_RESET   1<<1;
#define RTC_CLKSRC  1<<4

#endif
