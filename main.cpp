#include <iostream>
#include "process_generator.h"

int main()
{
    ProcessGenerator generator;

    Process p = generator.getNewProcess();
    p.print();

    return 0;
}
