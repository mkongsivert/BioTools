/**
 * \file bitvector.hpp
 *
 * \authors Mackenzie Kong-Sivert
 *
 * \brief Declares the bit_vector, rank_support, select_support, and
 * FM_text classes.
 */

#ifndef BBHASH_HPP_INCLUDED
#define BBHASH_HPP_INCLUDED 1

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sdsl/bit_vectors.hpp>

using namespace sdsl;

/**
 * \class BitString
 * \brief Represents a string of bits
 *
 * \details TODO
 *
 * \remarks
 *   TODO
 */
class bbhash {
public:
    /**
     * \brief Default constructor
     *
     * \note Will not be used; only exists for compilation
     */
    bbhash();
    /**
     * \brief Parameterized constructor
     *
     * \note
     */
    bbhash(std::vector<std::string> keys, uint64_t n, uint64_t gamma);

    std::ostream& print(std::ostream& out) const; ///< Printing

    /**
    * \brief saves the hash table into a file
    *
    * \note
    */
    void save(std::string& fname);
    /**
    * \brief loads data into the hash table
    *
    * \note
    */
    void load(std::string& fname);
    /**
    * \brief returns the size of the data structure
    *
    * \note
    */
    uint64_t size();

private:
    uint64_t size_;
    bit_vector table_;
    std::unordered_set<std::string> remaining_;
    uint64_t lengths_[3];
    uint64_t seeds_[3];

    /**
    * \brief fills one array/layer of the hash table
    *
    * \returns vector of all colliding elements
    *
    * \note
    */
    std::vector<std::string> build_array(std::vector<std::string> keys, uint64_t n, uint64_t seed);

    /**
    * \brief lookup function for the hash table
    *
    * \param key a string to look up
    *
    * \returns a bool indicating whether key is present in the table
    *
    * \note
    */
    bool lookup(std::string key);

    /**
    * \brief recursive helper function for lookup function
    *
    * \param key a string to look up
    *
    * \param ind the index at which to start the search
    *
    * \returns a bool indicating whether key is present in the table
    *
    * \note
    */
    bool lookup_helper(std::string key, uint64_t ind);
};

/**
 * \brief Print operator.
 * 
 */
inline std::ostream& operator<<(std::ostream& out, const bbhash& text)
{
    return text.print(out);
}

#endif // BBHASH_HPP_INCLUDED