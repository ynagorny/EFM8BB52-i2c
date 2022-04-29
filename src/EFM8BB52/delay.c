#include <EFM8BB52/SYSCLK.h>
#include <EFM8BB52/delay.h>

// Timer 5 will run at SYSCLK / 12, so max time will be 65,535 * 1,000,000,000 / ( SYSCLK / 12 ) nanoseconds
#define ONE_STEP_DELAY_NS (1000000000 / (SYSCLK_FREQUENCY / 12))
#define MAX_STEP_DELAY_US (65535 * ONE_STEP_DELAY_NS / 1000)

void delay_step_us(u32 microseconds) {
  u16 start_at;

  if (microseconds == 0) {
      return;
  }

  start_at = 65536 - (microseconds) * 1000 / ONE_STEP_DELAY_NS;

  SFRPAGE = 0x10;

  TMR5CN0 =
      TMR5CN0_TF5H__NOT_SET |
      TMR5CN0_TF5L__NOT_SET |
      TMR5CN0_TF5LEN__DISABLED |
      TMR5CN0_TF5CEN__DISABLED |
      TMR5CN0_T5SPLIT__16_BIT_RELOAD |
      TMR5CN0_TR5__STOP |
      TMR5CN0_T5XCLK__SYSCLK_DIV_12;

  TMR5CN1 =
      TMR5CN1_RLFSEL__NONE |
      TMR5CN1_T5CSEL__LFOSC;

  TMR5RLL = 0x00;
  TMR5RLH = 0x00;

  TMR5L = start_at & 0xFF;
  TMR5H = start_at >> 8;

  TMR5CN0_TR5 = 1;

  while (!TMR5CN0_TF5H) {}

  TMR5CN0_TR5 = 0;

  SFRPAGE = 0x00;
}

void delay_us(u32 microseconds) {
  while (microseconds > MAX_STEP_DELAY_US) {
      delay_step_us(MAX_STEP_DELAY_US);
      microseconds -= MAX_STEP_DELAY_US;
  }

  delay_step_us(microseconds);
}

void delay_ms(u16 milliseconds) {
  unsigned long microseconds = ((u32) milliseconds) * 1000;
  delay_us(microseconds);
}

void delay(u16 seconds) {
  unsigned long microseconds = ((u32) seconds) * 1000000;
  delay_us(microseconds);
}

