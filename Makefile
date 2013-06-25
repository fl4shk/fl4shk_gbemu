# I am new to creating makefiles.

cc=g++
flags=-masm=intel -std=gnu++0x -Wall -O3 -lsfml-graphics -lsfml-window  -lsfml-audio -lsfml-network -lsfml-system

all: fl4shk_gbemu

fl4shk_gbemu: src/fl4shk_gbemu.cpp
		$(cc) $(flags) src/fl4shk_gbemu.cpp -o fl4shk_gbemu

clean:  
		rm -rf fl4shk_gbemu
