#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

class constants {
	public:
	inline static uint64_t knightAttacks[64];
	inline static uint64_t rayAttacks[64][8];
	inline static uint64_t kingAttacks[64];
	inline static uint64_t pawnAttacks[64];
	inline static uint64_t castlingSquares[4];

	static const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
	static const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
	static const uint64_t notABFile = 0xFCFCFCFCFCFCFCFC;
	static const uint64_t notGHFile = 0x3F3F3F3F3F3F3F3F;
};

#endif