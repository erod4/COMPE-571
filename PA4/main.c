#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

Page_Table_Entry page_tables[PROCESSES][MAX_VPT]; // Each process gets a page table with 128 entries
Physical_Page physical_memory[MAIN_MEMORY_SIZE];  // Main memory with 32 addressable locations
int main(int argc, char const *argv[])
{
    int page_faults = 0;
    FILE *file;
    file = fopen(argv[1], "r");

    if (argc < 3)
    {
        printf("Error: Missing arguments.\n");
        printf("Usage: %s <input_file> <replacement_algorithm>\n", argv[0]);
        printf("Valid replacement algorithms: RAND, FIFO, LRU, PER\n");
        return 1;
    }

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

        // Extract VPN and offset from virtual address
        int virtual_page_number = virtual_address >> OFFSET;
        int offset = virtual_address & ((1 << OFFSET) - 1);

        // Error checking
        if (task_num < 1 || task_num > PROCESSES || virtual_page_number > MAX_VPT)
        {
            printf("Invalid task number or virtual page number\n");
        }

        Page_Table_Entry *pte = &page_tables[task_num - 1][virtual_page_number];

        // if page is in memory
        if (pte->valid)
        {
        }
        else
        {
            // page fault
            page_faults++;
            printf("Task %d: Page fault for virtual address %hu (VPN: %d)\n", task_num, virtual_address, virtual_page_number);
            // check if main memory is full

            if (is_memory_full())

            {
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
            else
            {
                // Find the next available physical page
                int physical_page_idx = next_available_physical_page();
                // Update the page table entry
                pte->physical_page = physical_page_idx;
                pte->valid = 1;
                pte->dirty_bit = 0;
                pte->reference_bit = 0;
                // Update the physical memory
                physical_memory[physical_page_idx].valid = 1;
                physical_memory[physical_page_idx].process_id = task_num - 1;
                physical_memory[physical_page_idx].virtual_page = virtual_page_number;
                physical_memory[physical_page_idx].dirty_bit = 0;
            }
        }
        i++;
    }
    printf("Total page faults: %d\n", page_faults);
    fclose(file);
    return 0;
}

void init_memory()
{
    // Initialize page tables
    for (int process = 0; process < PROCESSES; process++)
    {
        for (int i = 0; i < MAX_VPT; i++)
        {
            page_tables[process][i].physical_page = 0;
            page_tables[process][i].valid = 0;
            page_tables[process][i].dirty_bit = 0;
            page_tables[process][i].reference_bit = 0;
        }
    }
    // Initialize main memory
    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        physical_memory[i].valid = 0;
        physical_memory[i].process_id = -1;
        physical_memory[i].virtual_page = -1;
        physical_memory[i].dirty_bit = 0;
    }
}

void RAND()
{
}

void FIFO()
{
}

void LRU()
{
}

void PER()
{
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