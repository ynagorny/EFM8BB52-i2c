#include <EFM8BB52/SYSCLK.h>

#define HIGH_FREQUENCY_CUTOFF 24000000

bool is_divider_ready() {
  return (CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__READY;
}

void wait_divider_ready() {
  while (! is_divider_ready()) { }
}

void set_clock_and_divider(char clock, char divider) {
  wait_divider_ready();
  CLKSEL = clock | divider;
}

void SYSCLK_configure(void) {
  // two step for setting high frequencies
#if SYSCLK_FREQUENCY > HIGH_FREQUENCY_CUTOFF
   set_clock_and_divider(CLKSEL_CLKSL__HFOSC0_clk24p5, CLKSEL_CLKDIV__SYSCLK_DIV_1);
#endif
  set_clock_and_divider(SYSCLK_CLOCK, SYSCLK_DIV);
}
