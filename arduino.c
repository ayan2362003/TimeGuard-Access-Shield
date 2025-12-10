// arduino.c

#include <LPC21xx.h>
#include "types.h"
#include "arduino_defines.h"

void pinMode(u32 PinNo, u32 PinDir)
{
		if(PinNo<32)
		{
			if(PinDir == OUTPUT)
			{
				IODIR0 |= 1 << PinNo;
			}
			else
			{
				IODIR0 |= ~(1 << PinNo);
			}
		}
		else if(PinNo>=32 && PinNo<=47)
		{
			if(PinDir == OUTPUT)
			{
				IODIR1 |= (1<<(PinNo-16));
			}
			else
			{
				IODIR1 |= ~(1 << (PinNo-16));
			}
		}
}

void digitalWrite(u32 PinNo, u32 bit)
{
	if(PinNo<32)
	{
		bit ? (IOSET0 = 1 << PinNo) : (IOCLR0 = 1 << PinNo);
	}
	else if(PinNo>=32 && PinNo<=47)
	{
		bit ? (IOSET1 = 1 << (PinNo-16)) : (IOCLR1 = 1 << (PinNo-16));
	}
}

u32 digitalRead(u32 PinNo)
{
	u32 bit;
	if(PinNo<32)
	{
		bit = ((IOPIN0 >> PinNo)&1);
	}
	else if(PinNo>=32 && PinNo<=47)
	{
		bit = ((IOPIN1>>(PinNo-16))&1);
	}
	return bit;
}

void PortMode(u32 PinStartNo, u32 nPins, u32 PinsDir)
{
	if(PinStartNo < 32)
	{
		if(PinsDir == OUTPUT)
		{
			IODIR0 |= ((1<<nPins)-1) << PinStartNo;
		}
		else
		{
			IODIR0 |= ~(((1<<nPins)-1) << PinStartNo);
		}
	}
	else if(PinStartNo>=32 && PinStartNo<=47)
	{
		if(PinStartNo == OUTPUT)
		{
			IODIR1  |= (((1<<nPins)-1) >> (PinStartNo-16));
		}
		else
		{
			IODIR1 |= ~(((1<<nPins)-1) >> (PinStartNo-16));
		}
	}
}

void write2Pins(u32 PinStartNo, u32 nPins, u32 data)
{
	if(PinStartNo<32)
	{
		IOCLR0 = ((1<<nPins)-1) << PinStartNo;
		IOSET0 = data << PinStartNo;
	}
	else if(PinStartNo>=32 && PinStartNo<=47)
	{
		IOCLR0 = ((1<<nPins)-1) << (PinStartNo-16);
		IOSET0 = data << (PinStartNo-16);
	}
}

u32 readPins(u32 PinStartNo, u32 nPins)
{
	u32 data;
	if(PinStartNo<32)
	{
		data = ((IOPIN0>>PinStartNo)& ((1<<nPins)-1));
	}
	else if(PinStartNo>=32 && PinStartNo<=47)
	{
		data = ((IOPIN0 >> (PinStartNo-16)) & ((1 << nPins)-1));
	}
	return data;
}

