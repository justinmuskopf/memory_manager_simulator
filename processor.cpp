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
    time_t currentTime = time(NULL);

    int numProcs = processes.size();

    for (int i = 0; i < numProcs; i++)
    {
        if (currentTime > processes[i].completionTime)
        {
            std::cout << "Process " << i + 1 << " has completed execution.\n";

            totalMemoryUsed -= processes[i].footprint;
            processes.erase(processes.begin() + i);
        }
    }
}

void Processor::execute(Process &process)
{

    float secondsToExecute = process.cycles / static_cast<float>(CPU_OPS_PER_SEC);

    process.arrivalTime = time(NULL);
    process.completionTime = process.arrivalTime + ceil(secondsToExecute);

    totalMemoryUsed += process.footprint;

    processes.push_back(process);
}
