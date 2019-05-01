#include <iostream>
#include "memory_block.h"
#include "memory_manager.h"

MemoryHole::MemoryHole(UINT64 start, UINT64 stop)
{
    _pid = 0;
    resize(start, stop);
}

// Get the start of the hole
UINT64 MemoryHole::begin()
{
    return range.first;
}

// Get the end of the hole
UINT64 MemoryHole::end()
{
    return range.second;
}

// Get the pid of the hole
UINT64 MemoryHole::pid()
{
    return _pid;
}

// Remove the denizen from the hole
void MemoryHole::vacate()
{
    _pid = 0;
}

// Set the hole's PID
void MemoryHole::setPid(UINT64 newPid)
{
    _pid = newPid;
}

// Compares two MemoryHoles
bool MemoryHole::operator==(const MemoryHole &other)
{
    return (begin() == other.range.first && end() == other.range.second);
}

// Is the hole occupied?
bool MemoryHole::occupied()
{
    return (_pid != 0);
}

// Re-sets the beginning/end of a MemoryHole
void MemoryHole::resize(UINT64 start, UINT64 stop)
{
    if (stop < start)
    {
        std::cout << "ERROR! Invalid Memory Bounds Provided to Memory Manager! (<" << start << ", " << stop << ">)\n";
        exit(1);
    }

    // Set start/stop
    range.first = start;
    range.second = stop;

    // The new size
    _size = stop - start;

    if (MemoryManager::VERBOSE)
        std::cout << "New memory hole created: <" << start << ", " << stop << ">\n";
}

// Get the size of the hole
UINT64 MemoryHole::size()
{
    return _size;
}

// Prints all holes currently contained in memory
void MemoryBlock::print()
{
    for (MemoryHole hole : memory)
    {
        UINT64 holeSize = hole.size();
        float holeSizeMB = holeSize / static_cast<float>(MB); 
        std::cout << "\t<" << hole.begin() << ", " << hole.end() << "> (" << holeSize << " B | " << holeSizeMB << " MB)";
        if (hole.occupied())
            std::cout << " - " << "OCCUPIED PID: " << hole.pid() << "\n";
        else
            std::cout << " - " << "UNOCCUPIED\n";
    }
}
