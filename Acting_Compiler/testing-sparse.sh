#!/bin/bash

echo ""
echo "Sparse M-M Matrix Comparisons"
x=1
arr=("M-M-sparse" "M-M-sparse-mod")
echo "Time for Loop(millisec),Element[0][0],Element[31][31],Element[510][0],Element[511][511]"
for i in "${arr[@]}"; do
    echo "$i,0,0,0"
    while [ $x -le 5 ]; do
        gcc -fopenmp $i.c -o m
        ./m
        x=$((x+1))
    done
    echo " "
    x=1
done`