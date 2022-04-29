#ifndef _EFM8BB52_DELAY_H_
#define _EFM8BB52_DELAY_H_

/* all methods here will reconfigure and use Timer 5 */

void delay(unsigned int seconds);
void delay_ms(unsigned int milliseconds);
void delay_us(unsigned long microseconds);

#endif
