CXX = g++
CPPFLAGS = -std=gnu++17 -Ofast -mavx2
NAME = chess_engine

engine.o: engine/engine.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

xboardInterface.o: xboard/xboardInterface.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

bitboard.o: bitboard/bitboard.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

main.o: main.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

build: bitboard/constants.h main.o bitboard.o xboardInterface.o engine.o bitboard/bitboard.h bitboard/constants.h misc/misc.h xboard/xboardInterface.h engine/engine.h
	$(CXX) $(CPPFLAGS) -o $(NAME) $^

run:
	./$(NAME)

clean:
	rm *.o