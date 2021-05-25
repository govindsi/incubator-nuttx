/****************************************************************************
 * arch/risc-v/src/k210/hardware/k210_memorymap.h
 *
 * Derives from software originally provided by Canaan Inc
 *
 *   Copyright 2018 Canaan Inc
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_RISCV_SRC_K210_HARDWARE_K210_MEMORYMAP_H
#define __ARCH_RISCV_SRC_K210_HARDWARE_K210_MEMORYMAP_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Base Address ****************************************************/

#define K210_CLINT_BASE   0x02000000
#define K210_PLIC_BASE    0x0c000000

#ifdef CONFIG_K210_WITH_QEMU
#define K210_UART0_BASE   0x10010000
#else
#define K210_UART0_BASE   0x38000000
#endif
#define K210_GPIOHS_BASE  0x38001000

/* Under AHB 32 bit */
#define K210_DMAC_BASE	  0x50000000

/* Under APB1 32 bit */
#define K210_GPIO_BASE	  0x50200000
#define K210_UART1_BASE	  0x50210000
#define K210_UART2_BASE	  0x50220000
#define K210_UART3_BASE	  0x50230000
#define K210_SPI_SLAVE_B  0x50240000
#define K210_I2S0_BASE	  0x50250000
#define K210_I2S1_BASE	  0x50260000
#define K210_I2S2_BASE	  0x50270000
#define K210_I2C0_BASE	  0x50280000
#define K210_I2C1_BASE	  0x50290000
#define K210_I2C2_BASE	  0x502A0000
#define K210_FPIOA_BASE   0x502B0000
#define K210_SHA256_BASE  0x502C0000
#define K210_TIMER0_BASE  0x502D0000
#define K210_TIMER1_BASE  0x502E0000
#define K210_TIMER2_BASE  0x502F0000

/* Under APB2 32 bit */
#define K210_WDT0_BASE    0x50400000
#define K210_WDT1_BASE    0x50410000
#define K210_OTP_BASE     0x50420000
#define K210_DVP_BASE     0x50430000
#define K210_SYSCTL_BASE  0x50440000
#define K210_AES_BASE     0x50450000
#define K210_RTC_BASE     0x50460000

/* Under APB3 32 bit */
#define K210_SPI0_BASE    0x52000000
#define K210_SPI1_BASE    0x53000000
#define K210_SPI3_BASE    0x54000000
#endif /* __ARCH_RISCV_SRC_K210_HARDWARE_K210_MEMORYMAP_H */
