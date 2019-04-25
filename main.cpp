#include <iostream>
#include "process_generator.h"
#include "memory_manager.h"

void Question_1()
{
    ProcessGenerator generator;
    MemoryManager memoryManager;

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingMalloc(procs);
}


int main()
{
    Question_1();

    return 0;
}
