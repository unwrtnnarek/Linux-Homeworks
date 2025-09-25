#include "Sort.h"

void sortComplex(std::vector<Complex>& arr) {
    bool swapped;
    for (size_t i = 0; i < arr.size(); i++) {
        swapped = false;
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j].abs() > arr[j+1].abs()) {
                std::swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break; 
    }
}
