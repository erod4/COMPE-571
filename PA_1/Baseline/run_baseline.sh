gcc baseline.c -o baseline

n_values=(100000000 1000000000 10000000000)

# Run the program for each n value
for n in "${n_values[@]}"; do
    echo "-----------------------------------"

    echo "Running program for n = $n"
    
    # Use echo to provide input to the program
    echo "$n" | ./baseline
    
    echo "-----------------------------------"
done