CXX = g++
CPPFLAGS = -std=gnu++17 -Ofast
NAME = chess_engine
TEST_NAME = chess_engine_test

engine.o: engine/engine.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

xboardInterface.o: xboard/xboardInterface.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

bitboard.o: bitboard/bitboard.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

main.o: main.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

eval.o: eval/evaluator.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

search.o: search/search.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

build: bitboard/constants.h main.o search.o eval.o bitboard.o xboardInterface.o engine.o bitboard/bitboard.h bitboard/constants.h misc/misc.h xboard/xboardInterface.h engine/engine.h
	$(CXX) $(CPPFLAGS) -o $(NAME) $^

test.o: test.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

build_test: bitboard/constants.h test.o eval.o search.o bitboard.o xboardInterface.o engine.o bitboard/bitboard.h bitboard/constants.h misc/misc.h xboard/xboardInterface.h engine/engine.h
	$(CXX) $(CPPFLAGS) -o $(TEST_NAME) $^

run:
	./$(NAME) abc

run_test:
	./$(TEST_NAME) abc

run_xboard_1:
	xboard -fcp "make run" -debug

run_xboard_2:
	xboard -fcp "make run" -scp "make run"

run_against_pulsar_2:
	xboard -variant 3check -fcp "make run" -scp "pulsar2009-9b-64 mxT-2" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false 

run_against_pulsar_3:
	xboard -variant 3check -fcp "make run" -scp "pulsar2009-9b-64 mxT-3" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false 

run_against_pulsar_4:
	xboard -variant 3check -fcp "make run" -scp "pulsar2009-9b-64 mxT-4" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false 

set_path:
	PATH=$$PATH:~/Desktop/pulsar/

clean:
	touch clean_me.o
	touch $(NAME)
	touch $(TEST_NAME)
	rm ./$(NAME)
	rm ./$(TEST_NAME)
	rm *.o