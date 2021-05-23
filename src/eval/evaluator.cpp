#include "evaluator.h"

uint64_t files[] = {
	0x101010101010101ULL,
	0x202020202020202ULL,
	0x404040404040404ULL,
	0x808080808080808ULL,
	0x1010101010101010ULL,
	0x2020202020202020ULL,
	0x4040404040404040ULL,
	0x8080808080808080ULL
};

void MoveEvaluator::init(std::string filename) {

}

uint64_t popcount(uint64_t b) {
	return __builtin_popcountll(b);
}

int32_t MoveEvaluator::eval(Bitboard &b, int8_t player) {
	uint64_t materialScore = 0;
	for(int p = pawn; p < king; p++) {
		uint64_t ourPieces = b.occupancy[b.sideToPlay] & b.pieces[p];
		uint64_t theirPieces = b.occupancy[!b.sideToPlay] & b.pieces[p];
		materialScore += materialScores[p] * (popcount(ourPieces) - popcount(theirPieces));
	}

	uint64_t ourAttacks = b.getAllAttacks(b.sideToPlay);
	uint64_t theirAttacks = b.getAllAttacks(!b.sideToPlay);

	uint64_t checkScore = checks[b.checks[b.sideToPlay]] - checks[b.checks[!b.sideToPlay]];

	int32_t mobilityScore = popcount(ourAttacks) - popcount(theirAttacks);

	int score = mobilityScore + materialScore + checkScore;

	/*
	if(b.ourSide == white) {
		if(b.ply < 10 && !(b.occupancy[b.ourSide] & b.pieces[queen] & 0x8)) {
			score -= 50000;
		}
	}
	else {
		if(b.ply < 10 && !(b.occupancy[b.ourSide] & b.pieces[queen] & 0x800000000000000ULL)) {
			score -= 50000;
		}
	}*/

	/*
	uint64_t ourPawns = b.occupancy[b.sideToPlay] & b.pieces[pawn];
	uint64_t theirPawns = b.occupancy[!b.sideToPlay] & b.pieces[pawn];
	for(uint64_t file = 0; file < 8; file++) {
		//double pawn
		if(popcount(ourPawns & files[file]) > 1) {
			score -= 30;
		}
		if(popcount(theirPawns & files[file]) > 1) {
			score += 30;
		}
	}*/

	return score * player;
}
