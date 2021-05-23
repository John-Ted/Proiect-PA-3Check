#include "bitboard.h"

uint64_t Bitboard::S1(uint64_t b) { return b >> 8; }
uint64_t Bitboard::N1(uint64_t b) { return b << 8; }
uint64_t Bitboard::E1(uint64_t b) { return (b & constants::notHFile) << 1ULL; }
uint64_t Bitboard::NE1(uint64_t b) { return (b & constants::notHFile) << 9ULL; }
uint64_t Bitboard::SE1(uint64_t b) { return (b & constants::notHFile) >> 7ULL; }
uint64_t Bitboard::W1(uint64_t b) { return (b & constants::notAFile) >> 1ULL; }
uint64_t Bitboard::SW1(uint64_t b) { return (b & constants::notAFile) >> 9ULL; }
uint64_t Bitboard::NW1(uint64_t b) { return (b & constants::notAFile) << 7ULL; }

uint64_t Bitboard::NNE(uint64_t b) { return (b & constants::notHFile) << 17; }
uint64_t Bitboard::NEE(uint64_t b) { return (b & constants::notGHFile) << 10; }
uint64_t Bitboard::SEE(uint64_t b) { return (b & constants::notGHFile) >> 6; }
uint64_t Bitboard::SSE(uint64_t b) { return (b & constants::notHFile) >> 15; }
uint64_t Bitboard::NNW(uint64_t b) { return (b & constants::notAFile) << 15; }
uint64_t Bitboard::NWW(uint64_t b) { return (b & constants::notABFile) << 6; }
uint64_t Bitboard::SWW(uint64_t b) { return (b & constants::notABFile) >> 10; }
uint64_t Bitboard::SSW(uint64_t b) { return (b & constants::notAFile) >> 17; }

uint64_t Bitboard::rankMask(uint64_t sq) { return 0xffULL << (sq & 56ULL); }

uint64_t Bitboard::fileMask(uint64_t sq) { return 0x0101010101010101ULL << (sq & 7ULL); }

uint64_t Bitboard::diagonalMask(uint64_t sq)
{
	const uint64_t maindia = 0x8040201008040201ULL;
	uint64_t diag = 8ULL * (sq & 7ULL) - (sq & 56ULL);
	uint64_t nort = -diag & (diag >> 31ULL);
	uint64_t sout = diag & (-diag >> 31ULL);
	return (maindia >> sout) << nort;
}

uint64_t Bitboard::antiDiagMask(uint64_t sq)
{
	const uint64_t maindia = 0x0102040810204080ULL;
	uint64_t diag = 56ULL - 8ULL * (sq & 7ULL) - (sq & 56ULL);
	uint64_t nort = -diag & (diag >> 31ULL);
	uint64_t sout = diag & (-diag >> 31ULL);
	return (maindia >> sout) << nort;
}

uint64_t Bitboard::rankMaskEx(int sq) { return (1ULL << sq) ^ rankMask(sq); }
uint64_t Bitboard::fileMaskEx(int sq) { return (1ULL << sq) ^ fileMask(sq); }
uint64_t Bitboard::diagonalMaskEx(int sq) { return (1ULL << sq) ^ diagonalMask(sq); }
uint64_t Bitboard::antiDiagMaskEx(int sq) { return (1ULL << sq) ^ antiDiagMask(sq); }

uint64_t Bitboard::rookMask(int sq) { return rankMask(sq) | fileMask(sq); }
uint64_t Bitboard::bishopMask(int sq) { return diagonalMask(sq) | antiDiagMask(sq); }

uint64_t Bitboard::rookMaskEx(int sq) { return rankMask(sq) ^ fileMask(sq); }
uint64_t Bitboard::bishopMaskEx(int sq) { return diagonalMask(sq) ^ antiDiagMask(sq); }

uint64_t Bitboard::queenMask(int sq) { return rookMask(sq) | bishopMask(sq); }
uint64_t Bitboard::queenMaskEx(int sq) { return rookMask(sq) ^ bishopMask(sq); }

uint64_t Bitboard::kingAttacks(uint64_t kingSet)
{
	uint64_t attacks = E1(kingSet) | W1(kingSet);
	kingSet |= attacks;
	attacks |= N1(kingSet) | S1(kingSet);
	return attacks;
}

