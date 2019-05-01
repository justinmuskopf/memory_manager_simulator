#include <ctime>
#include <iostream>
#include <cstdlib>
#include "memory_manager.h"

bool MemoryManager::VERBOSE = false;

float get_elapsed(clock_t beginTime)
{
    clock_t cycles = clock() - beginTime;

    return cycles / static_cast<float>(CLOCKS_PER_SEC);
}

// MemoryManager constructor
MemoryManager::MemoryManager(int systemMemoryInMB, bool _verbose)
{
    VERBOSE = _verbose;

    systemMemorySize = systemMemoryInMB * MB;
}

// Execute all 50 processes using malloc/free
TimeResults MemoryManager::executeProcessesUsingMalloc(ProcessVector processes)
{
    float totalMallocTime = 0;
    float totalFreeTime = 0;

    clock_t begin;

    int numProcs = processes.size();

    std::cout << "Running malloc/free test with " << numProcs << " processes...\n";

    for (int i = 0; i < numProcs; i++)
    {
        begin = clock();

        Process process = processes[i];
        UINT64 memoryNeeded = process.footprint;

        std::cout << "\t" << i + 1 << ": " << "Allocating/freeing " << memoryNeeded << " bytes (" << process.footprintMB << " MB)\n";

        // Malloc, execute, free process
        process.memory = (void *)malloc(memoryNeeded);
        if (process.memory == NULL)
        {
            std::cout << "Failed to allocate memory to process " << i + 1 << "!\n";
            exit(1);            
        }

        totalMallocTime += get_elapsed(begin);

        processor.execute(process);

        begin = clock();
        free(process.memory);
        totalFreeTime += get_elapsed(begin);
    }

    // Wait for processor to finish
    while (processor.perform());


    return TimeResults {totalMallocTime, totalFreeTime, 0};
}

// Initializes system memory
void MemoryManager::initSystemMemory()
{
    // Malloc the system memory
    systemMemory = malloc(systemMemorySize);
    if (systemMemory == NULL)
    {
        perror("malloc:");
        exit(1);
    }

    // The amount of free memory
    freeMemory = systemMemorySize;

    // The first hole takes the entirety of the memory
    MemoryHole initHole(0, systemMemorySize);
    block.memory.push_back(initHole);
}

// Is this request for memory valid?
bool MemoryManager::validMemoryRequest(UINT64 footprint)
{
    return footprint <= freeMemory;
}

// Frees the system memory
void MemoryManager::freeSystemMemory()
{
    free(systemMemory);
}

// Our custom memory allocation algorithm
float MemoryManager::my_malloc(Process process)
{
    clock_t begin = clock();

    // The bytes needed by the process
    UINT64 bytesNeeded = process.footprint;
    if (bytesNeeded > freeMemory)
    {
        std::cout << "ERROR! Cannot allocate memory (" << bytesNeeded << "B) above available memory size (" << freeMemory << "B)!\n";
        exit(1);
    }

    int foundHoleIdx = -1;

    // Search for a hole of a big enough size
    int numHoles = block.memory.size();
    for (int i = 0; i < numHoles; i++)
    {
        MemoryHole currentHole = block.memory[i];
        if (currentHole.occupied() || currentHole.size() < bytesNeeded)
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

    // The found/or consolidated hole
    MemoryHole foundHole = block.memory[foundHoleIdx];

    // Get and erase the found hole from memory
    MemoryVector::iterator it = block.memory.begin() + foundHoleIdx;
    it = block.memory.erase(it);

    // Get the process's hole's end
    UINT64 procHoleEnd = foundHole.begin() + bytesNeeded - 1;

    // Create the process's hole
    MemoryHole procHole(foundHole.begin(), procHoleEnd);
    procHole.setPid(process.pid);

    // Resize the found hole to accomodate process
    foundHole.resize(procHoleEnd + 1, foundHole.end());
    foundHole.vacate();

    // Insert both holes into memory
    it = block.memory.insert(it, procHole);
    block.memory.insert(it, foundHole);

    // Update free memory
    freeMemory -= bytesNeeded;

    return get_elapsed(begin);
}

// Consolidates memory when enough free memory is available
// for a process to execute, but no single hole is large enough
void MemoryBlock::consolidate()
{
    // Holds all of the unoccupied holes
    MemoryVector fragments;
    
    UINT64 holeSize = 0;

    // Loop through holes and get those that are unoccupied
    MemoryVector::iterator it = memory.begin();
    while (it != memory.end())
    {
        if (it->occupied())
        {
            it++;
            continue;
        }

        holeSize += it->size();

        fragments.push_back(*it);
        it = memory.erase(it);
    }

    // No fragments found...
    if (fragments.size() == 0)
    {
        return;
    }

    // Make a new hole starting from the beginning
    MemoryHole newHole(0, holeSize);

    // The starting point for the rest of the holes
    UINT64 lastEnd = newHole.end();

    // Move every hole to be directly next to one another 
    it = memory.begin();
    while (it != memory.end())
    {
        it->resize(lastEnd + 1, lastEnd + it->size() + 1);
        lastEnd = it->end();

        it++;
    }

    // Insert the consolidated hole
    memory.insert(memory.begin(), newHole);

    std::cout << "Holes after consolidation:\n";
    print();
}

// Our custom memory freeing algorithm
float MemoryManager::my_free(Process process)
{
    clock_t begin = clock();

    int numHoles = block.memory.size();
    int holeIdx = -1;

    // Find this process's memory partition
    for (int i = 0; i < numHoles; i++)
    {
        MemoryHole hole = block.memory[i];

        if (hole.pid() != process.pid)
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

    // Hole is now unused, update free memory
    block.memory[holeIdx].vacate();
    freeMemory += process.footprint;

    return get_elapsed(begin);
}


// Gives processes to the "Processor" when they are capable of being serviced
TimeResults MemoryManager::executeProcessesUsingCustom(ProcessVector processes)
{
    // Initialize system memory
    initSystemMemory();

    ProcessVector processesRunning;
    ProcessVector completedProcesses;

    float totalMallocTime = 0;
    float totalFreeTime = 0;

    // The control loop
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
                if (VERBOSE)
                    std::cout << "Removing completed process with PID: " << it->pid << "\n";
                
                totalFreeTime += my_free(*it);
                completedProcesses.push_back(*it);
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
                    if (VERBOSE)
                        std::cout << "Executing queued process with PID: " << it->pid << "\n";

                    totalMallocTime += my_malloc(*it);
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
                if (VERBOSE)
                    std::cout << "Queueing process with PID: " << it->pid << "\n";
                
                queue.push_back(*it);
                it = processes.erase(it);
            }
            else
            {
                if (VERBOSE)
                    std::cout << "Executing process with PID: " << it->pid << "\n";

                totalMallocTime += my_malloc(*it);

                processor.execute(*it);
                processesRunning.push_back(*it);
                
                it = processes.erase(it);
            }
        }

    } while (processor.perform() || processes.size() || processesRunning.size() || queue.size());

    std::cout << "Holes after execution:\n";
    block.print();


    float overheadTime = 0;
    for (Process process : completedProcesses)
    {
        overheadTime += (process.actualCompletionTime - process.predictedCompletionTime) / static_cast<float>(CLOCKS_PER_SEC);
    }

    overheadTime /= completedProcesses.size();

    return TimeResults {totalMallocTime, totalFreeTime, overheadTime};
}
