//Dependencies
#include "lpc18xx.h"

//Signature value
#define Sign_Value 0x5A5A5A5A

//Linker file constants
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

//Function declaration
void SystemInit(void);
void __libc_init_array(void);
int main(void);

//Default empty handler
void Default_Handler(void);

//Cortex-M4 core handlers
void Reset_Handler          (void);
void NMI_Handler            (void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler            (void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__((weak, alias("Default_Handler")));

//Peripheral handlers
void DAC_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));
void DMA_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));
void FLASHEEPROM_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void ETHERNET_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void LCD_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));
void USB0_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void USB1_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void SCT_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));
void RITIMER_IRQHandler     (void) __attribute__((weak, alias("Default_Handler")));
void TIMER0_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void TIMER1_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void TIMER2_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void TIMER3_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void MCPWM_IRQHandler       (void) __attribute__((weak, alias("Default_Handler")));
void ADC0_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void ADC1_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void SSP0_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void SSP1_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void USART0_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void UART1_IRQHandler       (void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void I2S0_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void I2S1_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT0_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT1_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT2_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT3_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT4_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT5_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT6_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void PIN_INT7_IRQHandler    (void) __attribute__((weak, alias("Default_Handler")));
void GINT0_IRQHandler       (void) __attribute__((weak, alias("Default_Handler")));
void GINT1_IRQHandler       (void) __attribute__((weak, alias("Default_Handler")));
void EVENTROUTER_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void C_CAN1_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void ATIMER_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));
void WWDT_IRQHandler        (void) __attribute__((weak, alias("Default_Handler")));
void C_CAN0_IRQHandler      (void) __attribute__((weak, alias("Default_Handler")));
void QEI_IRQHandler         (void) __attribute__((weak, alias("Default_Handler")));

//Vector table
__attribute__((section(".vectors")))
const uint32_t vectorTable[69] =
{
   //Initial stack pointer
   (uint32_t) (&_estack),

   //Core handlers
   (uint32_t) Reset_Handler,
   (uint32_t) NMI_Handler,
   (uint32_t) HardFault_Handler,
   (uint32_t) MemManage_Handler,
   (uint32_t) BusFault_Handler,
   (uint32_t) UsageFault_Handler,
   (uint32_t) Sign_Value,
   (uint32_t) 0,
   (uint32_t) 0,
   (uint32_t) 0,
   (uint32_t) SVC_Handler,
   (uint32_t) DebugMon_Handler,
   (uint32_t) 0,
   (uint32_t) PendSV_Handler,
   (uint32_t) SysTick_Handler,

   //Peripheral handlers
   (uint32_t) DAC_IRQHandler,         //D/A Converter
   (uint32_t) 0,                      //Reserved
   (uint32_t) DMA_IRQHandler,         //General Purpose DMA
   (uint32_t) 0,                      //Reserved
   (uint32_t) FLASHEEPROM_IRQHandler, //ORed flash bank A, flash bank B, EEPROM interrupt
   (uint32_t) ETHERNET_IRQHandler,    //Ethernet
   (uint32_t) SDIO_IRQHandler,        //SD/MMC
   (uint32_t) LCD_IRQHandler,         //LCD
   (uint32_t) USB0_IRQHandler,        //USB0
   (uint32_t) USB1_IRQHandler,        //USB1
   (uint32_t) SCT_IRQHandler,         //State Configurable Timer
   (uint32_t) RITIMER_IRQHandler,     //Repetitive Interrupt Timer
   (uint32_t) TIMER0_IRQHandler,      //Timer0
   (uint32_t) TIMER1_IRQHandler,      //Timer1
   (uint32_t) TIMER2_IRQHandler,      //Timer2
   (uint32_t) TIMER3_IRQHandler,      //Timer3
   (uint32_t) MCPWM_IRQHandler,       //Motor Control PWM
   (uint32_t) ADC0_IRQHandler,        //A/D Converter 0
   (uint32_t) I2C0_IRQHandler,        //I2C0
   (uint32_t) I2C1_IRQHandler,        //I2C1
   (uint32_t) 0,                      //Reserved
   (uint32_t) ADC1_IRQHandler,        //A/D Converter 1
   (uint32_t) SSP0_IRQHandler,        //SSP0
   (uint32_t) SSP1_IRQHandler,        //SSP1
   (uint32_t) USART0_IRQHandler,      //USART0
   (uint32_t) UART1_IRQHandler,       //UART1
   (uint32_t) USART2_IRQHandler,      //USART2
   (uint32_t) USART3_IRQHandler,      //USART3
   (uint32_t) I2S0_IRQHandler,        //I2S0
   (uint32_t) I2S1_IRQHandler,        //I2S1
   (uint32_t) 0,                      //Reserved
   (uint32_t) 0,                      //Reserved
   (uint32_t) PIN_INT0_IRQHandler,    //GPIO pin interrupt 0
   (uint32_t) PIN_INT1_IRQHandler,    //GPIO pin interrupt 1
   (uint32_t) PIN_INT2_IRQHandler,    //GPIO pin interrupt 2
   (uint32_t) PIN_INT3_IRQHandler,    //GPIO pin interrupt 3
   (uint32_t) PIN_INT4_IRQHandler,    //GPIO pin interrupt 4
   (uint32_t) PIN_INT5_IRQHandler,    //GPIO pin interrupt 5
   (uint32_t) PIN_INT6_IRQHandler,    //GPIO pin interrupt 6
   (uint32_t) PIN_INT7_IRQHandler,    //GPIO pin interrupt 7
   (uint32_t) GINT0_IRQHandler,       //GPIO global interrupt 0
   (uint32_t) GINT1_IRQHandler,       //GPIO global interrupt 1
   (uint32_t) EVENTROUTER_IRQHandler, //Event Router
   (uint32_t) C_CAN1_IRQHandler,      //C_CAN1
   (uint32_t) 0,                      //Reserved
   (uint32_t) 0,                      //Reserved
   (uint32_t) ATIMER_IRQHandler,      //ATIMER
   (uint32_t) RTC_IRQHandler,         //RTC
   (uint32_t) 0,                      //Reserved
   (uint32_t) WWDT_IRQHandler,        //WDT
   (uint32_t) 0,                      //Reserved
   (uint32_t) C_CAN0_IRQHandler,      //C_CAN0
   (uint32_t) QEI_IRQHandler,         //QEI
};


/**
 * @brief Reset handler
 **/

void Reset_Handler(void)
{
   uint32_t *src;
   uint32_t *dest;

   //System initialization
   SystemInit();

   //Initialize the relocate segment
   src = &_etext;
   dest = &_srelocate;

   if(src != dest)
   {
      while(dest < &_erelocate)
      {
         *dest++ = *src++;
      }
   }

   //Clear the zero segment
   for(dest = &_szero; dest < &_ezero;)
   {
      *dest++ = 0;
   }

   //Set the vector table base address
   src = (uint32_t *) &_sfixed;
   SCB->VTOR = ((uint32_t) src & SCB_VTOR_TBLOFF_Msk);

   //C library initialization
#if defined (__cplusplus)
   __libc_init_array();
#endif

   //Branch to main function
   main();

   //Endless loop
   while(1);
}


/**
 * @brief Default interrupt handler
 **/

void Default_Handler(void)
{
   while(1)
   {
   }
}
