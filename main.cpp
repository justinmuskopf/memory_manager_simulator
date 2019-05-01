#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "process_generator.h"
#include "memory_manager.h"

// Completes Question 1
// 50 random processes
// Using malloc/free
void Question_1(bool verbose=false)
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10, verbose);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    TimeResults times;
    times = memoryManager.executeProcessesUsingMalloc(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}

// Completes Question 2
// 50 random processes
// Using custom malloc/free @ 10MB RAM
void Question_2(bool verbose=false)
{
    ProcessGenerator generator;
    MemoryManager memoryManager(10, verbose);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    std::cout << "Running custom malloc/free with 50 processes (consuming at most 10MB) and 10 MB system memory...\n";

    TimeResults times;
    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
}

// Completes Question 3a
// 50 random processes that take at most 10MB of RAM
// Only 5MB of RAM available
void Question_3a(bool verbose=false)
{
    ProcessGenerator generator;
    MemoryManager memoryManager(5, verbose);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);
    
    std::cout << "Running custom malloc/free with 50 processes and system memory at 50%...\n";

    TimeResults times;
    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";
    std::cout << "Average time expended on waiting for sufficient memory: " << times.overhead << " seconds\n";
}

// Completes Question 3b
// 50 random processes that take at most 10MB of RAM
// Only 1MB of RAM available
void Question_3b(bool verbose=false)
{
    ProcessGenerator generator;
    MemoryManager memoryManager(1, verbose);

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    TimeResults times;

    std::cout << "Running custom malloc/free with 50 processes and system memory at 10%...\n";

    times = memoryManager.executeProcessesUsingCustom(procs);

    std::cout << "Total time given to Malloc: " << times.mallocTime << "\n";
    std::cout << "Total time given to Free: " << times.freeTime << "\n";

    std::cout << "Average time expended on waiting for sufficient memory: " << times.overhead << " seconds\n";
}

// Clears the screen
void clearScreen()
{
    system("clear");
    system("clear");
}

int main(int argc, char *argv[])
{
    bool verbose = false;
    if (argc == 2 && !strcmp(argv[1], "-v"))
    {
        verbose = true;
    }

    Question_1(verbose);
    std::cout << "Question 1 completed. Press enter to execute Question 2.\n";
    getchar();
    clearScreen();

    Question_2(verbose);
    std::cout << "Question 2 completed. Press enter to execute Question 3a.\n";
    getchar();
    clearScreen();

    Question_3a(verbose);
    std::cout << "Question 3a completed. Press enter to execute Question 3b.\n";
    getchar();
    clearScreen();

    Question_3b(verbose);
    std::cout << "Question 3b completed.\n";

    return 0;
}
