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



int main()
{
    std::vector<size_t> testSizes = {1000, 2000, 4000, 8000};
    for (auto input : testSizes) {
        std::cout << "n = " << input << ":\n";

        std::cout << "  Vector: " 
             << timeContains<CompareItemName, std::vector>(input) << " ms\n";

        std::cout << "  List:   " 
             << timeContains<CompareItemName, std::list>(input) << " ms\n";

        std::cout << "  Hash:   " 
             << timeContains<CompareItemName, std::unordered_set>(input) << " ms\n";

        std::cout << "  Tree:   "
                << timeContainsTree<CompareItemName>(input) << " ms\n";
                
        std::cout << std::endl;
    }


    return 0;
}