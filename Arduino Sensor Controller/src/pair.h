#pragma once

template<typename T, typename Y>
struct Pair
{
    T first;
    Y second;

    Pair() {};

    Pair(const T  &first, const Y  &second) : first(first), second(second) {};
    Pair(      T &&first, const Y  &second) : second(second) { this->first = first; };
    Pair(const T  &first,       Y &&second) : first(first) { this->second = second; };
    Pair(      T &&first,       Y &&second) { this->first = first; this->second = second; };

    Pair<T, Y> & operator=(const Pair<T, Y> &rhs)
    {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }
};
