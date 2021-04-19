#include "engine.h"

Engine::Engine()
{
	bitboard = Bitboard(initialPosition);
	bitboard.sideToPlay = sides::black;
	out.open("debug.txt", std::ios::out);
	out.setf(std::ios::unitbuf);
	shouldQuit = false;

	gen = std::mt19937(rd());
}

void Engine::debugMove(Move m)
{
	out << "NEW MOVE: " << (uint32_t)m.movedPiece << " "
		<< bitboard.num2squareStr(m.squareFrom)
		<< bitboard.num2squareStr(m.squareTo) << "\n";
	out << bitboard.chessBoard2str() << "\n\n";
}

void Engine::selectAndPlayMove(std::vector<Move> &moves, bool castle)
{
	if (!moves.size())
	{
		return;
	}
	Move m;
	if(castle) {
		m = moves[0];
	}
	else {
		std::uniform_int_distribution<> dis(0, std::distance(moves.begin(), moves.end()) - 1);
		auto ind = moves.begin();
		std::advance(ind, dis(gen));
		m = *ind;
	}
	out << "ATTENTION: " << m.movedPiece << "\n";
	debugMove(m);
	bitboard.makeMove(m, moveStack);
	std::cout << "move " << bitboard.num2squareStr(m.squareFrom)
			  << bitboard.num2squareStr(m.squareTo) << "\n";
	switch (m.promotion)
	{
	case pieces::bishop:
		std::cout << 'b';
		break;

	case pieces::knight:
		std::cout << 'n';
		break;

	case pieces::rook:
		std::cout << 'r';
		break;

	case pieces::queen:
		std::cout << 'q';
		break;

	default:
		break;
	}
	std::cout << '\n';
}

void Engine::processCommand(Command c)
{
	switch (c.commandType)
	{
	case CommandType::null:
		break;
	case CommandType::new_game:
		moveStack = std::stack<Bitboard>();
		bitboard = Bitboard(initialPosition);
		bitboard.sideToPlay = sides::white;
		bitboard.ourSide = sides::black;
		go = true;
		break;
	case CommandType::set_white:
		bitboard.ourSide = sides::white;
		break;
	case CommandType::set_black:
		bitboard.ourSide = sides::black;
		break;
	case CommandType::protover:
		std::cout << "feature sigint=0\n";
		std::cout << "feature san=0\n";
		std::cout << "feature myname=\"" << name << "\"\n";
		std::cout << "feature variants=\"3check\"\n";
		std::cout << "feature done=1\n";
		std::cout
			<< "tellopponent Bun venit in engine. Modulul de "
			   "trash-talking nu este inca implementat, dar cand va fi, acest "
			   "engine va fi mai toxic decat intreaga comunitate de "
			   "League of Legends combinata\n";
		break;
	case CommandType::quit:
		shouldQuit = true;
		break;
	case CommandType::go:
		go = true;
		if ((bitboard.sideToPlay == bitboard.ourSide) && go)
		{
			bool castle;
			std::vector<Move> moves = bitboard.generateMoves(castle);
			selectAndPlayMove(moves, castle);
		}
		break;
	case CommandType::force:
		go = false;
		break;
	case CommandType::move:
		debugMove(command2move(c));
		bitboard.makeMove(command2move(c), moveStack);
		if (rand() < RAND_MAX / 1000)
		{
			std::cout << "resign\n";
			return;
		}
		if ((bitboard.sideToPlay == bitboard.ourSide) && go)
		{
			bool castle;
			std::vector<Move> moves = bitboard.generateMoves(castle);
			selectAndPlayMove(moves, castle);
		}
		break;
	};
}

