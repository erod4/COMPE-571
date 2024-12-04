#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int dirty_page_writes = 0;
int disk_references = 0;
int current_time = 0;
int memory_references_count = 0;

int fifo_head = 0;
int fifo_tail = 0;

int fifo_queue[MAIN_MEMORY_SIZE]; // FIFO queue to keep track of the order of pages in memory

Page_Table_Entry page_tables[PROCESSES][MAX_VPT]; // Each process gets a page table with 128 entries
Physical_Page physical_memory[MAIN_MEMORY_SIZE];  // Main memory with 32 addressable locations

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        printf("Error: Missing arguments.\n");
        printf("Usage: %s <input_file> <replacement_algorithm>\n", argv[0]);
        printf("Valid replacement algorithms: RAND, FIFO, LRU, PER\n");
        return 1;
    }
    int page_faults = 0;

    FILE *file;
    file = fopen(argv[1], "r");
    const char *replacement_algorithm = argv[2];

    // Validate the replacement algorithm
    if (strcmp(replacement_algorithm, "RAND") != 0 &&
        strcmp(replacement_algorithm, "FIFO") != 0 &&
        strcmp(replacement_algorithm, "LRU") != 0 &&
        strcmp(replacement_algorithm, "PER") != 0)
    {
        printf("Error: Invalid replacement algorithm '%s'.\n", replacement_algorithm);
        printf("Valid replacement algorithms: RAND, FIFO, LRU, PER\n");
        return 1;
    }
    if (file == NULL)
    {
        printf("Could not find file, ensure it is at base directory and a .txt file.\n\r");
        return 1;
    }
    init_memory();

    // loop through file and read memory references
    Memory_Reference memory_references[MAX_MEMORY];
    int i = 0;
    while (fscanf(file, "%d %hu %c", &memory_references[i].task_num, &memory_references[i].virtual_address, &memory_references[i].rw) != EOF)
    {
        int task_num = memory_references[i].task_num;
        uint16_t virtual_address = memory_references[i].virtual_address;

        // extract VPN and offset from virtual address
        int virtual_page_number = virtual_address >> OFFSET;
        int offset = virtual_address & ((1 << OFFSET) - 1);

        // error checking
        if (task_num < 1 || task_num > PROCESSES || virtual_page_number >= MAX_VPT)
        {
            i++;
            current_time++;
            printf("Invalid task number or virtual page number\n");
            printf("Task number: %d, Virtual page number: %d\n", task_num, virtual_page_number);
            if (memory_references_count % RESET_INTERVAL == 0)
            {
                printf("PER Replacement: Resetting all reference bits.\n");
                reset_ref_bits();
            }
            continue;
        }

        Page_Table_Entry *pte = &page_tables[task_num - 1][virtual_page_number];

        // if page is in memory
        if (pte->valid)
        {
            // update the dirty if its a write operation
            if (memory_references[i].rw == 'W')
            {
                pte->dirty_bit = 1;
                physical_memory[pte->physical_page].dirty_bit = 1;
            }
            // update the time of access (for LRU)
            pte->last_access_time = current_time;
            // update the reference bit (for PER)
            pte->reference_bit = 1;
        }
        else
        {
            // page fault
            page_faults++;
            // increment disk references
            disk_references++;
            // check if main memory is full

            if (is_memory_full())

            {
                // printf("Main memory is full\n");
                // Select a page replacement algorithm and replace a page accordingly
                if (strcmp(replacement_algorithm, "RAND") == 0)
                    RAND();
                else if (strcmp(replacement_algorithm, "FIFO") == 0)
                    FIFO();
                else if (strcmp(replacement_algorithm, "LRU") == 0)
                    LRU();
                else if (strcmp(replacement_algorithm, "PER") == 0)
                    PER();
            }

            // find the next available physical page
            int physical_page_idx = next_available_physical_page();
            // printf("Next available physical page: %d\n", physical_page_idx);
            if (physical_page_idx == -1)
            {
                printf("Error: Could not find next available physical page\n");
                return 1;
            }

            // printf("Task %d: Storing virtual page %d in physical page %d\n", task_num, virtual_page_number, physical_page_idx);

            // update the page table entry
            pte->physical_page = physical_page_idx;
            pte->valid = 1;
            pte->dirty_bit = (memory_references[i].rw == 'W') ? 1 : 0;
            pte->reference_bit = 1;
            pte->last_access_time = current_time;
            // update the physical memory
            physical_memory[physical_page_idx].valid = 1;
            physical_memory[physical_page_idx].process_id = task_num - 1;
            physical_memory[physical_page_idx].virtual_page = virtual_page_number;
            physical_memory[physical_page_idx].dirty_bit = (memory_references[i].rw == 'W') ? 1 : 0;

            if (strcmp(replacement_algorithm, "FIFO") == 0)
            {
                // Update the FIFO queue
                fifo_queue[fifo_tail] = physical_page_idx;
                fifo_tail = (fifo_tail + 1) % MAIN_MEMORY_SIZE;
            }
        }
        i++;
        current_time++;
        memory_references_count++;
        if (memory_references_count % RESET_INTERVAL == 0)
        {
            // printf("PER Replacement: Resetting all reference bits.\n");
            reset_ref_bits();
        }
    }
    printf("Total page faults: %d\n", page_faults);
    printf("Total disk references: %d\n", disk_references);
    printf("Total dirty page writes: %d\n", dirty_page_writes);
    fclose(file);
    return 0;
}

