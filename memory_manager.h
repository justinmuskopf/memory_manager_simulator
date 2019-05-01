#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <utility>
#include <ctime>
#include "process_generator.h"
#include "processor.h"

struct MemoryHole
{
    MemoryHole(UINT64, UINT64);
    
    UINT64 begin();
    UINT64 end();
    void resize(UINT64, UINT64);
    bool occupied();
    bool operator==(const MemoryHole &other);
    
    std::pair<UINT64, UINT64> range;
    UINT64 pid;
    UINT64 size;
};

typedef std::vector<MemoryHole> MemoryVector;

struct MemoryBlock
{
    MemoryVector memory;
    void print();
    void consolidate();
};

float get_elapsed(clock_t beginTime);

struct TimeResults
{
    float mallocTime;
    float freeTime;
};

class MemoryManager
{
    public:
        MemoryManager(int systemMemoryInMB);
        TimeResults executeProcessesUsingMalloc(ProcessVector processes);
        TimeResults executeProcessesUsingCustom(ProcessVector processes);

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
