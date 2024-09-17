gcc multithreading.c -o baseline

n_values=(100000000 1000000000 10000000000)
threads=(2 4 8)

# Run the program for each n value
for n in "${n_values[@]}"; do
    echo "-----------------------------------"

    echo "Running program for n = $n"
    
    # Use echo to provide input to the program
   
    for t in "${threads[@]}";do
        echo "Running program for $t threads"
        echo "$n\n$t" | ./baseline
    done

    echo "-----------------------------------"
done