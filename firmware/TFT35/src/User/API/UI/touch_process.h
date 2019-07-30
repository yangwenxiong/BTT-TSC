#ifndef _TOUCH_PROCESS_H_
#define _TOUCH_PROCESS_H_

#include "includes.h"


#define CMD_RDX 0xD0
#define CMD_RDY 0x90

enum
{
  KNOB_INC=1,
  KNOB_DEC,
};

#define KEY_CLICK          0x0000  // key¶¨ÒåÎª u16, 16(u16 16bit) - 3(3 bits flag ) = 13 bit ËùÒÔ u16 ×î´óÖ§³Ö 2^13 = 8192 ¸ö¼üÖµ
#define KEY_DOUBLE_CLICK   0x2000  //µÚÈýÎ»ÓÃÀ´±êÊ¶Ë«»÷¶¯×÷
#define KEY_LONG_RELEASE   0x4000  //µÚ¶þÎ»ÓÃÀ´±êÊ¶³¤°´ºóÊÍ·Å¶¯×÷
#define KEY_LONG_CLICK     0x8000  //µÚÒ»Î»ÓÃÀ´±êÊ¶³¤°´¶¯×÷


void TSC_Calibration(void);
u8   isPress(void);
u16  KEY_GetValue(u8 total_rect,const GUI_RECT* menuRect);
u16  KNOB_GetRV(GUI_RECT *knob);

void loopTouchScreen(void);

extern void (*TSC_ReDrawIcon)(u8 positon, u8 is_press);

void Buzzer_Config(void);
void Buzzer_DeConfig(void);
void PS_ON_Config(void);                        //´òÍê¹Ø»ú³õÊ¼»¯ÅäÖÃ
void PS_ON_DeConfig(void);                      //´òÍê¹Ø»úÅäÖÃ¸´Î»
void Material_Check_DeConfig(void);             //¶ÏÁÏ¼ì²â¸´Î»ÅäÖÃ
void Material_Check_Config(void);               //¶ÏÁÏ¼ì²â³õÊ¼»¯ÅäÖÃ
void openBuzzer(u16 h_us, u16 l_us);

#endif

