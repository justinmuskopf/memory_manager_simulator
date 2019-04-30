#include <iostream>
#include "process_generator.h"
#include "memory_manager.h"

// Completes Question 1
// 50 random processes
// Using malloc/free
void Question_1()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingMalloc(procs);
}

// Completes Question 2
// 50 random processes
// Using custom malloc/free @ 10MB RAM
void Question_2()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}

// Completes Question 3a
// 50 random processes that take at most 10MB of RAM
// Only 5MB of RAM available
void Question_3a()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(5);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}

// Completes Question 3b
// 50 random processes that take at most 10MB of RAM
// Only 1MB of RAM available
void Question_3b()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(1);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    memoryManager.executeProcessesUsingCustom(procs);
}


int main()
{
    //Question_1();
    //Question_2();
    //Question_3a();
    Question_3b();

    return 0;
}
