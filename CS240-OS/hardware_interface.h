//
//  hardware_interface.h
//  Test240
//
//  Created by Mootaz N. Elnozahy on 9/17/16.
//  Copyright © 2016 Mootaz N. Elnozahy. All rights reserved.
//

#ifndef hardware_interface_h
#define hardware_interface_h

/* Memory interface */
#define PAGE_SIZE           4096                // Page is 4KB
#define KERNEL_MEM_LIMIT    (1 << 20)           // Kernel code and libraries loads in first megabyte of memory, don't overwrite!!!
extern void set_ptbr(void * physical_addr);     // Sets the Page Table Base Register to the root of the page table
extern int  get_memory_size(void);              // Returns the amount of physical memory available
extern void initialize_memory(void);            // Starts the memory intialization, set on virtual memory
extern void * map_physical_page(void * physical_addr);          // Map a physical page starting at physical_addr into kernel
extern void * map_page_table_node(void * physical_addr);        // This is just a call to map_physical_page where the actual
                                                              // mapping takes place
typedef union {
    struct {
        int read:1;
        int write:1;
        int execute:1;
        int modified:1;
        int valid:1;
        int swapped:1;
        int shared:1;
        int unused:3;
        unsigned address:20;
    }pte;
    int pte_;
}pte;

// test version control v2

#define MEMORY_READ         0               // Access type, passed if there is an exception in memory access
#define MEMORY_WRITE        1               // Access type, passed if there is an exception in memory access

/* Disk interface */
typedef int t_id;
#define SECTOR_SIZE 512
#define MAX_SECTORS_PER_OP  32
void start_disk();
t_id read_disk(unsigned sector_num, unsigned num_sectors, void * buffer);
t_id write_disk(unsigned sector_num, unsigned num_sectors, void * buffer);

/* Console interface */
void start_console();
void write_console(unsigned num_chars, char * buffer);

/* Context interface */
typedef struct {
    unsigned    pc;
    unsigned    sp;
    unsigned    reg[16];
    double      dreg[16];
} context;

extern context machine_context;

/* Control interface */

extern void halt();             // the halt instruction: Hardware stops in low-energy mode, wakes up only on interrupts
extern void iret();             // hardware switches to user mode, starts running at the location defined by machine_context

extern void set_ivec(unsigned num, void (*handler)(int));
                                // sets entry "num" in interrupt vector to the address of the handling function
extern void shutdown_machine(); // Tell the hardware to shutdown

#define IVEC_SIZE   8
#define I_CLK       0           // Clock interrupt
#define I_DSK       1           // Disk interrupt
#define I_CNSL      2           // Console interrupt
#define I_TRAP      3           // trap from a user program because of a system call
#define I_EXCEPT    4           // exception from a user program because of a problem in execution
#define I_CHECK     5           // service processor interrupt, machine check, hardware is malfunctioning
#define I_UNUSED1   6
#define I_UNUSED2   7


/* IO interface */

/* Development interface */

#define DEBUG_MEM           0x00000001
#define DEBUG_DISK          0x00000002
#define DEBUG_CONSOLE       0x00000004
#define DEBUG_CONTROL       0x00000008
#define DEBUG_GENERAL       0x00000010
#define DEBUG_FATAL         0x00000020
#define DEBUG_ALL           0x0000003f
#define DEBUG_NONE          0x00000000

extern void set_debug_mode(unsigned flag);
extern void reset_debug_mode();

/* Trap Definitions */
#define TRAP_CREATE_PROCESS             0x00
#define TRAP_EXIT                       0x01
#define TRAP_DESTROY_PROCESS            0x02
#define TRAP_FREEZE_PROCESS             0x03
#define TRAP_RESUME_PROCESS             0x04
#define TRAP_GET_MY_PID                 0x05

