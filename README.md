# ROBDD – Reduced Ordered Binary Decision Diagram in C

## Author
Dmytro Nitsenko

---

# Project Description

This project implements a Reduced Ordered Binary Decision Diagram (ROBDD) in the C programming language.

The implementation supports:
- recursive BDD construction,
- node reduction,
- duplicate subtree merging,
- variable order optimization,
- automatic testing with random Boolean expressions.

The project was created as part of a Data Structures and Algorithms assignment.

---

# Features

- Create a BDD from a Boolean expression
- Evaluate expressions using the BDD
- Reduce duplicate nodes using a unique table
- Optimize variable ordering
- Automatic correctness testing
- Memory cleanup

---

# Supported Boolean Expression Format

The program supports Boolean expressions using:

| Operator | Meaning |
|---|---|
| ! | NOT |
| + | OR |
| Concatenation | AND |

Examples:

text AB+!C A!B+CD !A!B+C 

---

# Example BDD Structure

text         A       /   \      0     1     /       \    B         C   / \       / \  0   1     1   1 

Each internal node represents a variable.
Each branch corresponds to assigning 0 or 1.
Terminal nodes contain the final result of the Boolean function.

---

# Main Functions

| Function | Description |
|---|---|
| BDD_create() | Creates a reduced BDD |
| BDD_use() | Evaluates the BDD |
| BDD_create_with_best_order() | Searches for a better variable order |
| buildBDD() | Recursively constructs the BDD |
| getUniqueNode() | Prevents duplicate nodes |
| testBDD() | Tests correctness |
| runTests() | Runs automatic tests |

---

# ROBDD Reduction

The implementation performs reduction during construction.

Two reduction rules are used:

1. If:
text low == high 
the node is skipped.

2. If an identical node already exists:
text same variable same low branch same high branch 
the existing node is reused.

This creates a canonical reduced BDD.

---

# Variable Order Optimization

The function:

c BDD_create_with_best_order() 

tries multiple random variable orders.

For each order:
1. a BDD is created,
2. the node count is measured,
3. the smallest BDD is selected.

Reduction percentage:

text (normalNodes - bestNodes) / normalNodes * 100% 

---

# Testing

The project contains automatic randomized testing.

The testing system:
- generates random Boolean expressions,
- creates normal and optimized BDDs,
- evaluates all possible input combinations,
- compares BDD results with direct expression evaluation.

---

# Example Output

text Manual test (AB+!C):  000 -> 1 001 -> 0 010 -> 1 011 -> 0  Random tests:  Test 1: A!B+CD | nodes normal = 45 best = 28 | reduction = 37.78% 

---

# Complexity

## BDD Construction

Worst case complexity:

text O(2^n) 

where n is the number of variables.

---

## BDD Traversal

BDD evaluation complexity:

text O(n) 

because only one path from root to terminal node is traversed.

---

# Compilation

Compile using GCC:

bash gcc main.c -o bdd 

---

# Run

bash ./bdd 

---

# Technologies Used

- C
- Dynamic memory allocation
- Recursion
- ROBDD reduction
- Randomized testing

---

# Conclusion

This project successfully implements a Reduced Ordered Binary Decision Diagram in C.

The optimized variable ordering significantly reduces the size of the resulting BDD in many cases while preserving correctness.
