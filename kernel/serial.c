#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <com_hist.h>
#include <string.h>
#include <memory.h>

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int serial_poll(device dev, char *buffer, size_t len)
{
    size_t bufferSize = 0;
    size_t cursorPos = 0;

    while (bufferSize < len) {
        if ((inb(dev + LSR) & 1) == 1) {
            char c = inb(dev); /* read one byte */
            if (c == 27) {
                // Wait for byte to be read
                while(!(inb(dev + LSR) & 1));
                char c2 = inb(dev); // bracket for arrow key [C [D
                if (c2 == '['){
                    while(!(inb(dev + LSR) & 1)); // wait for third byte
                    char c3 = inb(dev); // check for letter after bracket, ABCD
                    if (c3 == 'D' && cursorPos > 0){
                        // navigate to left and hover
                        cursorPos--;
                        outb(dev, '\b');
                        
                    } else if (c3 == 'C' && cursorPos < bufferSize){
                        // navigate to right and hover
                        cursorPos++;
                        outb(dev, '\033');
                        outb(dev, '[');
                        outb(dev, 'C');
                    } else if (c3 == 'A') {
						// Up arrow
						if (hist_get_index() == 0) {
							hist_set(0, buffer);
						}
						
						// clear the buffer
						memset(buffer, 0, bufferSize);

						char* hist = hist_next();
						if (hist == NULL) {
							// move cursor to the start
							while (cursorPos > 0) {
								outb(dev, '\b');
								cursorPos--;
							}
							// Clear the buffer
							while (cursorPos < bufferSize) {
								outb(dev, ' ');
								cursorPos++;
							}
							bufferSize = 0;
							// move cursor to the start again
							while (cursorPos > 0) {
								outb(dev, '\b');
								cursorPos--;
							}
							continue;
						}

						strncpy(buffer, hist, strlen(hist));
						
						// move cursor to start
						while (cursorPos > 0) {
							outb(dev, '\b');
							cursorPos--;
						}
						size_t new_buff_size = strlen(buffer);
						// write in new buffer data
						while (cursorPos < bufferSize || cursorPos < new_buff_size) {
							if (cursorPos < new_buff_size)
								outb(dev, buffer[cursorPos]);
							else
								outb(dev, ' ');
							cursorPos++;
						}
						bufferSize = new_buff_size;
						while (cursorPos > bufferSize) {
							outb(dev, '\b');
							cursorPos--;
						}
						
					} else if (c3 == 'B') {
						// Down arrow

						//buffer = hist_previous();
						memset(buffer, 0, bufferSize);

						char* hist = hist_previous();
						if (hist == NULL) {
							// move cursor to the start
							while (cursorPos > 0) {
								outb(dev, '\b');
								cursorPos--;
							}
							// Clear the buffer
							while (cursorPos < bufferSize) {
								outb(dev, ' ');
								cursorPos++;
							}
							bufferSize = 0;
							// move cursor to the start again
							while (cursorPos > 0) {
								outb(dev, '\b');
								cursorPos--;
							}
							continue;
						}

						strncpy(buffer, hist, strlen(hist));
						
						// move cursor to start
						while (cursorPos > 0) {
							outb(dev, '\b');
							cursorPos--;
						}
						size_t new_buff_size = strlen(buffer);
						// write in new buffer data
						while (cursorPos < bufferSize || cursorPos < new_buff_size) {
							if (cursorPos < new_buff_size)
								outb(dev, buffer[cursorPos]);
							else
								outb(dev, ' ');
							cursorPos++;
						}
						bufferSize = new_buff_size;
						while (cursorPos > bufferSize) {
							outb(dev, '\b');
							cursorPos--;
						}

                        if (hist_get_index() == 0)
                            sys_free_mem(hist_get(0));

					} else if(c3 =='3'){ 
                        // Delete key implementation; [3~
                        while(!(inb(dev + LSR) & 1));
                        char c4 = inb(dev);
                        if(c4 == '~'){
                            if (cursorPos < bufferSize){
                                // only deletes if cursorPos is on a character
                                for (size_t i = cursorPos; i < bufferSize - 1; i++){
                                    // shift buffer left
                                    buffer[i] = buffer[i + 1];
                                }
                                
                                buffer[bufferSize - 1] = '\0';
                                bufferSize--;
                                // redraw from cursor to end
                                for (size_t i = cursorPos; i < bufferSize; i++) {
                                    outb(dev, buffer[i]);
                                }
                                // erase leftover character
                                outb(dev, ' ');
                                
                                // move back to position
                                for (size_t i = cursorPos; i < bufferSize + 1; i++) {
                                    outb(dev, '\b');
                                }
                            }
                        }
                    } else if(c3 == 'H') {
                        while (cursorPos > 0) {
                            outb(dev, '\b');
                            cursorPos--;
                        }
                    } else if(c3 == 'F') {
                        while (cursorPos < bufferSize) {
                            outb(dev, buffer[cursorPos]);
                            cursorPos++;
                        }
                    }
                }
                continue;
            }

            //handle alphanumerics and special characters
            if ((c >= 32 && c <= 126))
            {	
                if (bufferSize < len - 1){
                    // shift characters
                    for (size_t i = bufferSize; i > cursorPos; i--){
                        buffer[i] = buffer[i - 1];
                    }
                    buffer[cursorPos] = c;
                    bufferSize++;
                    cursorPos++;

                    // redraw from cursor to end
                    for (size_t i = cursorPos - 1; i < bufferSize; i++){
                        outb(dev, buffer[i]);
                    }
                    // Move cursor back to after inserted char
                    for(size_t i = cursorPos; i < bufferSize; i++){
                        outb(dev, '\b');
                    }
                }
                continue;
            }

            // backspace handling
            if (c == 127 || c == 8){
                if (cursorPos > 0) {
                    outb(dev, '\b');
                    for (size_t i = cursorPos - 1; i < bufferSize - 1; i++){
                        buffer[i] = buffer[i + 1];
                    }
                    buffer[bufferSize - 1] = '\0';
                    bufferSize--;
                    cursorPos--;
                    
                    for (size_t i = cursorPos; i < bufferSize; i++) {
                        outb(dev, buffer[i]);
                    }
                    outb(dev, ' ');
                    
                    for (size_t i = cursorPos; i < bufferSize + 1; i++) {
                        outb(dev, '\b');
                    }
                }
                continue;
            }

            if (c == '\n' || c == '\r')
            {
                sys_free_mem(hist_get(0));
                hist_index_set(0);
                if (bufferSize > 0)
                	hist_add(buffer);
                break;
            }
        }
    }

    outb(COM1, '\n');
    return bufferSize;
}
