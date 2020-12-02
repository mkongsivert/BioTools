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

}

bbhash::bbhash(std::vector<std::string> keys, uint64_t n, uint64_t gamma)
{
    std::vector<std::string> keys_to_hash = keys;
    size_ = n;
    uint64_t len = gamma*n;
    bit_vector all_tables[3];
    for (uint64_t i = 0; i < 3; ++i)
    {
        bit_vector table = bit_vector(len, 0);
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
        remaining_.insert(*itr);
    }

    // "concatenate" all the bitvectors together
    // There should be a more efficient way to do this, but I couldn't find
    // any sort of concat feature for bit_vector types
    table_ = bit_vector(size_*gamma, 0);
    uint64_t pre = 0;
    for (uint64_t i = 0; i < 3; ++i)
    {
        for (uint64_t j = 0; j < all_tables[i].size(); ++j)
        {
            table_[pre+j] = all_tables[i][j];
        }
        pre += all_tables[i].size();
    }
}

std::vector<std::string> bbhash::build_array(bit_vector table, std::vector<std::string> keys, uint64_t len, uint64_t seed)
{
    //build table of collisions
    bit_vector coll = bit_vector(len, 0);
    for (auto itr = keys.begin(); itr != keys.end(); ++itr)
    {
        XXH64_hash_t hash = XXH64(static_cast<void*>(const_cast<char*>(itr->c_str())), itr->length(), seed)%len;
        std::cout << *itr << " hashes to " << hash << std::endl;
        if (table[hash])
        {
            coll[hash] = 1;
        }
        else
        {
            table[hash] = 0;
        }
    }

    //build hash table and collision list
    std::vector<std::string> collisions;
    for (auto itr = keys.begin(); itr != keys.end(); ++itr)
    {
        XXH64_hash_t hash = XXH64(static_cast<void*>(const_cast<char*>(itr->c_str())), itr->length(), seed)%len;
        if (coll[hash] == 1)
        {
            table[hash] = 0;
            collisions.push_back(*itr);
        }
        std::cout << coll[hash] << std::endl;
    }
    return collisions;
}

bool bbhash::lookup(std::string key)
{
    return lookup_helper(key, 0, 0);
}

bool bbhash::lookup_helper(std::string key, uint64_t ind, uint64_t pre)
{
    if (ind < 3)
    {
        if (!lengths_[ind]) // mod by 0 is undefined
        {
            return false;
        }

        XXH64_hash_t hash = XXH64(static_cast<void*>(const_cast<char*>(key.c_str())), key.length(), seeds_[ind])%lengths_[ind];
        std::cout << "want to find hash " << hash << std::endl;
        if (table_[hash+pre] == 1)
        {
            return true;
        }
        else
        {
            return lookup_helper(key, ind+1, pre+lengths_[ind]);
        }
    }
    else
    {
        return remaining_.count(key)>0;
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
    keys0.push_back("just like heaven");
    keys0.push_back("seventeen seconds");
    keys0.push_back("play for today");
    keys0.push_back("fascination street");
    keys0.push_back("friday i'm in love");
    keys0.push_back("boys don't cry");
    bbhash test0 = bbhash(keys0, 2, 1);
    bool what = test0.lookup("lovecats");
    std::cout << "should be 1: " << what << std::endl;
    return 0;
}
