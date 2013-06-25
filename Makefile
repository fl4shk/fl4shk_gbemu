# [Insert witty comment here]

cc=g++
cflags=-masm=intel -std=gnu++0x -Wall -O3 -c
lflags=-masm=intel -std=gnu++0x -Wall -O3 -lsfml-graphics -lsfml-window  -lsfml-audio -lsfml-network -lsfml-system
compile=$(cc) $(cflags)
build=$(cc) $(lflags)

all: 
		make -C src/

clean:  
		make clean -C src/

objclean:
		make objclean -C src/
