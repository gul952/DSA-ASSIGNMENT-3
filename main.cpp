#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <map>
#include <string>
#include "sorting.h"

using namespace std;

// --- 1. DATA GENERATION HELPERS ---

enum DistType { RANDOM, NEARLY_SORTED, REVERSE, SKEWED };

vector<Record> generateData(int n, int k, DistType type) {
    vector<Record> data(n);
    random_device rd;
    mt19937 gen(rd());
    
    if (type == REVERSE) {
        for(int i=0; i<n; i++) data[i] = {n - i, i}; // Descending keys
    }
    else if (type == SKEWED) {
        // Zipfian-like approximation: Square the random normalized value
        // Result: Many small numbers, few large numbers
        uniform_real_distribution<> dis(0, 1);
        for(int i=0; i<n; i++) {
            double r = dis(gen);
            data[i] = {(int)(r * r * k), i};
        }
    }
    else { // RANDOM and NEARLY_SORTED
        uniform_int_distribution<> distrib(0, k);
        for(int i=0; i<n; i++) data[i] = {distrib(gen), i};
        
        if (type == NEARLY_SORTED) {
            // Sort first
            sort(data.begin(), data.end(), [](const Record& a, const Record& b){ return a.key < b.key; });
            // Swap 5% of pairs to make it "nearly" sorted
            int swaps = n * 0.05;
            if (swaps < 1) swaps = 1;
            uniform_int_distribution<> idxDis(0, n-1);
            for(int i=0; i<swaps; i++) {
                int a = idxDis(gen);
                int b = idxDis(gen);
                swap(data[a], data[b]);
            }
        }
    }
    return data;
}

// --- 2. VERIFICATION HELPERS ---

bool verify(const vector<Record>& arr, bool checkStability = true) {
    for (size_t i = 1; i < arr.size(); i++) {
        // Check Sorted Order
        if (arr[i].key < arr[i - 1].key) return false; 
        // Check Stability: If keys are equal, IDs must be increasing
        if (checkStability && arr[i].key == arr[i - 1].key) {
            if (arr[i].id < arr[i - 1].id) return false; 
        }
    }
    return true;
}

// Helper for the initial sanity check output
void runSingleCheck(string name, void (*sortFunc)(vector<Record>&), int n, int k) {
    auto data = generateData(n, k, RANDOM);
    auto start = chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration = end - start;
    bool stable = verify(data, true);
    
    cout << left << setw(25) << name 
         << " | Time: " << setw(8) << duration.count() << "ms"
         << " | Stable: " << (stable ? "YES" : "NO") << endl;
}

// --- 3. MEASUREMENT HELPER (For CSV Tables) ---

double getRunTime(void (*sortFunc)(vector<Record>&), vector<Record> data) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

int main() {

    cout << "==========================================================" << endl;
    cout << "PHASE 1: VERIFICATION & STABILITY CHECKS (n=10000)" << endl;
    cout << "==========================================================" << endl;
    
    int n = 10000;
    int k = 10000;
    
    // Test Stable Algorithms
    runSingleCheck("Counting Sort (Stable)", countingSortStable, n, k);
    runSingleCheck("LSD Radix Sort", radixSortLSD, n, k);
    runSingleCheck("Bucket Sort", bucketSort, n, k);
    runSingleCheck("Pigeonhole Sort", pigeonholeSort, n, k);

    // Test Unstable Algorithm explicitly
    auto dataUnstable = generateData(n, k, RANDOM);
    countingSortUnstable(dataUnstable);
    bool isStable = verify(dataUnstable, true); 
    cout << left << setw(25) << "Counting Sort (Unstable)" 
         << " | Time: " << setw(8) << "N/A" 
         << " | Stable: " << (isStable ? "YES" : "NO (Expected)") << endl;

    cout << endl;

    cout << "==========================================================" << endl;
    cout << "PHASE 2: GENERATING DATA FOR REPORT TABLES" << endl;
    cout << "==========================================================" << endl;
    
    // Map of algorithms to loop through easily
    map<string, void(*)(vector<Record>&)> algos;
    algos["Counting Sort"] = countingSortStable;
    algos["LSD Radix Sort"] = radixSortLSD;
    algos["Bucket Sort"] = bucketSort;
    algos["Pigeonhole Sort"] = pigeonholeSort;

    // Vary N, keep K approx N
    cout << "\n--- TABLE 2: SCALING (Copy to CSV/Excel) ---\n";
    cout << "N,Algorithm,Time_ms\n";
    vector<int> sizes = {1000, 10000, 50000, 100000}; 
    
    for (int currN : sizes) {
        for (auto const& [name, func] : algos) {
            // Generate Fresh Random Data
            auto data = generateData(currN, currN, RANDOM);
            double t = getRunTime(func, data);
            cout << currN << "," << name << "," << t << endl;
        }
    }

    // Fixed N, Vary K
    cout << "\n--- TABLE 3: RANGE SENSITIVITY (Copy to CSV/Excel) ---\n";
    cout << "K,Algorithm,Time_ms\n";
    int n_range = 10000;
    vector<int> ranges = {1000, 10000, 100000, 1000000}; 
    
    for (int currK : ranges) {
        cout << currK << ",Counting Sort," << getRunTime(countingSortStable, generateData(n_range, currK, RANDOM)) << endl;
        cout << currK << ",LSD Radix Sort," << getRunTime(radixSortLSD, generateData(n_range, currK, RANDOM)) << endl;
        cout << currK << ",Pigeonhole Sort," << getRunTime(pigeonholeSort, generateData(n_range, currK, RANDOM)) << endl;
    }

    // Fixed N, Fixed K, Vary Data Type
    cout << "\n--- TABLE 4: DISTRIBUTIONS (Copy to CSV/Excel) ---\n";
    cout << "Distribution,Algorithm,Time_ms\n";
    int n_dist = 20000;
    int k_dist = 20000;
    
    struct DistCase { string name; DistType type; };
    vector<DistCase> cases = {
        {"Random", RANDOM},
        {"Nearly Sorted", NEARLY_SORTED},
        {"Reverse", REVERSE},
        {"Skewed", SKEWED}
    };

    for (const auto& d : cases) {
        for (auto const& [name, func] : algos) {
            auto data = generateData(n_dist, k_dist, d.type);
            double t = getRunTime(func, data);
            cout << d.name << "," << name << "," << t << endl;
        }
    }

    return 0;
}
