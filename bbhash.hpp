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

    /**
     * \class rank_support
     * \brief Class / structure that “wraps” the underlying bit-vector.
     */
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