Move Engine::command2move(Command c)
{
	Move m;
	m.squareFrom = c.moveFrom;
	m.squareTo = c.moveTo;
	m.movedPiece = pieces::empty;
	m.sideThatMoved = bitboard.sideToPlay;
	m.capturedPiece = pieces::empty;
	m.newEnPassant = 0;
	m.newCastlingFlags = bitboard.castleFlags;
	m.promotion = pieces::empty;
	for (int i = 0; i < 6; i++)
	{
		if (bitboard.occupancy[!m.sideThatMoved] & bitboard.pieces[i] &
			(1ULL << c.moveTo))
		{
			m.capturedPiece = i;
			break;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (bitboard.occupancy[m.sideThatMoved] & bitboard.pieces[i] &
			(1ULL << c.moveFrom))
		{
			m.movedPiece = i;
			break;
		}
	}
	if (m.movedPiece == pieces::pawn)
	{
		if (m.sideThatMoved == white)
		{
			if (m.squareFrom >= Bitboard::squareString2num("a2") &&
				m.squareFrom <= Bitboard::squareString2num("h2") &&
				m.squareTo == (m.squareFrom + 16))
			{
				m.newEnPassant = m.squareFrom + 8;
			}
		}
		else
		{
			if (m.squareFrom >= Bitboard::squareString2num("a7") &&
				m.squareFrom <= Bitboard::squareString2num("h7") &&
				m.squareTo == (m.squareFrom - 16))
			{
				m.newEnPassant = m.squareFrom - 8;
			}
		}
	}
	if (((1ULL << c.moveTo) == bitboard.enPassant) &&
		m.movedPiece == pieces::pawn)
	{
		m.moveFlags |= (1 << specialMoves::en_passant);
	}
	else if (c.promotion != pieces::empty)
	{
		m.moveFlags |= (1 << promotion);
		switch (c.promotion)
		{
		case 'b':
			m.promotion = bishop;
			break;
		case 'n':
			m.promotion = knight;
			break;
		case 'r':
			m.promotion = rook;
			break;
		case 'q':
			m.promotion = queen;
			break;
		}
	}
	else
	{
		if (m.sideThatMoved == sides::white)
		{
			if (m.movedPiece == pieces::king &&
				m.squareFrom == Bitboard::squareString2num("e1"))
			{
				if (m.squareTo == Bitboard::squareString2num("c1"))
				{
					m.moveFlags |= 1 << white_castle_long;
					m.newCastlingFlags = bitboard.castleFlags & ~(1 << white_castle_long);
				}
				else if (m.squareTo == Bitboard::squareString2num("g1"))
				{
					m.moveFlags |= 1 << white_castle_short;
					m.newCastlingFlags =
						bitboard.castleFlags & ~(1 << white_castle_short);
				}
			}
		}
		else
		{
			if (m.movedPiece == pieces::king &&
				m.squareFrom == Bitboard::squareString2num("e8"))
			{
				if (m.squareTo == Bitboard::squareString2num("c8"))
				{
					m.moveFlags |= 1 << black_castle_long;
					m.newCastlingFlags = bitboard.castleFlags & ~(1 << black_castle_long);
				}
				else if (m.squareTo == Bitboard::squareString2num("g8"))
				{
					m.moveFlags |= 1 << black_castle_short;
					m.newCastlingFlags =
						bitboard.castleFlags & ~(1 << black_castle_short);
				}
			}
		}
	}
	return m;
}

void Engine::testBoard()
{
	std::ifstream f("perft_ref.txt");
	std::string in;
	int n;
	char trash;
	f >> n;
	bool allGood = true;
	std::stack<Bitboard> moveStack;
	for (int i = 1; i <= n; i++)
	{
		bool goodTest = true;
		std::string fen;
		std::getline(f, fen);
		std::getline(f, fen);
		std::cerr << "FEN: " << fen << '\n';
		int numDepths;
		f >> numDepths;
		bitboard = Bitboard(fen);
		moveStack = std::stack<Bitboard>();
		for (int j = 1; j <= numDepths; j++)
		{
			//std::cerr << "------------BOARD--------------\n" << bitboard.chessBoard2str();
			//std::cerr << "-------------------------------\n";
			uint64_t perftRes = bitboard.perft(j, moveStack);
			uint64_t correctRes;
			f >> correctRes;
			if (perftRes != correctRes)
			{
				allGood = false;
				goodTest = false;
				std::cout << "On test " << i << ", perft(" << j
						  << ") does not give the correct result. ";
				std::cout << "Expected " << correctRes << " but got " << perftRes
						  << "\n";
			}
			else {
				std::cout << "Depth " << j << ": " << perftRes << " correct result\n";
			}
		}
		if (goodTest)
		{
			std::cout << "Test " << i << " passed\n";
		}
	}
	if (allGood)
	{
		std::cout << "All tests passed!\n";
	}
}