#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <utility>
#include <ctime>
#include "process_generator.h"
#include "processor.h"
#include "memory_block.h"


float get_elapsed(clock_t beginTime);

struct TimeResults
{
    float mallocTime;
    float freeTime;
    float overhead;
};

class MemoryManager
{
    public:
        MemoryManager(int systemMemoryInMB, bool verbose=false);
        TimeResults executeProcessesUsingMalloc(ProcessVector processes);
        TimeResults executeProcessesUsingCustom(ProcessVector processes);

        static bool VERBOSE;

    private:
        void initSystemMemory();
        void freeSystemMemory();
        float my_malloc(Process process);
        float my_free(Process process);
        bool validMemoryRequest(UINT64 footprint);

        Processor processor;
        
        void *systemMemory;
        UINT64 systemMemorySize;
        UINT64 freeMemory;

        MemoryBlock block;
        ProcessVector queue;
};

#endif //MEMORY_MANAGER_H
