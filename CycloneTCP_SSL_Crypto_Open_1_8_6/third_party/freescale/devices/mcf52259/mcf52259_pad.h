/********************************************************************/
/* Coldfire C Header File
 *
 *     Date      : 2009/04/30
 *     Revision  : 0.8
 *
 *     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *     http      : www.freescale.com
 *     mail      : support@freescale.com
 */

#ifndef __MCF52259_PAD_H__
#define __MCF52259_PAD_H__


/*********************************************************************
*
* Common GPIO
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PAD_PSRR                         (*(vuint32*)(0x40100078))
#define MCF_PAD_PDSR                         (*(vuint32*)(0x4010007C))
#define MCF_PAD_PSRRL                        (*(vuint32*)(0x40100080))
#define MCF_PAD_PSRRH                        (*(vuint16*)(0x40100086))
#define MCF_PAD_PDSRL                        (*(vuint32*)(0x40100088))
#define MCF_PAD_PDSRH                        (*(vuint16*)(0x4010008E))


/* Bit definitions and macros for MCF_PAD_PSRR */
#define MCF_PAD_PSRR_PSRR0                   (0x1)
#define MCF_PAD_PSRR_PSRR1                   (0x2)
#define MCF_PAD_PSRR_PSRR2                   (0x4)
#define MCF_PAD_PSRR_PSRR3                   (0x8)
#define MCF_PAD_PSRR_PSRR4                   (0x10)
#define MCF_PAD_PSRR_PSRR5                   (0x20)
#define MCF_PAD_PSRR_PSRR6                   (0x40)
#define MCF_PAD_PSRR_PSRR7                   (0x80)
#define MCF_PAD_PSRR_PSRR8                   (0x100)
#define MCF_PAD_PSRR_PSRR9                   (0x200)
#define MCF_PAD_PSRR_PSRR10                  (0x400)
#define MCF_PAD_PSRR_PSRR11                  (0x800)
#define MCF_PAD_PSRR_PSRR12                  (0x1000)
#define MCF_PAD_PSRR_PSRR13                  (0x2000)
#define MCF_PAD_PSRR_PSRR14                  (0x4000)
#define MCF_PAD_PSRR_PSRR15                  (0x8000)
#define MCF_PAD_PSRR_PSRR16                  (0x10000)
#define MCF_PAD_PSRR_PSRR17                  (0x20000)
#define MCF_PAD_PSRR_PSRR18                  (0x40000)
#define MCF_PAD_PSRR_PSRR19                  (0x80000)
#define MCF_PAD_PSRR_PSRR20                  (0x100000)
#define MCF_PAD_PSRR_PSRR21                  (0x200000)
#define MCF_PAD_PSRR_PSRR22                  (0x400000)
#define MCF_PAD_PSRR_PSRR23                  (0x800000)
#define MCF_PAD_PSRR_PSRR24                  (0x1000000)
#define MCF_PAD_PSRR_PSRR25                  (0x2000000)
#define MCF_PAD_PSRR_PSRR26                  (0x4000000)
#define MCF_PAD_PSRR_PSRR27                  (0x8000000)
#define MCF_PAD_PSRR_PSRR28                  (0x10000000)
#define MCF_PAD_PSRR_PSRR29                  (0x20000000)
#define MCF_PAD_PSRR_PSRR30                  (0x40000000)
#define MCF_PAD_PSRR_PSRR31                  (0x80000000)

