#ifndef _EFM8BB52_SMB0_H_
#define _EFM8BB52_SMB0_H_

#include <EFM8BB52/types.h>

#include <settings.h>

typedef enum {
  SMB_write = 1,
  SMB_read  = 2,
  SMB_write_then_read = 3,
} SMB_direction;

typedef enum {
  SMB_unconfigured =  0,
  SMB_configured,
  SMB_idle,
  SMB_leader_starting,
  SMB_leader_sending,
  SMB_leader_receiving,
} SMB_state;


/*** Configuration ***/

// call to configure SMB0
void SMB0_configure(void);

// stop all transmissions immediately and reset
void SMB0_reset();

/*** COMMON ***/

SMB_state SMB0_get_state(void);

/*** LEADER ***/

// call to start or restart transfer
// on every byte received SMB_acknowledge_received_callback will be called if set
void SMB0_start(byte to_address, SMB_direction direction, u8 receive_count);
void SMB0_start_write(byte to_address);
void SMB0_start_read(byte from_address, u8 receive_count);
void SMB0_start_write_then_read(byte address, u8 receive_count);

// it will wait for all data to be sent or all data to be received and then stop
void SMB0_stop();

/*** FOLOWER ***/

// allow to be a follower on the address with the mask
// if read buffer is not empty, automatic not acknowledged will be set, call SMB0_read_reset()
void SMB0_follow(byte address, byte mask);

// disable follower mode
void SMB0_unfollow(void);

/*** WRITING ***/

// returns how many bytes can be accepted for writing
u8 SMB0_write_room(void);

// add to send buffer, if buffer is full, it will wait
void SMB0_write(byte d);

/*** READING ***/

// returns number of bytes available to read
u8 SMB0_received(void);

// call to read next received byte of data; it will block if no data is available
byte SMB0_read(void);

// call to reset read buffer
void SMB0_read_reset(void);


#endif
