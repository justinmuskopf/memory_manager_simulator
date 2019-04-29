#include "process_generator.h"

class Processor 
{
    public:
        Processor();
        void execute(Process &process);
        bool perform();
        UINT64 getMemoryUsed();

    private:
        void removeCompletedProcesses();
        const UINT64 CPU_OPS_PER_SEC = CPU_SPEED_GHZ * 1e9;
        UINT64 totalMemoryUsed;
        ProcessVector processes;
};
