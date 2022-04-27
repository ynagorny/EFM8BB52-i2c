#ifndef _EFM8BB52_XBAR_H_
#define _EFM8BB52_XBAR_H_

#include <settings.h>

#ifndef XBAR_ENABLED
#error "XBAR_ENABLED needs to be defined"
#endif

#ifndef XBAR_P0_SKIP
#error "XBAR_P0_SKIP needs to be defined"
#endif

#ifndef XBAR_P1_SKIP
#error "XBAR_P1_SKIP needs to be defined"
#endif

#ifndef XBAR_P2_SKIP
#error "XBAR_P2_SKIP needs to be defined"
#endif

#ifndef PORT_P0_MODE_IN
#error "PORT_P0_MODE_IN needs to be defined"
#endif

#ifndef PORT_P0_MODE_OUT
#error "PORT_P0_MODE_OUT needs to be defined"
#endif

#ifndef PORT_P0_LATCH
#error "PORT_P0_LATCH needs to be defined"
#endif

#ifndef PORT_P1_MODE_IN
#error "PORT_P1_MODE_IN needs to be defined"
#endif

#ifndef PORT_P1_MODE_OUT
#error "PORT_P1_MODE_OUT needs to be defined"
#endif

#ifndef PORT_P1_LATCH
#error "PORT_P1_LATCH needs to be defined"
#endif

#ifndef PORT_P2_MODE_IN
#error "PORT_P2_MODE_IN needs to be defined"
#endif

#ifndef PORT_P2_MODE_OUT
#error "PORT_P2_MODE_OUT needs to be defined"
#endif

#ifndef PORT_P2_LATCH
#error "PORT_P2_LATCH needs to be defined"
#endif


void pins_configure(void);

#endif