uint64_t Bitboard::knightAttacks(uint64_t knights)
{
	uint64_t l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7fULL;
	uint64_t l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3fULL;
	uint64_t r1 = (knights << 1) & 0xfefefefefefefefeULL;
	uint64_t r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfcULL;
	uint64_t h1 = l1 | r1;
	uint64_t h2 = l2 | r2;
	return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t Bitboard::squareString2mask(std::string square)
{
	int file = square[0] - 'a';
	int rank = square[1] - '1';
	return 1ULL << (rank * 8 + file);
}

uint64_t Bitboard::squareString2num(std::string square)
{
	int file = square[0] - 'a';
	int rank = square[1] - '1';
	return rank * 8 + file;
}

Bitboard::Bitboard() {}

Bitboard::Bitboard(std::string FENstring)
{
	checks[0] = checks[1] = 0;
	ply = 0;
	occupancy[0] = occupancy[1] = 0;
	for (int i = 0; i < 6; i++)
	{
		pieces[i] = 0;
	}

	std::istringstream stringIn(FENstring);

	std::string board;
	stringIn >> board;

	uint64_t rank = 7, file = 0;

	hash = constants::checkHash[0][0] ^ constants::checkHash[1][0];

	for (char c : board)
	{
		if (c == '/')
		{
			file = 0;
			rank--;
		}
		else
		{
			if (std::isdigit(c))
			{
				file += c - '0';
			}
			else
			{
				uint64_t sq = 8 * rank + file;
				uint64_t side;
				char piece = c;
				if (std::isupper(piece))
				{
					occupancy[sides::white] |= 1ULL << sq;
					side = sides::white;
					piece = std::tolower(piece);
				}
				else
				{
					occupancy[sides::black] |= 1ULL << sq;
					side = sides::black;
				}
				switch (piece)
				{
				case 'p':
					pieces[pieces::pawn] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::pawn][sq];
					break;
				case 'n':
					pieces[pieces::knight] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::knight][sq];
					break;
				case 'b':
					pieces[pieces::bishop] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::bishop][sq];
					break;
				case 'r':
					pieces[pieces::rook] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::rook][sq];
					break;
				case 'q':
					pieces[pieces::queen] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::queen][sq];
					break;
				case 'k':
					pieces[pieces::king] |= 1ULL << sq;
					hash ^= constants::pieceHash[side][pieces::king][sq];
					break;
				}
				file++;
			}
		}
	}

	std::string in;
	stringIn >> in;

	if (in[0] == 'w')
	{
		sideToPlay = 0;
	}
	else
	{
		sideToPlay = 1;
		hash ^= constants::sideHash;
	}

	castleFlags = 0;
	stringIn >> in;
	for (int i = 0; i < in.length(); i++)
	{
		if (in[i] == 'K')
		{
			castleFlags |= 1 << white_castle_short;
		}
		else if (in[i] == 'Q')
		{
			castleFlags |= 1 << white_castle_long;
		}
		else if (in[i] == 'k')
		{
			castleFlags |= 1 << black_castle_short;
		}
		else if (in[i] == 'q')
		{
			castleFlags |= 1 << black_castle_long;
		}
	}

	hash ^= constants::castleHash[castleFlags];

	stringIn >> in;
	if (in != "-")
	{
		enPassant = squareString2mask(in);
		hash ^= constants::epHash[squareString2num(in)];
	}
	else
	{
		enPassant = 0;
	}
}

void Bitboard::initConstants()
{
	for (int sq = 0; sq < 64; sq++)
	{
		constants::kingAttacks[sq] = kingAttacks(1ULL << sq);
		constants::knightAttacks[sq] = knightAttacks(1ULL << sq);
	}

	std::ifstream rayIn("rays.bin", std::ios::in | std::ios::binary);

	for (int sq = 0; sq < 64; sq++)
	{
		rayIn.read((char *)constants::rayAttacks[sq], 8 * sizeof(uint64_t));
	}

	rayIn.close();

	constants::castlingSquares[0] = 0xEULL;
	constants::castlingSquares[1] = 0x60ULL;
	constants::castlingSquares[2] = 0xE00000000000000ULL;
	constants::castlingSquares[3] = 0x6000000000000000ULL;

	std::ifstream magicIn("magic.bin", std::ios::in | std::ios::binary);

	magicIn.read((char *)constants::rookTable, 0x19000 * sizeof(uint64_t));

	for (int i = 0; i < 64; i++)
	{
		magicIn.read((char *)&(constants::rookMagics[i].mask), sizeof(constants::rookMagics[i].mask));
		magicIn.read((char *)&(constants::rookMagics[i].magic), sizeof(constants::rookMagics[i].magic));
		uint64_t offset;
		magicIn.read((char *)&offset, sizeof(offset));
		constants::rookMagics[i].attacks = constants::rookTable + offset;
		magicIn.read((char *)&(constants::rookMagics[i].shift), sizeof(constants::rookMagics[i].shift));
	}

	magicIn.read((char *)constants::bishopTable, 0x1480 * sizeof(uint64_t));

	for (int i = 0; i < 64; i++)
	{
		magicIn.read((char *)&(constants::bishopMagics[i].mask), sizeof(constants::bishopMagics[i].mask));
		magicIn.read((char *)&(constants::bishopMagics[i].magic), sizeof(constants::bishopMagics[i].magic));
		uint64_t offset;
		magicIn.read((char *)&offset, sizeof(offset));
		constants::bishopMagics[i].attacks = constants::bishopTable + offset;
		magicIn.read((char *)&(constants::bishopMagics[i].shift), sizeof(constants::bishopMagics[i].shift));
	}

	magicIn.close();

	std::ifstream hashIn("hash.bin", std::ios::in | std::ios::binary);

	for (int side = 0; side <= 1; side++)
	{
		for (int piece = 0; piece < 6; piece++)
		{
			for (int square = 0; square < 64; square++)
			{
				hashIn.read((char *)&constants::pieceHash[side][piece][square], sizeof(uint64_t));
			}
		}
	}

	for (int sq = 0; sq < 64; sq++)
	{
		hashIn.read((char *)&constants::epHash[sq], sizeof(uint64_t));
	}

	for (int c = 0; c < 16; c++)
	{
		hashIn.read((char *)&constants::castleHash[c], sizeof(uint64_t));
	}

	hashIn.read((char *)&constants::sideHash, sizeof(uint64_t));

	for (int s = 0; s <= 1; s++)
	{
		for (int check = 0; check <= 3; check++)
		{
			hashIn.read((char *)&constants::checkHash[s][check], sizeof(uint64_t));
		}
	}

	hashIn.close();
}

std::string Bitboard::bitboard2str(uint64_t bitboard, int highlight, char character)
{
	std::string result;
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			uint64_t sq = i * 8 + j;
			if (sq == highlight)
			{
				result += character;
			}
			else if (bitboard & (1ULL << sq))
			{
				result += "#";
			}
			else
			{
				result += ".";
			}
		}
		result += "\n";
	}
	return result;
}

uint64_t Bitboard::wPushPawns1(uint64_t pawns)
{
	return N1(pawns) & ~(occupancy[0] | occupancy[1]);
}

uint64_t Bitboard::wPushPawns2(uint64_t pawns)
{
	const uint64_t rank4 = 0x00000000FF000000ULL;
	uint64_t singlePushes = wPushPawns1(pawns);
	return N1(singlePushes) & ~(occupancy[0] | occupancy[1]) & rank4;
}

