CC    = g++

FLAGS = -std=c++11 -g -Wall -pedantic

chess.out: chess.cpp
	 $(CC) $(FLAGS) chess.cpp -o chess.out

clean:
	rm -f *.o *.out
