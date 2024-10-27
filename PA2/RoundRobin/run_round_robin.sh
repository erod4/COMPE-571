#!/bin/bash

# Compile the fcfs.c file
gcc round_robin.c -o round_robin

# Run the compiled program in a loop 5 times
for i in {1..10}
do
echo "Execution $i"
  ./round_robin
done
