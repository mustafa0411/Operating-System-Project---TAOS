;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Core bits of the kernel that must be written in assembly. Holds a Multiboot
; header (so QEMU will load it directly), creates a basic stack, jumps to
; kmain(), then attempts to power off QEMU when kmain() returns. If powering
; off QEMU fails, it simply halts the CPU.
;
; You should not need to make any modifications to this file. Doing so
; may result in losing points.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bits 32
global start
extern kmain

;; GRUB style multiboot header
MODULEALIGN equ 1<<0			; align modules on page boundaries
MEMINFO     equ 1<<1			; provide memory map
FLAGS       equ MODULEALIGN | MEMINFO	; multiboot enable
MAGIC       equ 0x1badb002		; used to check boot success
CHECKSUM    equ -(MAGIC + FLAGS)

;; initial stack size
STACKSIZE equ 0x4000

section .text
align 4
multiboot_header:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

;; kernel entry point
start:
	mov esp, stack + STACKSIZE	;; establish a stack
	call kmain			;; jump to C code

	cli				;; disable interrupts

	mov edx, 0x604			;; QEMU ACPI port
	mov eax, 0x2000			;; power off
	out dx, ax

.hang:					;; if power off fails
	hlt				;; halt the CPU
	jmp .hang			;; repeat as necessary

section .bss
align 4
stack:	resb STACKSIZE	; reserve stack on doubleword boundary
