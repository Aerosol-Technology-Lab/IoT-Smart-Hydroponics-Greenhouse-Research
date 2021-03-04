#pragma once

template<typename T, typename Y>
struct Pair
{
    T first;
    Y second;

    Pair() {};

    Pair(T first, Y second) : first(first), second(second) {};

    Pair<T, Y> & operator=(const Pair<T, Y> &rhs)
    {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }
};
