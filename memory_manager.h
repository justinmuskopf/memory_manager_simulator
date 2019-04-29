#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <utility>
#include "process_generator.h"
#include "processor.h"

typedef std::pair<int, int> MemoryHole;

typedef std::vector<MemoryHole> MemoryBlock;


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

        MemoryBlock memory;
        ProcessVector queue;
};

#endif //MEMORY_MANAGER_H
