#include <iostream>
#include <chrono>
#include <vector>
using namespace std::chrono;

int main() {

    vector<int> registerValues;

    constexpr std::chrono::year y{2025};
    std::cout << "Welcome to the " << static_cast<int>(y) << " Virtual CPU!" << std::endl;

    int* A = new int(5);
    std::cout << "The number is: " << *A << std::endl;
    
    delete A;
    
    return 0;
}