#ifndef _EFM8BB52_SMB0_H_
#define _EFM8BB52_SMB0_H_

#include <EFM8BB52/types.h>

#include <settings.h>

void SMB0_configure(void);

// TRUE if another transfer is in progress
bool SMB0_busy(void);

// TRUE if the last transfer was a success
// returns FALSE if SMB0 is busy
bool SMB0_success(void);

// waits for SMB0 to complete last transfer and returns SMB0_success()
bool SMB0_complete(void);

// sends request to follower, receives response
// if nothing to send, supply NULL request and request_size == 0
// if nothing to receive, supply NULL response and response_size == 0
// if another transfer is in progress, it will wait
void SMB0_transfer(byte to_address, byte data * request, byte request_size, byte data* response, byte max_response_size, byte data* response_size);

#endif