uint64_t Bitboard::bPushPawns1(uint64_t bpawns)
{
	return S1(bpawns) & ~(occupancy[0] | occupancy[1]);
}

uint64_t Bitboard::bPushPawns2(uint64_t bpawns)
{
	const uint64_t rank5 = 0x000000FF00000000ULL;
	uint64_t singlePushs = bPushPawns1(bpawns);
	return S1(singlePushs) & ~(occupancy[0] | occupancy[1]) & rank5;
}

uint64_t Bitboard::wPawnEastAttacks(uint64_t wpawns) { return NE1(wpawns); }
uint64_t Bitboard::wPawnWestAttacks(uint64_t wpawns) { return NW1(wpawns); }

uint64_t Bitboard::bPawnEastAttacks(uint64_t bpawns) { return SE1(bpawns); }
uint64_t Bitboard::bPawnWestAttacks(uint64_t bpawns) { return SW1(bpawns); }

uint64_t Bitboard::getPositiveRayAttacks(int direction, uint64_t square)
{
	uint64_t attacks = constants::rayAttacks[square][direction];
	uint64_t blockers = attacks & (occupancy[0] | occupancy[1]);
	square = __builtin_ffsl(blockers | 0x8000000000000000ULL) - 1;
	return attacks ^ constants::rayAttacks[square][direction];
}

uint64_t Bitboard::getNegativeRayAttacks(int direction, uint64_t square)
{
	uint64_t attacks = constants::rayAttacks[square][direction];
	uint64_t blockers = attacks & (occupancy[0] | occupancy[1]);
	square = __builtin_clzl(blockers | 1ULL) ^ 63;
	return attacks ^ constants::rayAttacks[square][direction];
}

template <bool excludeOwnSquares>
uint64_t Bitboard::getBishopAttacks(uint64_t square, uint8_t side)
{
	/*
	uint64_t attacks = getPositiveRayAttacks(directions::NoWe, square);
	attacks |= getPositiveRayAttacks(directions::NoEa, square);
	attacks |= getNegativeRayAttacks(directions::SoWe, square);
	attacks |= getNegativeRayAttacks(directions::SoEa, square);*/
	uint32_t index = constants::bishopMagics[square].index(occupancy[white] | occupancy[black]);
	uint64_t attacks = constants::bishopMagics[square].attacks[index];
	if constexpr (excludeOwnSquares)
	{
		return attacks & ~occupancy[side];
	}
	else
	{
		return attacks;
	}
}

template <bool excludeOwnSquares>
uint64_t Bitboard::getRookAttacks(uint64_t square, uint8_t side)
{
	/*
	uint64_t attacks = getPositiveRayAttacks(directions::Nort, square);
	attacks |= getPositiveRayAttacks(directions::East, square);
	attacks |= getNegativeRayAttacks(directions::Sout, square);
	attacks |= getNegativeRayAttacks(directions::West, square);*/
	uint32_t index = constants::rookMagics[square].index(occupancy[white] | occupancy[black]);
	uint64_t attacks = constants::rookMagics[square].attacks[index];
	if constexpr (excludeOwnSquares)
	{
		return attacks & ~occupancy[side];
	}
	else
	{
		return attacks;
	}
}

template <bool excludeOwnSquares>
uint64_t Bitboard::getQueenAttacks(uint64_t square, uint8_t side)
{
	return getRookAttacks<excludeOwnSquares>(square, side) | getBishopAttacks<excludeOwnSquares>(square, side);
}

uint64_t Bitboard::getAllAttacks(uint8_t side)
{
	uint64_t attacks = 0;
	uint64_t sideOccupancy = occupancy[side];

	if (side == sides::white)
	{
		attacks |= wPawnWestAttacks(sideOccupancy & pieces[pieces::pawn]);
		attacks |= wPawnEastAttacks(sideOccupancy & pieces[pieces::pawn]);
	}
	else
	{
		attacks |= bPawnWestAttacks(sideOccupancy & pieces[pieces::pawn]);
		attacks |= bPawnEastAttacks(sideOccupancy & pieces[pieces::pawn]);
	}

	attacks |= knightAttacks(sideOccupancy & pieces[pieces::knight]);
	attacks |= kingAttacks(sideOccupancy & pieces[pieces::king]);

	uint64_t rooks = sideOccupancy & pieces[pieces::rook];
	while (rooks)
	{
		uint64_t nextRook = __builtin_ffsl(rooks) - 1;
		attacks |= getRookAttacks(nextRook, side);
		rooks &= rooks - 1; //~(1ULL << nextRook);
	}

	uint64_t bishops = sideOccupancy & pieces[pieces::bishop];
	while (bishops)
	{
		uint64_t nextBishop = __builtin_ffsl(bishops) - 1;
		attacks |= getBishopAttacks(nextBishop, side);
		bishops &= bishops - 1; //~(1ULL << nextBishop);
	}

	uint64_t queens = sideOccupancy & pieces[pieces::queen];
	while (queens)
	{
		uint64_t nextQueen = __builtin_ffsl(queens) - 1;
		attacks |= getQueenAttacks(nextQueen, side);
		queens &= queens - 1; //~(1ULL << nextQueen);
	}

	return attacks & ~sideOccupancy;
}

