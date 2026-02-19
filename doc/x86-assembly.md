# x86 Assembly

For some modules, you will have to write at least some portion of your code
in assembly. You should not require more than those instructions listed here.

- `call *address*`  
Calls *address* as a function. *Address* needs to be either a label in the
current assembly source file, or an identifier marked `extern` in the current
assembly source file. The return address will automatically be pushed on to
the stack.

- `cli`  
Disable interrupts. No further interrupts will be handled until they are
reenabled with `sti`.

- `in *dest*, *src*`  
Read one, two, or four bytes from the I/O address *src*, saving it to the
register *dest*. *Dest* must be one of the registers `AL` (read one byte),
`AX` (two bytes), or `EAX` (four bytes). *Src* must be a literal value or
the register `DX`.

- `iret`  
Return from an Interrupt Service Routine. The CPU will pop the return address,
CS, and EFLAGS from the stack.

- `mov *dest*, *src*`  
Copy the value from *src*, which may be a register, memory location, or literal
value, to *dest*, which may be a register or memory location.

- `out *dest*, *src*`  
Write one, two, or four bytes from *src* to the I/O address *dest*. *Dest* must
be a literal value or the register `DX`. *Src* must be one of the registers
`AL` (write one byte), `AX` (two bytes), or `EAX` (four bytes).

- `pop *dest*`  
Pop the bottom value of the stack into the register *dest* and increment the
stack pointer by 4 bytes.

- `popa`  
Pop all general purpose registers from the stack and increment the stack pointer
the appropriate amount.

- `push *src*`  
Decrement the stack pointer by 4 bytes and push the contents of the register or
literal value *src* onto the stack.

- `pusha`  
Push all the general purpose registers from the stack and decrement the stack
pointer by the appropriate amount.

- `ret`  
Return from an ordinary (non-ISR) function. The CPU will pop the return address
from the stack.

- `sti`  
Enable interrupts.
