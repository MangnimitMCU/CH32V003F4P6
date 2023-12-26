#ifndef _CLCDI2C_H_
#define _CLCDI2C_H_

#include "debug.h"

#define ON      1
#define OFF     0

#define LOW     0
#define HIGH    1

#define LCD_BL  LCDbit.bit3
#define LCD_RS  LCDbit.bit0
#define LCD_RW  LCDbit.bit1
#define LCD_EN  LCDbit.bit2
#define LCD_D4  LCDbit.bit4
#define LCD_D5  LCDbit.bit5
#define LCD_D6  LCDbit.bit6
#define LCD_D7  LCDbit.bit7

typedef union{
    u8 bytes;
    struct{
        u8 bit0:1;
        u8 bit1:1;
        u8 bit2:1;
        u8 bit3:1;
        u8 bit4:1;
        u8 bit5:1;
        u8 bit6:1;
        u8 bit7:1;
    };
}_userbit;

/* Function Prototype */
void LCD_Backlight(u8);
void LCD_WriteCommand(u8);
void LCD_WriteData(u8);
void LCD_SetCursor(u8, u8);
void LCD_Print(u8 *);
void LCD_PrintXY(u8, u8, u8 *);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Config(void);

#endif /* _CLCDI2C_H_ */
