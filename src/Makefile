C++ 	 = *.cc
FLAGS 	 = gcc -g -Wall -Werror -Wextra
DOPFLAGS = -lm -lstdc++ -std=c++17
TESTFLAGS= -lgtest -lgmock -pthread
TESTRUNS = --gtest_repeat=100000 --gtest_break_on_failure
VALGFULL = --leak-check=full
VALGORIG = --track-origins=yes

s21_matrix_oop.a:
	$(FLAGS) $(C++) $(DOPFLAGS) -c
	ar -rcs s21_matrix_oop.a *.o
	rm -rf *.o

test: clean s21_matrix_oop.a
	$(FLAGS) $(C++) *.a -o test.out $(DOPFLAGS) $(TESTFLAGS) && ./test.out $(TESTRUNS)
	
lint:
	clang-format -i -style=Google *.cc *.hpp

valgrind: clean test
	valgrind $(VALGFULL) $(VALGORIG) -s ./test.out

clean:
	rm -rf *.out *.dSYM *.o *.a *.gcno