bool Bitboard::isSquareAttacked(uint8_t square, uint8_t side)
{
	uint64_t sq = 1ULL << square;
	uint64_t sideOccupancy = occupancy[side];
	if (side == sides::white)
	{
		uint64_t wAttacks = wPawnWestAttacks(sideOccupancy & pieces[pieces::pawn]);
		uint64_t eAttacks = wPawnEastAttacks(sideOccupancy & pieces[pieces::pawn]);
		if (wAttacks & sq || eAttacks & sq)
		{
			return true;
		}
	}
	else
	{
		uint64_t wAttacks = bPawnWestAttacks(sideOccupancy & pieces[pieces::pawn]);
		uint64_t eAttacks = bPawnEastAttacks(sideOccupancy & pieces[pieces::pawn]);
		if (wAttacks & sq || eAttacks & sq)
		{
			return true;
		}
	}
	uint64_t attacks = knightAttacks(sideOccupancy & pieces[pieces::knight]);
	if (attacks & sq)
	{
		return true;
	}
	attacks = getBishopAttacks<false>(square, side);
	if (attacks & (sideOccupancy & (pieces[bishop] | pieces[queen])))
	{
		return true;
	}
	attacks = getRookAttacks<false>(square, side);
	if (attacks & (sideOccupancy & (pieces[rook] | pieces[queen])))
	{
		return true;
	}
	attacks = constants::kingAttacks[square];
	if (attacks & (sideOccupancy & pieces[king]))
	{
		return true;
	}
	return false;
}

void Bitboard::addPiece(uint32_t pieceType, uint32_t side, uint8_t square)
{
	occupancy[side] |= 1ULL << square;
	pieces[pieceType] |= 1ULL << square;
}

void Bitboard::removePiece(uint32_t pieceType, uint32_t side, uint8_t square)
{
	occupancy[side] &= ~(1ULL << square);
	pieces[pieceType] &= ~(1ULL << square);
}

void Bitboard::movePiece(uint32_t pieceType, uint32_t side, uint8_t squareFrom, uint8_t squareTo)
{
	removePiece(pieceType, side, squareFrom);
	addPiece(pieceType, side, squareTo);
}

std::string Bitboard::chessBoard2str()
{
	std::string s;
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			uint8_t sq = i * 8 + j;
			if (pieces[pieces::pawn] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "P" : "p";
			}
			else if (pieces[pieces::knight] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "N" : "n";
			}
			else if (pieces[pieces::bishop] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "B" : "b";
			}
			else if (pieces[pieces::rook] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "R" : "r";
			}
			else if (pieces[pieces::queen] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "Q" : "q";
			}
			else if (pieces[pieces::king] & (1ULL << sq))
			{
				s += (occupancy[0] & (1ULL << sq)) ? "K" : "k";
			}
			else
			{
				s += ".";
			}
		}
		s += "\n";
	}
	return s;
}

void Bitboard::makeMove(const Move &move, std::stack<Bitboard> &moveStack)
{
	moveStack.push(*this);
	if (move.capturedPiece != pieces::empty)
	{
		hash ^= constants::pieceHash[!move.sideThatMoved][move.capturedPiece][move.squareTo];
		removePiece(move.capturedPiece, !move.sideThatMoved, move.squareTo);
	}

	movePiece(move.movedPiece, move.sideThatMoved,
			  move.squareFrom, move.squareTo);

	ply++;

	int opponentKingSquare = __builtin_ffsll(occupancy[!sideToPlay] & pieces[king]) - 1;

	if (isSquareAttacked(opponentKingSquare, sideToPlay))
	{
		hash ^= constants::checkHash[sideToPlay][checks[sideToPlay]];
		checks[sideToPlay] = std::min(checks[sideToPlay] + 1, 3);
		hash ^= constants::checkHash[sideToPlay][checks[sideToPlay]];
	}

	hash ^= constants::pieceHash[sideToPlay][move.movedPiece][move.squareFrom];
	hash ^= constants::pieceHash[sideToPlay][move.movedPiece][move.squareTo];

	sideToPlay = !move.sideThatMoved;
	hash ^= constants::sideHash;

	hash ^= constants::epHash[__builtin_ffsll(enPassant) - 1];

	hash ^= constants::castleHash[castleFlags];
	castleFlags = move.newCastlingFlags;
	hash ^= constants::castleHash[castleFlags];

	if (move.newEnPassant != 0)
	{
		hash ^= constants::epHash[move.newEnPassant];
		this->enPassant = 1ULL << move.newEnPassant;
	}
	else
	{
		this->enPassant = 0ULL;
	}

	if (move.moveFlags != 0)
	{
		uint64_t moveFlag = __builtin_ffsl(move.moveFlags) - 1;
		switch (moveFlag)
		{
		case specialMoves::white_castle_long:
			movePiece(pieces::rook, move.sideThatMoved,
					  squareString2num("a1"), squareString2num("d1"));
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("a1")];
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("d1")];
			break;
		case specialMoves::white_castle_short:
			movePiece(pieces::rook, move.sideThatMoved,
					  squareString2num("h1"), squareString2num("f1"));
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("h1")];
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("f1")];
			break;
		case specialMoves::black_castle_long:
			movePiece(pieces::rook, move.sideThatMoved,
					  squareString2num("a8"), squareString2num("d8"));
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("a8")];
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("d8")];
			break;
		case specialMoves::black_castle_short:
			movePiece(pieces::rook, move.sideThatMoved,
					  squareString2num("h8"), squareString2num("f8"));
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("h8")];
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::rook][squareString2num("f8")];
			break;
		case specialMoves::promotion:
			removePiece(pieces::pawn, move.sideThatMoved, move.squareTo);
			addPiece(move.promotion, move.sideThatMoved, move.squareTo);
			hash ^= constants::pieceHash[move.sideThatMoved][pieces::pawn][move.squareTo];
			hash ^= constants::pieceHash[move.sideThatMoved][move.promotion][move.squareTo];
			break;
		case specialMoves::en_passant:
			if (move.sideThatMoved == sides::white)
			{
				removePiece(pieces::pawn, !move.sideThatMoved,
							move.squareTo - 8);
				hash ^= constants::pieceHash[move.sideThatMoved][pieces::pawn][move.squareTo - 8];
			}
			else
			{
				removePiece(pieces::pawn, !move.sideThatMoved,
							move.squareTo + 8);
				hash ^= constants::pieceHash[move.sideThatMoved][pieces::pawn][move.squareTo + 8];
			}
			break;
		}
	}
}

