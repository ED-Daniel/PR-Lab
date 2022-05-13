#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <math.h>
#include <fstream>

void func(std::vector<double> & numbers) {
    for (size_t i = 0; i < numbers.size(); i++)
        numbers[i] = round(numbers[i] * 100) / 100.0;
}

void func_parallel(std::vector<double> & numbers) {
    #pragma omp parallel for
    for (size_t i = 0; i < numbers.size(); i++)
        numbers[i] = round(numbers[i] * 100) / 100.0;
}

int main() {
    std::ofstream out("out(100000).txt");
    std::ifstream input("test3.txt");

    std::vector<double> data1;
    std::vector<double> data2;

    if (!input) {
        std::cerr << "file not found" << std::endl;
        return 1;
    }

    while (!input.eof()) {
        double inp;
        input >> inp;
        data1.push_back(inp);
        data2.push_back(inp);
    }

    auto start = std::chrono::high_resolution_clock::now();
    func(data1);
    auto end = std::chrono::high_resolution_clock::now();

    out << "Sync time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    func_parallel(data2);
    end = std::chrono::high_resolution_clock::now();

    out << "Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    for (size_t i = 0; i < 10; i++)
    {
        out << data2[i] << " ";
    }
    
    out << "..." << std::endl;

    return 0;
}