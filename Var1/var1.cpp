#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>

std::vector<std::pair<double, double>> tab(double(*func)(double), double minVal, double maxVal, double step) {
    std::vector<std::pair<double, double>> result; // Создается вектор пар
    for (double x = minVal; x < maxVal; x += step)
    {
        result.emplace_back(x, func(x)); // Каждое значение записывается в результат
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

    #pragma omp parallel private(thread_num, threadIteration, threadStopCondition) // Создаем параллельную часть
    {
        thread_num = omp_get_thread_num();
        while (!stopCondition) {
            #pragma omp critical
            {
                x += step; // Прибавляем шаг
                threadIteration = x; // Записываем итерацию потока
            }

            threadStopCondition = x > maxVal; // Проверяем не стоит ли остановить поток
            if (threadStopCondition) {
                stopCondition = true; // Если поток останавился
                #pragma omp flush(stopCondition) // То завершаем весь цикл
            }

            result.emplace_back(x, func(x)); // Записываем значение в вектор
        }
    }

    return result;
} 

double f(double x) {
    return x * x;
}

int main() {
    std::ofstream out("out(-10000 10000 0.5).txt");
    double borders = 10000;

    auto start = std::chrono::high_resolution_clock::now();
    auto tabs = tab(f, -borders, borders, 0.5);
    auto end = std::chrono::high_resolution_clock::now();

    out << "Sync time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto tabs_p = tab_parallel(f, -borders, borders, 0.5);
    end = std::chrono::high_resolution_clock::now();

    out << "Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    for (auto i : tabs_p) {
        out << "X: " << i.first << "\t" << "Y: " << i.second << std::endl;
    }
}