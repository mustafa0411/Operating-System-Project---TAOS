/**************************************************************************
* Core kernel functionality needed to get an OS running in protected mode
* on a 32-bit x86 platform.
*
* You should not need to make any modifications to this file. Doing so
* may result in losing points.
***************************************************************************/

/* ************************************************************************
 * Global Descriptor Table
 * ************************************************************************/
#include <stdint.h>
#include <mpx/gdt.h>

struct gdt_entry {
        uint16_t limit_low;     /** first 16 bits of limit */
        uint16_t base_low;      /** first 16 bits of base */
        uint8_t base_mid;       /** bits 16-23 of base */
        uint8_t access;         /** next 8 bits; access flags */
        uint8_t flags;          /** page granularity, size */
        uint8_t base_high;      /** last 8 bits of the base */
} __attribute__((packed));

struct gdt_descriptor {
        uint16_t size;
        struct gdt_entry * base;
} __attribute__((packed));

void gdt_init(void)
{
	/* declared static so that they have permanenent lifetime while not being global */
	static struct gdt_entry table[] = {
		{ 0x0000, 0x0, 0x0, 0x00, 0x00, 0x0 },	// NULL
		{ 0xffff, 0x0, 0x0, 0x9a, 0xff, 0x0 },	// CS
		{ 0xffff, 0x0, 0x0, 0x92, 0xff, 0x0 },	// DS
		{ 0xffff, 0x0, 0x0, 0xfa, 0xff, 0x0 },	// User CS
		{ 0xffff, 0x0, 0x0, 0xf2, 0xff, 0x0 },	// User DS
	};

	static struct gdt_descriptor gdt = {
		.size = sizeof(table) - 1,
		.base = table,
	};

	__asm__ volatile ("lgdt %0" :: "m"(gdt));
	__asm__ volatile ("mov %%ax, %%ds" :: "a"(0x10));
	__asm__ volatile ("mov %%ax, %%es" :: "a"(0x10));
	__asm__ volatile ("mov %%ax, %%fs" :: "a"(0x10));
	__asm__ volatile ("mov %%ax, %%gs" :: "a"(0x10));
	__asm__ volatile ("mov %%ax, %%ss" :: "a"(0x10));
}

/* ************************************************************************
 * Interrupts
 * ************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <mpx/panic.h>
#include <mpx/interrupts.h>
#include <mpx/io.h>

#define REQUIRED_INTERRUPTS	(32)

// Programmable Interrupt Controllers
#define PIC1 0x20
#define PIC2 0xA0

// Initialization Code Words
#define ICW1 0x11
#define ICW4 0x01

/**
 The i386 can do an io wait by accessing another port.
 Mainly used in initializing the PIC.
*/
#define io_wait() __asm__ volatile ("outb $0x80")
#define outbwait(x, y) do { outb(x, y); io_wait(); } while (0)

/** Pointer to an Interrupt Service Routine */
typedef void (*isr_function)(void *);

/** A single entry in the IDT */
struct idt_entry {
        uint16_t base_low;      /** offset bits 0..15 */
        uint16_t sselect;       /** stack selector in gdt or ldt */
        uint8_t zero;           /** this stays zero; unused */
        uint8_t flags;          /** attributes */
        uint16_t base_high;     /** offset bits 16..31 */
} __attribute__((packed));

/** The metadata for the IDT */
struct idt_descriptor {
        uint16_t size;
        struct idt_entry *base;
} __attribute__((packed));

extern void sys_call_isr(void*);

// Interrupt Descriptor Table
static struct idt_entry idt_entries[256] = { { 0, 0, 0, 0, 0 } };

#define simple_isr(fn, msg) \
	static __attribute__((interrupt)) void fn(void *int_frame) \
	{ \
		(void)int_frame; kpanic(msg); \
	}

simple_isr(divide_error, "Division by zero")
simple_isr(debug, "Debug")
simple_isr(nmi, "Non-maskable Interrupt")
simple_isr(breakpoint, "Breakpoint")
simple_isr(overflow, "Overflow")
simple_isr(bounds, "Bounds error")
simple_isr(invalid_op, "Invalid operation")
simple_isr(device_not_available, "Device not available")
simple_isr(double_fault, "Double fault")
simple_isr(coprocessor_segment, "Coprocessor segment error")
simple_isr(invalid_tss, "Invalid TSS")
simple_isr(segment_not_present, "Segment not present")
simple_isr(stack_segment, "Stack segment error")
simple_isr(general_protection, "General protection fault")
simple_isr(page_fault, "Page Fault")
simple_isr(reserved, "Reserved")
simple_isr(coprocessor, "Coprocessor error")

static __attribute__((interrupt)) void rtc_isr(void *int_frame)
{
	(void)int_frame;
	cli();
	outb(0xa0, 0x20);
	sti();
}

