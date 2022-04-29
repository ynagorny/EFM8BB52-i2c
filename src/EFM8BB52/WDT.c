#include <SI_EFM8BB52_Register_Enums.h>

#include <EFM8BB52/WDT.h>

void WDT_disable(void) {
  WDTCN = 0xDE;
  WDTCN = 0xAD;
}

void WDT_on_startup(void) {
#if WDT_ENABLED == -1
  WDT_disable();
#endif
}

void WDT_configure(void) {
#if WDT_ENABLED == 0
  WDT_disable();
#endif
}
