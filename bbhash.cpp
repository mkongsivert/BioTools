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
#include <chrono>

using namespace sdsl;

bbhash::bbhash()
{

}

bbhash::bbhash(std::vector<std::string> keys, uint64_t n, double gamma)
{
    std::vector<std::string> keys_to_hash = keys;
    size_ = n;
    uint64_t len = gamma*n;
    for (uint64_t i = 0; i < 3; ++i)
    {
        tables_[i] = bit_vector(len, 0);
        lengths_[i] = len;
        uint64_t seed = std::rand(); // doesn't really need to be random
        seeds_[i] = seed;
        keys_to_hash = build_array(tables_[i], keys_to_hash, len, seed);
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

}

std::vector<std::string> bbhash::build_array(bit_vector& table, std::vector<std::string> keys, uint64_t len, uint64_t seed)
{
    //build table of collisions
    bit_vector coll = bit_vector(len, 0);
    for (auto itr = keys.begin(); itr != keys.end(); ++itr)
    {
        XXH64_hash_t hash = XXH64(static_cast<void*>(const_cast<char*>(itr->c_str())), itr->length(), seed)%len;
        if (table[hash])
        {
            coll[hash] = 1;
        }
        else
        {
            table[hash] = 1;
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
    }
    return collisions;
}

bool bbhash::lookup(std::string key)
{
    return lookup_helper(key, 0);
}

bool bbhash::lookup_helper(std::string key, uint64_t ind)
{
    if (ind < 3)
    {
        if (!lengths_[ind]) // mod by 0 is undefined
        {
            return false;
        }

        XXH64_hash_t hash = XXH64(static_cast<void*>(const_cast<char*>(key.c_str())), key.length(), seeds_[ind])%lengths_[ind];
        if (tables_[ind][hash] == 1)
        {
            return true;
        }
        else
        {
            return lookup_helper(key, ind+1);
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

uint64_t bbhash::size()
{
    return size_;
}

std::ostream& bbhash::print(std::ostream& out) const
{
    // TODO: finish writing this
    out << size_;
    return out;
}

bbhash load(std::string& fname, uint64_t n, double gamma)
{
    std::string line;
    std::vector<std::string> keys;
    std::ifstream myfile (fname);
    if (myfile.is_open())
    {
        for (uint64_t i = 0; i < n; ++i)
        {
            getline(myfile,line);
            keys.push_back(line);
        }
        myfile.close();
        return bbhash(keys, n, gamma);
    }

    else
    {
        std::cout << "Unable to open file"; 
        return bbhash();
    }
}

int main()
{
    std::string fname = "words.txt";
    auto t0 = std::chrono::high_resolution_clock::now();
    bbhash test0 = load(fname, 1000000, 1);
    auto t1 = std::chrono::high_resolution_clock::now();
    bbhash test1 = load(fname, 1000000, 1.25);
    auto t2 = std::chrono::high_resolution_clock::now();
    bbhash test2 = load(fname, 1000000, 1.5);
    auto t3 = std::chrono::high_resolution_clock::now();
    bbhash test3 = load(fname, 1000000, 1.75);
    auto t4 = std::chrono::high_resolution_clock::now();
    bbhash test4 = load(fname, 1000000, 2);
    auto t5 = std::chrono::high_resolution_clock::now();
    auto dur0 = std::chrono::duration_cast<std::chrono::microseconds>( t1 - t0 ).count();
    auto dur1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    auto dur2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
    auto dur3 = std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count();
    auto dur4 = std::chrono::duration_cast<std::chrono::microseconds>( t5 - t4 ).count();
    std::cout << "time to construct: " << dur0 << "; " << dur1 << "; " << dur2 << "; " << dur3 << "; " << dur4 << std::endl;
    return 0;
}