void Bitboard::unmakeMove(const Move &move, std::stack<Bitboard> &moveStack)
{
	*this = moveStack.top();
	moveStack.pop();
}

bool Bitboard::checkLegal(Move move)
{
	
	static std::stack<Bitboard> tmpStack;
	makeMove(move, tmpStack);
	//bool res = !(getAllAttacks(sideToPlay) & (occupancy[!sideToPlay] & pieces[pieces::king]));
	uint64_t kingSquare = __builtin_ffsll(occupancy[move.sideThatMoved] & pieces[pieces::king]) - 1;
	bool res = !isSquareAttacked(kingSquare, !move.sideThatMoved);
	unmakeMove(move, tmpStack);
	return res;
	//return true;
}

std::vector<Move> Bitboard::generateMoves(bool &castle)
{
	std::vector<Move> moves;
	generatePawnCaptures(moves);
	generateEnPassant(moves);
	generatePawnPushes(moves);
	generateKnightMoves(moves);
	generateBishopMoves(moves);
	generateRookMoves(moves);
	generateQueenMoves(moves);
	generateCastling(moves);
	generateNormalKingMoves(moves);
	return moves;
}

void Bitboard::generatePawnPushes(std::vector<Move> &moves)
{
	uint64_t singlePawnPushes;
	if (sideToPlay == sides::white)
	{
		singlePawnPushes = wPushPawns1(occupancy[sideToPlay] & pieces[pieces::pawn]);
	}
	else
	{
		singlePawnPushes = bPushPawns1(occupancy[sideToPlay] & pieces[pieces::pawn]);
	}

	uint64_t doublePawnPushes;
	if (sideToPlay == sides::white)
	{
		doublePawnPushes = wPushPawns2(occupancy[sideToPlay] & pieces[pieces::pawn]);
	}
	else
	{
		doublePawnPushes = bPushPawns2(occupancy[sideToPlay] & pieces[pieces::pawn]);
	}

	while (doublePawnPushes)
	{
		uint8_t nextPush = __builtin_ffsl(doublePawnPushes) - 1;
		uint8_t from = (sideToPlay == sides::white) ? nextPush - 16 : nextPush + 16;
		uint8_t newEnPassant = (sideToPlay == sides::white) ? nextPush - 8 : nextPush + 8;
		//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
		Move m(from, nextPush, 0, castleFlags,
			   sideToPlay, 0, pieces::pawn, pieces::empty, newEnPassant);
		if (checkLegal(m))
		{
			moves.push_back(m);
		}
		doublePawnPushes &= doublePawnPushes - 1;
	}

	while (singlePawnPushes)
	{
		uint8_t nextPush = __builtin_ffsl(singlePawnPushes) - 1;
		uint8_t from = (sideToPlay == sides::white) ? nextPush - 8 : nextPush + 8;
		uint8_t flags = 0;
		//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
		if ((sideToPlay == sides::white && (nextPush > 55)) || (sideToPlay == sides::black && (nextPush < 8)))
		{
			flags = 1 << specialMoves::promotion;
			Move m(from, nextPush, flags, castleFlags,
				   sideToPlay, pieces::queen, pieces::pawn, pieces::empty, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
				for (int i = pieces::knight; i <= pieces::rook; i++)
				{
					m.promotion = i;
					moves.push_back(m);
				}
			}
		}
		else
		{
			Move m(from, nextPush, 0, castleFlags,
				   sideToPlay, 0, pieces::pawn, pieces::empty, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
			}
		}
		singlePawnPushes &= singlePawnPushes - 1;
	}
}


void Bitboard::generateKnightMoves(std::vector<Move> &moves)
{
	uint64_t knights = pieces[pieces::knight] & occupancy[sideToPlay];
	while (knights)
	{
		uint64_t nextKnight = __builtin_ffsl(knights) - 1;
		uint64_t possibleMoves = constants::knightAttacks[nextKnight] & ~occupancy[sideToPlay];
		uint64_t attacks = possibleMoves & occupancy[!sideToPlay];
		uint64_t quiet = possibleMoves & ~attacks;
		while (attacks)
		{
			uint64_t nextAttack = __builtin_ffsl(attacks) - 1;
			for (int i = 5; i >= 0; i--)
			{
				if ((1ULL << nextAttack) & pieces[i] & occupancy[!sideToPlay])
				{
					Move m(nextKnight, nextAttack, 0, castleFlags,
						   sideToPlay, 0, pieces::knight, i,  0);
					if (checkLegal(m))
					{
						moves.push_back(m);
					}
					break;
				}
			}
			attacks &= attacks - 1;
		}

		while (quiet)
		{
			uint64_t nextQuiet = __builtin_ffsl(quiet) - 1;
			Move m(nextKnight, nextQuiet, 0, castleFlags,
				   sideToPlay, 0, pieces::knight, pieces::empty, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
			}
			quiet &= quiet - 1;
		}
		knights &= knights - 1;
	}
}


