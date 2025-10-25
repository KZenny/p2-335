#include "Compare.hpp"
#include "HashInventory.hpp"
#include "Inventory.hpp"
#include "Item.hpp"
#include "ItemAVL.hpp"
#include "TreeInventory.hpp"
#include <iostream>
#include <list>
#include <vector>
#include <unordered_set>
#include <chrono>
#include "ItemGenerator.hpp"

template <typename Comparator, typename Container>
float timeQuery(size_t n, bool byWeight = false) {
    float total_time = 0.0f;
    ItemGenerator gen(42);
    Inventory<Comparator, Container> inv;

    // Insert n random items
    for (size_t i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    for (int i = 0; i < 10; ++i) {
        Item start, end;

        if (byWeight) {
            // Test 2: CompareItemWeight
            float w = gen.randomFloat(0.1, 30.0);
            start.weight_ = w;
            end.weight_ = w + 0.1f;
        } else {
            // Test 1: CompareItemName
            start.name_ = gen.randomUsedName();
            end.name_ = gen.randomUsedName();

            Comparator comp;
            if (comp.lessThan(end, start)) {
                std::swap(start, end);
            }
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        auto result = inv.query(start, end);
        auto t2 = std::chrono::high_resolution_clock::now();

        total_time += std::chrono::duration<float, std::milli>(t2 - t1).count();
    }

    return total_time / 10.0f;
}

template<class Comparator, template<class...> class Container>
float timeContains(size_t n) {
    ItemGenerator gen(42); 

    Inventory<Comparator, Container<Item>> inv;

    for (size_t i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    std::vector<std::string> contained;
    for (int i = 0; i < 100; ++i) {
        contained.push_back(gen.randomUsedName());
    }

    std::vector<std::string> missing;
    for (int i = 0; i < 100; ++i) {
        missing.push_back(gen.randomItem().name_);
    }

    float total_ms = 0.0f;

    for (const auto& name : contained) {
        auto start = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto end = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<float, std::milli>(end - start).count();
    }

    for (const auto& name : missing) {
        auto start = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto end = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<float, std::milli>(end - start).count();
    }

    return total_ms / 200.0f;
}

template <class Comparator>
float timeContainsTree(size_t n)
{
    ItemGenerator gen(42);           
    Inventory<Comparator, Tree> inv;  

    for (size_t i = 0; i < n; ++i)
        inv.pickup(gen.randomItem());

    std::vector<std::string> contained;
    for (int i = 0; i < 100; ++i)
        contained.push_back(gen.randomUsedName());

    std::vector<std::string> missing;
    for (int i = 0; i < 100; ++i)
        missing.push_back(gen.randomItem().name_);


    float total_ms = 0.0f;

    for (const auto& name : contained) {
        auto start = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto end = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<float, std::milli>(end - start).count();
    }

    for (const auto& name : missing) {
        auto start = std::chrono::high_resolution_clock::now();
        inv.contains(name);
        auto end = std::chrono::high_resolution_clock::now();
        total_ms += std::chrono::duration<float, std::milli>(end - start).count();
    }
    return total_ms / 200.0f;
}

int main()
{
    std::cout << "================================= Contains Timing =================================\n";
    std::vector<size_t> testSizes = {1000, 2000, 4000, 8000,};
    for (auto input : testSizes) {
        std::cout << "n = " << input << ":\n";

        std::cout << "  Vector: " << timeContains<CompareItemName, std::vector>(input) << " ms\n";

        std::cout << "  List:   " << timeContains<CompareItemName, std::list>(input) << " ms\n";

        std::cout << "  Hash:   " << timeContains<CompareItemName, std::unordered_set>(input) << " ms\n";

        std::cout << "  Tree:   " << timeContainsTree<CompareItemName>(input) << " ms\n";
        std::cout << std::endl;
    }

    std::cout << "================================= Query Timing =================================\n";
    std::vector<size_t> inputs = {1000, 2000, 4000, 8000};
    for (auto n : inputs) {
        std::cout << "n = " << n << ":\n";

        std::cout << "  Vector (Name): " << timeQuery<CompareItemName, std::vector<Item>>(n) << " ms\n";
        std::cout << "  List (Name):   " << timeQuery<CompareItemName, std::list<Item>>(n) << " ms\n";
        std::cout << "  Hash (Name):   " << timeQuery<CompareItemName, std::unordered_set<Item>>(n) << " ms\n";
        std::cout << "  Tree (Name):   " << timeQuery<CompareItemName, Tree>(n) << " ms\n";

        std::cout << "\n  Vector (Weight): " << timeQuery<CompareItemWeight, std::vector<Item>>(n, true) << " ms\n";
        std::cout << "  List (Weight):   " << timeQuery<CompareItemWeight, std::list<Item>>(n, true) << " ms\n";
        std::cout << "  Hash (Weight):   " << timeQuery<CompareItemWeight, std::unordered_set<Item>>(n, true) << " ms\n";std::cout << "  Tree (Weight):   " << timeQuery<CompareItemWeight, Tree>(n, true) << " ms\n";
    }
    return 0;
}