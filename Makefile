# I am new to creating makefiles.

CC=g++-4.7
CFLAGS=-masm=intel -std=gnu++0x -Wall -O3 -lsfml-graphics -lsfml-window  -lsfml-audio -lsfml-network -lsfml-system

all: fl4shk_gbemu

fl4shk_gbemu: src/fl4shk_gbemu.cpp
				$(CC) $(CFLAGS) src/fl4shk_gbemu.cpp -o fl4shk_gbemu

