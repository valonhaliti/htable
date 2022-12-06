#ifndef INCLUDE_HASHSET_H
#define INCLUDE_HASHSET_H

#include "shared_data.h"
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <iostream>
#include <random>
#include <shared_mutex>
#include <vector>

class Hashset {

private:
    uint64_t hash_func(uint64_t value);

public:
    struct Entry {
        uint64_t value;
        Entry* next = nullptr;
        Entry(uint64_t v, Entry* n)
            : value(v)
            , next(n)
        {
        }
    };

    size_t table_size;
    std::vector<Entry*> dir;
    std::deque<Entry> entries;

    void insert(uint64_t value);

    Entry* find(uint64_t value);

    void del(uint64_t value);

    Hashset(size_t size)
        : dir(size)
        , table_size(size)
    {
    }
};

#endif