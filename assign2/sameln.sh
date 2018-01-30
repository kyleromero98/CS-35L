#!/bin/bash

IFS=$'\n'
dir=$1

filelist=`ls -a $dir | sort` #get files and sort lexicographically

declare -a dot
declare -a reg

dotcount=0
regcount=0

for file in $filelist
do
    fullpath="$dir/$file"
    if [ ! -r $fullpath ]
    then
	echo "Error: You do not have valid read permissions"
    fi
    if [[ -f $fullpath && ! -L $fullpath ]]
    then
	if [[ $file == .* ]]
	then
	    dot[$dotcount]="$dir/$file"
	    dotcount=$((dotcount + 1))
	else
	    reg[$regcount]="$dir/$file"
	    regcount=$((regcount + 1 ))
	fi
    fi
done    

dot=("${dot[@]}" "${reg[@]}")
count=${#dot[@]}

for (( i=0; i<$count; i++ ))
do
    for (( j=$i+1; j<$count; j++ ))
    do
	cmp -s "${dot[$i]}" "${dot[$j]}"
	if [ $? -eq 0 ]
	then
	    ln -f "${dot[$i]}" "${dot[$j]}"
	fi
    done
done
