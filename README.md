# Non-Comparison Sorting Algorithms: An Empirical Analysis

This repository contains the source code and experimental data for a technical report analyzing the performance characteristics of several non-comparison based sorting algorithms: Counting Sort, LSD Radix Sort, Bucket Sort, and Pigeonhole Sort.

The primary goal of this project is to empirically demonstrate how these algorithms circumvent the $\Omega(n \log n)$ lower bound applicable to comparison-based sorts, and to analyze their sensitivity to key distribution and input size.

## 📁 Project Structure

The project is structured into three main C++ files:

| File | Description |
|------|-------------|
| `sorting.h` | Defines the `Record` struct (used for stability checking) and declares the prototypes for all implemented sorting algorithms. |
| `sorting.cpp` | Contains the complete implementation of Counting Sort (Stable/Unstable), LSD Radix Sort, Bucket Sort, and Pigeonhole Sort. |
| `main.cpp` | The experimental driver. This file generates diverse data distributions, measures execution time, verifies correctness and stability, and outputs the results in CSV format for analysis. |

## 🛠️ Build and Run Instructions

This project requires a standard C++ compiler (like `g++` or `clang`) that supports the C++11 standard or newer.

### 1. Compilation

Navigate to the project directory and use the following command to compile the executable:

```bash
g++ main.cpp sorting.cpp -o sorting_analysis -std=c++11 -O3
```

The `-O3` optimization flag is highly recommended to obtain accurate, fast timing results.

### 2. Running the Experiment

Execute the compiled binary. The program will output the verification results first, followed by the raw data tables in a CSV-ready format.

```bash
./sorting_analysis
```

### Example Output Snippet (Verification Phase)

The initial output confirms correctness and stability:

```
==========================================================
PHASE 1: VERIFICATION & STABILITY CHECKS (n=10000)
==========================================================
Counting Sort (Stable)    | Time: 1.234ms | Stable: YES
LSD Radix Sort            | Time: 0.890ms | Stable: YES
Bucket Sort               | Time: 3.456ms | Stable: YES
Pigeonhole Sort           | Time: 1.567ms | Stable: YES
Counting Sort (Unstable)  | Time: N/A     | Stable: NO (Expected)

... (CSV Data Follows) ...
```

## 📊 Experimental Findings Summary

The analysis was divided into three main experiments:

### 1. Scaling Performance (Varying $N$)

**Finding:** The efficiency hypothesis was confirmed: all non-comparison algorithms demonstrated the expected linear scaling ($\mathcal{O}(n)$). Counting Sort achieved superior performance and the lowest execution times across all input sizes.

| Algorithm | Scaling Behavior ($k \approx n$) |
|-----------|-----------------------------------|
| **Counting Sort** | Fastest overall; near-perfect linear scaling. |
| **LSD Radix Sort** | Very fast; excellent scaling due to limited digit passes. |
| **Bucket Sort** | Highest overhead; performance gap widens as $N$ increases. |
| **Pigeonhole Sort** | Efficient, but slightly slower than Counting Sort. |

### 2. Range Sensitivity (Varying $K$)

**Finding:** Performance for $\mathcal{O}(n+k)$ algorithms (Counting Sort and Pigeonhole Sort) degrades directly as the key range ($K$) increases, while Radix Sort remains robust due to its digit-based approach.

- Pigeonhole Sort showed the most severe performance degradation as $K$ grew large ($K > N$), confirming its practical limitations to scenarios where $K = \mathcal{O}(N)$.
- Counting Sort's time increased linearly with $K$, as predicted by its $\mathcal{O}(n+k)$ complexity.
- LSD Radix Sort maintained consistently low times, validating its dependency on the number of digits ($d$), not the absolute range $K$.

### 3. Impact of Data Distribution

**Finding:** Algorithms that rely on uniformity assumptions, such as Bucket Sort, are significantly impacted by non-random distributions.

- Bucket Sort recorded the highest execution times, particularly under Skewed and Reverse distributions, aligning with its predicted $\mathcal{O}(n^2)$ worst-case complexity when its buckets become unevenly loaded.
- Counting Sort maintained its low execution time across all distributions, confirming its robustness regardless of the data ordering.
- LSD Radix Sort was also highly stable across different data types.
