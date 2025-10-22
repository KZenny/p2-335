#include "HashInventory.hpp"

/**
 * @brief Default constructor for the Inventory template class.
 *
 * Initializes an empty inventory with no items, no equipped item,
 * and zero total weight.
 *
 * @tparam Comparator The comparison class for querying items
 */
template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::Inventory() : items_{std::unordered_set<Item>{}}, equipped_{nullptr}, weight_{0.0} {}

/**
 * @brief Retrieves the value stored in `equipped_`
 * @return The Item pointer stored in `equipped_`
 */
template <class Comparator>
Item* Inventory<Comparator, std::unordered_set<Item>>::getEquipped() const
{
    return equipped_;
}

/**
 * @brief Equips a new item.
 * @param itemToEquip A pointer to the item to equip.
 * @post Updates `equipped` to the specified item
 * without deallocating the original.
 */
template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::equip(Item* itemToEquip)
{
    equipped_ = itemToEquip;
}

/**
 * @brief Discards the currently equipped item.
 * @post Deallocates the item pointed to by `equipped`
 * and sets `equipped` to nullptr, if `equipped` is not nullptr already.
 */
template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::discardEquipped()
{
    if (equipped_ != nullptr) {
        delete equipped_;
        equipped_ = nullptr;
    }
}

/**
 * @brief Retrieves the value stored in `weight_`
 * @return The float value stored in `weight_`
 */
template <class Comparator>
float Inventory<Comparator, std::unordered_set<Item>>::getWeight() const
{
    return weight_;
}

/**
 * @brief Retrieves the size of items_
 */
template <class Comparator>
size_t
Inventory<Comparator, std::unordered_set<Item>>::size() const
{
    return items_.size();
}

/**
 * @brief Retrieves a copy of the container holding inventory items.
 *
 * @return Container of items in the inventory
 */
template <class Comparator>
std::unordered_set<Item>
Inventory<Comparator, std::unordered_set<Item>>::getItems() const
{
    return items_;
}

/**
 * @brief Attempts to add a new item to the inventory.
 *
 * @param target Item to be added to the inventory
 * @return true if the item was successfully added, false if an item
 *         with the same name already exists
 * @post Updates the weight_ member to reflect the new Item pickup
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::pickup(const Item& target)
{
    auto result = items_.insert(target); // returns a pair (iterator, bool)
    if (result.second) { // insertion is successful
        weight_ += target.weight_; // update weight_ to reflect the new item pickup
        return true; // successful pickup
    } else { //
        return false; // item with same name already exists, not added to items_
    }
}

/**
 * @brief Attempts to remove an item from the inventory by name.
 *
 * @param name Name of the item to be removed
 * @return true if the item was successfully removed, false if the
 *         item was not found in the inventory
 * @post Updates the weight_ member to reflect removing the Item
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::discard(
    const std::string& itemName)
{
    auto itr = items_.find(Item(itemName)); 
    if (itr != items_.end()) { // item is found
        weight_ -= itr->weight_; // update weight_ to reflect the item removal
        items_.erase(itr); // remove the item from the inventory
        return true; // successful removal
    } else {
        return false; // item not found, removal failed
    }
}

/**
 * @brief Checks if an item with the given name exists in the inventory.
 *
 * @param name Name of the item to search for
 * @return true if the item exists in the inventory, false otherwise
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::contains(
    const std::string& itemName) const
{
    auto itr = items_.find(Item(itemName));
    return itr != items_.end();
}

/**
 * @brief Queries the inventory for items within a specified range.
 *
 * Returns a set of items that fall between the start and end items
 * according to the specified Comparator (inclusive on both ends)
 *
 * @param start An Item whose compared property is the lower bound of the query range
 * @param end An Item whose compared property is the upper bound of the query range
 * @return std::unordered_set of items within the specified range
 *
 * @note Returns an empty set if the end item is less than the start item
 * @example To select all Items with weights 0.4 to 10.9, we'd setup the class & parameters as such:
 *  - this Inventory object is of type Inventory<CompareItemWeight>
 *  - start = Item("some_name", 0.4, ItemType::None)
 *  - end = = Item("some_other_name", 10.9, ItemType::None)
 *
 */
template <class Comparator>
std::unordered_set<Item>
Inventory<Comparator, std::unordered_set<Item>>::query(const Item& start,
    const Item& end) const
{
    std::unordered_set<Item> result_set;
    if(Comparator::lessThan(end, start)) {
        return result_set; // empty set if end < start
    }

    for (const auto& item : items_) { // iterate through all items in inventory
        if (Comparator::leq(start, item) && Comparator::leq(item, end)) { //  if item is within range, insert into result_set
            result_set.insert(item);
        }
    }

    return result_set; // return the set of items within the specified range
}

/**
 * @brief Destructor for the Inventory class.
 * @post Deallocates any dynamically allocated resources.
 */
template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::~Inventory()
{
    discardEquipped();
}