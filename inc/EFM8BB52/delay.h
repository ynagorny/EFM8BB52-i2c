#ifndef _EFM8BB52_DELAY_H_
#define _EFM8BB52_DELAY_H_

#include <EFM8BB52/types.h>

/* all methods here will reconfigure and use Timer 5 */

void delay(u16 seconds);
void delay_ms(u16 milliseconds);
void delay_us(u32 microseconds);

#endif
