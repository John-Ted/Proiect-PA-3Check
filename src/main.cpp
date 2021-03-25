#include <iostream>
#include <cstdint>
#include <immintrin.h>
#include "engine/engine.h"


int main() {
	Bitboard::initConstants();
	Engine e;
	//e.testBoard();
	XBoardInterface interface;
	srand(time(NULL));
	while(!e.shouldQuit) {
		Command c = interface.getCommand(!e.go);
		e.processCommand(c);
	}
	return 0;
}