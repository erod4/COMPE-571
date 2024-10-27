gcc mlfq.c -o mlfq


for i in {1..10}
do
echo "Execution $i"
./mlfq
done