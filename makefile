all:
	gcc -O3 -std=c++17 src/BobCheckers.cpp src/BobCheckersAPI.hh src/BobCheckersBackend.hh -o bin/BobCheckers.exe -lstdc++ 

debug:
	gcc -O1 -std=c++17 src/BobCheckers.cpp src/BobCheckersAPI.hh src/BobCheckersBackend.hh -o bin/BobCheckers.exe -lstdc++ 

run:
	./bin/BobCheckers.exe
