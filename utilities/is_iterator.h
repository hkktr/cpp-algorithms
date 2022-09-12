#ifndef CPP_ALGORITHMS_UTILITIES_IS_ITERATOR_H
#define CPP_ALGORITHMS_UTILITIES_IS_ITERATOR_H

#include <iterator>
#include <type_traits>

template <class T, class = void>
constexpr bool is_iterator = false;

template <class T>
constexpr bool is_iterator<
    T, std::void_t<typename std::iterator_traits<T>::iterator_category>> = true;

#endif  // CPP_ALGORITHMS_UTILITIES_IS_ITERATOR_H