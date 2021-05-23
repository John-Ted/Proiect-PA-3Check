#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

struct Magic {
	uint64_t mask;
	uint64_t magic;
	uint64_t* attacks;
	int shift;

	inline uint32_t index(uint64_t occupancy) {
		uint64_t ind = (occupancy & mask);
		ind = ind * magic; 
		return ind >> shift;
	}
};

class constants {
	public:
	
	inline static uint64_t knightAttacks[64];
	inline static uint64_t rayAttacks[64][8];
	inline static uint64_t kingAttacks[64];
	inline static uint64_t pawnAttacks[64];
	inline static uint64_t castlingSquares[4];
	inline static uint64_t rookEmptyBoardAttacks[64];
	inline static uint64_t bishopEmptyBoardAttacks[64];
	inline static uint64_t rookTable[0x19000];
	inline static uint64_t bishopTable[0x1480];

	inline static Magic rookMagics[64];
	inline static Magic bishopMagics[64];

	inline static uint64_t pieceHash[2][6][64];
	inline static uint64_t epHash[64];
	inline static uint64_t castleHash[16];
	inline static uint64_t sideHash;

	inline static uint64_t checkHash[2][4];

	static const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
	static const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
	static const uint64_t notABFile = 0xFCFCFCFCFCFCFCFC;
	static const uint64_t notGHFile = 0x3F3F3F3F3F3F3F3F;

	static const uint64_t AFile = 0x101010101010101ULL;
	static const uint64_t HFile = 0x8080808080808080ULL;
	static const uint64_t Rank1 = 0xFFULL;
	static const uint64_t Rank8 = 0xFF00000000000000ULL;
};

#endif