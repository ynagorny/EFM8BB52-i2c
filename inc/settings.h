#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <SI_EFM8BB52_Register_Enums.h>


/*** Console ***/

#define CONSOLE_ENABLED   1   // 1 = Enabled, 0 = Disabled


/*** SYSCLK ***/

// Choose one of clock sources:
// CLKSEL_CLKSL__HFOSC0_clk24p5         24.5 MHz  - Internal High Frequency Oscillator - default
// CLKSEL_CLKSL__EXTOSC                 unknown   - External Oscillator circuit
// CLKSEL_CLKSL__LFOSC                  80 kHz    - Internal Low-Frequency Oscillator
// CLKSEL_CLKSL__HFOSC0_clk49           49 MHz    - Internal High Frequency Oscillator
// CLKSEL_CLKSL__HFOSC0_clk24p5_div_1p5 16.33 MHz - Internal High Frequency Oscillator 24.5 MHz, pre-scaled by 1.5
// CLKSEL_CLKSL__FSRCO_clk10            10 MHz    - Internal Fast Start Up Oscillator
// CLKSEL_CLKSL__FSRCO_clk2p5           2.5 MHz   - Internal Fast Start Up Oscillator with 4x prescale (2.5 MHz)
// CLKSEL_CLKSL__HFOSC0_clk49_div_1p5   33.67 MHz - Internal High Frequency Oscillator 49 MHz, pre-scaled by 1.5.
#define SYSCLK_CLOCK  CLKSEL_CLKSL__HFOSC0_clk24p5


// Choose one of clock source dividers:
// CLKSEL_CLKDIV__SYSCLK_DIV_1          divide by 1
// CLKSEL_CLKDIV__SYSCLK_DIV_2          divide by 2
// CLKSEL_CLKDIV__SYSCLK_DIV_4          divide by 4
// CLKSEL_CLKDIV__SYSCLK_DIV_8          divide by 8 - default
// CLKSEL_CLKDIV__SYSCLK_DIV_16         divide by 16
// CLKSEL_CLKDIV__SYSCLK_DIV_32         divide by 32
// CLKSEL_CLKDIV__SYSCLK_DIV_64         divide by 64
// CLKSEL_CLKDIV__SYSCLK_DIV_128        divide by 128
#define SYSCLK_DIV    CLKSEL_CLKDIV__SYSCLK_DIV_8


/*** WDT ***/

#define WDT_ENABLED   -1  // -1 = disable on startup, 0 = disabled, 1 = enabled


/*** Pins Configuration ***/

#define XBAR_ENABLED  1   // 1 = Enabled, 0 = Disabled

#define XBAR_P0_SKIP   \
  P0SKIP_B0__SKIPPED | \
  P0SKIP_B1__SKIPPED | \
  P0SKIP_B2__SKIPPED | \
  P0SKIP_B3__SKIPPED | \
  P0SKIP_B4__NOT_SKIPPED | /* UART0-TX */ \
  P0SKIP_B5__NOT_SKIPPED | /* UART0-RX */ \
  P0SKIP_B6__SKIPPED | \
  P0SKIP_B7__SKIPPED;

#define XBAR_P1_SKIP   \
  P1SKIP_B0__SKIPPED | \
  P1SKIP_B1__SKIPPED | \
  P1SKIP_B2__SKIPPED | \
  P1SKIP_B3__SKIPPED | \
  P1SKIP_B4__SKIPPED | \
  P1SKIP_B5__NOT_SKIPPED | /* SMB0-SDA */ \
  P1SKIP_B6__NOT_SKIPPED | /* SMB0-SCL */ \
  P1SKIP_B7__SKIPPED;

#define XBAR_P2_SKIP   \
  P2SKIP_B0__SKIPPED | \
  P2SKIP_B1__SKIPPED | \
  P2SKIP_B2__SKIPPED | \
  P2SKIP_B3__SKIPPED | \
  P2SKIP_B4__SKIPPED | \
  P2SKIP_B5__SKIPPED | \
  P2SKIP_B6__SKIPPED | \
  P2SKIP_B7__SKIPPED;

// Port 0

#define PORT_P0_MODE_IN \
  P0MDIN_B0__ANALOG | \
  P0MDIN_B1__ANALOG | \
  P0MDIN_B2__ANALOG | \
  P0MDIN_B3__ANALOG | \
  P0MDIN_B4__DIGITAL | /* UART0-TX */ \
  P0MDIN_B5__DIGITAL | /* UART0-RX */\
  P0MDIN_B6__ANALOG | \
  P0MDIN_B7__ANALOG;

