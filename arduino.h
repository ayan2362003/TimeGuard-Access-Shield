// arduino.h
#include "types.h"

void pinMode(u32 PinNo, u32 PinDir);
void digitalWrite(u32 PinNo, u32 bit);
u32 digitalRead(u32 PinNo);

void PortMode(u32 PinStartNo, u32 npins, u32 PinsDir);
void write2Pins(u32 PinStartNo, u32 npins, u32 data);
u32 readPins(u32 PinStartNo, u32 nPins);


