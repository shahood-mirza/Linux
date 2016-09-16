#!/bin/bash
file="program_a_file.txt"
touch "$file"
for ((i=1; i<=1000; i++))
do
    lastNum=`tail -1 $file`
    expr $lastNum + 1 >> "$file"
done
