#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>

#include "../xboard/xboardInterface.h"
#include "../bitboard/bitboard.h"
#include "../move/move.h"

class Engine {
	public:
	Engine();
	void testBoard();
	void processCommand(Command c);
	Move command2move(Command c);
	void writeDebug();
	
	std::ofstream out;
	void debugMove(Move m);
	std::stack<Bitboard> moveStack;
	std::string name = "3CheckRapist";
	std::string initialPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
	Bitboard bitboard;
	bool go;
	bool shouldQuit;
};

#endif