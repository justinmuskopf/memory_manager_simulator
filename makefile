
G++ = g++ -Wall -std=c++11
G++_FLAGS = $(G++) -c
ALL = process_generator.o main.o memory_manager.o processor.o memory_block.o
EXEC_NAME = project_2

all: clean $(ALL)
	$(G++) $(ALL) -o $(EXEC_NAME)

memory_manager.o:
	$(G++_FLAGS) memory_manager.cpp

process_generator.o:
	$(G++_FLAGS) process_generator.cpp

processor.o:
	$(G++_FLAGS) processor.cpp

main.o:
	$(G++_FLAGS) main.cpp

memory_block.o:
	$(G++_FLAGS) memory_block.cpp

clean:
	rm -rf $(ALL) $(EXEC_NAME)
