#include <ctime>
#include <iostream>
#include <cstdlib>
#include "memory_manager.h"


MemoryManager::MemoryManager()
{

}

void MemoryManager::executeProcessesUsingMalloc(ProcessVector processes)
{
    int numProcs = processes.size();

    std::cout << "Running malloc/free test with " << numProcs << " processes...\n";

    for (int i = 0; i < numProcs; i++)
    {
        Process process = processes[i];

        UINT64 memoryNeeded = process.footprint;

        std::cout << "\t" << i + 1 << ": " << "Allocating/freeing " << memoryNeeded << " bytes (" << process.footprintMB << " MB)\n";

        process.memory = (void *)malloc(memoryNeeded);
        if (process.memory == NULL)
        {
            std::cout << "Failed to allocate memory to process " << i + 1 << "!\n";
            exit(1);            
        }

        processor.execute(process);

        free(process.memory);

    }
}
