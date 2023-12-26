
#include "debug.h"
#include "clcdi2c.h"


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    LCD_Config();

    LCD_PrintXY(0, 0, "HELLO");
    LCD_PrintXY(8, 1, "CH32V003");

    while(1)
    {

    }
}
