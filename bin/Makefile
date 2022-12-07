CC=g++
all:main

DEPS = cpu.h debug.h main_class.h system.h thread.h traits.h list.h semaphore.h

main: cpu.o debug.o system.o semaphore.o thread.o main.o main_class.o
	g++ cpu.o debug.o system.o semaphore.o thread.o main.o main_class.o -o main

cpu.o: cpu.cc $(DEPS)
	g++ -c cpu.cc

debug.o: debug.cc $(DEPS)
	g++ -c debug.cc

main_class.o: main_class.cc $(DEPS)
	g++ -c main_class.cc

main.o: main.cc system.cc $(DEPS)
	g++ -c main.cc

system.o: system.cc $(DEPS)
	g++ -c system.cc

semaphore.o: semaphore.cc $(DEPS)
	g++ -c semaphore.cc

thread.o: thread.cc $(DEPS)
	g++ -c thread.cc

clean:
	rm *.o main