/**
 * \file bbhash.cpp
 *
 * \authors Mackenzie Kong-Sivert
 *
 * \brief Implements the bbhash hash table
 */

#include "bbhash.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

bbhash::bbhash()
{

}

bbhash::bbhash(std::vector<std::string> keys, uint64_t n, uint64_t gamma)
{

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
    return 0;
}