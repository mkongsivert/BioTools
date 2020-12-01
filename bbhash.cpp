/**
 * \file bbhash.cpp
 *
 * \authors Mackenzie Kong-Sivert
 *
 * \brief Implements the bbhash hash table
 */

#include "bbhash.hpp"
#include "xxhash.h"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace sdsl;

bbhash::bbhash()
{
    lengths_ = {0,0,0};
}

bbhash::bbhash(std::vector<std::string> keys, uint64_t n, uint64_t gamma)
{
    lengths_ = {0,0,0} // initialize all lengths to 0 by default
    std::vector<std::string> keys_to_hash = keys;
    size_ = n;
    uint64_t len = gamma*n;
    bit_vector all_tables[3];
    for (uint64_t i = 0; i < 3; ++i)
    {
        table = bit_vector(len, 0);
        lengths_[i] = len;
        uint64_t seed = std::rand(); // doesn't really need to be random
        seeds_[i] = seed;
        keys_to_hash = build_array(table, keys_to_hash, len, seed);
        all_tables[i] = table;
        if (keys_to_hash.empty())
        {
            break;
        }
        len = gamma*keys_to_hash.size();
        size_ += keys_to_hash.size();
    }
    // build regular hash table for remaining elements
    for (auto itr = keys_to_hash.begin(); itr != keys_to_hash.end(); ++itr)
    {
        remaining_.insert(*itr)
    }

    // "concatenate" all the bitvectors together
    // There should be a more efficient way to do this, but I couldn't find
    // any sort of concat feature for bit_vector types
    table_ = bit_vector(size_*gamma, 0);
    uint64_t pre = 0;
    for (uint64_t i = 0; i < 3; ++i)
    {
        for (uint64_t j = 0; j < all_tables[i].size())
        {
            table_[pre+j] = all_tables[i][j];
        }
        pre += all_tables[i].size();
    }
}

std::vector<std::string> bbhash::build_array(bit_vector table, std::vector<std::string> keys, uint64_t len, uint64_t seed)
{
    //build table of collisions
    bool coll[len] = {false};
    for (auto itr1 = keys.begin(); itr1 != keys.end(); ++itr1)
    {
        XXH64_hash_t hash = XXH64(*itr, sizeof(*itr), seed)%len;
        if (table[hash])
        {
            coll[hash] = true;
        }
        else
        {
            table[hash] = true;
        }
    }

    //build hash table and collision list
    std::vector<std::string> collisions;
    for (auto itr = keys.begin(); itr != keys.end; ++itr)
    {
        XXH64_hash_t hash = XXH64(itr->c_str(), itr->length(), seed)%len;
        if (coll[hash])
        {
            table[hash] = false;
            collisions.append(*itr)
        }
        
    }
    return collisions;
}

bool bbhash::lookup(std::string key)
{
    return lookup_helper(key, 0, 0);
}

bool bbhash::lookup_helper(std::string key, uint64_t ind, uint64_t pre)
{
    XXH64_hash_t hash = XXH64(key, sizeof(key), seeds_[ind])%lengths_[ind];
    if (ind < 3)
    {
        if (table_[key+pre])
        {
            return true;
        }
        else
        {
            lookup_helper(key, ind+1, pre+lengths_[ind]);
        }
    }
    else
    {
        return collisions_.count(key)>0;
    }
}

void bbhash::save(std::string& fname)
{

}

void bbhash::load(std::string& fname)
{

}

uint64_t bbhash::size()
{
    return size_;
}

std::ostream& bbhash::print(std::ostream& out) const
{
    out << size_;
    return out;
}

int main()
{
    std::vector<std::string> keys0;
    keys0.push_back("lovecats");
    keys0.push_back("lovesong");
    test0 = bbhash(keys0, 2, 1);
    return 0;
}