#ifndef TT_H
#define TT_H

#include <unordered_map>
#include "../bitboard/bitboard.h"
#include "../eval/evaluator.h"

enum NodeType {
	TT_ALPHA,
	TT_BETA,
	TT_EXACT,
	TT_INVALID
};

struct NodeInfo {
	int type;
	int score;
	int depth;
	Move bestMove;
};

const NodeInfo Invalid = {TT_INVALID, 0, 0, Move()};

extern std::unordered_map<uint64_t, NodeInfo> tt;
extern std::unordered_map<uint64_t, std::vector<Move>> movesTable;

int probeTT(const Bitboard& b, int alpha, int beta, int depth, NodeInfo &inf);
int negamax(Bitboard &b, int alpha, int beta, int depth, int colour, std::stack<Bitboard> &moveStack, MoveEvaluator& eval);

bool isValidTT(const Bitboard& b, const Move& m);

int search(Bitboard b, std::stack<Bitboard> &moveStack, MoveEvaluator &eval, Move& bestMove);

#endif