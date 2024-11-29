#ifndef MAIN_H_
#define MAIN_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 65535    // 64KB=2^16Bytes [0,65535]
#define OFFSET 9            // 9-bits for offset (2^9 = 512) [8,0]
#define VPN 16              // 7-bits for VPN (2^7 = 128) [15,9]
#define MAX_VPT 128         // 128 entries in the VPT
#define PROCESSES 3         // 3 processes(change)
#define MAIN_MEMORY_SIZE 32 // 32 addresable locations in main memory

// Struct to hold memory references
typedef struct
{
    int task_num;
    uint16_t virtual_address;
    char rw;
} Memory_Reference;

// Struct to hold page table entries
typedef struct
{
    int physical_page;
    int valid;
    int dirty_bit;
    int reference_bit;
} Page_Table_Entry;


//Struct for physical memory
typedef struct {
    int valid;
    int process_id;
    int virtual_page;
    int dirty_bit;
}Physical_Page;


/**
 * Function to initialize page tables and main memory
 */
void init_memory();

/**
 * Replacement algorithm in which the victim page (i.e., the page to be replaced) is selected in random
 */
void RAND();

/**
 * Replacement algorithm in which the oldest page in the memory gets replaced.
 */
void FIFO();

/**
 * Replacement algorithm in which the least recently used page in the memory gets replaced.
 */
void LRU();

/**
 * 
 */
void PER();

/**
 * Function to check if main memory is full
 * @return 1 if memory is full, 0 otherwise
 */
int is_memory_full();


/**
 * Funtion to give next available physical page in main memory
 * @return next available physical page
 */
int next_available_physical_page();
#endif /* MAIN_H_ */

