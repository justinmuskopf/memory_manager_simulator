#include <ctime>
#include <iostream>
#include <cstdlib>
#include "memory_manager.h"

MemoryHole::MemoryHole(UINT64 start, UINT64 stop)
{
    pid = 0;
    resize(start, stop);
}


bool MemoryHole::operator==(const MemoryHole &other)
{
    return (range.first == other.range.first && range.second == other.range.second);
}

void MemoryHole::resize(UINT64 start, UINT64 stop)
{
    range.first = start;
    range.second = stop;

    size = stop - start;

    std::cout << "New memory hole created: <" << start << ", " << stop << ">\n";
}


void MemoryBlock::print()
{
    std::cout << "Holes after execution:\n";
    for (MemoryHole hole : memory)
    {
        UINT64 holeSize = hole.size;
        float holeSizeMB = holeSize / static_cast<float>(MB); 
        std::cout << "\t<" << hole.range.first << ", " << hole.range.second << "> (" << holeSize << " B | " << holeSizeMB << " MB)\n"; 
    }
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
    block.memory.push_back(initHole);
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

    int foundHoleIdx = -1;

    int numHoles = block.memory.size();
    for (int i = 0; i < numHoles; i++)
    {
        MemoryHole currentHole = block.memory[i];
        if (currentHole.pid != 0 || currentHole.size < bytesNeeded)
        {
            continue;
        }

        foundHoleIdx = i;
    }

    // No hole big enough, consolidate fragments
    if (foundHoleIdx == -1)
    {
        block.consolidate();
        foundHoleIdx = 0;
    }

    MemoryHole foundHole = block.memory[foundHoleIdx];

    MemoryVector::iterator it = block.memory.begin() + foundHoleIdx;
    it = block.memory.erase(it);

    UINT64 procHoleEnd = foundHole.range.first + bytesNeeded;

    MemoryHole procHole(foundHole.range.first, procHoleEnd);
    procHole.pid = process.pid;

    foundHole.resize(procHoleEnd + 1, foundHole.range.second);
    foundHole.pid = 0;

    it = block.memory.insert(it, procHole);
    block.memory.insert(it, foundHole);

    freeMemory -= bytesNeeded;
}


void MemoryBlock::consolidate()
{
    MemoryVector fragments;
    
    MemoryVector::iterator it = memory.begin();
    while (it != memory.end())
    {
        if (it->pid != 0)
        {
            it++;
            continue;
        }

        fragments.push_back(*it);
        it = memory.erase(it);
    }
    
    if (fragments.size() == 0)
    {
        return;
    }

    UINT64 holeSize = 0;
    for (MemoryHole hole : fragments)
    {
        holeSize += hole.size;
    }

    MemoryHole newHole(0, holeSize);

    UINT64 lastEnd = newHole.range.second;

    it = memory.begin();
    while (it != memory.end())
    {
        it->range.first = lastEnd + 1;
        it->range.second = it->range.first + it->size;
        lastEnd = it->range.second;

        it++;
    }

    memory.insert(memory.begin(), newHole);
}

void MemoryManager::my_free(Process process)
{
    int numHoles = block.memory.size();
    int holeIdx = -1;

    // Find this process's memory partition
    for (int i = 0; i < numHoles; i++)
    {
        MemoryHole hole = block.memory[i];

        if (hole.pid != process.pid)
        {
            continue;
        }

        holeIdx = i;
        break;
    }

    // Hole not found, memory error
    if (holeIdx == -1)
    {
        std::cout << "ERROR! Could not find memory partition of PID: " << process.pid << "\n";
        exit(1);
    }

    block.memory[holeIdx].pid = 0;
    freeMemory += process.footprint;
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
                    processor.execute(*it);
                    processesRunning.push_back(*it);
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

                processor.execute(*it);
                processesRunning.push_back(*it);
                
                it = processes.erase(it);
            }
        }

    } while (processor.perform() || processes.size() || processesRunning.size() || queue.size());

    block.print();
}
