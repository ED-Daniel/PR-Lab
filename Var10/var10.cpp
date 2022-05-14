#include <iostream>
#include <omp.h>
#include <chrono>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>

void findConsonants(const std::string & text) {
    std::set<char> consonants {'q', 'w', 'r', 't', 'y', 'p', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'};
    std::set<char> found;

    for (size_t i = 0; i < text.size(); i++)
    {
        if (consonants.find(text[i]) != consonants.end()) {
            found.insert(text[i]);
        }
    }

    std::set<char> diff;
    std::set_difference(consonants.begin(), consonants.end(), found.begin(), found.end(), std::inserter(diff, diff.end()));

    for (auto i : diff) std::cout << i << " " << std::endl;
    
}

void findConsonantsParallel(const std::string & text) {
    // Все согласные записаны в сет
    std::set<char> consonants {'q', 'w', 'r', 't', 'y', 'p', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'};
    std::set<char> found; // Создаем сет для найденных согласных

    // Параллелим for
    #pragma omp parallel for
    for (size_t i = 0; i < text.size(); i++)
    {
        // Если нашли согласную
        if (consonants.find(tolower(text[i])) != consonants.end()) {
            found.insert(text[i]); // То добавляем ее в сет
        }
    }

    std::set<char> diff;
    // Высчитываем разницу двух сетов и записываем все в отдельный сет
    std::set_difference(consonants.begin(), consonants.end(), found.begin(), found.end(), std::inserter(diff, diff.end()));

    for (auto i : diff) std::cout << i << " " << std::endl; // Выводим на экран по заданию
    
}

// Вспомогательный метод для считывания всего текста в строку из файла
std::string readFileIntoString(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int main() {
    std::ofstream out("out1.txt");
    std::string test = readFileIntoString("test1.txt");
    
    auto start = std::chrono::high_resolution_clock::now();
    findConsonants(test);
    auto end = std::chrono::high_resolution_clock::now();

    out << "Sync time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    findConsonantsParallel(test);
    end = std::chrono::high_resolution_clock::now();

    out << "Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
    
    return 0;
}