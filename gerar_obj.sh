#!/bin/bash
COM=gcc
for file in *.c; do
	new_string="${file:0:-2}"
	$COM -c -o bin/"${new_string}.o" $file -lncurses 	
	echo "${file} feito!"
done
