#pragma once

#include <vector>
#include "FixedPoint/FixedPoint.hpp"
#include "PrimeField/PrimeField.hpp"

class RandGenerator
{
private:
    size_t n_hold = 1000;
    std::vector<long long int> rands;

    RandGenerator(){};
    ~RandGenerator(){};

    unsigned long long generateRand();

public:
    static RandGenerator *getInstance()
    {
        static RandGenerator obj;
        return &obj;
    };

    template <class T>
    T getRand(long long min_val = 0, long long max_val = 9223372036854775807ll);
    template <class T>
    std::vector<T> getRandVec(
        long long min_val = 0, long long max_val = 9223372036854775807ll, int n = 5
    );
};
