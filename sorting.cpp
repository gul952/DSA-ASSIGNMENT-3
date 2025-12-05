#include "sorting.h"
#include <vector>
#include <algorithm>
#include <cmath>

// Helper to find min and max for range calculation
void getMinMax(const std::vector<Record>& arr, int& minVal, int& maxVal) {
    if (arr.empty()) return;
    minVal = arr[0].key;
    maxVal = arr[0].key;
    for (const auto& rec : arr) {
        if (rec.key < minVal) minVal = rec.key;
        if (rec.key > maxVal) maxVal = rec.key;
    }
}

// --- 1. Counting Sort (Stable) ---
void countingSortStable(std::vector<Record>& arr) {
    if (arr.empty()) return;

    int minVal, maxVal;
    getMinMax(arr, minVal, maxVal);
    int range = maxVal - minVal + 1;

    std::vector<int> count(range, 0);
    std::vector<Record> output(arr.size());

    // 1. Frequency Count
    for (const auto& rec : arr) {
        count[rec.key - minVal]++;
    }

    // 2. Cumulative Count
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    // 3. Build Output (Right-to-Left for Stability)
    for (int i = arr.size() - 1; i >= 0; i--) {
        int idx = arr[i].key - minVal;
        output[count[idx] - 1] = arr[i];
        count[idx]--;
    }

    // 4. Copy back
    arr = output;
}

// --- 2. Counting Sort (Non-Stable) ---
void countingSortUnstable(std::vector<Record>& arr) {
    if (arr.empty()) return;

    int minVal, maxVal;
    getMinMax(arr, minVal, maxVal);
    int range = maxVal - minVal + 1;

    std::vector<int> count(range, 0);

    // 1. Frequency Count
    for (const auto& rec : arr) {
        count[rec.key - minVal]++;
    }

    // 2. Overwrite input array
    // This destroys the original 'Record' structure association (instability)
    int index = 0;
    for (int i = 0; i < range; i++) {
        while (count[i] > 0) {
            // We just place the key. The 'id' at arr[index] is NOT moved with the key.
            // It remains whatever was there originally. This breaks stability.
            arr[index].key = i + minVal; 
            count[i]--;
            index++;
        }
    }
}

// --- 3. LSD Radix Sort ---
void radixSortLSD(std::vector<Record>& arr) {
    if (arr.empty()) return;

    int minVal, maxVal;
    getMinMax(arr, minVal, maxVal);
    
    // Handle negatives by shifting
    int shift = (minVal < 0) ? -minVal : 0;
    for(auto& r : arr) r.key += shift;
    int maxKey = maxVal + shift;

    // Do counting sort for every digit. exp is 10^i
    for (int exp = 1; maxKey / exp > 0; exp *= 10) {
        int n = arr.size();
        std::vector<Record> output(n);
        int count[10] = {0};

        for (int i = 0; i < n; i++)
            count[(arr[i].key / exp) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i].key / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        arr = output;
    }

    // Restore original keys
    if(shift > 0) {
        for(auto& r : arr) r.key -= shift;
    }
}

// --- 4. Bucket Sort ---
void bucketSort(std::vector<Record>& arr) {
    if (arr.empty()) return;
    
    int minVal, maxVal;
    getMinMax(arr, minVal, maxVal);
    
    int n = arr.size();
    int bucketCount = n; 
    std::vector<std::vector<Record>> buckets(bucketCount);
    long long range = (long long)maxVal - minVal + 1;
    
    for (int i = 0; i < n; i++) {
        int idx = (int)(((long long)(arr[i].key - minVal) * bucketCount) / range);
        if (idx >= bucketCount) idx = bucketCount - 1;
        buckets[idx].push_back(arr[i]);
    }
    
    int index = 0;
    for (int i = 0; i < bucketCount; i++) {
        // We use stable_sort to ensure the overall Bucket Sort is stable
        std::stable_sort(buckets[i].begin(), buckets[i].end(), 
            [](const Record& a, const Record& b) {
                return a.key < b.key;
            });
            
        for (const auto& item : buckets[i]) {
            arr[index++] = item;
        }
    }
}

// --- 5. Pigeonhole Sort ---
void pigeonholeSort(std::vector<Record>& arr) {
    if (arr.empty()) return;

    int minVal, maxVal;
    getMinMax(arr, minVal, maxVal);
    int range = maxVal - minVal + 1;

    std::vector<std::vector<Record>> holes(range);

    for (const auto& rec : arr) {
        holes[rec.key - minVal].push_back(rec);
    }

    int index = 0;
    for (int i = 0; i < range; i++) {
        for (const auto& rec : holes[i]) {
            arr[index++] = rec;
        }
    }
}