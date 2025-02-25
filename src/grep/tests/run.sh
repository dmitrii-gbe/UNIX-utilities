#!/bin/bash

flags=(-n -i -c -h -l -v -s -on -o -ov -iv -iv -ch -lh -oi)

text_file="tests/text"
text_file1="tests/text1"

iteration_count=30
while [ $iteration_count != 0 ]; do
tests/generator tests/text > tests/samples
tests/generator tests/text1 > tests/samples1
for flag in ${flags[@]}
	do
		#run commands
		# grep $flag -f tests/samples $text_file
		# ./s21_grep $flag -f tests/samples $text_file
		#store result
		grep $flag -f tests/patterns -f tests/samples -f tests/samples1 $text_file $text_file1 > grep_res 
		./s21_grep $flag -f tests/patterns -f tests/samples -f tests/samples1 $text_file $text_file1 > s21_grep_res

		# echo "Flag is: $flag" 
		echo "$(diff -s grep_res s21_grep_res)"
    	if [[ $(diff -q grep_res s21_grep_res) ]]
    	then
        	echo -e "\nFlag that caused the error is \n\t$flag\n"
        	echo -e "Diff:"
			echo "$(diff -s grep_res s21_grep_res)"
        	exit
    	fi
		if [[ $1 -eq 1 ]]
		then
		echo -e "Memory leak test started"
		valgrind --tool=memcheck --leak-check=full --log-file=valgrind_report -q ./s21_grep $flag -f tests/patterns -f tests/samples -f tests/samples1 $text_file $text_file1 > s21_grep_res
		if [[ -s valgrind_report ]]
		then
			echo -e "Memory leak detected"
			cat valgrind_report
			echo -e "Test that caused error is ./s21_grep $flag -f tests/patterns -f tests/samples -f tests/samples1 $text_file $text_file1 > s21_grep_res"
			exit
		fi
		fi
	done
	iteration_count=$((iteration_count - 1))
	echo -e "Iterations left $iteration_count"
done
rm s21_grep_res grep_res
rm tests/samples tests/samples1
rm valgrind_report
echo -e "\033[32mAll tests passed\033]32m"