void Bitboard::generateRookMoves(std::vector<Move> &moves)
{
	uint64_t rooks = pieces[pieces::rook] & occupancy[sideToPlay];
	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	while (rooks)
	{
		uint64_t nextRook = __builtin_ffsl(rooks) - 1;

		uint64_t rookAttacks = getRookAttacks(nextRook, sideToPlay);

		while (rookAttacks)
		{
			uint64_t nextAttack = __builtin_ffsl(rookAttacks) - 1;
			uint8_t castling = castleFlags;
			if (sideToPlay == sides::white)
			{
				if (nextRook == squareString2num("a1"))
				{
					castling &= ~(1 << specialMoves::white_castle_long);
				}
				else if (nextRook == squareString2num("h1"))
				{
					castling &= ~(1 << specialMoves::white_castle_short);
				}
			}
			else
			{
				if (nextRook == squareString2num("a8"))
				{
					castling &= ~(1 << specialMoves::black_castle_long);
				}
				else if (nextRook == squareString2num("h8"))
				{
					castling &= ~(1 << specialMoves::black_castle_short);
				}
			}

			uint8_t capture = pieces::empty;
			if (occupancy[!sideToPlay] & (1ULL << nextAttack))
			{
				for (int i = 0; i < 6; i++)
				{
					if (occupancy[!sideToPlay] & pieces[i] & (1ULL << nextAttack))
					{
						capture = i;
						break;
					}
				}
			}

			Move m(nextRook, nextAttack, 0, castling,
				   sideToPlay, 0, pieces::rook, capture, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
			}

			rookAttacks &= rookAttacks - 1;
		}
		rooks &= rooks - 1;
	}
}


void Bitboard::generateBishopMoves(std::vector<Move> &moves)
{
	uint64_t bishops = pieces[pieces::bishop] & occupancy[sideToPlay];
	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	while (bishops)
	{
		uint64_t nextBishop = __builtin_ffsl(bishops) - 1;

		uint64_t bishopAttacks = getBishopAttacks(nextBishop, sideToPlay);

		while (bishopAttacks)
		{
			uint64_t nextAttack = __builtin_ffsl(bishopAttacks) - 1;
			uint8_t capture = pieces::empty;
			if (occupancy[!sideToPlay] & (1ULL << nextAttack))
			{
				for (int i = 0; i < 6; i++)
				{
					if (occupancy[!sideToPlay] & pieces[i] & (1ULL << nextAttack))
					{
						capture = i;
						break;
					}
				}
			}
			Move m(nextBishop, nextAttack, 0, castleFlags,
				   sideToPlay, 0, pieces::bishop, capture, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
			}

			bishopAttacks &= bishopAttacks - 1;
		}
		bishops &= bishops - 1;
	}
}

void Bitboard::generateQueenMoves(std::vector<Move> &moves)
{
	uint64_t queens = pieces[pieces::queen] & occupancy[sideToPlay];
	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	while (queens)
	{
		uint64_t nextQueen = __builtin_ffsl(queens) - 1;

		uint64_t queenAttacks = getQueenAttacks(nextQueen, sideToPlay);

		while (queenAttacks)
		{
			uint64_t nextAttack = __builtin_ffsl(queenAttacks) - 1;
			uint8_t capture = pieces::empty;
			if (occupancy[!sideToPlay] & (1ULL << nextAttack))
			{
				for (int i = 0; i < 6; i++)
				{
					if (occupancy[!sideToPlay] & pieces[i] & (1ULL << nextAttack))
					{
						capture = i;
						break;
					}
				}
			}
			Move m(nextQueen, nextAttack, 0, castleFlags,
				   sideToPlay, 0, pieces::queen, capture, 0);
			if (checkLegal(m))
			{
				moves.push_back(m);
			}

			queenAttacks &= queenAttacks - 1;
		}
		queens &= queens - 1;
	}
}

void Bitboard::generateNormalKingMoves(std::vector<Move> &moves)
{
	//uint64_t enemyAttacks = getAllAttacks(!sideToPlay);
	uint8_t kingSquare = __builtin_ffsl(pieces[pieces::king] & occupancy[sideToPlay]) - 1;
	uint64_t kingMoves = constants::kingAttacks[kingSquare] & ~occupancy[sideToPlay];
	uint8_t castling = castleFlags;
	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	if (sideToPlay == sides::white)
	{
		castling &= ~(1 << white_castle_short);
		castling &= ~(1 << white_castle_long);
	}
	else
	{
		castling &= ~(1 << black_castle_short);
		castling &= ~(1 << black_castle_long);
	}
	while (kingMoves)
	{
		uint8_t capture = pieces::empty;
		uint8_t nextMove = __builtin_ffsl(kingMoves) - 1;
		if (occupancy[!sideToPlay] & (1ULL << nextMove))
		{
			for (int i = 0; i < 6; i++)
			{
				if (occupancy[!sideToPlay] & pieces[i] & (1ULL << nextMove))
				{
					capture = i;
					break;
				}
			}
		}
		Move m(kingSquare, nextMove, 0, castling, sideToPlay, 0,
			   pieces::king, capture, 0);
		if (checkLegal(m))
		{
			moves.push_back(m);
		}
		kingMoves &= kingMoves - 1;
	}
}