static void idt_set_gate(size_t idx, isr_function fn, uint16_t sel, uint8_t flags)
{
	uintptr_t base = (uintptr_t)fn;
	struct idt_entry *new_entry = idt_entries + idx;
	new_entry->base_low = (base & 0xFFFF);
	new_entry->base_high = (base >> 16) & 0xFFFF;
	new_entry->sselect = sel;
	new_entry->zero = 0;
	new_entry->flags = flags;
}

void idt_init(void)
{
	/* declared static so that it has a permanenent lifetime while not being global */
	static struct idt_descriptor idt = {
		.size = sizeof(idt_entries) - 1,
		.base = idt_entries,
	};
	__asm__ volatile ("lidt %0" :: "m"(idt));
}

void idt_install(int vector, void (*handler)(void *))
{
	idt_set_gate(vector, handler, 0x08, 0x8e);
}

void irq_init(void)
{  
	// Necessary interrupt handlers for protected mode
	isr_function isrs[] = {
		divide_error,
		debug,
		nmi,
		breakpoint,
		overflow,
		bounds,
		invalid_op,
		device_not_available,
		double_fault,
		coprocessor_segment,
		invalid_tss,
		segment_not_present,
		stack_segment,
		general_protection,
		page_fault,
		reserved,
		coprocessor,
	};

	size_t nisrs = (uint8_t)(sizeof(isrs) / sizeof(isrs[0]));
	const uint16_t sel = 0x08;	// CS
	const uint8_t flags = 0x8e;	// FIXME: MAGIC NUMBER

	for (size_t i = 0; i < nisrs; i++) {
		idt_set_gate(i, isrs[i], sel, flags);
	}

	for (size_t i = nisrs; i < REQUIRED_INTERRUPTS; i++) {
		idt_set_gate(i, reserved, sel, flags);
	}

	// Ignore interrupts from the real time clock
	idt_set_gate(0x08, rtc_isr, sel, flags);

	// Handle sys_req() -> sys_call_isr
	idt_set_gate(0x60, sys_call_isr, sel, flags);
}

void pic_init(void)
{
	outbwait(PIC1, ICW1);   //send initialization code words 1 to PIC1
	outbwait(PIC2, ICW1);   //send icw1 to PIC2
	outbwait(PIC1+1, 0x20); //icw2: remap irq0 to 32
	outbwait(PIC2+1, 0x28); //icw2: remap irq8 to 40
	outbwait(PIC1+1, 4);    //icw3
	outbwait(PIC2+1, 2);    //icw3
	outbwait(PIC1+1, ICW4); //icw4: 80x86, automatic handling
	outbwait(PIC2+1, ICW4); //icw4: 80x86, automatic handling
	outbwait(PIC1+1, 0xFF); //disable irqs for PIC1
	outbwait(PIC2+1, 0xFF); //disable irqs for PIC2
}

/* ************************************************************************
 * Kernel Panic
 * ************************************************************************/
#include <mpx/panic.h>
#include <mpx/serial.h>
#include <mpx/interrupts.h>
#include <sys_req.h>
#include <string.h>
#include <stdnoreturn.h>

noreturn void kpanic(const char *msg)
{
	cli();
	char prefix[] = "Panic: ";
	char suffix[] = "\r\nPress Ctrl-A, then X to quit QEMU.\r\n";
	serial_out(COM1, prefix, strlen(prefix));
	serial_out(COM1, msg, strlen(msg));
	serial_out(COM1, suffix, strlen(suffix));
	for (;;) {
		__asm__ volatile ("hlt");
	}
}

/* ************************************************************************
 * Virtual Memory
 * ************************************************************************/
#include <mpx/panic.h>
#include <mpx/vm.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

// The physical start of the heap
// TODO: this is very magic
#define KHEAP_BASE	0xD000000

// The size of the primitive kernel heap
#define KHEAP_SIZE	0x10000

// 4 KB pages
#define PAGE_SIZE	0x1000

// 64 MB total memory
// TODO: learn this from boot parameters
#define MEM_SIZE	0x4000000

// number of frames
#define NFRAMES		(MEM_SIZE / PAGE_SIZE)

// bits per frame
#define FRAME_BIT	(sizeof(uint32_t) * CHAR_BIT)

/*
  Page entry structure
  Describes a single page in memory
*/
typedef struct {
	uint32_t present:1;
	uint32_t writeable:1;
	uint32_t usermode:1;
	uint32_t accessed:1;
	uint32_t dirty:1;
	uint32_t reserved:7;
	uint32_t frameaddr:20;
} page_entry;

/*
  Page table structure
  Contains 1024 pages/frames
*/
typedef struct {
	page_entry pages[1024];
} page_table;

/*
  Page directory structure
  Limited to 1024 tables for now
*/
typedef struct {
	page_table *tables[1024];
	uint32_t tables_phys[1024];
} page_dir;

// bitmap of frames
static uint32_t frames[NFRAMES / FRAME_BIT] = { 0 };

// kernel page directory
static page_dir *kdir;

// physical end of kernel image
// defined by linker
extern void *__end;

