#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <optional>
#include <algorithm>

// Function to perform insertion sort
void insertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Bubble sort implementation (fixed)
void bubbleSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    for (size_t n = arr.size(); n > 1; --n) {
        bool swapped = false;
        for (size_t i = 0; i + 1 < n; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // already sorted
    }
}

// Simple LRU cache: front = most recent, back = least recent
template <typename K, typename V>
class LRUCache {
    size_t capacity_;
    std::list<std::pair<K, V>> items_; // front = most recent
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map_;
public:
    explicit LRUCache(size_t capacity) : capacity_(capacity) {}

    bool get(const K& key, V& out) {
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        items_.splice(items_.begin(), items_, it->second);
        out = it->second->second;
        return true;
    }

    std::optional<K> put(const K& key, const V& value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return std::nullopt;
        }
        items_.emplace_front(key, value);
        map_[key] = items_.begin();
        if (map_.size() > capacity_) {
            auto last = items_.end(); --last;
            K evictedKey = last->first;
            map_.erase(last->first);
            items_.pop_back();
            return evictedKey;
        }
        return std::nullopt;
    }

    void display() const {
        std::cout << "[MRU] ";
        bool first = true;
        for (auto it = items_.begin(); it != items_.end(); ++it) {
            if (!first) std::cout << " -> ";
            std::cout << it->first;
            first = false;
        }
        std::cout << " [LRU]\n";
    }

    size_t size() const { return map_.size(); }
    size_t capacity() const { return capacity_; }
};

// Demo: show initial mixed list (cake ingredients + junk), then reorganize using accesses
// and evict items when capacity (10) is exceeded. Only show before and after plus evicted items.
void runLRUDemo() {
    std::vector<std::string> cakeIngredients = {
        "flour", "sugar", "eggs", "butter", "milk", "baking powder", "vanilla"
    };

    std::vector<std::string> junkItems = {
        "soap", "screwdriver", "nail", "coin", "candle"
    };

    std::vector<std::string> initialList = {
        "soap", "flour", "coin", "screwdriver", "sugar", "candle",
        "eggs", "butter", "nail", "milk", "baking powder", "vanilla"
    };

    const size_t capacity = 10; // limit
    LRUCache<std::string, std::string> cache(capacity);
    std::vector<std::string> evictedItems;

    std::cout << "\nInitial mixed list (items that belong to a cake and items that don't):\n";
    for (const auto& it : initialList) {
        std::cout << it << (it == initialList.back() ? "\n" : ", ");
    }

    // Populate cache silently, collecting evicted items
    for (const auto& item : initialList) {
        auto evicted = cache.put(item, item);
        if (evicted) evictedItems.push_back(*evicted);
    }

    // Simulate accesses to prioritize cake ingredients
    std::vector<std::string> accesses = {
        "eggs", "sugar", "flour", "eggs", "vanilla", "milk", "butter"
    };
    std::string tmp;
    for (const auto& a : accesses) {
        cache.get(a, tmp);
    }

    std::cout << "\nAfter reorganization (most-used cake ingredients moved to front):\n";
    cache.display();

    if (!evictedItems.empty()) {
        std::cout << "\nItems evicted because capacity exceeded (" << capacity << "):\n";
        for (size_t i = 0; i < evictedItems.size(); ++i) {
            std::cout << evictedItems[i] << (i + 1 == evictedItems.size() ? "\n" : ", ");
        }
    }
    else {
        std::cout << "\nNo items were evicted.\n";
    }

    std::cout << '\n';
}

int main()
{
    std::cout << "Hello World!\n";

    std::vector<int> data = { 12, 11, 13, 5, 6, 2, 7, 8, 5 };

    std::cout << "Given Array: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << '\n';

    std::cout << "Press I for Insertion sort, B for Bubble sort, or L for LRU demo: ";
    char c = std::cin.get();

    if (c == 'I' || c == 'i') {
        std::cout << "\nPerforming Insertion Sort...\n";
        insertionSort(data);

        std::cout << "\nSorted array (Insertion Sort): ";
        for (int num : data) {
            std::cout << num << " ";
        }
        std::cout << '\n';
    }
    else if (c == 'B' || c == 'b') {
        std::cout << "\nPerforming Bubble Sort...\n";
        bubbleSort(data);

        std::cout << "\nSorted array (Bubble Sort): ";
        for (int num : data) {
            std::cout << num << " ";
        }
        std::cout << '\n';
    }
    else if (c == 'L' || c == 'l') {
        runLRUDemo();
    }
    else {
        std::cout << "\nInvalid input. Exiting program.\n";
        return 0;
    }

    return 0;
}