CC := gcc
CFLAGS := -I inc -std=c99 -Wall -Werror -Wextra -Wpedantic
OBJS := out/frontend.o 
D_OS = -D __My_Linux__
FLAGS = -Wall -Wextra -Werror -std=c11
LIB_DIR = brick_game/tetris/*.c
LIB = backend.a
RM = rm -rf

VERSION = 1.0
TARGET = tetris
DIST_DIR = $(TARGET)-$(VERSION)
DIST_SOURCES = ./Makefile ./tests ./gui ./brick_game main.c

TEST_DIR = ./tests/*.c
LIBS_FOR_CHECK_PCKG = -lcheck -lsubunit -lm -lpthread 
GCOV_FLAG = -fprofile-arcs -ftest-coverage

all: clean install 
	./tetris
	
install: backend.a tetris 
	cp tetris ./bin


uninstall: 
	$(RM) bin
	$(RM) out

dist:
	mkdir -p $(DIST_DIR)
	cp -r $(DIST_SOURCES) $(DIST_DIR)
	tar -czf $(DIST_DIR).tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

doxy: clean
	sudo doxygen Doxyfile

dvi: clean doxy
	(cd ./doxygen/latex; sudo pdflatex refman.tex)

tetris: $(OBJS) main.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) -lncurses

out/%.o: ./gui/cli/%.c
	mkdir out
	$(CC) $(CFLAGS) -c $< -o $@

check_clang:
	cp ../materials/linters/.clang-format .
	clang-format -n main.c tetris.h
	clang-format -n $(LIB_DIR) ./brick_game/tetris/*.h
	clang-format -n ./tests/*.c ./tests/*.h
	clang-format -n ./gui/cli/*.c ./gui/cli/*.h
	rm -rf .clang-format

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i main.c tetris.h
	clang-format -i $(LIB_DIR) ./brick_game/tetris/*.h
	clang-format -i ./tests/*.c ./tests/*.h
	clang-format -i ./gui/cli/*.c ./gui/cli/*.h
	rm -rf .clang-format

# main.o:
# 	$(CC) $(CFLAGS) main.c -o out/main.o  -lncurses
clean: uninstall
	$(RM) out/*.o tetris
	$(RM) *.a *.o
	$(RM) back test
	$(RM) tetris-1.0.tar.gz
	$(RM) report test.info test.out
	$(RM) doxygen
	$(RM) out

backend.a:
	$(CC) $(D_OS) -c $(FLAGS) $(LIB_DIR)
	ar -rcs $(LIB) *.o
	ranlib $(LIB)
	mkdir ./bin
	touch ./bin/high-score.txt
	# echo 0 >> ./bin/high-score.txt
	$(RM) *.o

gcov_report: 
	$(CC) $(CFLAGS) -o test.out $(TEST_DIR) $(LIB_DIR) $(LIBS_FOR_CHECK_PCKG) $(GCOV_FLAG)
	./test.out
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	$(RM) *.gcda *.gcno

back: 
	$(CC) $(D_OS) -c $(FLAGS) brick_game/tetris/*.c
	$(CC) $(D_OS) *.o -o back
	./back

test: clean backend.a
	$(RM) test
	$(CC) $(CFLAGS) $(TEST_DIR) $(LIB) $(LIBS_FOR_CHECK_PCKG) -o test
	./test

valgrind:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s  ./test
