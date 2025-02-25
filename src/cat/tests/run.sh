#!/bin/bash

flags=(-n -s -E -T -e -t -v -b -sbven)
iteration_count=100

TestFilesCount=$iteration_count
	while [ $TestFilesCount != 0 ]; do
		tests/generator -n > tests/"Non_Empty_${TestFilesCount}.test"
		touch tests/"Empty_${TestFilesCount}.test"
		tests/generator -lf > tests/"LF_${TestFilesCount}.test"
	TestFilesCount=$((TestFilesCount - 1))
done

while [ $iteration_count != 0 ]; do
for flag in ${flags[@]}
	do
		./s21_cat $flag "tests/LF_${iteration_count}.test"  "tests/Non_Empty_${iteration_count}.test" "tests/Empty_${iteration_count}.test" "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" > s21_cat_res
		cat $flag "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" "tests/Empty_${iteration_count}.test" "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test"  "tests/Non_Empty_${iteration_count}.test" > sys_cat_res

		# echo "Flag is: $flag" 
		echo "$(diff -s sys_cat_res s21_cat_res)"
    	if [[ $(diff -q sys_cat_res s21_cat_res) ]]
    	then
        	echo -e "\nIncorrect processing of flag \n\t$flag\n"
        	echo -e "The test files in ./tests:"
        	echo -e "run ./s21_cat $flag "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" "tests/Empty_${iteration_count}.test" "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test"  "tests/Non_Empty_${iteration_count}.test" > s21_cat_res"
        	exit
    	fi
		if [[ $1 -eq 1 ]]
		then
		echo -e "Memory leak test started"
		valgrind --tool=memcheck --leak-check=full --log-file=valgrind_report -q ./s21_cat $flag "tests/LF_${iteration_count}.test"  "tests/Non_Empty_${iteration_count}.test" "tests/Empty_${iteration_count}.test" "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" > s21_cat_res
		if [[ -s valgrind_report ]]
		then
			echo -e "Memory leak detected"
			cat valgrind_report
			echo -e "Test that caused error is ./s21_cat $flag "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test" "tests/Empty_${iteration_count}.test" "tests/LF_${iteration_count}.test" "tests/Non_Empty_${iteration_count}.test"  "tests/Non_Empty_${iteration_count}.test" > s21_cat_res"
			exit
		fi
		fi
	done
	iteration_count=$((iteration_count - 1))
	echo -e Iteration left "$iteration_count"
done

rm tests/*.test
rm sys_cat_res
rm s21_cat_res
rm valgrind_report
echo -e "All tests passed"

