# [Insert witty comment here]

cc=g++
cflags=-masm=intel -std=gnu++0x -Wall -O3 -c
lflags=-masm=intel -std=gnu++0x -Wall -O3 -lsfml-graphics -lsfml-window  -lsfml-audio -lsfml-network -lsfml-system
compile=$(cc) $(cflags)
build=$(cc) $(lflags)

all: fl4shk_gbemu

fl4shk_gbemu: cpu.o mmu.o gpu.o gbemu.o main.o 
		$(build) cpu.o mmu.o gpu.o gbemu.o main.o -o ../fl4shk_gbemu

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
