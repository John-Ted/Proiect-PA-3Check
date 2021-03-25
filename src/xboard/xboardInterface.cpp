#include "xboardInterface.h"

XBoardInterface::XBoardInterface() {
	std::ios::sync_with_stdio(false);
	std::cout.setf(std::ios::unitbuf);
}

Command XBoardInterface::getCommand(bool go) {
	Command c;
	c.commandType = CommandType::null;
	std::string line;
	std::getline(std::cin, line);
	std::istringstream instream(line);
	std::string in;
	instream >> in;

	if(in == "protover") {
		c.commandType = CommandType::protover;
		instream >> in;
	}
	else if(in == "new") {
		c.commandType = CommandType::new_game;
	}
	else if(in == "force") {
		c.commandType = CommandType::force;
	}
	else if(in == "go") {
		c.commandType = CommandType::go;
	}
	else if(in == "white") {
		c.commandType = CommandType::set_white;
	}
	else if(in == "black") {
		c.commandType = CommandType::set_black;
	}
	else if(in == "quit") {
		c.commandType = CommandType::quit;
	}
	else {
		if((in.length() == 4 || in.length() == 5) && 
		(std::isdigit(in[1]) && std::isdigit(in[3]))) {
			c.commandType = CommandType::move;
			c.moveFrom = Bitboard::squareString2num(in.substr(0, 2));
			c.moveTo = Bitboard::squareString2num(in.substr(2, 2));
			if(in.length() == 5) {
				c.promotion = in[4];
			}
		}
	}
	return c;
}