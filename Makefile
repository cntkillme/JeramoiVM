.PHONY: all clean

all:
	g++ --std=c++17 -Wall -Wextra -Wpedantic -Iinclude/ src/main.cpp -o main.out

clean:
	rm -f main.out
