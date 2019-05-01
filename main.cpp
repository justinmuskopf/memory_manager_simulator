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

    TimeResults times;
    times = memoryManager.executeProcessesUsingMalloc(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}

// Completes Question 2
// 50 random processes
// Using custom malloc/free @ 10MB RAM
void Question_2()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    TimeResults times;
    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}

// Completes Question 3a
// 50 random processes that take at most 10MB of RAM
// Only 5MB of RAM available
void Question_3a()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(5);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    TimeResults times;
    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}

// Completes Question 3b
// 50 random processes that take at most 10MB of RAM
// Only 1MB of RAM available
void Question_3b()
{
    ProcessGenerator generator;
    MemoryManager memoryManager(1);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    TimeResults times;

    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}


int main()
{
    Question_1();
    Question_2();
    Question_3a();
    Question_3b();

    return 0;
}
