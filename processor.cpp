#include <ctime>
#include <cmath>
#include <iostream>
#include "processor.h"


bool Processor::perform()
{
    removeCompletedProcesses();

    return processes.size() > 0;
}

void Processor::removeCompletedProcesses()
{
    ProcessVector::iterator it = processes.begin();
    while (it != processes.end())
    {
        if (it->completed())
        {
            std::cout << "Process " << it->pid << " has completed execution.\n";
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
    process.actualCompletionTime = process.arrivalTime + (secondsToExecute * CLOCKS_PER_SEC);

    processes.push_back(process);
}
