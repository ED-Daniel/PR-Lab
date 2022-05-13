#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>

double findArea(const std::vector<std::pair<double, double>> & coordinates) {
    double sum1 = 0;
    double sum2 = 0;

    for (size_t i = 0; i < coordinates.size() - 1; i++)
    {
        sum1 += coordinates[i].first * coordinates[i + 1].second;
        sum2 += coordinates[i + 1].first * coordinates[i].second;
    }

    return 0.5 * abs(
        sum1 - 
        sum2 + 
        coordinates[coordinates.size() - 1].first * coordinates[0].second -
        coordinates[0].first * coordinates[coordinates.size() - 1].second);
}

double findAreaParallel(const std::vector<std::pair<double, double>> & coordinates) {
    double sum1 = 0;
    double sum2 = 0;

    #pragma omp parallel for
    for (size_t i = 0; i < coordinates.size() - 1; i++)
    {
        sum1 += coordinates[i].first * coordinates[i + 1].second;
        sum2 += coordinates[i + 1].first * coordinates[i].second;
        std::cout << "Calculating by " << omp_get_thread_num() << "th thread" << std::endl;
    }

    return 0.5 * abs(
        sum1 - 
        sum2 + 
        coordinates[coordinates.size() - 1].first * coordinates[0].second -
        coordinates[0].first * coordinates[coordinates.size() - 1].second);
}

int main() {
    std::ifstream input("test3.txt");
    std::ofstream out("out(1000000).txt");

    std::vector<std::pair<double, double>> coors;

    if (!input) {
        std::cerr << "No file found" << std::endl;
        return 1;
    }

    while (!input.eof()) {
        double x, y;
        input >> x;
        input >> y;
        coors.emplace_back(x, y);
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto area = findArea(coors);
    auto end = std::chrono::high_resolution_clock::now();

    out << "Sync time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto area_p = findAreaParallel(coors);
    end = std::chrono::high_resolution_clock::now();

    out << "Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
    out << "Area: " << area_p << std::endl;
}