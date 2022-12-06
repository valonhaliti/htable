#include "Hashset.h"

#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

void Hashset::insert(uint64_t value)
{
    uint64_t hash = hash_func(value);
    auto& d = dir[hash];
    entries.emplace_back(value, d);
    d = &entries.back();
}

Hashset::Entry* Hashset::find(uint64_t value)
{
    uint64_t hash = hash_func(value);
    auto& d = dir[hash];
    for (auto e = d; e; e = e->next)
        if (e->value == value)
            return e;
    return nullptr;
}

void Hashset::del(uint64_t value)
{
    auto hash = hash_func(value);
    bool first = true;

    auto& d = dir[hash];
    Entry* prev = nullptr;

    for (auto e = d; e; e = e->next) {
        if (e->value == value) {
            if (prev != nullptr) {
                prev->next = e->next;
            } else {
                prev = e->next;
            }
            break;
        }
        prev = e;

        first = false;
    }

    if (first) {
        d = prev;
    }
}

uint64_t Hashset::hash_func(uint64_t value)
{
    return value % table_size;
}