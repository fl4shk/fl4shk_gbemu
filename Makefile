# [Insert witty comment here]

all: 
		make -C src/

clean:  
		make clean -C src/

objclean:
		make objclean -C src/

rebuild:
		make clean && make
