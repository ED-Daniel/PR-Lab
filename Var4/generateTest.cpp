#include <iostream>
#include <fstream>
#include <random>

int main() {
    std::ofstream out("test4.txt");

    std::random_device rd;   // non-deterministic generator
    std::mt19937 gen(rd());  // to seed mersenne twister.
    std::uniform_int_distribution<> dist(-100, 100);

    for (size_t i = 0; i < 10000000; i++)
    {
        out << dist(gen) << " " << dist(gen) << "\n";
    }
}