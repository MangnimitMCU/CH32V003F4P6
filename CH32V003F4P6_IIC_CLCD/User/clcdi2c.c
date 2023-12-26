#include "clcdi2c.h"
volatile _userbit LCDbit;

#define buffer_size   5
u8 lcd_tx[buffer_size] = {0};

/////////////////////////////////////////////////////////////////////
void I2C_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitSturcture={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // PC1-->SDA    PC2-->SCL
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitSturcture.I2C_ClockSpeed = 100000;
    I2C_InitSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitSturcture.I2C_OwnAddress1 = 0;
    I2C_InitSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitSturcture);

    I2C_DMACmd(I2C1, ENABLE);

    I2C_Cmd(I2C1, ENABLE);
}

/////////////////////////////////////////////////////////////////////
void IIC_Start(u8 addr, u8 I2C_Direction) {
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET); ////////////
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction);

    if(I2C_Direction == 0) {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

/////////////////////////////////////////////////////////////////////
void IIC_WriteByte(u8 byte) {
    I2C_SendData(I2C1, byte);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/////////////////////////////////////////////////////////////////////
u8 IIC_ReadByte(u8 ack) {
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
    if(ack) {
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    }
    else {
        I2C_AcknowledgeConfig(I2C1, DISABLE);
    }
    return I2C_ReceiveData(I2C1);
}

/////////////////////////////////////////////////////////////////////
void IIC_Stop(void) {
    I2C_GenerateSTOP(I2C1, ENABLE);
}

/////////////////////////////////////////////////////////////////////
void IOExpansion_Write(u8 *io_data, u8 size) {
    u8 i;
    IIC_Start((0x27<<1), I2C_Direction_Transmitter);
    for(i=0; i<size; i++) {
        IIC_WriteByte(*io_data++);
    }
    IIC_Stop();
}

/////////////////////////////////////////////////////////////////////
void LCD_Backlight(u8 bl_data) {
    LCD_BL = 1;
    if(!bl_data) { LCD_BL = 0; }
    lcd_tx[0] = LCDbit.bytes;
    IOExpansion_Write(lcd_tx, 1);
}

/////////////////////////////////////////////////////////////////////
void LCD_Write(u8 lcd_data) {
    Delay_Us(100);
    LCD_D4 = lcd_data&0x01?HIGH:LOW;
    LCD_D5 = lcd_data&0x02?HIGH:LOW;
    LCD_D6 = lcd_data&0x04?HIGH:LOW;
    LCD_D7 = lcd_data&0x08?HIGH:LOW;
    lcd_tx[0] = LCDbit.bytes;
    lcd_tx[2] = LCDbit.bytes;
    LCD_EN = 1;
    lcd_tx[1] = LCDbit.bytes;
    LCD_EN = 0;
    IOExpansion_Write(lcd_tx, 3);
}

/////////////////////////////////////////////////////////////////////
void LCD_WriteByte(u8 lcd_data) {
    LCD_Write(lcd_data>>4); //msb
    LCD_Write(lcd_data);    //lsb
}

/////////////////////////////////////////////////////////////////////
void LCD_WriteCommand(u8 lcd_data) {
    // RS-Leave Low
    LCD_WriteByte(lcd_data);
}

/////////////////////////////////////////////////////////////////////
void LCD_WriteData(u8 lcd_data) {
    LCD_RS = 1;
    LCD_WriteByte(lcd_data);
    LCD_RS = 0;
}

/////////////////////////////////////////////////////////////////////
const static unsigned char r_offset[]={0, 64, 16, 80};
void LCD_SetCursor(u8 c, u8 r) {
    r &= 0x03;
    LCD_WriteCommand(0x80 | (c + r_offset[r]));
}

/////////////////////////////////////////////////////////////////////
void LCD_Print(u8 * pointer) {
    while(* pointer) {
        LCD_WriteData(* pointer++);
    }
}

/////////////////////////////////////////////////////////////////////
void LCD_PrintXY(u8 c, u8 r, u8 * pointer) {
    LCD_SetCursor(c, r);
    while(* pointer) {
        LCD_WriteData(* pointer++);
    }
}

/////////////////////////////////////////////////////////////////////
void LCD_Clear(void) {
    LCD_WriteCommand(0x01);
    Delay_Ms(5);
}

/////////////////////////////////////////////////////////////////////
void LCD_Home(void) {
    LCD_WriteCommand(0x02);
    Delay_Ms(5);
}

/////////////////////////////////////////////////////////////////////
void LCD_Config(void) {
    I2C_Config();
    Delay_Ms(15);
    LCD_Write(0x33);
    Delay_Ms(5);
    LCD_Write(0x33);
    Delay_Us(100);
    LCD_Write(0x33);
    Delay_Us(100);
    LCD_Write(0x22);
    Delay_Us(100);
    LCD_WriteCommand(0x28);
    LCD_WriteCommand(0x08);
    LCD_Clear();
    LCD_WriteCommand(0x0C);
    LCD_Backlight(ON);
}

