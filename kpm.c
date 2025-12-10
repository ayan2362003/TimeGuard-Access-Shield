//kpm.c
#include "types.h"
#include "kpm_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include <LPC21xx.h>

/*u32 kpmLUT[4][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}};*/
u8 kpmLUT[4][4] = {
    {'7','8','9','/'},
    {'4','5','6','*'},
    {'1','2','3','-'},
    {'C','0','=','E'}
};


void Init_KPM(void)
{
	//rows output and grounded
	IODIR1 |= 15 << ROW0;
	
	//cols input & high, anyway by default
}

u32 ColScan(void)
{
	u32 status;
	
	status = (((IOPIN1>>COL0)&15)<15)?0:1;
	return status;
}

u32 RowCheck(void)
{
	u32 r;
	for(r=0; r<=3; r++)
	{
		//starting with row0, upto row3
		//ground only one row, keep other rows high
		IOPIN1 = ((IOPIN1& ~(15<<ROW0)) | (~(1<<r)<<ROW0));
		
		//check if key was pressed in that row
		if(ColScan()==0) break;
	}
	//re-initilize rows to thier state
	IOCLR1 = 15 << ROW0;
	return r;
}

u32 ColCheck(void)
{
	u32 c;
	for(c=0; c<=3; c++)
	{
		//starting with COL0 upto COL3
		//check one COL at a time for low
		
		if(((IOPIN1>>(COL0+c))&1)==0)
			break;
	}
	return c;
}

u32 KeyScan(void)
{
	u32 r, c, keyV;
	
	//Intilize KPM
	
	//wait for any key press detection
	
	while(ColScan());
	
	//upon key press detection, &key is in pressed state
	//identfy row in which key was pressed
	r=RowCheck();
	
	//Identify COL in which key was pressed
	c = ColCheck();
	
	//extract key value from LUT
	keyV = kpmLUT[r][c];
	return keyV;
}

