gcc multithreading.c -o multithreading

n_values=(100000000 1000000000 10000000000)
threads=(2 4 8)

# Run the program for each n value
for n in "${n_values[@]}"; do
    echo "\033[0;32m\n\n----------------------------------------------------------------------------------------------------------\033[0m"
    echo "\033[0;32mRunning program for n = $n\033[0m"
    echo "\033[0;32m----------------------------------------------------------------------------------------------------------\033[0m"

    # Use echo to provide input to the program
   
    for t in "${threads[@]}";do
        echo "\n\nRunning program for $t threads"
        echo "$n\n$t" | ./multithreading
    done

    echo "\033[0;32m----------------------------------------------------------------------------------------------------------\033[0m"
done