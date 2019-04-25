#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "process_generator.h"


class MemoryManager
{
    public:
        MemoryManager();
        void executeProcessesUsingMalloc(ProcessVector processes);

    private:

        Processor processor;
};

#endif //MEMORY_MANAGER_H
