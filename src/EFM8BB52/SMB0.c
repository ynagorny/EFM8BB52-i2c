#include <EFM8BB52/SYSCLK.h>
#include <EFM8BB52/SMB0.h>

#define SMB0_REG_BANK 1
#pragma NOAREGS

#define SMB0_TX_BUFFER_SIZE 8
#define SMB0_RX_BUFFER_SIZE 8

/*** Private ***/

SMB_state data _SMB0_state = SMB_unconfigured;

byte data _SMB0_address;
SMB_direction data _SMB0_direction;

byte data _SMB0_tx[SMB0_TX_BUFFER_SIZE];
u8 data _SMB0_tx_read_at = 0;
u8 data _SMB0_tx_write_at = 0;

byte data _SMB0_rx[SMB0_RX_BUFFER_SIZE];
u8 data _SMB0_rx_read_at = 0;
u8 data _SMB0_rx_write_at = 0;
u8 data _SMB0_rx_max_write_at = 0;

void SMB0_reset_tx_buffer(void) {
  _SMB0_tx_read_at = 0;
  _SMB0_tx_write_at = 0;
}

void SMB0_reset_rx_buffer(void) {
  _SMB0_rx_read_at = 0;
  _SMB0_rx_write_at = 0;
}

void SMB0_reset_buffers(void) {
  SMB0_reset_tx_buffer();
  SMB0_reset_rx_buffer();
}

bool SMB0_has_data_to_tx(void) {
  return _SMB0_tx_read_at < _SMB0_tx_write_at;
}

byte SMB0_get_next_byte_to_send(void) {
  if (_SMB0_tx_read_at < _SMB0_tx_write_at) {
    byte d = _SMB0_tx[_SMB0_tx_read_at++];
    if (_SMB0_tx_read_at == _SMB0_tx_write_at) {
        _SMB0_tx_read_at = _SMB0_tx_write_at = 0;
    }
    return d;
  } else {
    return 0x00;
  }
}

bool SMB0_need_to_receive_more() {
  return _SMB0_rx_write_at < _SMB0_rx_max_write_at;
}

bool SMB0_need_to_receive_last() {
  return _SMB0_rx_write_at + 1 == _SMB0_rx_max_write_at;
}

void SMB0_receive_byte(byte d) {
  if (_SMB0_rx_write_at < _SMB0_rx_max_write_at) {
      _SMB0_rx[_SMB0_rx_write_at++] = d;
  }
}

void SMB0_timer2_config_and_start(void) {
  TMR2CN0_T2SPLIT = 1;

  // Use SYSCLK as the clock source for low part of Timer2
  CKCON0 |= CKCON0_T2ML__SYSCLK;

  // Set Timer2 Low overflow rate to 3x of SMB0 SCL rate
#define SMB0_SYSCLK_PERIODS (SYSCLK_FREQUENCY / (SMB0_SCL_SPEED * 3))
#if SMB0_SYSCLK_PERIODS > 255
#error "SMB0_SCL_SPEED is too low"
#elif SMB0_SYSCLK_PERIODS < 4
#error "SMB0_SCL_SPEED is too high"
#endif

  TMR2RLL = 256 - SMB0_SYSCLK_PERIODS;
}

void SMB0_control_config(void) {
  SMB0ADM = SMB0ADM_EHACK__ADR_ACK_AUTOMATIC;
  SMB0CF = SMB0CF_INH__SLAVE_DISABLED | SMB0CF_SMBCS__TIMER2_LOW;
  XBR0 |= XBR0_SMB0E__ENABLED;
}

void SMB0_config(void) {
  SMB0_timer2_config_and_start();
  SMB0_control_config();

  _SMB0_state = SMB_configured;
}

void SMB0_enable(void) {
  SMB0CN0 = 0x00;
  EIE1 |= EIE1_ESMB0__ENABLED;
  SMB0CF |= SMB0CF_ENSMB__ENABLED;

  _SMB0_state = SMB_idle;
}

void SMB0_disable(void) {
  SMB0CF &= ~SMB0CF_ENSMB__BMASK;
  SMB0CN0 &= ~(SMB0CN0_STA__BMASK | SMB0CN0_STO__BMASK | SMB0CN0_ACKRQ__BMASK);
  EIE1 &= ~EIE1_ESMB0__BMASK;
  SMB0CN0 = 0x00;

  SMB0_reset_buffers();

  _SMB0_state = SMB_configured;
}

void SMB0_setup_address(byte address) {
  _SMB0_address = address;
}

