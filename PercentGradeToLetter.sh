#!/bin/bash
x=0
while [ $x -lt 4 ]; do
read name grade
if [ $grade -gt 79 ]; then
mark=A
elif [ $grade -gt 69 ]; then
mark=B
elif [ $grade -gt 59 ]; then
mark=C
elif [ $grade -gt 49 ]; then
mark=D
else
mark=F
fi
echo "$name		$grade		$mark" >> output
x=$(($x+1))
done
