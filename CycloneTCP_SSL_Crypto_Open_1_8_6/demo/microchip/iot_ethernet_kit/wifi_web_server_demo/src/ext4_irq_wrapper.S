//Dependencies
#include "isr_support.h"

   .global ext4IrqWrapper
   .extern extInt4Handler
   .extern xISRStackTop

   .set nomips16
   .set noreorder
   .set noat

   .ent ext4IrqWrapper

//External 4 interrupt
ext4IrqWrapper:
   //Save the current task context
   portSAVE_CONTEXT
   //Call interrupt handler
   jal ext4IrqHandler
   nop
   //Restore the context of the next task to execute
   portRESTORE_CONTEXT

   .end ext4IrqWrapper
