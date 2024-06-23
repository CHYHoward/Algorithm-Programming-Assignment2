CC = g++
CFLAGS = -c

all : bin/mps
	@echo -n ""

bin/mps		:src/mps.cpp
		$(CC) $< -o $@
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*