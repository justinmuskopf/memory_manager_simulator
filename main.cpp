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

void Question_2()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}

void Question_3a()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(5);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}

void Question_3b()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(1);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}


int main()
{
    Question_3b();

    return 0;
}
