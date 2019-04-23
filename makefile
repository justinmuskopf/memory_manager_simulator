
all: clean processGenerator main memoryManager

memoryManager:
	g++ processGenerator main -o memoryManager

processGenerator:
	g++ -c process_generator.cpp -o processGenerator

main:
	g++ -c main.cpp -o main

clean:
	rm -rf processGenerator main memoryManager
