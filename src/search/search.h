#include "../eval/evaluator.h"
#include "tt.h"

class MoveSearch {
	public:
	MoveSearch(MoveEvaluator evaluator);

	MoveEvaluator eval;
};