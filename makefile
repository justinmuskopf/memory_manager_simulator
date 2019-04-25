
all: clean processGenerator main memoryManager
	g++ memoryManager processGenerator main -o project_2 -std=c++11

memoryManager:
	g++ -c memory_manager.cpp -o memoryManager

processGenerator:
	g++ -c process_generator.cpp -o processGenerator

main:
	g++ -c main.cpp -o main

clean:
	rm -rf processGenerator main memoryManager
