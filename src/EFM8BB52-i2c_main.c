#include <stdio.h>

#include <EFM8BB52/EFM8BB52.h>

#define DISPLAY_ADDRESS 0x3C

#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_CHARGE_PUMP_ON 0x14
#define SSD1306_PIN_MODE 0xDA
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_SET_MULTIPLEX 0x1F
#define SSD1306_DISPLAY_ENTIRE_OFF 0xA4
#define SSD1306_DISPLAY_ENTIRE_ON 0xA5
#define SSD1306_ADDRESSING_MODE 0x20
#define SSD1306_PAGES 0x22
#define SSD1306_COLUMNS 0x21

SI_SBIT(LED0, SFR_P1, 4);                  // P1.4 LED0

void display_write_command(byte command) {
  SMB0_start_write(DISPLAY_ADDRESS);
  SMB0_write(0x00);
  SMB0_write(command);
  SMB0_stop();
}

void display_write_command2(byte command1, byte command2) {
  SMB0_start_write(DISPLAY_ADDRESS);
  SMB0_write(0x00);
  SMB0_write(command1);
  SMB0_write(command2);
  SMB0_stop();
}

void display_write_command3(byte command1, byte command2, byte command3) {
  SMB0_start_write(DISPLAY_ADDRESS);
  SMB0_write(0x00);
  SMB0_write(command1);
  SMB0_write(command2);
  SMB0_write(command3);
  SMB0_stop();
}

void display_write_command_list(byte commands[], u8 count) {
  u8 i;

  SMB0_start_write(DISPLAY_ADDRESS);
  SMB0_write(0x00);

  for (i = 0; i < count; i++) {
    SMB0_write(commands[i]);
  }

  SMB0_stop();
}

void SiLabs_Startup(void) {
  MCU_startup();
}

void configure(void) {
  MCU_configure();
}

int main(void) {
  unsigned char k = 0;
  unsigned int r, c;

  configure();

#ifdef CONSOLE_ENABLED
  printf("BB52 Explorer Kit I2C\n");
#endif

  // Enable interrupts
  IE_EA = 1;

  delay_ms(500);
  display_write_command(SSD1306_DISPLAY_OFF);
  display_write_command(SSD1306_DISPLAY_ENTIRE_OFF);
  display_write_command(SSD1306_SET_MULTIPLEX);
  display_write_command2(SSD1306_ADDRESSING_MODE, 0x00);
  display_write_command2(SSD1306_PIN_MODE, 0x02);
  display_write_command2(SSD1306_CHARGE_PUMP, 0x14);
  delay_ms(100);
  display_write_command(SSD1306_DISPLAY_ON);
  delay_ms(100);

  while(1) {
    k = k << 1;
    if (k == 0) k = 1;

    display_write_command3(SSD1306_PAGES, 0x00, 0x03);
    display_write_command3(SSD1306_COLUMNS, 0x00, 0x7F);

    for (r = 0; r < 4; r++) {
      SMB0_start_write(DISPLAY_ADDRESS);
      SMB0_write(0x40);
      for (c = 0; c < 128; c++) {
          SMB0_write(k);
      }
      SMB0_stop();
    }

    LED0 = !LED0;
  }
}
