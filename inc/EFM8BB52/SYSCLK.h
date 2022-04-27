#ifndef _EFM8BB52_SYSCLK_H_
#define _EFM8BB52_SYSCLK_H_

#include <settings.h>

// Calculate SYSCLK_CLOCK_FREQUENCY
#ifndef SYSCLK_CLOCK
#error "Define SYSCLK_CLOCK."
#else
#if SYSCLK_CLOCK == CLKSEL_CLKSL__HFOSC0_clk24p5
#define SYSCLK_CLOCK_FREQUENCY 24500000
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__EXTOSC
#define SYSCLK_CLOCK_FREQUENCY 0
#warning "Cannot determine SYSCLK_CLOCK_FREQUENCY for external source"
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__LFOSC
#define SYSCLK_CLOCK_FREQUENCY 80000
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__HFOSC0_clk49
#define SYSCLK_CLOCK_FREQUENCY 49000000
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__HFOSC0_clk24p5_div_1p5
#define SYSCLK_CLOCK_FREQUENCY 16333333
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__FSRCO_clk10
#define SYSCLK_CLOCK_FREQUENCY 10000000
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__FSRCO_clk2p5
#define SYSCLK_CLOCK_FREQUENCY 2500000
#elif SYSCLK_CLOCK == CLKSEL_CLKSL__HFOSC0_clk49_div_1p5
#define SYSCLK_CLOCK_FREQUENCY 33666667
#else
#error "Unknown SYSCLK_CLOCK"
#endif
#endif

// Calculate SYSCLK_DIVIDER
#ifndef SYSCLK_DIV
#error "Define SYSCLK_DIV."
#else
#if SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_1
#define SYSCLK_DIVIDER 1
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_2
#define SYSCLK_DIVIDER 2
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_4
#define SYSCLK_DIVIDER 4
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_8
#define SYSCLK_DIVIDER 8
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_16
#define SYSCLK_DIVIDER 16
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_32
#define SYSCLK_DIVIDER 32
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_64
#define SYSCLK_DIVIDER 64
#elif SYSCLK_DIV == CLKSEL_CLKDIV__SYSCLK_DIV_128
#define SYSCLK_DIVIDER 128
#else
#error "Unknown SYSCLK_DIV"
#endif
#endif

// Calculate SYSCLK_FREQUENCY
#define SYSCLK_FREQUENCY (SYSCLK_CLOCK_FREQUENCY / SYSCLK_DIVIDER)

// call to configure SYSCLK
void SYSCLK_configure(void);

#endif

