#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <utility>
#include "process_generator.h"
#include "processor.h"

struct MemoryHole
{
    MemoryHole(UINT64, UINT64);
    void resize(UINT64, UINT64);
    std::pair<UINT64, UINT64> range;
    UINT64 pid;
    UINT64 size;
    bool operator==(const MemoryHole &other);
};

typedef std::vector<MemoryHole> MemoryVector;

struct MemoryBlock
{
    MemoryVector memory;
    void print();
    void consolidate();
};


class MemoryManager
{
    public:
        MemoryManager(int systemMemoryInMB);
        void executeProcessesUsingMalloc(ProcessVector processes);
        void executeProcessesUsingCustom(ProcessVector processes);

    private:
        void initSystemMemory();
        void freeSystemMemory();
        void my_malloc(Process process);
        void my_free(Process process);
        bool validMemoryRequest(UINT64 footprint);

        Processor processor;
        
        void *systemMemory;
        UINT64 systemMemorySize;
        UINT64 freeMemory;

        MemoryBlock block;
        ProcessVector queue;
};

#endif //MEMORY_MANAGER_H
