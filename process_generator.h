#ifndef PROCESS_SCHEDULER_H
#define PROCESS_SCHEDULER_H

#include <vector>
#include <cstdlib>

// Constants
const long KB = 1024;
const long MB = KB * 1024;
const long GB = MB * 1024;

struct Process 
{
    long pid;
    long cycles;
    long footprint;

    Process(long pid, long cycles, long footprint);
    void print();
};

typedef std::vector<Process> ProcessVector;

class ProcessGenerator
{
    public:
        ProcessGenerator();
        Process getProcess();
        ProcessVector getNProcesses(int n);
        ProcessVector getNProcessesWithMaxMemory(int n, int memoryInMB);

    private:
        long randint(long min, long max);
        long getRandomMemoryFootprint();
        long getRandomCpuCycles();
        bool pidIsUsed(long pid);
        long getRandomPid();

        const long MIN_MEMORY_FOOTPRINT = 0.1 * MB;
        const long MAX_MEMORY_FOOTPRINT = 10 * MB;
        const long MIN_CPU_CYCLES = 10e6;
        const long MAX_CPU_CYCLES = 50e12;
        const long MIN_PID = 1;
        const long MAX_PID = 32768; // 2^15

        std::vector<long> pids;
};

#endif //PROCESS_SCHEDULER_H