void Bitboard::generateEnPassant(std::vector<Move> &moves)
{
	if (!enPassant)
	{
		return;
	}
	if ((enPassant & 0xFF0000ULL) && (sideToPlay == sides::white))
	{
		return;
	}
	if ((enPassant & 0xFF0000000000ULL) && (sideToPlay == sides::black))
	{
		return;
	}

	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	uint64_t eastAttacks, westAttacks;
	uint64_t pawns = occupancy[sideToPlay] & pieces[pieces::pawn];
	if (sideToPlay == sides::white)
	{
		eastAttacks = wPawnEastAttacks(pawns) & ~occupancy[sideToPlay];
		westAttacks = wPawnWestAttacks(pawns) & ~occupancy[sideToPlay];
	}
	else
	{
		eastAttacks = bPawnEastAttacks(pawns) & ~occupancy[sideToPlay];
		westAttacks = bPawnWestAttacks(pawns) & ~occupancy[sideToPlay];
	}

	if (!((eastAttacks | westAttacks) & enPassant))
	{
		return;
	}

	if (sideToPlay == sides::white)
	{
		if (SW1(enPassant) & occupancy[sideToPlay] & pieces[pieces::pawn])
		{
			uint8_t squareTo = __builtin_ffsl(enPassant) - 1;
			uint8_t squareFrom = __builtin_ffsl(SW1(enPassant)) - 1;

			uint8_t flags = 1 << specialMoves::en_passant;

			Move m(squareFrom, squareTo, flags, castleFlags,
				   sideToPlay, 0, pieces::pawn, pieces::empty, 0);

			if (checkLegal(m))
			{
				moves.push_back(m);
			}
		}
		if ((SE1(enPassant) & occupancy[sideToPlay] & pieces[pieces::pawn]))
		{
			uint8_t squareTo = __builtin_ffsl(enPassant) - 1;
			uint8_t squareFrom = __builtin_ffsl(SE1(enPassant)) - 1;

			uint8_t flags = 1 << specialMoves::en_passant;

			Move m(squareFrom, squareTo, flags, castleFlags,
				   sideToPlay, 0, pieces::pawn, pieces::empty, 0);

			if (checkLegal(m))
			{
				moves.push_back(m);
			}
		}
	}
	else
	{
		if (NW1(enPassant) & occupancy[sideToPlay] & pieces[pieces::pawn])
		{
			uint8_t squareTo = __builtin_ffsl(enPassant) - 1;
			uint8_t squareFrom = __builtin_ffsl(NW1(enPassant)) - 1;

			uint8_t flags = 1 << specialMoves::en_passant;

			Move m(squareFrom, squareTo, flags, castleFlags,
				   sideToPlay, 0, pieces::pawn, pieces::empty, 0);

			if (checkLegal(m))
			{
				moves.push_back(m);
			}
		}
		if (NE1(enPassant) & occupancy[sideToPlay] & pieces[pieces::pawn])
		{
			uint8_t squareTo = __builtin_ffsl(enPassant) - 1;
			uint8_t squareFrom = __builtin_ffsl(NE1(enPassant)) - 1;

			uint8_t flags = 1 << specialMoves::en_passant;

			Move m(squareFrom, squareTo, flags, castleFlags,
				   sideToPlay, 0, pieces::pawn, pieces::empty, 0);

			if (checkLegal(m))
			{
				moves.push_back(m);
			}
		}
	}
}

void Bitboard::generatePawnCaptures(std::vector<Move> &moves)
{
	uint64_t pawns = occupancy[sideToPlay] & pieces[pieces::pawn], attacksE, attacksW;
	if (sideToPlay == sides::white)
	{
		attacksE = wPawnEastAttacks(pawns) & occupancy[!sideToPlay];
		attacksW = wPawnWestAttacks(pawns) & occupancy[!sideToPlay];
	}
	else
	{
		attacksE = bPawnEastAttacks(pawns) & occupancy[!sideToPlay];
		attacksW = bPawnWestAttacks(pawns) & occupancy[!sideToPlay];
	}

	if (sideToPlay == sides::white)
	{
		while (attacksW)
		{
			uint8_t nextAttackW = __builtin_ffsl(attacksW) - 1;
			uint8_t squareFrom = __builtin_ffsl(SE1(1ULL << nextAttackW)) - 1;
			uint8_t capture = pieces::empty;
			//uint8_t enPass = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;

			uint64_t enemyPieces = occupancy[!sideToPlay];
			for (int i = 0; i < 6; i++)
			{
				if (enemyPieces & pieces[i] & (1ULL << nextAttackW))
				{
					capture = i;
					break;
				}
			}

			uint8_t flags = 0;
			uint8_t promotion = 0;
			if (nextAttackW > 55)
			{
				flags = 1 << specialMoves::promotion;
				promotion = pieces::queen;
				Move m(squareFrom, nextAttackW, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
					for (int i = pieces::knight; i <= pieces::rook; i++)
					{
						m.promotion = i;
						moves.push_back(m);
					}
				}
			}
			else
			{
				Move m(squareFrom, nextAttackW, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
				}
			}

			attacksW &= attacksW - 1;
		}

		while (attacksE)
		{
			uint8_t nextAttackE = __builtin_ffsl(attacksE) - 1;
			uint8_t squareFrom = __builtin_ffsl(SW1(1ULL << nextAttackE)) - 1;
			uint8_t capture = pieces::empty;
			//uint8_t enPass = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;

			uint64_t enemyPieces = occupancy[!sideToPlay];
			for (int i = 0; i < 6; i++)
			{
				if (enemyPieces & pieces[i] & (1ULL << nextAttackE))
				{
					capture = i;
					break;
				}
			}

			uint8_t flags = 0;
			uint8_t promotion = 0;
			if (nextAttackE > 55)
			{
				flags = 1 << specialMoves::promotion;
				promotion = pieces::queen;
				Move m(squareFrom, nextAttackE, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
					for (int i = pieces::knight; i <= pieces::rook; i++)
					{
						m.promotion = i;
						moves.push_back(m);
					}
				}
			}
			else
			{
				Move m(squareFrom, nextAttackE, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
				}
			}

			attacksE &= attacksE - 1;
		}
	}

	else
	{
		while (attacksW)
		{
			uint8_t nextAttackW = __builtin_ffsl(attacksW) - 1;
			uint8_t squareFrom = __builtin_ffsl(NE1(1ULL << nextAttackW)) - 1;
			uint8_t capture = pieces::empty;
			//uint8_t enPass = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;

			uint64_t enemyPieces = occupancy[!sideToPlay];
			for (int i = 0; i < 6; i++)
			{
				if (enemyPieces & pieces[i] & (1ULL << nextAttackW))
				{
					capture = i;
					break;
				}
			}

			uint8_t flags = 0;
			uint8_t promotion = 0;
			if (nextAttackW < 8)
			{
				flags = 1 << specialMoves::promotion;
				promotion = pieces::queen;
				Move m(squareFrom, nextAttackW, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
					for (int i = pieces::knight; i <= pieces::rook; i++)
					{
						m.promotion = i;
						moves.push_back(m);
					}
				}
			}
			else
			{
				Move m(squareFrom, nextAttackW, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
				}
			}

			attacksW &= attacksW - 1;
		}

		while (attacksE)
		{
			uint8_t nextAttackE = __builtin_ffsl(attacksE) - 1;
			uint8_t squareFrom = __builtin_ffsl(NW1(1ULL << nextAttackE)) - 1;
			uint8_t capture = pieces::empty;
			//uint8_t enPass = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;

			uint64_t enemyPieces = occupancy[!sideToPlay];
			for (int i = 0; i < 6; i++)
			{
				if (enemyPieces & pieces[i] & (1ULL << nextAttackE))
				{
					capture = i;
					break;
				}
			}

			uint8_t flags = 0;
			uint8_t promotion = 0;
			if (nextAttackE < 8)
			{
				flags = 1 << specialMoves::promotion;
				promotion = pieces::queen;
				Move m(squareFrom, nextAttackE, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
					for (int i = pieces::knight; i <= pieces::rook; i++)
					{
						m.promotion = i;
						moves.push_back(m);
					}
				}
			}
			else
			{
				Move m(squareFrom, nextAttackE, flags, castleFlags,
					   sideToPlay, promotion, pieces::pawn, capture, 0);
				if (checkLegal(m))
				{
					moves.push_back(m);
				}
			}

			attacksE &= attacksE - 1;
		}
	}
}


