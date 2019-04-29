#ifndef PROCESS_SCHEDULER_H
#define PROCESS_SCHEDULER_H

#include <vector>
#include <cstdlib>

typedef unsigned long long int UINT64;

// Constants
const UINT64 KB  = 1024;
const UINT64 MB  = KB * 1024;
const UINT64 GB  = MB * 1024;
const UINT64 GHZ = 1e9;

const float CPU_SPEED_GHZ        = 3.5;                 // Value used for CPU speed simulation
const UINT64 CPU_OPS_PER_SEC      = CPU_SPEED_GHZ * GHZ; // Number of CPU operations per second
const UINT64 MAX_PROCESS_RUN_TIME = 5;                   // Maximum number of seconds a process should "run"

struct Process 
{
    Process(UINT64 pid, UINT64 cycles, UINT64 footprint);
    void setFootprint(UINT64 fp);
    void print();
    bool running();
    bool completed();

    UINT64 pid;
    UINT64 cycles;
    UINT64 footprint;
    UINT64 arrivalTime;
    UINT64 completionTime;
    float footprintMB;

    void *memory;
};

typedef std::vector<Process> ProcessVector;



class ProcessGenerator
{
    public:
        ProcessGenerator(bool verbose=false);
        Process getProcess();
        ProcessVector getNProcesses(int n);
        ProcessVector getNProcessesWithMaxMemory(int n, int memoryInMB);

    private:
        UINT64 randint(UINT64 min, UINT64 max);
        UINT64 getRandomMemoryFootprint();
        UINT64 getRandomCpuCycles();
        bool pidIsUsed(UINT64 pid);
        UINT64 getRandomPid();

        const UINT64 MIN_MEMORY_FOOTPRINT = 0.1 * MB;
        const UINT64 MAX_MEMORY_FOOTPRINT = 10 * MB;
        const UINT64 MIN_CPU_CYCLES = 10e6;
        const UINT64 MAX_CPU_CYCLES = CPU_OPS_PER_SEC * MAX_PROCESS_RUN_TIME;
        const UINT64 MIN_PID = 1;
        const UINT64 MAX_PID = 32768; // 2^15

        bool verbose;

        std::vector<UINT64> pids;
};

#endif //PROCESS_SCHEDULER_H