/* Bit definitions and macros for MCF_PAD_PDSR */
#define MCF_PAD_PDSR_PDSR0                   (0x1)
#define MCF_PAD_PDSR_PDSR1                   (0x2)
#define MCF_PAD_PDSR_PDSR2                   (0x4)
#define MCF_PAD_PDSR_PDSR3                   (0x8)
#define MCF_PAD_PDSR_PDSR4                   (0x10)
#define MCF_PAD_PDSR_PDSR5                   (0x20)
#define MCF_PAD_PDSR_PDSR6                   (0x40)
#define MCF_PAD_PDSR_PDSR7                   (0x80)
#define MCF_PAD_PDSR_PDSR8                   (0x100)
#define MCF_PAD_PDSR_PDSR9                   (0x200)
#define MCF_PAD_PDSR_PDSR10                  (0x400)
#define MCF_PAD_PDSR_PDSR11                  (0x800)
#define MCF_PAD_PDSR_PDSR12                  (0x1000)
#define MCF_PAD_PDSR_PDSR13                  (0x2000)
#define MCF_PAD_PDSR_PDSR14                  (0x4000)
#define MCF_PAD_PDSR_PDSR15                  (0x8000)
#define MCF_PAD_PDSR_PDSR16                  (0x10000)
#define MCF_PAD_PDSR_PDSR17                  (0x20000)
#define MCF_PAD_PDSR_PDSR18                  (0x40000)
#define MCF_PAD_PDSR_PDSR19                  (0x80000)
#define MCF_PAD_PDSR_PDSR20                  (0x100000)
#define MCF_PAD_PDSR_PDSR21                  (0x200000)
#define MCF_PAD_PDSR_PDSR22                  (0x400000)
#define MCF_PAD_PDSR_PDSR23                  (0x800000)
#define MCF_PAD_PDSR_PDSR24                  (0x1000000)
#define MCF_PAD_PDSR_PDSR25                  (0x2000000)
#define MCF_PAD_PDSR_PDSR26                  (0x4000000)
#define MCF_PAD_PDSR_PDSR27                  (0x8000000)
#define MCF_PAD_PDSR_PDSR28                  (0x10000000)
#define MCF_PAD_PDSR_PDSR29                  (0x20000000)
#define MCF_PAD_PDSR_PDSR30                  (0x40000000)
#define MCF_PAD_PDSR_PDSR31                  (0x80000000)

/* Bit definitions and macros for MCF_PAD_PSRRL */
#define MCF_PAD_PSRRL_PSRR0                  (0x1)
#define MCF_PAD_PSRRL_PSRR1                  (0x2)
#define MCF_PAD_PSRRL_PSRR2                  (0x4)
#define MCF_PAD_PSRRL_PSRR3                  (0x8)
#define MCF_PAD_PSRRL_PSRR4                  (0x10)
#define MCF_PAD_PSRRL_PSRR5                  (0x20)
#define MCF_PAD_PSRRL_PSRR6                  (0x40)
#define MCF_PAD_PSRRL_PSRR7                  (0x80)
#define MCF_PAD_PSRRL_PSRR8                  (0x100)
#define MCF_PAD_PSRRL_PSRR9                  (0x200)
#define MCF_PAD_PSRRL_PSRR10                 (0x400)
#define MCF_PAD_PSRRL_PSRR11                 (0x800)
#define MCF_PAD_PSRRL_PSRR12                 (0x1000)
#define MCF_PAD_PSRRL_PSRR13                 (0x2000)
#define MCF_PAD_PSRRL_PSRR14                 (0x4000)
#define MCF_PAD_PSRRL_PSRR15                 (0x8000)
#define MCF_PAD_PSRRL_PSRR16                 (0x10000)
#define MCF_PAD_PSRRL_PSRR17                 (0x20000)
#define MCF_PAD_PSRRL_PSRR18                 (0x40000)
#define MCF_PAD_PSRRL_PSRR19                 (0x80000)
#define MCF_PAD_PSRRL_PSRR20                 (0x100000)
#define MCF_PAD_PSRRL_PSRR21                 (0x200000)
#define MCF_PAD_PSRRL_PSRR22                 (0x400000)
#define MCF_PAD_PSRRL_PSRR23                 (0x800000)
#define MCF_PAD_PSRRL_PSRR24                 (0x1000000)
#define MCF_PAD_PSRRL_PSRR25                 (0x2000000)
#define MCF_PAD_PSRRL_PSRR26                 (0x4000000)
#define MCF_PAD_PSRRL_PSRR27                 (0x8000000)
#define MCF_PAD_PSRRL_PSRR28                 (0x10000000)
#define MCF_PAD_PSRRL_PSRR29                 (0x20000000)
#define MCF_PAD_PSRRL_PSRR30                 (0x40000000)
#define MCF_PAD_PSRRL_PSRR31                 (0x80000000)

