#ifndef _EFM8BB52_WDT_H_
#define _EFM8BB52_WDT_H_

#include <settings.h>

#ifndef WDT_ENABLED
#error "WDT_ENABLED is not defined"
#endif

void WDT_on_startup(void);

void WDT_configure(void);

#endif