void Bitboard::generateCastling(std::vector<Move> &moves)
{
	uint64_t attacks = getAllAttacks(!sideToPlay);
	//uint8_t prevEnpassant = (enPassant != 0) ? __builtin_ffsl(enPassant) - 1 : 0;
	if (sideToPlay == sides::white)
	{
		uint8_t squareFrom = squareString2num("e1");
		if (castleFlags & (1 << specialMoves::white_castle_long))
		{
			if (!(attacks & 0x1CULL) && !((occupancy[0] | occupancy[1]) & 0xEULL) && (occupancy[sides::white] & pieces[pieces::rook] & 1))
			{
				uint8_t squareTo = squareString2num("c1");
				uint8_t flags = 1 << white_castle_long;
				uint8_t newCastleFlags = castleFlags & ~(1 << white_castle_long);
				newCastleFlags &= ~(1 << white_castle_short);

				Move m(squareFrom, squareTo, flags, newCastleFlags,
					   sideToPlay, 0, pieces::king, pieces::empty,
					   0);

				moves.push_back(m);
			}
		}

		if (castleFlags & (1 << specialMoves::white_castle_short))
		{
			if (!(attacks & 0x70ULL) && !((occupancy[0] | occupancy[1]) & 0x60ULL) && (occupancy[sides::white] & pieces[pieces::rook] & 0x80ULL))
			{
				uint8_t squareTo = squareString2num("g1");
				uint8_t flags = 1 << white_castle_short;
				uint8_t newCastleFlags = castleFlags & ~(1 << white_castle_short);
				newCastleFlags &= ~(1 << white_castle_long);

				Move m(squareFrom, squareTo, flags, newCastleFlags,
					   sideToPlay, 0, pieces::king, pieces::empty, 0);

				moves.push_back(m);
			}
		}
	}

	else
	{
		uint8_t squareFrom = squareString2num("e8");
		if (castleFlags & (1 << specialMoves::black_castle_long))
		{
			if (!(attacks & 0x1C00000000000000ULL) && !((occupancy[0] | occupancy[1]) & 0xE00000000000000ULL) && (occupancy[sides::black] & pieces[pieces::rook] & 0x100000000000000ULL))
			{
				uint8_t squareTo = squareString2num("c8");
				uint8_t flags = 1 << black_castle_long;
				uint8_t newCastleFlags = castleFlags & ~(1 << black_castle_long);
				newCastleFlags &= ~(1 << black_castle_short);

				Move m(squareFrom, squareTo, flags, newCastleFlags,
					   sideToPlay, 0, pieces::king, pieces::empty, 0);

				moves.push_back(m);
			}
		}

		if (castleFlags & (1 << specialMoves::black_castle_short))
		{
			if (!(attacks & 0x7000000000000000ULL) && !((occupancy[0] | occupancy[1]) & 0x6000000000000000ULL) && (occupancy[sides::black] & pieces[pieces::rook] & 0x8000000000000000ULL))
			{
				uint8_t squareTo = squareString2num("g8");
				uint8_t flags = 1 << black_castle_short;
				uint8_t newCastleFlags = castleFlags & ~(1 << black_castle_short);
				newCastleFlags &= ~(1 << black_castle_long);

				Move m(squareFrom, squareTo, flags, newCastleFlags,
					   sideToPlay, 0, pieces::king, pieces::empty, 0);

				moves.push_back(m);
			}
		}
	}
}

const bool Bitboard::operator==(const Bitboard &b) {
	for(int i = 0; i < 6; i++) {
		for(int s = 0; s <= 1; s++) {
			if(occupancy[s] & pieces[i] != b.occupancy[s] & b.pieces[i]) {
				return false;
			}
		}
	}
	if(checks[0] != b.checks[0] || b.checks[1] != b.checks[1]) {
		return false;
	}
	if(enPassant != b.enPassant) {
		return false;
	}
	if(castleFlags != b.castleFlags) {
		return false;
	}
	if(sideToPlay != b.sideToPlay) {
		return false;
	}
	return true;
}

uint64_t Bitboard::perft(int depth, std::stack<Bitboard> &moveStack)
{
	bool castle;
	std::vector<Move> moves = generateMoves(castle);
	if (depth == 1)
	{
		return moves.size();
	}

	uint64_t numMoves = 0;
	for (Move m : moves)
	{
		makeMove(m, moveStack);
		numMoves += perft(depth - 1, moveStack);
		unmakeMove(m, moveStack);
	}
	return numMoves;
}