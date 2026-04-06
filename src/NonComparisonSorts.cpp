#include "NonComparisonSorts.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// Counting Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #1 -- Steps 1a, 1b, 1c (Count Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #2 -- Step 2 (Placement Phase)
//
void counting_sort(std::vector<int>& data) {
    if (data.size() <= 1) return;
    
    // 1. Find the min and max values
    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    
    // 2. Create a count array of size (max - min + 1)
    int k = (max_val - min_val) + 1;
    std::vector<int> count(k, 0);
    
    // 3. Count occurrences of each value
    for (int num : data) {
        count[num - min_val]++;
    }
    
    // 4. Overwrite data with sorted values using the counts
    int idx = 0;
    for (int i = 0; i < k; ++i) {
        while (count[i] > 0) {
            data[idx++] = i + min_val;
            count[i]--;
        }
    }
}

// ---------------------------------------------------------------------------
// Bucket Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #3 -- Steps 1a, 1b (Distribution Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #4 -- Steps 2-3 (Sort + Concatenate)
//
void bucket_sort(std::vector<int>& data, int num_buckets) {
    if (data.size() <= 1) return;
    
    // 1. Find the min and max values to determine bucket ranges
    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    
    if (min_val == max_val) return;
    
    // 2. Create buckets and distribute elements
    int range = max_val - min_val + 1;
    int range_per_bucket = static_cast<int>(std::ceil(static_cast<double>(range) / num_buckets));
    std::vector<std::vector<int>> buckets(num_buckets);
    
    for (int num : data) {
        int bucket_index = (num - min_val) / range_per_bucket;
        if (bucket_index >= num_buckets) bucket_index = num_buckets - 1;
        buckets[bucket_index].push_back(num);
    }
    
    // 3. Sort each bucket and concatenate back to data
    int idx = 0;
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
        for (int num : bucket) {
            data[idx++] = num;
        }
    }
}

// ---------------------------------------------------------------------------
// Radix Sort (LSD)
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #5 -- Step 0 + Pass 1 Detail
// ? SEE DIAGRAM: cpp_diagrams.md #6 -- Passes 2-3 (Stability in Action)
//
void radix_sort(std::vector<int>& data) {
    if (data.size() <= 1) return;
    
    // 1. Find the maximum value to determine the number of digits
    int max_val = *std::max_element(data.begin(), data.end());
    
    // Handle negative numbers by finding absolute max
    if (max_val < 0) {
        // If all negative, negate for processing, then negate back
        for (int& num : data) num = -num;
        max_val = *std::max_element(data.begin(), data.end());
        radix_sort(data);
        for (int& num : data) num = -num;
        std::reverse(data.begin(), data.end());
        return;
    }
    
    // 2. For each digit position (ones, tens, hundreds, ...)
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        // Counting sort on the current digit
        std::vector<int> count(10, 0);
        std::vector<int> output(data.size());
        
        // Count occurrences of each digit
        for (int num : data) {
            int digit = (num / exp) % 10;
            count[digit]++;
        }
        
        // Convert count to cumulative positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // Place elements in sorted order (traverse from right to left for stability)
        for (int i = static_cast<int>(data.size()) - 1; i >= 0; --i) {
            int digit = (data[i] / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
        }
        
        // Copy output back to data
        data = output;
    }
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------
void print_vector(const std::vector<int>& data, const std::string& label) {
    if (!label.empty()) std::cout << label << ": ";
    std::cout << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
