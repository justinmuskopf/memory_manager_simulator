#include <ctime>
#include <iostream>
#include <cstdlib>
#include "memory_manager.h"

MemoryHole::MemoryHole(UINT64 start, UINT64 stop)
{
    range.first = start;
    range.second = stop;

    std::cout << "New memory hole created: <" << start << ", " << stop << ">\n";
}

UINT64 MemoryHole::size()
{
    return range.second - range.first;
}

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

    MemoryHole initHole(0, systemMemorySize);
    memory.push_back(initHole);
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
    UINT64 bytesNeeded = process.footprint;

    if (bytesNeeded > freeMemory)
    {
        std::cout << "ERROR! Cannot allocate memory (" << bytesNeeded << "B) above available memory size (" << freeMemory << "B)!\n";
        exit(1);
    }


    int numHoles = memory.size();
    for (int i = 0; i < numHoles; i++)
    {
        MemoryHole currentHole = memory[i];
        if (currentHole.size() < bytesNeeded)
        {
            continue;
        }

        MemoryBlock::iterator it = memory.begin() + i;
        it = memory.erase(it);

        UINT64 procHoleEnd = currentHole.range.first + bytesNeeded;

        MemoryHole procHole(currentHole.range.first, procHoleEnd);

        currentHole.range.first = procHoleEnd + 1;

        it = memory.insert(it, procHole);
        memory.insert(it, currentHole);
    }

}

void MemoryManager::my_free(Process process)
{
    


}


void MemoryManager::executeProcessesUsingCustom(ProcessVector processes)
{
    initSystemMemory();

    int numProcs = processes.size();

    std::cout << "Running custom malloc/free test with " << numProcs << " processes...\n";

    int procIdx = 0;

    ProcessVector processesRunning;

    do
    {
        // Remove and free any finished processes
        ProcessVector::iterator it = processesRunning.begin();
        while (it != processesRunning.end())
        {
            if (!it->completed())
            {
                it++;
            }
            else
            {
                std::cout << "Removing completed process with PID: " << it->pid << "\n";
                my_free(*it);
                it = processesRunning.erase(it);
            }
        }
        
        // Execute any process in queue that can be executed
        if (queue.size() > 0)
        {
            it = queue.begin();
            while (it != queue.end())
            {
                if (!validMemoryRequest(it->footprint))
                {
                    it++;
                }
                else
                {
                    std::cout << "Allocating process with PID: " << it->pid << "\n";

                    my_malloc(*it);
                    processesRunning.push_back(*it);
                    processor.execute(*it);
                    it = queue.erase(it);
                }
            }
        }

        // Either execute or queue a process
        it = processes.begin();
        while (it != processes.end())
        {
            if (!validMemoryRequest(it->footprint))
            {
                std::cout << "Queueing process with PID: " << it->pid << "\n";
                
                queue.push_back(*it);
                it = processes.erase(it);
            }
            else
            {
                std::cout << "Executing process with PID: " << it->pid << "\n";
    
                my_malloc(*it);
                processesRunning.push_back(*it);
                processor.execute(*it);
                it = processes.erase(it);
            }
        }

    } while (processor.perform() || processes.size() || processesRunning.size());
}
