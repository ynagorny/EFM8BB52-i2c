#include <EFM8BB52/SYSCLK.h>
#include <EFM8BB52/SMB0.h>

#define SMB0_REG_BANK 1

/*** Private ***/

bit _SMB0_busy;
bit _SMB0_success;

byte data _SMB0_address;

byte data* _SMB0_tx;
byte data  _SMB0_tx_remains;

byte data* _SMB0_rx;
byte data  _SMB0_rx_remains;
byte data* _SMB0_rx_size;

void SMB0_timer2_config(void) {
  TMR2CN0_T2SPLIT = 1;

  // Use SYSCLK as the clock source for low part of Timer2
  CKCON0 |= CKCON0_T2ML__SYSCLK;

  // Set Timer2 Low overflow rate to 3x of SMB0 SCL rate
#define SMB0_SYSCLK_PERIODS (SYSCLK_FREQUENCY / (SMB0_SCL_SPEED * 3))
#if SMB0_SYSCLK_PERIODS > 255
#error "SMB0_SCL_SPEED is too low"
#elif SMB0_SYSCLK_PERIODS < 8
#error "SMB0_SCL_SPEED is too high"
#endif

  TMR2RLL = 256 - SMB0_SYSCLK_PERIODS;
}

void SMB0_control_config(void) {
  SMB0CF = SMB0CF_INH__SLAVE_DISABLED | SMB0CF_SMBCS__TIMER2_LOW;
  XBR0 |= XBR0_SMB0E__ENABLED;
}

void SMB0_config(void) {
  SMB0_timer2_config();
  SMB0_control_config();
}

void SMB0_enable(void) {
  SMB0CN0 = 0x00;
  EIE1 |= EIE1_ESMB0__ENABLED;
  SMB0CF |= SMB0CF_ENSMB__ENABLED;

  _SMB0_success = 1;
  _SMB0_busy = 0;
}

void SMB0_disable(void) {
  SMB0CF &= ~SMB0CF_ENSMB__BMASK;
  SMB0CN0 &= ~(SMB0CN0_STA__BMASK | SMB0CN0_STO__BMASK | SMB0CN0_ACKRQ__BMASK);
  EIE1 &= ~EIE1_ESMB0__BMASK;
  SMB0CN0 = 0x00;

  _SMB0_success = 0;
  _SMB0_busy = 0;
}

void SMB0_reset(void) {
  SMB0_disable();
  SMB0_enable();
}

void SMB0_complete_and_reset(void) {
  if (! SMB0_complete()) {
      SMB0_reset();
  }
}

void SMB0_setup_address(byte address) {
  _SMB0_address = address;
}

void SMB0_setup_tx(byte data* tx, byte tx_size) {
  _SMB0_tx = tx;
  _SMB0_tx_remains = tx_size;
}

void SMB0_setup_rx(byte data* rx, byte max_rx_size, byte data* rx_size) {
  _SMB0_rx = rx;
  _SMB0_rx_remains = max_rx_size;
  *(_SMB0_rx_size = rx_size) = 0;
}

void SMB0_start(void) {
  SMB0CN0_STA = 1;
  _SMB0_busy = 1;
}

SI_INTERRUPT_USING(SMB0_ISR, SMBUS0_IRQn, SMB0_REG_BANK) {
  SFRPAGE = 0x00;

  switch (SMB0CN0 & (SMB0CN0_MASTER__BMASK | SMB0CN0_TXMODE__BMASK | SMB0CN0_STA__BMASK | SMB0CN0_STO__BMASK)) {
    case SMB0CN0_MASTER__MASTER | SMB0CN0_TXMODE__TRANSMITTER | SMB0CN0_STA__SET:
      SMB0CN0_STA = 0;
      SMB0CN0_STO = 0;
      SMB0DAT = (_SMB0_address << 1) + (_SMB0_tx_remains > 0 ? 0 : 1);
      break;

    case SMB0CN0_MASTER__MASTER | SMB0CN0_TXMODE__TRANSMITTER:
      if (SMB0CN0_ACK) {
          if (_SMB0_tx_remains) {
              SMB0DAT = *_SMB0_tx++;
              _SMB0_tx_remains--;
          } else {
              SMB0CN0_STO = 1;
              _SMB0_success = 1;
              _SMB0_busy = 0;
          }
      } else {
          SMB0CN0_STO = 1;
          _SMB0_success = (_SMB0_tx_remains == 0);
          _SMB0_busy = 0;
      }
      break;

    default:
      SMB0_reset();
      _SMB0_success = 0;
      _SMB0_busy = 0;
  }

  SMB0CN0_SI = 0;
}

/*** Public ***/

void SMB0_configure(void) {
#if SMB0_ENABLED == 1
  SFRPAGE = 0x00;
  SMB0_config();
  SMB0_enable();
#endif
}

bool SMB0_busy(void) {
  return _SMB0_busy;
}

bool SMB0_success(void) {
  return _SMB0_success;
}

bool SMB0_complete(void) {
  while(SMB0_busy()) {}
  return SMB0_success();
}

void SMB0_transfer(byte address, byte data* request, byte request_size, byte data* response, byte max_response_size, byte data* response_size) {
  SFRPAGE = 0x00;

  SMB0_complete_and_reset();

  SMB0_setup_address(address);
  SMB0_setup_tx(request, request_size);
  SMB0_setup_rx(response, max_response_size, response_size);

  SMB0_start();
}
