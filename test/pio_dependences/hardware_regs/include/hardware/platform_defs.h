/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_PLATFORM_DEFS_H
#define _HARDWARE_PLATFORM_DEFS_H

// This header is included from C and assembler - intended mostly for #defines; guard other stuff with #ifdef __ASSEMBLER__

#ifndef _u
#ifdef __ASSEMBLER__
#define _u(x) x
#else
#define _u(x) x ## u
#endif
#endif
#define _u(x) x ## u

#define NUM_CORES 2u
#define NUM_DMA_CHANNELS 12u
#define NUM_DMA_TIMERS 4u
#define NUM_IRQS 32u
#define NUM_USER_IRQS 6u
#define NUM_PIOS 2u
#define NUM_PIO_STATE_MACHINES 4u
#define NUM_PWM_SLICES 8u
#define NUM_SPIN_LOCKS 32u
#define NUM_UARTS 2u
#define NUM_I2CS 2u
#define NUM_SPIS 2u
#define NUM_TIMERS 4u
#define NUM_ADC_CHANNELS 5u

#define NUM_BANK0_GPIOS 30u
#define NUM_QSPI_GPIOS 6u

#define PIO_INSTRUCTION_COUNT 32u

// PICO_CONFIG: XOSC_KHZ, The crystal oscillator frequency in kHz, type=int, default=12000, advanced=true, group=hardware_base
// NOTE:  The system and USB clocks are generated from the frequency using two PLLs.
// If you override this define, or SYS_CLK_KHZ/USB_CLK_KHZ below, you will *also* need to add your own adjusted PLL set-up defines to
// override the defaults which live in src/rp2_common/hardware_clocks/include/hardware/clocks.h
// Please see the comments there about calculating the new PLL setting values.
#ifndef XOSC_KHZ
#define XOSC_KHZ 12000u
#endif

// PICO_CONFIG: SYS_CLK_KHZ, The system operating frequency in kHz, type=int, default=125000, advanced=true, group=hardware_base
#ifndef SYS_CLK_KHZ
#define SYS_CLK_KHZ 125000u
#endif

// PICO_CONFIG: USB_CLK_KHZ, USB clock frequency. Must be 48MHz for the USB interface to operate correctly, type=int, default=48000, advanced=true, group=hardware_base
#ifndef USB_CLK_KHZ
#define USB_CLK_KHZ 48000u
#endif

// For backwards compatibility define XOSC_MHZ if the frequency is indeed an integer number of Mhz.
#if defined(XOSC_KHZ) && !defined(XOSC_MHZ) && (XOSC_KHZ % 1000 == 0)
#define XOSC_MHZ (XOSC_KHZ / 1000)
#endif

// For backwards compatibility define SYS_CLK_MHZ if the frequency is indeed an integer number of Mhz.
#if defined(SYS_CLK_KHZ) && !defined(SYS_CLK_MHZ) && (SYS_CLK_KHZ % 1000 == 0)
#define SYS_CLK_MHZ (SYS_CLK_KHZ / 1000)
#endif

// For backwards compatibility define USB_CLK_MHZ if the frequency is indeed an integer number of Mhz.
#if defined(USB_CLK_KHZ) && !defined(USB_CLK_MHZ) && (USB_CLK_KHZ % 1000 == 0)
#define USB_CLK_MHZ (USB_CLK_KHZ / 1000)
#endif

#define FIRST_USER_IRQ (NUM_IRQS - NUM_USER_IRQS)
#define VTABLE_FIRST_IRQ 16

#endif

