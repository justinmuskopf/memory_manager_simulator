#include <iostream>
#include "process_generator.h"
#include "memory_manager.h"

void Question_1()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingMalloc(procs);
}

void Question_3()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}


int main()
{
    Question_3();

    return 0;
}
