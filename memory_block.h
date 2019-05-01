#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include "process_generator.h"

class MemoryHole
{ 
    public:
        MemoryHole(UINT64, UINT64);

        UINT64 begin();
        UINT64 end();
        void resize(UINT64, UINT64);
        void vacate();
        bool occupied();
        bool operator==(const MemoryHole &other);
        UINT64 size();
        void setPid(UINT64 newPid);
        UINT64 pid();

    private:
        std::pair<UINT64, UINT64> range;
        UINT64 _pid;
        UINT64 _size;
};


typedef std::vector<MemoryHole> MemoryVector;

struct MemoryBlock
{
    MemoryVector memory;
    void print();
    void consolidate();
};

#endif //MEMORY_BLOCK_H