#define TRAP_CREATE_MEMORY_OBJECT       0x10
#define TRAP_ADJUST_MEMORY_OBJECT_SIZE  0x11
#define TRAP_DELETE_MEMORY_OBJECT       0x12
#define TRAP_FREE_MEMORY                0x13
#define TRAP_USED_MEMORY                0x14
#define TRAP_MEMORY_SIZE                0x15
#define TRAP_GET_DATA_SEGMENT_BASE      0x16
#define TRAP_GET_DATA_SEGMENT_SIZE      0x17
#define TRAP_CREATE_PERSISTENT_OBJECT   0x20
#define TRAP_DELETE_PERSISTENT_OBJECT   0x21
#define TRAP_GET_PERSISTENT_OBJECT_SIZE 0x22
#define TRAP_MAP_PERSISTENT_OBJECT      0x23
#define TRAP_UNMAP_PERSISTENT_OBJECT    0x24
#define TRAP_TRUNCATE_PERSISTENT_OBJECT 0x25
#define TRAP_FREE_PERSISTENT_STORE_SPAC 0x26
#define TRAP_USED_PERSISTENT_STORE_SPAC 0x27
#define TRAP_NUM_OF_PERSISTENT_OBJECTS  0x28
#define TRAP_GET_PERSISTENT_OBJECT_KEY  0x29

#define TRAP_READ_CONSOLE               0x30
#define TRAP_WRITE_CONSOLE              0x31

#define TRAP_CREATE_VIRTUAL_MACHINE     0x40
#define TRAP_SHUTDOWN_VIRTUAL_MACHINE   0x41

#define TRAP_SHUTDOWN                   0x50

/* Error interface */
#define PM_OK   0x00    // No errror
#define PM_NR   0x01    // No resources
#define PM_BE   0x02    // Bad or ill formed executable
#define PM_NE   0x03    // No such executable exists
#define PM_NP   0x04    // No such process exists
#define PM_NF   0x05    // Process is not frozen
#define PM_OE   0x07    // Unknown error

#define MM_OK   0x00    // No error
#define MM_NR   0x01    // No resources
#define MM_BM   0x02    // Bad map
#define MM_OE   0x07    // Unknown error

#define OS_OK   0x00    // No error
#define OS_NR   0x01    // No resources
#define OS_BM   0x02    // Bad map
#define OS_KE   0x03    // Key already exists
#define OS_KN   0x04    // No such key
#define OS_BO   0x05    // Bad offset
#define OS_BE   0x06    // Beyond the end of the object data range
#define OS_OE   0x07    // Unknown error

#define IO_OK   0x00    // No error
#define IO_BB   0x02    // Bad buffer
#define IO_OE   0x07    // Unknown error

#define HY_OK   0x00    // No error
#define HY_NR   0x01    // No resources
#define HY_BP   0x02    // Bad virtual machine identifier
#define HY_NO   0x03    // Operating system does not exist

/* Exceptions interface */
/* e0 */
#define EXCEPTION_BAD_ADDRESS           0x01    // Bad memory access in user mode
#define EXCEPTION_ILLEGAL_INSTRUCTION   0x02    // Illegal instruction in user mode
/* e1 */
#define EXCEPTION_IRET                  0x10    // iret called in user mode
#define EXCEPTION_HALT                  0x11    // halt called in supervisor mode
#define EXCEPTION_DISK_READ             0x12    // Trying to access disk directly from user mode
#define EXCEPTION_DISK_WRITE            0x13    // Trying to access disk directly from user mode
#define EXCEPTION_CONSOLE_WRITE         0x14    // Trying to access console directly from user mode
#define EXCEPTION_START_MEMORY          0x15    // Trying to start memory from user mode
#define EXCEPTION_START_DISK            0x16    // Trying to start disk from user mode
#define EXCEPTION_START_CONSOLE         0x17    // Trying to start console directly from user mode
#define EXCEPTION_IVEC                  0x18    // Attempt to manipulate the interrupt vector directly from user mode
#define EXCEPTION_BASE_PAGE_TABLE_REG   0x19    // Attempt to manipulate the base page table register from user mode
#define EXCEPTION_MAP_MEMORY            0x20    // Attempt to access the MMU directly from user mode
#define EXCEPTION_GET_MEMORY_SIZE       0x21    // Attempt to read MMU register directly from user mode
#define EXCEPTION_SHUTDOWN              0x22    // Attempt to shutdown the machine from user mode while bypassing OS

typedef struct {
    unsigned long long   e0;              // Exception type, see the two definitions above
    unsigned long long   e1;              // Further qualifying information about the exception type
    unsigned long long   a[4];            // Arguments to operation if any (up to 4)
} exception_context;

extern exception_context e_context;



#endif /* hardware_interface_h */