SI_INTERRUPT_USING(SMB0_ISR, SMBUS0_IRQn, SMB0_REG_BANK) {
  SFRPAGE = 0x00;

  switch (SMB0CN0 & (SMB0CN0_MASTER__BMASK | SMB0CN0_TXMODE__BMASK | SMB0CN0_STA__BMASK | SMB0CN0_STO__BMASK)) {
    case SMB0CN0_MASTER__MASTER | SMB0CN0_TXMODE__TRANSMITTER | SMB0CN0_STA__SET:
      if (_SMB0_state == SMB_leader_starting) {
        bool write = _SMB0_direction == SMB_write || _SMB0_direction == SMB_write_then_read;
        SMB0CN0_STA = 0;
        SMB0CN0_STO = 0;
        SMB0DAT = (_SMB0_address << 1) | (write ? 0 : 1);
        _SMB0_state = write ? SMB_leader_sending : SMB_leader_receiving;
      } else {
        SMB0_reset();
      }
      break;

    case SMB0CN0_MASTER__MASTER | SMB0CN0_TXMODE__TRANSMITTER:
      if (_SMB0_state == SMB_leader_sending) {
        if (SMB0CN0_ACK && SMB0_has_data_to_tx()) {
          SMB0DAT = SMB0_get_next_byte_to_send();
        } else {
          if (_SMB0_direction == SMB_write_then_read) {
            SMB0CN0_STA = 1;

            _SMB0_direction = SMB_read;
            _SMB0_state = SMB_leader_starting;
          } else {
            SMB0CN0_STO = 1;
            _SMB0_state = SMB_idle;
          }
        }
      } else if (_SMB0_state == SMB_leader_receiving) {
        if (SMB0CN0_ACK) {
          if (SMB0_need_to_receive_last()) {
            SMB0CN0_ACK = 0;
          } else {
            SMB0CN0_ACK = 1;
          }
        } else {
          SMB0CN0_STO = 1;
          _SMB0_state = SMB_idle;
        }
      } else {
        SMB0_reset();
      }
      break;

  case SMB0CN0_MASTER__MASTER | SMB0CN0_TXMODE__RECEIVER:
    if (_SMB0_state == SMB_leader_receiving) {
      SMB0_receive_byte(SMB0DAT);

      if (SMB0_need_to_receive_more()) {
          if (SMB0_need_to_receive_last()) {
            SMB0CN0_ACK = 0;
          } else {
            SMB0CN0_ACK = 1;
          }
      } else {
          SMB0CN0_STO = 1;
          _SMB0_state = SMB_idle;
      }
    } else {
      SMB0_reset();
    }
    break;

    default:
      SMB0_reset();
  }

  SMB0CN0_SI = 0;
}

void SMB0_wait_for_state(SMB_state state) {
  while (_SMB0_state != state) { }
}

/*** Public ***/

void SMB0_configure(void) {
  if (_SMB0_state != SMB_unconfigured) return;

#if SMB0_ENABLED == 1
  SMB0_config();
  SMB0_enable();
#endif
}

void SMB0_reset(void) {
  SMB0_disable();
  SMB0_enable();
}

void SMB0_start(byte to_address, SMB_direction direction, u8 receive_count) {
  SMB0_wait_for_state(SMB_idle);

  _SMB0_address = to_address;
  _SMB0_direction = direction;
  _SMB0_rx_max_write_at = receive_count;

  SMB0_reset_buffers();

  _SMB0_state = SMB_leader_starting;

  SMB0CN0_STA = 1;
}

void SMB0_start_write(byte to_address) {
  SMB0_start(to_address, SMB_write, 0);
}

void SMB0_start_read(byte from_address, u8 receive_count) {
  SMB0_start(from_address, SMB_read, receive_count);
}

void SMB0_start_write_then_read(byte address, u8 receive_count) {
  SMB0_start(address, SMB_write_then_read, receive_count);
}


void SMB0_stop(void) {
  SMB0_wait_for_state(SMB_idle);
}

void SMB0_write(byte d) {
  while (_SMB0_tx_write_at >= SMB0_TX_BUFFER_SIZE) { }
  IE_EA = 0;
  _SMB0_tx[_SMB0_tx_write_at++] = d;
  IE_EA = 1;
}

byte SMB0_read(void) {
  byte d;
  while (_SMB0_rx_read_at >= _SMB0_rx_write_at) { }
  IE_EA = 0;
  d = _SMB0_rx[_SMB0_rx_read_at++];
  if (_SMB0_rx_read_at == _SMB0_rx_write_at) {
      _SMB0_rx_max_write_at -= _SMB0_rx_write_at;
      _SMB0_rx_read_at = _SMB0_rx_write_at = 0;
  }
  IE_EA = 1;
  return d;
}


