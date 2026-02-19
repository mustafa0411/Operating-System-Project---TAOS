/***********************************************************************
* This file contains the sys_req() function linking MPX userland to
* kernel space, as well as functions that need to be instantiated as
* processes for modules R3 and R4.
*
* You should not need to make any modifications to this file. Doing so
* may result in losing points.
***********************************************************************/

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <mpx/serial.h>

#include <processes.h>
#include <sys_req.h>

/* For R3: How many times each process prints its message */
#define RC_1 1
#define RC_2 2
#define RC_3 3
#define RC_4 4
#define RC_5 5

/***********************************************************************/
/* Issue a request to the kernel. */
/***********************************************************************/
int sys_req(op_code op, ...)
{
	device dev = 0;
	char *buffer = NULL;
	size_t len = 0;

	if (op == READ || op == WRITE) {
		va_list ap;
		va_start(ap, op);
		dev = va_arg(ap, device);
		buffer = va_arg(ap, char *);
		len = va_arg(ap, size_t);
		va_end(ap);
	}

	int ret = 0;
	__asm__ volatile("int $0x60" : "=a"(ret) : "a"(op), "b"(dev), "c"(buffer), "d"(len));

	if (ret == -1 && (op == READ || op == WRITE)) {
		return (op == READ)
			? serial_poll(dev, buffer, len)
			: serial_out(dev, buffer, len);
	}

	return ret;
}

/***********************************************************************/
/* Code common to all R3 processes */
/* DO NOT TRY TO CREATE A PROCESS FOR THIS FUNCTION!!! */
/***********************************************************************/
static void r3_proc(int iterations, const char *procname)
{
	char output[100];
	char *crlf = "\r\n";

	char *dispatched = "dispatched ";
	memcpy(output, dispatched, strlen(dispatched) + 1);
	memcpy(output + strlen(output), procname, strlen(procname) + 1);
	memcpy(output + strlen(output), crlf, strlen(crlf) + 1);
	for (int i = 0; i < iterations; i++) {
		sys_req(WRITE, COM1, output, strlen(output));
		sys_req(IDLE);
	}

	char *exiting = "exiting ";
	memcpy(output, exiting, strlen(exiting) + 1);
	memcpy(output + strlen(output), procname, strlen(procname) + 1);
	memcpy(output + strlen(output), crlf, strlen(crlf) + 1);
	sys_req(WRITE, COM1, output, strlen(output));
	sys_req(EXIT);

	char *after = "FAILURE TO EXIT: ";
	memcpy(output, after, strlen(after) + 1);
	memcpy(output + strlen(output), procname, strlen(procname) + 1);
	memcpy(output + strlen(output), crlf, strlen(crlf) + 1);
	for (;;) {
		sys_req(WRITE, COM1, output, strlen(output));
		sys_req(EXIT);
	}
}

/***********************************************************************/
/* BEGIN R3 PROCESSES -- Create one process per function */
/***********************************************************************/
void proc1(void)
{
	r3_proc(RC_1, __func__);
}

void proc2(void)
{
	r3_proc(RC_2, __func__);
}

void proc3(void)
{
	r3_proc(RC_3, __func__);
}

void proc4(void)
{
	r3_proc(RC_4, __func__);
}

void proc5(void)
{
	r3_proc(RC_5, __func__);
}

/***********************************************************************/
/* The Idle process */
/* You must create a System process of lowest priority for this in R4. */
/***********************************************************************/
void sys_idle_process(void)
{
	char msg[] = "IDLE PROCESS EXECUTING.\r\n";
	
	for (;;) {
		sys_req(WRITE, COM1, msg, strlen(msg));
		sys_req(IDLE);
	}
}
