#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <vector>
#include <algorithm>

template<typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class CustomPriorityQueue {
public:
    CustomPriorityQueue() {}

    bool empty() const {
        return c.empty();
    }

    std::size_t size() const {
        return c.size();
    }

    // Returns a reference to the top element
    const T& top() const {
        return c.front();
    }

    // Pushes an element and maintains the heap property
    void push(const T& value) {
        c.push_back(value);
        std::push_heap(c.begin(), c.end(), comp);
    }

    // Removes the top element and maintains the heap property
    void pop() {
        std::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

private:
    Container c;
    Compare comp;
};

#endif // CUSTOM_QUEUE_H
