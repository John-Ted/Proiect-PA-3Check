#ifndef EVAL_H
#define EVAL_H

#include "../bitboard/bitboard.h"
#include <unordered_map>

class MoveEvaluator {
	public:
	int32_t eval(Bitboard &b, int8_t player);

	void init(std::string filename);
	private:
	int checks[4] = {0, 8, 50, 100000};
	int materialScores[5] = {1, 7, 12, 12, 20};
};

#endif