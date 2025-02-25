CC := gcc
FLAGS := -O2 -Werror -Wextra -Wall -std=c11
OBJECTS = ./obj
common_source_dir := ../common
binaries_directory := .
BIN = s21_grep
TESTS = ./tests



.PHONY: s21_grep all tests

$(BIN): $(OBJECTS)/s21_grep.o $(OBJECTS)/common.o
	$(CC) -o $@ $^ 

$(OBJECTS)/s21_grep.o: s21_grep.c 
	mkdir -p $(OBJECTS)
	$(CC) -c $^ ${FLAGS} -o $@

$(OBJECTS)/common.o: $(common_source_dir)/common.c $(common_source_dir)/common.h
	mkdir -p $(OBJECTS)
	$(CC) -c $(common_source_dir)/common.c ${FLAGS} -o $@ 

tests: $(common_source_dir)/common.c $(TESTS)/generate_tests.c
	make s21_grep
	$(CC) $(FLAGS) -Wno-unused-parameter $^ -o $(TESTS)/generator
	chmod "+x" $(TESTS)/run.sh
	$(TESTS)/run.sh 0
	make clean
	rm $(TESTS)/generator

leak_tests: $(common_source_dir)/common.c $(TESTS)/generate_tests.c
	make s21_grep
	$(CC) $(FLAGS) -Wno-unused-parameter $^ -o $(TESTS)/generator
	chmod "+x" $(TESTS)/run.sh
	$(TESTS)/run.sh 1
	make clean
	rm $(TESTS)/generator

clean:
	rm -rf $(OBJECTS)
	rm $(BIN)

rebuild:
	@make clean
	@make $(BIN)
