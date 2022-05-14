#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex s_lock;

void fill(std::vector<int> & v) {
    int x;
    std::cin >> x;
    std::lock_guard<std::mutex> lock(s_lock);
    v.push_back(x);
}

void print(std::vector<int> & v) {
    std::lock_guard<std::mutex> lock(s_lock);
    if (v[v.size() - 1] == 0) std::cout << (v.size() - 1) << " ";
}

int main() {
    int size;
    std::cin >> size;
    std::vector<int> v(size);

    for (int i = 0; i < size; i++)
    {
        std::thread read(fill, v);
        std::thread print(print, v);

        read.join();
        print.join();
    }
    
}