
all: clean processGenerator main memoryManager processor
	g++ memoryManager processGenerator processor main -o project_2 -std=c++11

memoryManager:
	g++ -c memory_manager.cpp -o memoryManager

processGenerator:
	g++ -c process_generator.cpp -o processGenerator

processor:
	g++ -c processor.cpp -o processor

main:
	g++ -c main.cpp -o main

clean:
	rm -rf processGenerator main memoryManager processor project_2