// current physical memory allocation address
static uintptr_t phys_alloc_addr = (uintptr_t) & __end;

// if 0, allocate physical memory, otherwise virtual
static int heap_is_initialized = 0;

static uint32_t alloc(uint32_t size)
{
	static uint32_t heap_addr = KHEAP_BASE;

	uint32_t base = heap_addr;
	heap_addr += size;

	if (heap_addr > KHEAP_BASE + KHEAP_SIZE) {
		kpanic("Heap is full!");
	}

	return base;
}

/*
 Finds and returns a page, allocating a new page table if necessary.
*/
static page_entry *get_page(uint32_t addr, page_dir * dir, int make_table)
{
	uint32_t index = addr / PAGE_SIZE / 1024;
	uint32_t offset = addr / PAGE_SIZE % 1024;

	// return it if it exists
	if (dir->tables[index]) {
		return &dir->tables[index]->pages[offset];
	}

	// create it if necessary
	if (make_table) {
		void *phys_addr = NULL;
		dir->tables[index] =
		    (page_table *) kmalloc(sizeof(page_table), 1, &phys_addr);
		dir->tables_phys[index] = ((uintptr_t) phys_addr) | 0x7;	//enable present, writable
		return &dir->tables[index]->pages[offset];
	}

	return NULL;
}

void *kmalloc(uint32_t size, int page_align, void **phys_addr)
{
	void *addr = NULL;

	// Allocate on the kernel heap if one has been created
	if (heap_is_initialized) {
		addr = (void *)alloc(size);
		if (phys_addr) {
			page_entry *page = get_page((uint32_t) addr, kdir, 0);
			*phys_addr =
			    (void *)((page->frameaddr * 0x1000) +
				     ((uintptr_t) addr & 0xFFF));
		}
	}
	// Else, allocate directly from physical memory
	else {
		if (page_align && (phys_alloc_addr & 0xFFFFF000)) {
			phys_alloc_addr &= 0xFFFFF000;
			phys_alloc_addr += 0x1000;
		}
		addr = (void *)phys_alloc_addr;
		if (phys_addr) {
			*phys_addr = addr;
		}
		phys_alloc_addr += size;
	}
	return addr;
}

/* Finds the first free page frame */
static uint32_t find_free(void)
{
	uint32_t i, j;
	for (i = 0; i < NFRAMES / FRAME_BIT; i++)
		if (frames[i] != 0xFFFFFFFF)	//if frame not full
			for (j = 0; j < FRAME_BIT; j++)	//find first free bit
				if (!(frames[i] & (1 << j)))
					return i * FRAME_BIT + j;

	return -1;		//no free frames
}

/* Marks a page frame bit as in use */
static void set_bit(uint32_t addr)
{
	uint32_t frame = addr / PAGE_SIZE;
	uint32_t index = frame / FRAME_BIT;
	uint32_t offset = frame % FRAME_BIT;
	frames[index] |= (1 << offset);
}

/*
 Marks a frame as in use in the frame bitmap, sets up the page,
 and saves the frame index in the page.
*/
static void new_frame(page_entry * page)
{
	if (page->frameaddr != 0) {
		return;
	}

	uint32_t index = find_free();
	if (index == (uint32_t) (-1)) {
		kpanic("Out of memory");
	}

	//mark a frame as in-use
	set_bit(index * PAGE_SIZE);
	page->present = 1;
	page->frameaddr = index;
	page->writeable = 1;
	page->usermode = 0;
}

void vm_init(void)
{
	// create kernel directory
	kdir = kmalloc(sizeof(*kdir), 1, 0);	//page aligned
	memset(kdir, 0, sizeof(*kdir));

	// get pages for kernel heap
	for (uint32_t i = KHEAP_BASE; i < (KHEAP_BASE + KHEAP_SIZE); i += 1) {
		get_page(i, kdir, 1);
	}

	// perform identity mapping of used memory
	// note: placement_addr gets incremented in get_page,
	// so we're mapping the first frames as well
	for (uint32_t i = 0; i < (phys_alloc_addr + 0x10000); i += PAGE_SIZE) {
		new_frame(get_page(i, kdir, 1));
	}

	// allocate heap frames now that the placement addr has increased.
	// placement addr increases here for heap
	for (uint32_t i = KHEAP_BASE; i < (KHEAP_BASE + KHEAP_SIZE); i += PAGE_SIZE) {
		new_frame(get_page(i, kdir, 1));
	}

	// generate a page fault for NULL pointer dereference
	memset(&kdir->tables[0]->pages[0], 0, sizeof(kdir->tables[0]->pages[0]));

	// load the kernel page directory
	__asm__ volatile ("mov %0,%%cr3" :: "b"(&kdir->tables_phys[0]));

	// enable paging
	uint32_t cr0;
	__asm__ volatile ("mov %%cr0,%0" : "=b"(cr0));
	cr0 |= 0x80000000;
	__asm__ volatile ("mov %0,%%cr0" :: "b"(cr0));

	heap_is_initialized = 1;
}
