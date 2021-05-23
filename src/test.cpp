#include "engine/engine.h"
#include <iostream>
#include <chrono>
#include "search/tt.h"

int main(int argc, char **argv) {
	Bitboard::initConstants();
	std::cout << argv[1] << "\n";
	Engine e(argv[1]);
	std::cout << "Validity tests:\n";
	e.testBoard();
	std::cout << "\nSpeed test\n";
	Bitboard b(e.initialPosition);
	Bitboard b2("4k3/2p3b1/2P2p2/2P1pn1p/b2r2BP/Np4PR/8/3K2N1 w - -");
	tt[b2.hash] = {TT_INVALID, 0, 0, Move()};
	std::stack<Bitboard> s;
	bool trash;
	std::chrono::high_resolution_clock::time_point start, end;
	start = std::chrono::high_resolution_clock::now();
	uint64_t res = b.perft(6, s);
	end = std::chrono::high_resolution_clock::now();
	std::cout << res << "\n";
	std::cout << "Done in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
	std::cout << b2.perft(6, s) << "\n";
	std::vector<Move> moves = b2.generateMoves(trash);
	for(Move& m: moves) {
		std::cout << b2.num2squareStr(m.squareFrom) << b2.num2squareStr(m.squareTo) << "\n";
	}
}