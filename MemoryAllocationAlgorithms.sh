#!/bin/bash

holeNum=0
reqNum=0

while read option;do
	if [ "$option" == "Holes:" ];then
		while read hole size;do
			if [ "$hole" == "Requests:" ];then
				option=$hole
				break
			else
				if [ "$hole" != " " ];then
					holes[holeNum]=$hole
					holeMem[holeNum]=$size
					let holeNum++
				elif [ "$hole" == " " ];then
						read option
						break
				fi
			fi
		done
	fi

	if [ "$option" == "Requests:" ];then
		while read memory size;do
			if [ "$memory" != " " ];then
				requests[reqNum]=$memory
				reqMem[reqNum]=$size
				let reqNum++
			elif [ "$memory" == " " ];then
					break
			fi
		done
	fi
done < $1

count=0
for i in ${holes[@]}
do
	firstFit[count]="Empty"
	nextFit[count]="Empty"
	worstFit[count]="Empty"
	bestFit[count]="Empty"
	let count++
done

##################################################################################
#                           FIRST FIT						 #
##################################################################################

i=0
for((i;i<${#reqMem[@]};i++))
do
	j=0
	memory=${reqMem[i]}
	for((j;j<${#holeMem[@]};j++))
	do
		hole=${holeMem[j]}
		if [ $((memory)) -le $((hole)) ] && [ "${firstFit[j]}" == "Empty" ];then
			firstFit[j]="${requests[i]}"
			break
		fi
	done
done

##################################################################################
#                           NEXT FIT						 #
##################################################################################

i=0
j=0
link=j
memry=${#holeMem[@]}

for((i;i<${#reqMem[@]};i++))
do
	count=0
	memory=${reqMem[i]}
	while [ $((count)) -lt $((memry)) ];do
		hole=${holeMem[j]}
		if [ $((memory)) -le $((hole)) ] && [ "${nextFit[j]}" == "Empty" ];then
			link=$j
			nextFit[link]="${requests[i]}"
			let count++
			break
		else
			let count++
			let j++
			if [ $((j)) -ge $((memry)) ];then
				let j=0
			fi
		fi
	done
done

##################################################################################
#                           BEST FIT						 #
##################################################################################

j=0
for i in ${holeMem[@]}
do
	tempMemory[j]=$i
	tempIndex[j]=$j
	let j++
done

for((i=0;i<${#tempMemory[@]}-1;i++))
do
	min=${tempMemory[i]}
	for((j=$i+1;j<${#tempMemory[@]};j++))
	do
		curr=${tempMemory[j]}
		if [ $curr -lt $min ];then
			temp=${tempMemory[j]}
			tempMemory[j]=${tempMemory[i]}
			tempMemory[i]=$temp
			temp=${tempIndex[j]}
			tempIndex[j]=${tempIndex[i]}
			tempIndex[i]=$temp
		fi
	done
done

i=0
for((i;i<${#reqMem[@]};i++))
do
	memory=${reqMem[i]}
	for((j=0;j<${#tempMemory[@]};j++))
	do
		hole=${tempMemory[j]}
		if [ $((memory)) -le $((hole)) ];then
		let index=${tempIndex[j]}
			if  [ "${bestFit[index]}" == "Empty" ];then
				bestFit[index]="${requests[i]}"
				break
			fi
		fi
	done
done

##################################################################################
#                           WORST FIT						 #
##################################################################################

i=0
j=0
k=0
for((i;i<${#reqMem[@]};i++))
do
	memory=$((${reqMem[i]}))
	for((j=0;j<${#holeMem[@]}-1;j++))
	do
		let link=j
		let max=$((${holeMem[j]}))
		for((k=1;k<${#holeMem[@]};k++))
		do
			let current=$((${holeMem[k]}))
			if [ $((max)) -eq 0 ];then
				break
			fi
			if [ $((max)) -lt $((current)) ] && [ "${worstFit[k]}" == "Empty" ];then
				let max=$current
				let link=k
			elif [ $((max)) -eq $((current)) ] && [ "${worstFit[k]}" == "Empty" ] ;then
					echo
			fi
		done
		if [ $((memory)) -le $((max)) ] &&  [ "${worstFit[link]}" == "Empty" ];then
			worstFit[link]="${requests[i]}"
			break
		fi
	done
done

##################################################################################

j=0
echo "First fit:">>$2
for i in ${firstFit[@]}
do
	echo "${holes[j++]}-$i">>$2
done

j=0
echo -e "\nNext fit:">>$2
for i in ${nextFit[@]}
do
	echo "${holes[j++]}-$i">>$2
done

j=0
echo -e "\nBest fit:">>$2
for i in ${bestFit[@]}
do
	echo "${holes[j++]}-$i">>$2
done

j=0
echo -e "\nWorst fit:">>$2
for i in ${worstFit[@]}
do
	echo "${holes[j++]}-$i">>$2
done
