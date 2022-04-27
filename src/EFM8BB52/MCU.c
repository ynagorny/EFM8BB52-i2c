#include <EFM8BB52/WDT.h>
#include <EFM8BB52/MCU.h>
#include <EFM8BB52/pins.h>
#include <EFM8BB52/SYSCLK.h>
#include <EFM8BB52/UART0.h>
#include <EFM8BB52/SMB0.h>

void MCU_startup(void) {
  WDT_on_startup();
}

void MCU_configure(void) {
  WDT_configure();
  SYSCLK_configure();
  pins_configure();
  UART0_configure();
  SMB0_configure();
}