#define PORT_P0_MODE_OUT \
  P0MDOUT_B0__OPEN_DRAIN | \
  P0MDOUT_B1__OPEN_DRAIN | \
  P0MDOUT_B2__OPEN_DRAIN | \
  P0MDOUT_B3__OPEN_DRAIN | \
  P0MDOUT_B4__PUSH_PULL |  /* UART0-TX */ \
  P0MDOUT_B5__OPEN_DRAIN | /* UART0-RX */\
  P0MDOUT_B6__OPEN_DRAIN | \
  P0MDOUT_B7__OPEN_DRAIN;

#define PORT_P0_LATCH \
  P0_B0__HIGH | \
  P0_B1__HIGH | \
  P0_B2__HIGH | \
  P0_B3__HIGH | \
  P0_B4__HIGH | \
  P0_B5__HIGH | \
  P0_B6__HIGH | \
  P0_B7__HIGH;

// Port 1

#define PORT_P1_MODE_IN \
  P1MDIN_B0__ANALOG | \
  P1MDIN_B1__ANALOG | \
  P1MDIN_B2__ANALOG | \
  P1MDIN_B3__ANALOG | \
  P1MDIN_B4__DIGITAL | /* LED */ \
  P1MDIN_B5__DIGITAL | /* SMB0-SDA */ \
  P1MDIN_B6__DIGITAL | /* SMB0-SCL */ \
  P1MDIN_B7__ANALOG;

#define PORT_P1_MODE_OUT \
  P1MDOUT_B0__OPEN_DRAIN | \
  P1MDOUT_B1__OPEN_DRAIN | \
  P1MDOUT_B2__OPEN_DRAIN | \
  P1MDOUT_B3__OPEN_DRAIN | \
  P1MDOUT_B4__PUSH_PULL | \
  P1MDOUT_B5__OPEN_DRAIN | /* SMB0-SDA */ \
  P1MDOUT_B6__OPEN_DRAIN | /* SMB0-SCL */ \
  P1MDOUT_B7__OPEN_DRAIN;

#define PORT_P1_LATCH \
  P1_B0__HIGH | \
  P1_B1__HIGH | \
  P1_B2__HIGH | \
  P1_B3__HIGH | \
  P1_B4__HIGH | \
  P1_B5__HIGH | \
  P1_B6__HIGH | \
  P1_B7__HIGH;

// Port 2

#define PORT_P2_MODE_IN \
  P2MDIN_B0__ANALOG | \
  P2MDIN_B1__ANALOG | \
  P2MDIN_B2__ANALOG | \
  P2MDIN_B3__ANALOG | \
  P2MDIN_B4__ANALOG | \
  P2MDIN_B5__ANALOG | \
  P2MDIN_B6__ANALOG | \
  P2MDIN_B7__ANALOG;

#define PORT_P2_MODE_OUT \
  P2MDOUT_B0__OPEN_DRAIN | \
  P2MDOUT_B1__OPEN_DRAIN | \
  P2MDOUT_B2__OPEN_DRAIN | \
  P2MDOUT_B3__OPEN_DRAIN | \
  P2MDOUT_B4__OPEN_DRAIN | \
  P2MDOUT_B5__OPEN_DRAIN | \
  P2MDOUT_B6__OPEN_DRAIN | \
  P2MDOUT_B7__OPEN_DRAIN;

#define PORT_P2_LATCH \
  P2_B0__HIGH | \
  P2_B1__HIGH | \
  P2_B2__HIGH | \
  P2_B3__HIGH | \
  P2_B4__HIGH | \
  P2_B5__HIGH | \
  P2_B6__HIGH | \
  P2_B7__HIGH;

/*** UART0 ***/

// Enabling UART0 allocates Timer1 for it
#define UART0_ENABLED   CONSOLE_ENABLED

// UART0 baud rate
#define UART0_BAUD  115200


/*** SMB0 & SMB1 ***/

// Enabling SMB0 and/or SMB1 allocates Timer2 for it
// *** NOT IMPLEMENTED *** Enabling SMB0 allocates Timer3 for it
#define SMB0_ENABLED  1   // 1 = Enabled, 0 = Disabled
#define SMB1_ENABLED  0   // 1 = Enabled, 0 = Disabled

#define SMB0_SCL_SPEED   100000

#endif
