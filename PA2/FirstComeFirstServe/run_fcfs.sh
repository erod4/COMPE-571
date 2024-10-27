#!/bin/bash

# Compile the fcfs.c file
gcc fcfs.c -o fcfs

# Run the compiled program in a loop 5 times
for i in {1..10}
do
echo "Execution $i"
  ./fcfs
done
