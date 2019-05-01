#include "process_generator.h"

class Processor 
{
    public:
        void execute(Process &process);
        bool perform();

    private:
        void removeCompletedProcesses();
        const UINT64 CPU_OPS_PER_SEC = CPU_SPEED_GHZ * GHZ;
        ProcessVector processes;
};
