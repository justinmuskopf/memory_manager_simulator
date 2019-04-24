#include <iostream>
#include "process_generator.h"

int main()
{
    ProcessGenerator generator;

    ProcessVector procs = generator.getNProcessesWithMaxMemory(50, 10);

    for (int i = 0; i < 50; i++)
    {
        ;//procs[i].print();
    }

    return 0;
}
