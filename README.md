# Data Structures Benchmark in C

## Author
Oleksandr Nitsenko

---

# Project Description

This project benchmarks and compares the performance of several fundamental data structures implemented in C.

The goal of the project is to analyze:
- insertion performance,
- search performance,
- deletion performance,
- behavior under different input scenarios,
- scalability for increasing input sizes.

The project was created as part of the Data Structures and Algorithms course.

---

# Implemented Data Structures

The following data structures are implemented:

- AVL Tree
- Splay Tree
- Hash Table (Separate Chaining)
- Hash Table (Double Hashing)

Each structure supports:
- INSERT
- SEARCH
- DELETE

---

# Tested Scenarios

The benchmark tests several input distributions:

## Sorted Data

text id="ygq46y" 1, 2, 3, 4, 5, ... 

## Reverse Sorted Data

text id="9w2n5j" n, n-1, n-2, ... 

## Random Data

text id="b8upbh" 3, 8, 1, 5, 2, ... 

These scenarios help analyze how different structures behave depending on input order.

---

# Benchmark Sizes

The benchmark was executed with multiple dataset sizes:

text id="qgwpk2" 1 000 10 000 20 000 40 000 60 000 80 000 100 000 

---

# Measurement Methodology

- Time is measured using clock()
- Results are converted to microseconds per operation
- Each test is repeated 10 times
- Final values are averaged

Formula:

text id="on9dwb" time_per_operation = total_time / n 

---

# Output Format

Benchmark results are exported in CSV format:

text id="tth5do" STRUCTURE,SCENARIO,N,OPERATION,TIME 

Example:

text id="r6nmfx" AVL,SORTED,10000,INSERT,0.123 

---

# Graph Visualization

Graphs were generated using Python.

Each graph contains:
- X-axis → number of elements
- Y-axis → time per operation (µs)

Separate graphs were created for:
- INSERT
- SEARCH
- DELETE

---

# Performance Summary

## AVL Tree
- Stable O(log n)
- Predictable performance
- Not sensitive to input order

## Splay Tree
- Adaptive structure
- Fast repeated access
- Sensitive to access patterns

## Hash Table (Chaining)
- Average O(1)
- Handles collisions robustly

## Hash Table (Double Hashing)
- Memory efficient
- Very fast for large datasets
- Sensitive to load factor

---

# Compilation

Compile using GCC:

bash id="uzpbhy" gcc main.c -o benchmark 

---

# Run

bash id="rpxyyg" ./benchmark 

---

# Technologies Used

- C
- Dynamic memory allocation
- AVL Trees
- Splay Trees
- Hash Tables
- Benchmarking
- CSV output
- Python graph visualization

---

# Conclusion

The benchmark demonstrates significant differences between balanced trees and hash tables.

Hash tables provide the best average-case performance for large datasets, while AVL trees offer the most stable and predictable behavior.

The project also shows how input distribution can strongly affect the efficiency of certain data structures, especially self-adjusting trees such as Splay Trees.
