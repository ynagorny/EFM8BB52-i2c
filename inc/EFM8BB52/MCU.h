#ifndef _EFM8BB52_MCU_H_
#define _EFM8BB52_MCU_H_

// call as early as possible after a RESET
void MCU_startup(void);

// call to configure MCU
void MCU_configure(void);

#endif
