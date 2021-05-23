#include <iostream>
#include <cstdint>
#include <chrono>
#include <immintrin.h>
#include "engine/engine.h"


int main(int argc, char **argv) {
	Bitboard::initConstants();
	Engine e(argv[1]);

	//e.testBoard();
	//return 0;

	XBoardInterface interface;
	srand(time(NULL));
	while(!e.shouldQuit) {
		Command c = interface.getCommand(!e.go);
		e.processCommand(c);
	}

	return 0;
}