#include <LPC21xx.h>
#include "delay.h"
#include "lcd.h"

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK CCLK/5

#define ADC_CLK 3000000
#define CLKDIV ((PCLK/ADC_CLK)-1)

//defines for ADCR for
#define CLK_DIV_BITS 8 //@bit 8 to 15
#define PDN_BIT 21 //@bit 21
#define START_CONV_BITS 24 //@p0.24

//defines for ADDR 
#define RESULT_BITS 6 //@bit 6-15
#define DONE_BIT 31 //@bit 31

void Init()
{
	PINSEL1 = 1<<24;
	ADCR = 1<<PDN_BIT | CLKDIV<<CLK_DIV_BITS;
}

void Read_ADC(u32 chNo, u32 *adcDVal, f32 *analogReading)
{
	ADCR &= 0xffffff00;
	ADCR |= 1<<chNo | 1<<START_CONV_BITS;
	delay_us(3);
	
	while(((ADDR>>DONE_BIT)&1)==0);
	
	ADCR &= ~(1<<START_CONV_BITS);
	
	*adcDVal = ((ADDR>>RESULT_BITS)&1023);
	*analogReading = *adcDVal * (3.3/1024);
}
u32 adcDVal;
f32 eAR;
main()
{
	
	Init();
	Init_LCD();
	StrLCD("Adc Test");
	while(1)
	{
		Read_ADC(1, &adcDVal, &eAR);
		CmdLCD(0xc0+1);
		U32LCD(adcDVal);
	}
}

