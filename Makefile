.PHONY: clean

context.out: context.cpp
	g++ -Wall -pedantic -g3 -std=c++14 -pthread -o $@ $^

clean:
	rm -rf *.out *.o
