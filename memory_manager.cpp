#include <ctime>
#include <iostream>
#include <cstdlib>
#include "memory_manager.h"


MemoryManager::MemoryManager(int systemMemoryInMB)
{
    systemMemorySize = systemMemoryInMB * MB;
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

    while (processor.perform());
}

void MemoryManager::initSystemMemory()
{
    systemMemory = malloc(systemMemorySize);
    if (systemMemory == NULL)
    {
        perror("malloc:");
        exit(1);
    }

    freeMemory = systemMemorySize;
}

bool MemoryManager::validMemoryRequest(UINT64 footprint)
{
    return footprint <= freeMemory;
}

void MemoryManager::freeSystemMemory()
{
    free(systemMemory);
}

void MemoryManager::my_malloc(Process process)
{
    
}

void MemoryManager::my_free(Process process)
{

}


void MemoryManager::executeProcessesUsingCustom(ProcessVector processes)
{
    int numProcs = processes.size();

    std::cout << "Running custom malloc/free test with " << numProcs << " processes...\n";

    int procIdx = 0;

    ProcessVector processesRunning;

    do
    {
        // Remove and free any finished processes
        for (ProcessVector::iterator it = processesRunning.begin(); it != processesRunning.end(); it++)
        {
            if (!it->completed())
            {
                continue;
            }

            my_free(*it);
            processesRunning.erase(it);
        }
        
        // Execute any process in queue that can be executed
        if (queue.size() > 0)
        {
            for (ProcessVector::iterator it = queue.begin(); it != queue.end(); it++)
            {
                if (!validMemoryRequest(it->footprint))
                {
                    continue;
                }
                
                my_malloc(*it);
                processesRunning.push_back(*it);
                queue.erase(it);
                processor.execute(*it);
            }
        }

        // Either execute or queue a process
        for (ProcessVector::iterator it = processes.begin(); it != processes.end(); it++)
        {
            if (!validMemoryRequest(it->footprint))
            {
                queue.push_back(*it);
                processes.erase(it);

                continue;
            }
           
            my_malloc(*it);
            processesRunning.push_back(*it);
            processes.erase(it);
            processor.execute(*it);
        }

        procIdx = (procIdx + 1) % processes.size();

    } while (processor.perform() || processes.size());
}
