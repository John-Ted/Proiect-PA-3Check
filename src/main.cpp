#include <iostream>
#include <cstdint>
#include <chrono>
#include <immintrin.h>
#include "engine/engine.h"


int main() {
	Bitboard::initConstants();
	Engine e;
	
	XBoardInterface interface;
	srand(time(NULL));
	while(!e.shouldQuit) {
		Command c = interface.getCommand(!e.go);
		e.processCommand(c);
	}

	return 0;
}