#include <ctime>
#include <cmath>
#include <iostream>
#include "processor.h"


Processor::Processor()
{
    totalMemoryUsed = 0;
}

UINT64 Processor::getMemoryUsed()
{
    return totalMemoryUsed;
}

bool Processor::perform()
{
    removeCompletedProcesses();

    return processes.size() > 0;
}

void Processor::removeCompletedProcesses()
{
    int numProcs = processes.size();

    ProcessVector::iterator it = processes.begin();
    while (it != processes.end())
    {
        if (it->completed())
        {
            std::cout << "Process " << it->pid << " has completed execution.\n";

            totalMemoryUsed -= it->footprint;
            it = processes.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Processor::execute(Process &process)
{
    float secondsToExecute = process.cycles / static_cast<float>(CPU_OPS_PER_SEC);

    process.arrivalTime = clock();
    process.completionTime = process.arrivalTime + (secondsToExecute * CLOCKS_PER_SEC);

    totalMemoryUsed += process.footprint;

    processes.push_back(process);
}
