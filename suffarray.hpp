/**
 * \file bitvector.hpp
 *
 * \authors Mackenzie Kong-Sivert
 *
 * \brief Declares the bit_vector, rank_support, select_support, and
 * FM_text classes.
 */

#ifndef SUFFARRAY_HPP_INCLUDED
#define SUFFARRAY_HPP_INCLUDED 1

#include <cstddef>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <iostream>
#include <fstream>
#include <type_traits>

/**
 * \class Tree
 * \brief 
 * 
 * \details Why does C++ not have this built in in the first place?
*/
class Tree {

struct Node;

public:
    /**
     * \brief Default constructor
     * 
     * \note Creates an empty tree
    */
    Tree();

    /**
     * \brief Parameterized constructor
    */
   Tree(std::string sequence);

    /**
     * \brief this destroys and deallocates a tree
    */
   ~Tree();
    
    /**
     * \brief inserts an element into the treeset
     *
     * \param elem is the element to be inserted
     *
     * \pre the tree in its current state
     *
     * \post the tree with the element inserted
     *
     * \remarks runs in log(n) time and does not insert duplicates
     */
    void insert(char elem);

    /**
     * \brief prints the contents of the tree
     */
    std::ostream& print(std::ostream& out) const;

    /**
     * \brief converts the suffix tree into a corresponding array
     * 
     * \returns the corresponding suffix array as a list of strings
    */
    std::list<std::string> makesuffarray();

private:
    Node* root_; ///< the root node of the tree
    std::list<std::string::iterator> lastchar_; ///< the last char to be inserted into the tree (all locations)

    /**
     * \brief private recursive helper function for destructor
     */
    void destroy(Node*& root);

    /**
     * \brief helper function to print tree recursively
     */
    void printNodes(std::ostream& out, Node *const root) const;

    struct Node{
        Node(char value);
        void extend(char v);
        Node* children_[4]; ///< one slot for each next character (A, C, G, T)
        std::string value_; ///< the value contained within the node
        };
};

#endif // SUFFARRAY_HPP_INCLUDED