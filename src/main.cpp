#include <iostream>
#include <cstdint>
#include "engine/engine.h"


int main() {
	Bitboard::initConstants();
	Engine e;
	XBoardInterface interface;
	//e.testBoard();
	while(true) {
		Command c = interface.getCommand(!e.go);
		e.processCommand(c);
	}
	/*Bitboard b("3qkbnr/4pppp/8/1B1p4/4P3/1P6/5PPP/1NBQK1NR b Kk -");
	std::stack<Bitboard> s;
	for(Move m: b.generateMoves()) {
		std::cout << (uint32_t)m.movedPiece << Bitboard::num2squareStr(m.squareFrom)
		<< Bitboard::num2squareStr(m.squareTo) << '\n';
	}
	std::cout << b.perft(6, s) << '\n';*/
	/*Bitboard b;
	b = Bitboard(e.initialPosition);
	std::cout << b.chessBoard2str() << "\n";
	for(Move m: e.bitboard.generateMoves()) {
		if(m.movedPiece > 5) {
			std::cout << "OH NO!\n";
		}
	}*/
	return 0;
}