#include "Compare.hpp"

/**
* @brief Compares two items by name
* @param a First item to compare
* @param b Second item to compare
* @return true if a's name is less than b's name
*/
bool CompareItemName::lessThan(const Item& a, const Item& b) {
    return a.name_ < b.name_;
}

/**
* @brief Checks if two items have equal names
* @param a First item to compare
* @param b Second item to compare
* @return true if items have equal names
*/
bool CompareItemName::equal(const Item& a, const Item& b) {
    return a.name_ == b.name_;
}

/**
* @brief Checks if first item's name is less than or equal to second item's
* name
* @param a First item to compare
* @param b Second item to compare
* @return true if a's name is less than or equal to b's name
*/
bool CompareItemName::leq(const Item& a, const Item& b) {
    return lessThan(a, b) || equal(a, b);
}


bool CompareItemWeight::lessThan(const Item& a, const Item& b)
{
    return a.weight_ < b.weight_;
}

bool CompareItemWeight::equal(const Item& a, const Item& b)
{
    return std::abs(a.weight_ - b.weight_) < 0.00001; // Note the threshold for float equality (we can't just use
                                                      // '==')
}

bool CompareItemWeight::leq(const Item& a, const Item& b)
{
    return lessThan(a, b) || equal(a, b);
}

bool CompareItemType::lessThan(const Item& a, const Item& b)
{
    return a.type_ < b.type_;
}

bool CompareItemType::equal(const Item& a, const Item& b)
{
    return a.type_ == b.type_;
}

bool CompareItemType::leq(const Item& a, const Item& b)
{
    return lessThan(a, b) || equal(a, b);
}