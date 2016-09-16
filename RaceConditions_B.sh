#!/bin/bash
count=0;
file="program_b_file.txt"
touch $file
while [ $count -lt 1000 ]
do
    if ln $file ${file}.lock 2>/dev/null
    then
		lastNum=`tail -1 $file`
		expr $lastNum + 1 >> $file
		let count=$count+1;
		rm ${file}.lock
	fi
done
