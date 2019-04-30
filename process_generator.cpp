#include <ctime>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "process_generator.h"

#define SEC_TO_US 1e6

Process::Process(UINT64 _pid, UINT64 _cycles, UINT64 _footprint)
{
    pid = _pid;
    cycles = _cycles;
    setFootprint(_footprint);
    completionTime = 0;
}

void Process::setFootprint(UINT64 fp)
{
    footprint = fp;
    footprintMB = fp / static_cast<float>(MB);
}

bool Process::running()
{
    return completionTime > clock();
}

bool Process::completed()
{
    return completionTime != 0 && !running();
}

void Process::print()
{
    std::cout << "===================================\n";
    std::cout << "| PID: " << pid << "\n";
    std::cout << "| CPU Cycles: " << cycles << "\n";
    std::cout << "| Memory Footprint: " << footprint << " B (" << footprintMB << " MB)\n";
    std::cout << "===================================\n";
}

ProcessGenerator::ProcessGenerator(bool _verbose)
{
    srand(time(NULL));

    verbose = _verbose;
}

UINT64 ProcessGenerator::randint(UINT64 min, UINT64 max)
{
    UINT64 range = max - min;

    return (static_cast<UINT64>(rand() * 100) % range) + min;
}

UINT64 ProcessGenerator::getRandomMemoryFootprint()
{
    return randint(MIN_MEMORY_FOOTPRINT, MAX_MEMORY_FOOTPRINT);
}

UINT64 ProcessGenerator::getRandomCpuCycles()
{
    return randint(MIN_CPU_CYCLES, MAX_CPU_CYCLES);
}

bool ProcessGenerator::pidIsUsed(UINT64 pid)
{
    bool found = std::find(pids.begin(), pids.end(), pid) != pids.end();

    return found;
}

UINT64 ProcessGenerator::getRandomPid()
{
    UINT64 pid;

    do
    {
        pid = randint(MIN_PID, MAX_PID);
    } while(pidIsUsed(pid));

    return pid;
}

Process ProcessGenerator::getProcess()
{
    UINT64 pid = getRandomPid();
    UINT64 footprint = getRandomMemoryFootprint();
    UINT64 cycles = getRandomCpuCycles();

    std::cout << cycles/static_cast<float>(CPU_OPS_PER_SEC) << "\n";

    Process process(pid, cycles, footprint);

    pids.push_back(pid);

    return process;
}

ProcessVector ProcessGenerator::getNProcesses(int n)
{
    ProcessVector generatedProcesses;

    for (int i = 0; i < n; i++)
    {
        Process process = getProcess();

        generatedProcesses.push_back(process);
    }

    return generatedProcesses;
}

ProcessVector ProcessGenerator::getNProcessesWithMaxMemory(int n, int memoryInMB)
{
    ProcessVector generatedProcesses = getNProcesses(n);
    
    UINT64 memoryWantedInBytes = memoryInMB * MB;
    UINT64 maxProcessMemoryInBytes = memoryWantedInBytes / n;
    UINT64 memoryGenerated = 0;

    // Overwrite memory so that all procs fit in hole
    for (int i = 0; i < n; i++)
    {
        UINT64 newMem = randint(MIN_MEMORY_FOOTPRINT, maxProcessMemoryInBytes);

        generatedProcesses[i].setFootprint(newMem);

        memoryGenerated += newMem;

        maxProcessMemoryInBytes = (memoryWantedInBytes - memoryGenerated) / (n - i);
    }

    if (verbose)
        std::cout << "Total memory generated: " << memoryGenerated / static_cast<double>(MB) << "\n";

    std::random_shuffle(generatedProcesses.begin(), generatedProcesses.end());

    return generatedProcesses;
}
