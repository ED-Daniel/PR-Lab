#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>

std::vector<std::pair<double, double>> tab(double(*func)(double), double minVal, double maxVal, double step) {
    std::vector<std::pair<double, double>> result;
    for (double x = minVal; x < maxVal; x += step)
    {
        result.emplace_back(x, func(x));
    }
    return result;
}

std::vector<std::pair<double, double>> tab_parallel(double(*func)(double), double minVal, double maxVal, double step) {
    std::vector<std::pair<double, double>> result;
    
    int thread_num;

    double x = minVal;
    double threadIteration;

    bool stopCondition = false;
    bool threadStopCondition = false;
    
    omp_set_num_threads(4);

    #pragma omp parallel private(thread_num, threadIteration, threadStopCondition)
    {
        thread_num = omp_get_thread_num();
        while (!stopCondition) {
            #pragma omp critical
            {
                x += step;
                threadIteration = x;
            }

            threadStopCondition = x > maxVal;
            if (threadStopCondition) {
                stopCondition = true;
                #pragma omp flush(stopCondition)
            }

            result.emplace_back(x, func(x));
        }
    }

    return result;
} 

double f(double x) {
    return x * x;
}

int main() {
    std::ofstream out("out(-1000 1000 0.5).txt");
    double borders = 1000;

    auto start = std::chrono::high_resolution_clock::now();
    auto tabs = tab(f, -borders, borders, 0.5);
    auto end = std::chrono::high_resolution_clock::now();

    out << "Sync time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto tabs_p = tab_parallel(f, -borders, borders, 0.5);
    end = std::chrono::high_resolution_clock::now();

    out << "Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    // for (auto i : tabs_p) {
    //     out << "X: " << i.first << "\t" << "Y: " << i.second << std::endl;
    // }
}