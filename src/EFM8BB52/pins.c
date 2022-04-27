#include <EFM8BB52/pins.h>

void pins_XBAR_configure(void) {
  SFRPAGE = 0x20;

#if XBAR_ENABLED == 1
  XBR2 |= XBR2_XBARE__ENABLED;
  P0SKIP = XBAR_P0_SKIP;
  P1SKIP = XBAR_P1_SKIP;
  P2SKIP = XBAR_P2_SKIP;
#else
  XBR2 &= ~XBR2_XBARE__BMASK;
#endif
}

void pins_P0_configure(void) {
  SFRPAGE = 0x20;

  P0MDIN = PORT_P0_MODE_IN;
  P0MDOUT = PORT_P0_MODE_OUT;
  P0 = PORT_P0_LATCH;
}

void pins_P1_configure(void) {
  SFRPAGE = 0x20;

  P1MDIN = PORT_P1_MODE_IN;
  P1MDOUT = PORT_P1_MODE_OUT;
  P1 = PORT_P1_LATCH;
}

void pins_P2_configure(void) {
  SFRPAGE = 0x20;

  P2MDIN = PORT_P2_MODE_IN;
  P2MDOUT = PORT_P2_MODE_OUT;
  P2 = PORT_P2_LATCH;
}

void pins_configure(void) {
  pins_XBAR_configure();
  pins_P0_configure();
  pins_P1_configure();
  pins_P2_configure();
}
