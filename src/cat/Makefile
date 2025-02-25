CC := gcc
FLAGS := -O2 -Werror -Wextra -Wall -std=c11
OBJECTS = ./obj
common_source_dir := ../common
binaries_directory := .
BIN = s21_cat
TESTS = ./tests


.PHONY: s21_cat all tests

s21_cat: $(OBJECTS)/s21_cat.o $(OBJECTS)/common.o
	$(CC) -o $@ $^ 

$(OBJECTS)/s21_cat.o: s21_cat.c 
	mkdir -p $(OBJECTS)
	$(CC) -c $^ ${FLAGS} -o $@

$(OBJECTS)/common.o: $(common_source_dir)/common.c $(common_source_dir)/common.h
	mkdir -p $(OBJECTS)
	$(CC) -c $(common_source_dir)/common.c ${FLAGS} -o $@ 

tests: $(TESTS)/generate_tests.c
	make s21_cat
	$(CC) $(FLAGS) $^ -o $(TESTS)/generator
	chmod "+x" $(TESTS)/run.sh
	$(TESTS)/run.sh 0
	rm $(TESTS)/generator
	make clean

leak_tests: $(TESTS)/generate_tests.c
	make s21_cat
	$(CC) $(FLAGS) $^ -o $(TESTS)/generator
	chmod "+x" $(TESTS)/run.sh
	$(TESTS)/run.sh 1
	rm $(TESTS)/generator
	make clean

clean:
	rm -rf $(OBJECTS)
	rm s21_cat
	rm tests/*.test

rebuild:
	@make clean
	@make s21_cat
