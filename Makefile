.PHONY: all clean

all:
	g++ --std=c++17 -Wall -Wextra -Wpedantic main.cpp -o main.out

clean:
	rm -f main.out
