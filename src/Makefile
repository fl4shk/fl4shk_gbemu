# [Insert witty comment here]

cc=g++
cflags=-std=c++0x -Wall -g -c -fno-exceptions
lflags=-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
compile=$(cc) $(cflags)

all: fl4shk_gbemu

fl4shk_gbemu: main.o cpu.o mmu.o gpu.o gbemu.o 
		$(cc) main.o cpu.o mmu.o gpu.o gbemu.o -o ../fl4shk_gbemu $(lflags)

cpu.o: cpu.cpp
		$(compile) cpu.cpp

mmu.o: mmu.cpp
		$(compile) mmu.cpp

gpu.o: gpu.cpp
		$(compile) gpu.cpp

gbemu.o: gbemu.cpp
		$(compile) gbemu.cpp

main.o: main.cpp
		$(compile) main.cpp

clean:  
		rm -rf *.o && shred -u ../fl4shk_gbemu

objclean:
		rm -rf *.o

rebuild:
		make -C .. rebuild
