#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include <random>

#include "../xboard/xboardInterface.h"
#include "../bitboard/bitboard.h"
#include "../move/move.h"
#include "../search/tt.h"
#include "../eval/evaluator.h"

class Engine {
	public:
	Engine(std::string evalFilename);
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
	void selectAndPlayMove(std::vector<Move> &moves, bool castle);
	bool go;
	bool shouldQuit;

	MoveEvaluator eval;

	std::random_device rd;
	std::mt19937 gen;
};

#endif