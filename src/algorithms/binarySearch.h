#include <algorithm>

template<typename Iterator, typename Predicate>
Iterator binarySearch(Iterator first, Iterator last, const Predicate &predicate) {
    while (first < last) {
        Iterator middle = first + std::distance(first, last) / 2;
        if (predicate(*middle)) {
            last = middle;
        } else {
            first = middle + 1;
        }
    }
    return first;
}