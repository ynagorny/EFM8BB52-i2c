#include <stdio.h>

#include <EFM8BB52/EFM8BB52.h>

byte SSD1306_CHARGE_PUMP[] = {0x00, 0x8D};
byte SSD1306_CHARGE_PUMP_ON[] = {0x00, 0x14};
byte SSD1306_PIN_MODE[] = {0x00, 0xDA, 0x02};
byte SSD1306_DISPLAY_OFF[] = {0x00, 0xAE};
byte SSD1306_DISPLAY_ON[] = {0x00, 0xAF};
byte SSD1306_SET_MULTIPLEX[] = {0x00, 0x1F};
byte SSD1306_DISPLAY_ENTIRE_OFF[] = {0x00, 0xA4};
byte SSD1306_DISPLAY_ENTIRE_ON[] = {0x00, 0xA5};
byte SSD1306_ADDRESSING_MODE[] = {0x00, 0x20, 0x00};
byte SSD1306_RESET_POINTER[] = {0x00, 0x22, 0x00, 0x03, 0x21, 0x00, 0x7F};
byte SSD1306_SAMPLE[] = {0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

SI_SBIT(LED0, SFR_P1, 4);                  // P1.4 LED0

void SiLabs_Startup(void) {
  MCU_startup();
}

void configure(void) {
  MCU_configure();
}

int main(void) {
  unsigned char k = 0;
  unsigned int i;

  configure();

#ifdef CONSOLE_ENABLED
  printf("BB52 Explorer Kit I2C\n");
#endif

  // Enable interrupts
  IE_EA = 1;

  delay_ms(500);
  SMB0_transfer(0x3C, &SSD1306_DISPLAY_OFF, sizeof(SSD1306_DISPLAY_OFF), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_DISPLAY_ENTIRE_OFF, sizeof(SSD1306_DISPLAY_ENTIRE_OFF), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_SET_MULTIPLEX, sizeof(SSD1306_SET_MULTIPLEX), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_ADDRESSING_MODE, sizeof(SSD1306_ADDRESSING_MODE), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_PIN_MODE, sizeof(SSD1306_PIN_MODE), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_CHARGE_PUMP, sizeof(SSD1306_CHARGE_PUMP_ON), NULL, 0, NULL);
  SMB0_transfer(0x3C, &SSD1306_CHARGE_PUMP_ON, sizeof(SSD1306_CHARGE_PUMP_ON), NULL, 0, NULL);
  delay_ms(100);
  SMB0_transfer(0x3C, &SSD1306_DISPLAY_ON, sizeof(SSD1306_DISPLAY_ON), NULL, 0, NULL);
  delay_ms(100);

  while(1) {
    k = k << 1;
    if (k == 0) k = 1;

    SMB0_transfer(0x3C, &SSD1306_RESET_POINTER,     sizeof(SSD1306_RESET_POINTER), NULL, 0, NULL);

    for (i = 1; i <= 8; i++) {
      SSD1306_SAMPLE[i] = k;
    }

    for (i = 0; i < 64; i++) {
      SMB0_transfer(0x3C, &SSD1306_SAMPLE, sizeof(SSD1306_SAMPLE), NULL, 0, NULL);
    }

    LED0 = !LED0;
  }
}
