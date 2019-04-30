
all: clean processGenerator main memoryManager processor
	g++ memoryManager processGenerator processor main -o project_2 -std=c++11

memoryManager:
	g++ -c memory_manager.cpp -o memoryManager -std=c++11

processGenerator:
	g++ -c process_generator.cpp -o processGenerator -std=c++11

processor:
	g++ -c processor.cpp -o processor -std=c++11

main:
	g++ -c main.cpp -o main -std=c++11

clean:
	rm -rf processGenerator main memoryManager processor project_2
