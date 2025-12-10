//kpm.h
#ifndef KPM_H
#define KPM_H

#include "types.h"
//primary driver
void Init_KPM(void);
u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 KeyScan(void);

//secondary driver
//void ReadNum(s32*, s32*);

#endif

