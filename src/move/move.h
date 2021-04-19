#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

struct Move {
	uint8_t squareFrom;
	uint8_t squareTo;

	uint8_t moveFlags;
	//bit 0 - white castle long
	//bit 1 - white castle short
	//bit 2 - black castle long
	//bit 3 - black castle short
	//bit 4 - promotion
	//bit 5 - en passant

	uint8_t newCastlingFlags;

	uint8_t sideThatMoved;
	uint8_t promotion;

	uint8_t movedPiece;
	uint8_t capturedPiece;

	uint8_t newEnPassant;

	Move(){}

	Move(uint8_t squareFrom, uint8_t squareTo, uint8_t moveFlags,
	uint8_t newCastlingFlags, uint8_t sideThatMoved, uint8_t promotion,
	uint8_t movedPiece, uint8_t capturedPiece,
	uint64_t newEnPassant) {
		this->squareFrom = squareFrom;
		this->squareTo = squareTo;
		this->moveFlags = moveFlags;
		this->newCastlingFlags = newCastlingFlags;
		this->sideThatMoved = sideThatMoved;
		this->promotion = promotion;
		this->movedPiece = movedPiece;
		this->capturedPiece = capturedPiece;
		this->newEnPassant = newEnPassant;
	}
};

#endif