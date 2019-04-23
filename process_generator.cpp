#include <ctime>
#include <iostream>
#include "process_generator.h"

Process::Process(long _pid, long _cycles, long _footprint)
{
    pid = _pid;
    cycles = _cycles;
    footprint = _footprint;
}

void Process::print()
{
    std::cout << "===================================\n";
    std::cout << "| PID: " << pid << "\n";
    std::cout << "| CPU Cycles: " << cycles << "\n";
    std::cout << "| Memory Footprint: " << footprint << "\n";
    std::cout << "===================================\n";
}

ProcessGenerator::ProcessGenerator()
{
    srand(time(NULL));
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

long ProcessGenerator::getRandomPid()
{
    return randint(MIN_PID, MAX_PID);
}

Process ProcessGenerator::getNewProcess()
{
    long pid = getRandomPid();
    long footprint = getRandomMemoryFootprint();
    long cycles = getRandomCpuCycles();

    Process process(pid, cycles, footprint);

    processes.push_back(process);

    return process;
}

ProcessVector ProcessGenerator::getNNewProcesses(int n)
{
    ProcessVector generatedProcesses;

    for (int i = 0; i < n; i++)
    {
        Process process = getNewProcess();

        return generatedProcesses;
    }

    return generatedProcesses;
}
