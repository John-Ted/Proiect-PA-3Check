#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stack>

#include "constants.h"
#include "../move/move.h"
#include "../misc/misc.h"

enum sides
{
	white,
	black
};

enum directions {
    NoWe,
    Nort,
    NoEa,
    East,
    SoEa,
    Sout,
    SoWe,
    West,
};

enum pieces
{
	pawn,
	knight,
	bishop,
	rook,
	queen,
	king,
	empty
};

enum specialMoves {
	white_castle_long,
	white_castle_short,
	black_castle_long,
	black_castle_short,
	promotion,
	en_passant
};


class Bitboard
{
public:
	Bitboard();
	Bitboard(std::string FENstring);

	//bool operator==(Bitboard& b);

	static void initConstants();

	static uint64_t S1(uint64_t b);
	static uint64_t N1(uint64_t b);
	static uint64_t E1(uint64_t b);
	static uint64_t NE1(uint64_t b);
	static uint64_t SE1(uint64_t b);
	static uint64_t W1(uint64_t b);
	static uint64_t SW1(uint64_t b);
	static uint64_t NW1(uint64_t b);

	static uint64_t NNE(uint64_t b);
	static uint64_t NEE(uint64_t b);
	static uint64_t SEE(uint64_t b);
	static uint64_t SSE(uint64_t b);
	static uint64_t NNW(uint64_t b);
	static uint64_t NWW(uint64_t b);
	static uint64_t SWW(uint64_t b);
	static uint64_t SSW(uint64_t b);

	static uint64_t rankMask(uint64_t sq);

	static uint64_t fileMask(uint64_t sq);

	static uint64_t diagonalMask(uint64_t sq);
	static uint64_t antiDiagMask(uint64_t sq);

	static uint64_t rankMaskEx(int sq);
	static uint64_t fileMaskEx(int sq);
	static uint64_t diagonalMaskEx(int sq);
	static uint64_t antiDiagMaskEx(int sq);

	static uint64_t rookMask(int sq);
	static uint64_t bishopMask(int sq);

	static uint64_t rookMaskEx(int sq);
	static uint64_t bishopMaskEx(int sq);

	static uint64_t queenMask(int sq);
	static uint64_t queenMaskEx(int sq);

	static uint64_t kingAttacks(uint64_t kingSet);
	static uint64_t knightAttacks(uint64_t knights);

	static std::string bitboard2str(uint64_t bitboard, int highlight = -1, char character = 'T');

	static uint64_t squareString2mask(std::string square);
	static uint64_t squareString2num(std::string square);

	uint64_t wPushPawns1(uint64_t pawns);
	uint64_t wPushPawns2(uint64_t pawns);

	uint64_t bPushPawns1(uint64_t bpawns);
	uint64_t bPushPawns2(uint64_t bpawns);


	static uint64_t getPositiveRayAttacks(int direction, uint64_t square, uint64_t blockers);
	static uint64_t getNegativeRayAttacks(int direction, uint64_t square, uint64_t blockers);
	static uint64_t getRookEmptyBoardAttacks(uint64_t square);
	static uint64_t getBishopEmptyBoardAttacks(uint64_t square);
	static uint64_t generateBlockers(int square, uint64_t mask);
	static uint64_t getBishopAttacksClassic(uint64_t square, uint64_t blockers);
	static uint64_t getRookAttacksClassic(uint64_t square, uint64_t blockers);

	uint64_t getBishopAttacks(uint64_t square, uint8_t side);
	uint64_t getRookAttacks(uint64_t square, uint8_t side);
	uint64_t getQueenAttacks(uint64_t square, uint8_t side);

	uint64_t wPawnEastAttacks(uint64_t wpawns);
	uint64_t wPawnWestAttacks(uint64_t wpawns);

	uint64_t bPawnEastAttacks(uint64_t bpawns);
	uint64_t bPawnWestAttacks(uint64_t bpawns);

	uint64_t getAllAttacks(uint8_t side);


	std::vector<Move> generateMoves(bool &castle);
	void generatePawnCaptures(std::vector<Move> &moves);
	void generatePawnPushes(std::vector<Move> &moves);
	void generateKnightMoves(std::vector<Move> &moves);
	void generateRookMoves(std::vector<Move> &moves);
	void generateBishopMoves(std::vector<Move> &moves);
	void generateQueenMoves(std::vector<Move> &moves);
	void generateNormalKingMoves(std::vector<Move> &moves);
	void generateCastling(std::vector<Move> &moves);
	void generateEnPassant(std::vector<Move> &moves);


	uint64_t perft(int depth, std::stack<Bitboard> &moveStack);


	std::string chessBoard2str();

	bool checkLegal(Move move);


	void movePiece(uint32_t pieceType, uint32_t side,
				uint8_t squareFrom, uint8_t squareTo);
	void addPiece(uint32_t pieceType, uint32_t side, uint8_t square);
	void removePiece(uint32_t pieceType, uint32_t side, uint8_t square);

	void makeMove(const Move &move, std::stack<Bitboard> &moveStack);
	void unmakeMove(const Move &move, std::stack<Bitboard> &moveStack);

	static std::string num2squareStr(uint8_t square) {
		std::string squareStr;
		squareStr += (square % 8) + 'a';
		squareStr += (square / 8) + '1';
		return squareStr;
	}

	uint64_t occupancy[2];
	uint64_t pieces[6];

	uint64_t enPassant;
	uint8_t castleFlags;
	uint8_t sideToPlay;

	uint8_t ourSide;
};

#endif