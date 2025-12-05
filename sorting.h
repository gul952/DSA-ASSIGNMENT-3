#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <algorithm>
#include <iostream>

// The data structure used for stability testing
struct Record {
    int key;
    int id; // Unique sequential identifier to verify stability

    // Helper to print records
    friend std::ostream& operator<<(std::ostream& os, const Record& r) {
        return os << "{" << r.key << ", id:" << r.id << "}";
    }
};

// 1. Counting Sort (Stable) - As described in Algorithm 1
void countingSortStable(std::vector<Record>& arr);

// 2. Counting Sort (Non-Stable) - As described in Section 3.1.2
void countingSortUnstable(std::vector<Record>& arr);

// 3. LSD Radix Sort - As described in Algorithm 2
void radixSortLSD(std::vector<Record>& arr);

// 4. Bucket Sort - As described in Algorithm 3
void bucketSort(std::vector<Record>& arr);

// 5. Pigeonhole Sort - As described in Algorithm 4
void pigeonholeSort(std::vector<Record>& arr);

#endif // SORTING_H