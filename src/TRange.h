//
// Created by dmitry on 10.10.23.
//
#include <chrono>
#ifndef ALLOCATOR_TRANGE_H
#define ALLOCATOR_TRANGE_H
class TRange
{
    class RangeIterator
    {
    public:
        RangeIterator(int value_)
                : value(value_)
                {
        }

        bool operator!=(RangeIterator const& other) const
        {
            return value != other.value;
        }

        int const& operator*() const
        {
            return value;
        }

        RangeIterator& operator++()
        {
            ++value;
            return *this;
        }

    private:
        int value;
    };
public:
    TRange(int from, int to)
            : from(from), to(to)
            {
                start = std::chrono::steady_clock::now();
            }

    TRange(int to)
            : from(0), to(to)
            {
                start = std::chrono::steady_clock::now();
            }

    ~TRange()
    {
        stop = std::chrono::steady_clock::now();
        std::cout << " time ns: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()
        << std::endl;
    }

    RangeIterator begin() const
    {
        return RangeIterator(from);
    }

    RangeIterator end() const
    {
        return RangeIterator(to);
    }

private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point stop;
    int const from;
    int const to;

};

#endif //ALLOCATOR_TRANGE_H