void init_memory()
{
    // initialize page tables
    for (int process = 0; process < PROCESSES; process++)
    {
        for (int i = 0; i < MAX_VPT; i++)
        {
            page_tables[process][i].physical_page = 0;
            page_tables[process][i].valid = 0;
            page_tables[process][i].dirty_bit = 0;
            page_tables[process][i].reference_bit = 0;
            page_tables[process][i].last_access_time = 0;
        }
    }
    // initialize main memory
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        physical_memory[i].valid = 0;
        physical_memory[i].process_id = -1;
        physical_memory[i].virtual_page = -1;
        physical_memory[i].dirty_bit = 0;
        }
    // initialize FIFO queue
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        fifo_queue[i] = -1; // Use -1 to indicate empty slots
    }
}

void RAND()
{
    // generate a random number between 0 and 31
    int victim_page = rand() % MAIN_MEMORY_SIZE;

    // get victim process and virtual page
    int victim_process = physical_memory[victim_page].process_id;
    int victim_virtual_page = physical_memory[victim_page].virtual_page;

    // invalidating the page table entry
    page_tables[victim_process][victim_virtual_page].valid = 0;

    // handle dirty bit, if set increment dirty page writes and disk write
    if (physical_memory[victim_page].dirty_bit)
    {
        dirty_page_writes++;
        disk_references++; // Simulate disk write
    }
    // mark the physical page as free
    physical_memory[victim_page].valid = 0;
    physical_memory[victim_page].process_id = -1;
    physical_memory[victim_page].virtual_page = -1;
    physical_memory[victim_page].dirty_bit = 0;
}

void FIFO()
{
    // dequeue the oldest physical page index from the FIFO queue
    int victim_page = fifo_queue[fifo_head];

    // move the head pointer forward
    fifo_head = (fifo_head + 1) % MAIN_MEMORY_SIZE;

    // get victim process and virtual page
    int victim_process = physical_memory[victim_page].process_id;
    int victim_virtual_page = physical_memory[victim_page].virtual_page;

    // invalidating the page table entry of the victim
    if (victim_process != -1 && victim_virtual_page != -1)
    {
        page_tables[victim_process][victim_virtual_page].valid = 0;

        // handle dirty bit
        if (physical_memory[victim_page].dirty_bit)
        {
            dirty_page_writes++;
            disk_references++;
            // printf("FIFO Replacement: Victim page was dirty. Incremented dirty_page_writes and disk_references.\n");
        }
    }

    // mark the physical page as free
    physical_memory[victim_page].valid = 0;
    physical_memory[victim_page].process_id = -1;
    physical_memory[victim_page].virtual_page = -1;
    physical_memory[victim_page].dirty_bit = 0;

    // reset FIFO queue slot
    fifo_queue[victim_page] = -1;
}

