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
template <class T>
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
     * \brief this destroys and deallocates a tree
    */
   ~Tree();

   /**
     * \brief gives the size of the tree
     *
     * \returns the number of nodes in the tree
     *
     * \remarks the size of an empty tree is 0
     *
     *
     */
    size_t size() const;
    
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
    void insert(T elem);

    /**
     * \brief checks if an element is in the treeset
     *
     * \param elem is the element that we are searching for
     *
     * \returns true if the element is in the treeset and false otherwise
     *
     * \remarks runs in log(n) time
     */
    bool exists(T& elem);

    /**
     * \brief prints the tree in increasing order of value
     */
    std::ostream& print(std::ostream& out) const;

private:
    size_t size_; ///< size of tree
    Node* root_; ///< the root node of the tree

    /**
     * \brief private recursive helper function for destructor
     */
    void destroy(Node*& root);

    /**
     * \brief helper function to print tree recursively
     */
    void printNodes(std::ostream& out, Node *const root) const;

    struct Node{
        Node(T value);
        Node* left; ///< the node's left child
        Node* right; ///< the node's right child
        T value; ///< the value contained within the node
        };
};

#endif // SUFFARRAY_HPP_INCLUDED