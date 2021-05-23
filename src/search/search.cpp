#include "tt.h"
#include <algorithm>

#define MAX_DEPTH 5

std::unordered_map<uint64_t, NodeInfo> tt;

bool isValidTT(const Bitboard &b, const Move &m)
{
	return true;
	if (b.sideToPlay != m.sideThatMoved)
	{
		return false;
	}

	if (!((1ULL << m.squareFrom) & b.occupancy[m.sideThatMoved] & b.pieces[m.movedPiece]))
	{
		return false;
	}

	if ((1ULL << m.squareTo) & b.occupancy[m.sideThatMoved])
	{
		return false;
	}

	if (m.capturedPiece == empty)
	{
		if ((1ULL << m.squareTo) & b.occupancy[!m.sideThatMoved])
		{
			return false;
		}
	}
	else
	{
		if (!((1ULL << m.squareTo) & b.occupancy[!m.sideThatMoved] & b.pieces[m.capturedPiece]))
		{
			return false;
		}
	}

	if (m.movedPiece == pawn)
	{
		if (m.capturedPiece == empty && (b.occupancy[!m.sideThatMoved] & (1ULL << m.squareTo)))
		{
			return false;
		}
	}

	if (m.moveFlags == 0)
	{
		return true;
	}

	int moveFlag = __builtin_ffsl(m.moveFlags) - 1;
	switch (moveFlag)
	{
	case white_castle_short:
		if (!(b.castleFlags & (1 << white_castle_short)))
		{
			return false;
		}
		break;

	case white_castle_long:
		if (!(b.castleFlags & (1 << white_castle_long)))
		{
			return false;
		}
		break;

	case black_castle_short:
		if (!(b.castleFlags & (1 << black_castle_short)))
		{
			return false;
		}
		break;

	case black_castle_long:
		if (!(b.castleFlags & (1 << black_castle_long)))
		{
			return false;
		}
		break;

	case en_passant:
		if (b.enPassant != (1ULL << m.squareTo))
		{
			return false;
		}

	default:
		break;
	}

	return true;
}

int probeTT(const Bitboard &b, int alpha, int beta, int depth, NodeInfo &inf)
{
	if (tt.find(b.hash) == tt.end())
	{
		inf = Invalid;
		return -1;
	}
	NodeInfo info = tt[b.hash];
	Move &m = info.bestMove;
	if (info.depth >= depth)
	{
		if (!isValidTT(b, info.bestMove))
		{
			inf = Invalid;
			return -1;
		}
		if (info.type == TT_EXACT)
		{
			inf = info;
			return inf.score;
		}
		if (info.type == TT_ALPHA && info.score <= alpha)
		{
			inf = info;
			return alpha;
		}
		if (info.type == TT_BETA && info.score >= beta)
		{
			inf = info;
			return beta;
		}
	}
	inf = Invalid;
	return -1;
}

int qsearch(Bitboard b, int alpha, int beta, int colour, std::stack<Bitboard> moveStack, MoveEvaluator &eval)
{
	bool castle;
	std::vector<Move> moves = b.generateMoves(castle);
	if (moves.size() == 0)
	{
		return -INT32_MAX * colour;
	}

	int result = eval.eval(b, colour);

	if (result >= beta)
	{
		return beta;
	}
	if (alpha < result)
	{
		return alpha;
	}

	for (Move &m : moves)
	{
		if (m.capturedPiece != pieces::empty)
		{
			b.makeMove(m, moveStack);
			result = -qsearch(b, -beta, -alpha, -colour, moveStack, eval);
			b.unmakeMove(m, moveStack);

			if (result >= beta)
			{
				return beta;
			}
			if (alpha < result)
			{
				alpha = result;
			}
		}
	}
	return alpha;
}

int negamax(Bitboard &b, int alpha, int beta, int depth, int colour, std::stack<Bitboard> &moveStack, MoveEvaluator &eval)
{
	int nodeType = TT_ALPHA;
	bool betaCutoff = false;

	NodeInfo info;

	probeTT(b, alpha, beta, depth, info);
	if (info.type != TT_INVALID)
	{
		return info.score;
	}

	bool castle;
	std::vector<Move> moves = b.generateMoves(castle);
	if (moves.size() == 0)
	{
		return -INT32_MAX * colour;
	}

	if (depth <= 0)
	{
		return eval.eval(b, colour);
	}

	int result;
	Move bestMove = moves[0];
	for (Move &m : moves)
	{
		b.makeMove(m, moveStack);
		result = -negamax(b, -beta, -alpha, depth - 1, -colour, moveStack, eval);
		b.unmakeMove(m, moveStack);

		if (result > alpha)
		{
			bestMove = m;
			if (result >= beta)
			{
				alpha = beta;
				nodeType = TT_BETA;
				betaCutoff = true;
				break;
			}
			alpha = result;

			info.bestMove = bestMove;
			info.depth = depth;
			info.score = result;
			info.type = TT_ALPHA;

			tt[b.hash] = info;
		}
	}

	info.bestMove = bestMove;
	info.depth = depth;
	info.score = result;
	if(!betaCutoff) {
		info.type = TT_EXACT;
	}

	tt[b.hash] = info;

	return alpha;
}

int MTDf(Bitboard b, int f, int depth, std::stack<Bitboard> &moveStack, MoveEvaluator &eval)
{
	int32_t bounds[2] = {-INT32_MAX, INT32_MAX};
	do
	{
		int beta = f + (f == bounds[0]);
		f = negamax(b, beta - 1, beta, depth, b.sideToPlay == b.ourSide ? 1 : -1, moveStack, eval);
		bounds[f < beta] = f;
	} while (bounds[0] < bounds[1]);
	return f;
}

int search_root(Bitboard b, int alpha, int beta, int depth, int colour, std::stack<Bitboard> moveStack, MoveEvaluator &eval, Move &bestMove)
{
	bool castle;
	std::vector<Move> moves = b.generateMoves(castle);
	NodeInfo info;

	for (Move &m : moves)
	{
		b.makeMove(m, moveStack);
		int val = -negamax(b, -beta, -alpha, depth - 1, -colour, moveStack, eval);
		b.unmakeMove(m, moveStack);

		if (val > alpha)
		{
			bestMove = m;
			if (val >= beta)
			{
				info.bestMove = bestMove;
				info.depth = depth;
				info.score = beta;
				info.type = TT_BETA;

				tt[b.hash] = info;

				return beta;
			}

			alpha = val;
			info.bestMove = bestMove;
			info.depth = depth;
			info.score = val;
			info.type = TT_ALPHA;

			tt[b.hash] = info;
		}
	}

	info.bestMove = bestMove;
	info.depth = depth;
	info.score = alpha;
	info.type = TT_EXACT;

	tt[b.hash] = info;

	return alpha;
}

int search(Bitboard b, std::stack<Bitboard> &moveStack, MoveEvaluator &eval, Move &bestMove)
{
	int res;
	int startCol;
	res = search_root(b, -INT32_MAX, INT32_MAX, 1, b.sideToPlay == b.ourSide ? -1 : 1, moveStack, eval, bestMove);
	for (int d = 2; d <= MAX_DEPTH; d++)
	{
		if(d % 2 == 0) {
			startCol = b.sideToPlay == b.ourSide ? 1 : -1;
		}
		else {
			startCol = b.sideToPlay == b.ourSide ? -1 : 1;
		}
		res = search_root(b, -INT32_MAX, INT32_MAX, d, startCol, moveStack, eval, bestMove);
	}
	return res;
}