void LRU()
{

    int victim_page = -1;
    int min_time = 9999999;

    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        if (physical_memory[i].valid == 1)
        {
            int process_id = physical_memory[i].process_id;
            int virtual_page = physical_memory[i].virtual_page;
            int last_access_time = page_tables[process_id][virtual_page].last_access_time;
            // if last access time is less than min time
            if (last_access_time < min_time)
            {
                min_time = last_access_time;
                victim_page = i;
            }
            // if two pages have same access time
            else if (last_access_time == min_time)
            {
                if (physical_memory[i].dirty_bit == 0)
                {
                    victim_page = i;
                }
            }
            else if ((!physical_memory[i].dirty_bit && !physical_memory[victim_page].dirty_bit) ||
                     (physical_memory[i].dirty_bit && physical_memory[victim_page].dirty_bit))
            {
                if (i < victim_page)
                {
                    victim_page = i;
                }
            }
        }
    }
    if (victim_page == -1)
    {
        // printf("LRU Replacement: No valid page found to replace.\n");
        return;
    }

    // printf("LRU Replacement: Victim physical page selected for replacement: %d\n", victim_page);

    // get victim process and virtual page
    int victim_process = physical_memory[victim_page].process_id;
    int victim_virtual_page = physical_memory[victim_page].virtual_page;

    // invalidating the page table entry of the victim
    if (victim_process != -1 && victim_virtual_page != -1)
    {
        page_tables[victim_process][victim_virtual_page].valid = 0;

        // handle dirty bit
        if (physical_memory[victim_page].dirty_bit)
        {
            dirty_page_writes++;
            disk_references++;
        }
    }

    // mark the physical page as free
    physical_memory[victim_page].valid = 0;
    physical_memory[victim_page].process_id = -1;
    physical_memory[victim_page].virtual_page = -1;
    physical_memory[victim_page].dirty_bit = 0;
}

void PER()
{
    int victim_page = -1;
    int candidates[MAIN_MEMORY_SIZE];
    int candidate_count = 0;

    // Define an array of priorities
    typedef struct
    {
        int ref_bit;
        int dirty_bit;
    } Priority;

    Priority priorities[] = {
        {0, 0}, // Class 0: Not Referenced, Not Modified
        {0, 1}, // Class 1: Not Referenced, Modified
        {1, 0}, // Class 2: Referenced, Not Modified
        {1, 1}  // Class 3: Referenced, Modified
    };

    // Iterate over the priority classes
    for (int p = 0; p < 4; p++)
    {
        candidate_count = 0;
        for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
        {
            if (physical_memory[i].valid)
            {
                int pid = physical_memory[i].process_id;
                int vp = physical_memory[i].virtual_page;
                Page_Table_Entry *pte = &page_tables[pid][vp];

                if (pte->reference_bit == priorities[p].ref_bit &&
                    pte->dirty_bit == priorities[p].dirty_bit)
                {
                    candidates[candidate_count++] = i;
                }
            }
        }
        if (candidate_count > 0)
        {
            // Randomly select a victim from candidates
            int index = rand() % candidate_count;
            victim_page = candidates[index];
            break; // Found a candidate in the current priority class
        }
    }

    if (victim_page != -1)
    {
        // Proceed with replacing the victim page
        int victim_process = physical_memory[victim_page].process_id;
        int victim_virtual_page = physical_memory[victim_page].virtual_page;

        Page_Table_Entry *victim_pte = &page_tables[victim_process][victim_virtual_page];
        victim_pte->valid = 0;

        if (victim_pte->dirty_bit)
        {
            dirty_page_writes++;
            disk_references++; // Simulate disk write for dirty page
        }

        physical_memory[victim_page].valid = 0;
        physical_memory[victim_page].process_id = -1;
        physical_memory[victim_page].virtual_page = -1;
        physical_memory[victim_page].dirty_bit = 0;
    }
}

void reset_ref_bits()
{
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        if (physical_memory[i].valid)
        {
            int pid = physical_memory[i].process_id;
            int vp = physical_memory[i].virtual_page;
            page_tables[pid][vp].reference_bit = 0;
        }
    }
}

int is_memory_full()
{
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        if (physical_memory[i].valid == 0)
        {

            return 0; // Not full, at least one free slot
        }
    }
    return 1; // Full, no free slots
}

int next_available_physical_page()
{
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        if (physical_memory[i].valid == 0)
        {
            return i;
        }
    }
    return -1;
}