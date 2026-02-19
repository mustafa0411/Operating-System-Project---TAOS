#ifndef MPX_SYS_REQ_H
#define MPX_SYS_REQ_H

#include <mpx/device.h>

/**
 @file sys_req.h
 @brief System request function and constants
*/

typedef enum {
	EXIT,
	IDLE,
	READ,
	WRITE,
} op_code;
    
// error codes
#define INVALID_OPERATION	(-1)
#define INVALID_BUFFER		(-2)
#define INVALID_COUNT		(-3)

/**
 Request an MPX kernel operation.
 @param op_code One of READ, WRITE, IDLE, or EXIT
 @param ... As required for READ or WRITE
 @return Varies by operation
*/ 
int sys_req(op_code op, ...);
 
#endif
