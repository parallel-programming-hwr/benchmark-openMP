#!/usr/bin/env bash
if [ -z $1 ]
then
	THREADS=4
else
	THREADS=$1
	if [ $1 -le 0 ]
	then
		echo "thread num must be greater then 0"
		exit 1
	fi
fi

if [ -z $2 ]
then
	FILENAME=data.txt
else
	FILENAME=$2
fi

rm $FILENAME 2> /dev/null

for i in $(eval echo  {1..$THREADS}) 
do
	./run -r 100000000 -i 5 -n $i | tee -a  $FILENAME
done

	
