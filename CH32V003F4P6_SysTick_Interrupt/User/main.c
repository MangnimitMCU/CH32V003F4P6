/*********************************************************************
 *
 * Author : Somlak Mangnimit
 *
 */

#include "debug.h"


/* Global define */


/* Global Variable */

/*********************************************************************
 *
 * IO Initial
 *
 */
void IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); //Enable PD clock

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  //LED
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    IO_Init();

    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->SR = 0;
    SysTick->CMP = 24000000-1;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xF;

    while(1)
    {

    }
}

/*********************************************************************
 *
 * SysTick Interrupt Handler
 *
 */
__attribute__((interrupt)) void SysTick_Handler(void)
{
    if((SysTick->SR & (1 << 0)) == (1 << 0))
    {
        SysTick->SR &= ~(1 << 0);
        GPIO_WriteBit(GPIOD, GPIO_Pin_5, !GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_5));
    }
}
