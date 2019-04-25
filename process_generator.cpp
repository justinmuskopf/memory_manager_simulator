#include <ctime>
#include <iostream>
#include <algorithm>
#include "process_generator.h"


Process::Process(long _pid, long _cycles, long _footprint)
{
    pid = _pid;
    cycles = _cycles;
    footprint = _footprint;
}

void Process::print()
{
    float memoryInMB = (float)footprint / MB;

    std::cout << "===================================\n";
    std::cout << "| PID: " << pid << "\n";
    std::cout << "| CPU Cycles: " << cycles << "\n";
    std::cout << "| Memory Footprint: " << footprint << " B (" << memoryInMB << " MB)\n";
    std::cout << "===================================\n";
}

ProcessGenerator::ProcessGenerator(bool _verbose)
{
    srand(time(NULL));

    verbose = _verbose;
}

long ProcessGenerator::randint(long min, long max)
{
    long range = max - min;

    return (rand() % range) + min;
}

long ProcessGenerator::getRandomMemoryFootprint()
{
    return randint(MIN_MEMORY_FOOTPRINT, MAX_MEMORY_FOOTPRINT);
}

long ProcessGenerator::getRandomCpuCycles()
{
    return randint(MIN_CPU_CYCLES, MAX_CPU_CYCLES);
}

bool ProcessGenerator::pidIsUsed(long pid)
{
    bool found = std::find(pids.begin(), pids.end(), pid) != pids.end();

    return found;
}

long ProcessGenerator::getRandomPid()
{
    long pid;

    do
    {
        pid = randint(MIN_PID, MAX_PID);
    } while(pidIsUsed(pid));

    return pid;
}

Process ProcessGenerator::getProcess()
{
    long pid = getRandomPid();
    long footprint = getRandomMemoryFootprint();
    long cycles = getRandomCpuCycles();

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
    
    long memoryWantedInBytes = memoryInMB * MB;
    long maxProcessMemoryInBytes = memoryWantedInBytes / n;
    long memoryGenerated = 0;

    // Overwrite memory so that all procs fit in hole
    for (int i = 0; i < n; i++)
    {
        long newMem = randint(MIN_MEMORY_FOOTPRINT, maxProcessMemoryInBytes);

        generatedProcesses[i].footprint = newMem;

        memoryGenerated += newMem;

        maxProcessMemoryInBytes = (memoryWantedInBytes - memoryGenerated) / (n - i);
    }

    if (verbose)
        std::cout << "Total memory generated: " << memoryGenerated / static_cast<double>(MB) << "\n";

    std::random_shuffle(generatedProcesses.begin(), generatedProcesses.end());

    return generatedProcesses;
}
