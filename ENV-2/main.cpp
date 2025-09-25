#include <iostream>
#include <vector>
#include "Complex.h"
#include "Sort.h"

int main() {
    std::vector<Complex> numbers = {
        Complex(3, 4),  
        Complex(1, 1),  
        Complex(0, 2),  
        Complex(5, 12),  
        Complex(2, 0)    
    };

    std::cout << "Before sorting:\n";
    for (const auto& c : numbers)
        std::cout << c << " |z|=" << c.abs() << "\n";

    sortComplex(numbers);

    std::cout << "\nAfter sorting:\n";
    for (const auto& c : numbers)
        std::cout << c << " |z|=" << c.abs() << "\n";

    return 0;
}
