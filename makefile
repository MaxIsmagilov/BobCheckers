all:
	gcc -O3 -std=c++17 src/BobCheckers.cpp src/BobCheckersBackend.hh -o bin/BobCheckers.exe -lstdc++ 