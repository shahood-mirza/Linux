#!/bin/bash

while :; do

echo "Welcome to Shahood's Linux Menu"
echo "Press <Ctrl-C> or select option 4 to quit"
echo "1. Display the current directory"
echo "2. Run the vi editor"
echo "3. Compare directory listings"
echo "4. Quit"
echo -n "Enter your choice: "

read choice
case $choice in
	1) pwd
	ls -l;;
	2) vi;;
	3) echo -n "Enter a directory path: "
	read filepath

	if [ -e $filepath ]; then
		dir1=$(ls -l |wc -l)
		cd $filepath
		dir2=$(ls -l |wc -l)

		if [ $dir1 -gt $dir2 ]; then
			echo "Your current directory has more listings than $filepath"
		elif [ $dir1 -lt $dir2 ]; then
			echo "$filepath has more listings than yout current directory"
		else
			echo "Your current directory and $filepath have an equal number of listings"
		fi
	else
		echo "directory does not exist"
	fi
	;;

	4) exit;;
	*) echo "Unrecognized value"
esac

sleep 1

echo "Press Enter To Continue"

read
clear
done
