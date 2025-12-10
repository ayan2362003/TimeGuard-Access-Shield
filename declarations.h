//declarations
#include "defines.h"

//operation declarations
void init_system(void);
void Init_KPM(void);
void Init_LCD(void);
void display_RTC(void);

//RTC declarations
void RTC_Init(void);
void GETRTCTIMEINFO(s32 *, s32 *, s32 *s);
void DISPLAYRTCTIME(void);
void GETRTCDATEINFO(s32 *data, s32 *month, s32 *year);
void DISPLAYRTCDATE(void);


//lcd declarations
void CmdLCD(u8 cmdByte);
void WriteLCD(u8 Byte);
void CharLCD(u8);
void StrLCD(s8 *);
void U32LCD(u32);
void S32LCD(s32);
void BuildCGRAM(u8 *, u8);

//kpm_declarations
void Init_KPM(void);
u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 KeyScan(void);


//delay definations
void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);
