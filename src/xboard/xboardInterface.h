#ifndef XBOARDINTERFACE_H
#define XBOARDINTERFACE_H

#include <iostream>
#include "../bitboard/bitboard.h"
#include "../move/move.h"

enum CommandType {
	new_game,
	move,
	protover,
	force,
	go,
	set_white,
	set_black,
	null
};

struct Command {
	int commandType;
	uint8_t moveFrom;
	uint8_t moveTo;
	char promotion = pieces::empty;
};

class XBoardInterface {
	public:
	XBoardInterface();
	Command getCommand(bool force);
};

#endif