/* Bit definitions and macros for MCF_PAD_PSRRH */
#define MCF_PAD_PSRRH_PSRR0                  (0x1)
#define MCF_PAD_PSRRH_PSRR1                  (0x2)
#define MCF_PAD_PSRRH_PSRR2                  (0x4)
#define MCF_PAD_PSRRH_PSRR3                  (0x8)
#define MCF_PAD_PSRRH_PSRR4                  (0x10)
#define MCF_PAD_PSRRH_PSRR5                  (0x20)
#define MCF_PAD_PSRRH_PSRR6                  (0x40)
#define MCF_PAD_PSRRH_PSRR7                  (0x80)
#define MCF_PAD_PSRRH_PSRR8                  (0x100)
#define MCF_PAD_PSRRH_PSRR9                  (0x200)
#define MCF_PAD_PSRRH_PSRR10                 (0x400)
#define MCF_PAD_PSRRH_PSRR11                 (0x800)
#define MCF_PAD_PSRRH_PSRR12                 (0x1000)
#define MCF_PAD_PSRRH_PSRR13                 (0x2000)
#define MCF_PAD_PSRRH_PSRR14                 (0x4000)
#define MCF_PAD_PSRRH_PSRR15                 (0x8000)

/* Bit definitions and macros for MCF_PAD_PDSRL */
#define MCF_PAD_PDSRL_PDSR0                  (0x1)
#define MCF_PAD_PDSRL_PDSR1                  (0x2)
#define MCF_PAD_PDSRL_PDSR2                  (0x4)
#define MCF_PAD_PDSRL_PDSR3                  (0x8)
#define MCF_PAD_PDSRL_PDSR4                  (0x10)
#define MCF_PAD_PDSRL_PDSR5                  (0x20)
#define MCF_PAD_PDSRL_PDSR6                  (0x40)
#define MCF_PAD_PDSRL_PDSR7                  (0x80)
#define MCF_PAD_PDSRL_PDSR8                  (0x100)
#define MCF_PAD_PDSRL_PDSR9                  (0x200)
#define MCF_PAD_PDSRL_PDSR10                 (0x400)
#define MCF_PAD_PDSRL_PDSR11                 (0x800)
#define MCF_PAD_PDSRL_PDSR12                 (0x1000)
#define MCF_PAD_PDSRL_PDSR13                 (0x2000)
#define MCF_PAD_PDSRL_PDSR14                 (0x4000)
#define MCF_PAD_PDSRL_PDSR15                 (0x8000)
#define MCF_PAD_PDSRL_PDSR16                 (0x10000)
#define MCF_PAD_PDSRL_PDSR17                 (0x20000)
#define MCF_PAD_PDSRL_PDSR18                 (0x40000)
#define MCF_PAD_PDSRL_PDSR19                 (0x80000)
#define MCF_PAD_PDSRL_PDSR20                 (0x100000)
#define MCF_PAD_PDSRL_PDSR21                 (0x200000)
#define MCF_PAD_PDSRL_PDSR22                 (0x400000)
#define MCF_PAD_PDSRL_PDSR23                 (0x800000)
#define MCF_PAD_PDSRL_PDSR24                 (0x1000000)
#define MCF_PAD_PDSRL_PDSR25                 (0x2000000)
#define MCF_PAD_PDSRL_PDSR26                 (0x4000000)
#define MCF_PAD_PDSRL_PDSR27                 (0x8000000)
#define MCF_PAD_PDSRL_PDSR28                 (0x10000000)
#define MCF_PAD_PDSRL_PDSR29                 (0x20000000)
#define MCF_PAD_PDSRL_PDSR30                 (0x40000000)
#define MCF_PAD_PDSRL_PDSR31                 (0x80000000)

/* Bit definitions and macros for MCF_PAD_PDSRH */
#define MCF_PAD_PDSRH_PDSR0                  (0x1)
#define MCF_PAD_PDSRH_PDSR1                  (0x2)
#define MCF_PAD_PDSRH_PDSR2                  (0x4)
#define MCF_PAD_PDSRH_PDSR3                  (0x8)
#define MCF_PAD_PDSRH_PDSR4                  (0x10)
#define MCF_PAD_PDSRH_PDSR5                  (0x20)
#define MCF_PAD_PDSRH_PDSR6                  (0x40)
#define MCF_PAD_PDSRH_PDSR7                  (0x80)
#define MCF_PAD_PDSRH_PDSR8                  (0x100)
#define MCF_PAD_PDSRH_PDSR9                  (0x200)
#define MCF_PAD_PDSRH_PDSR10                 (0x400)
#define MCF_PAD_PDSRH_PDSR11                 (0x800)
#define MCF_PAD_PDSRH_PDSR12                 (0x1000)
#define MCF_PAD_PDSRH_PDSR13                 (0x2000)
#define MCF_PAD_PDSRH_PDSR14                 (0x4000)
#define MCF_PAD_PDSRH_PDSR15                 (0x8000)


#endif /* __MCF52259_PAD_H__ */
