/**
 * \file treeset.cpp
 *
 * \brief implementation for the treeset
 */

#include "suffarray.hpp"

#include <numeric>
#include <vector>

template <typename T>
Tree<T>::Tree() :
    size_(0),
    root_(nullptr)
{
    // Nothing to do here
}

template <typename T>
Tree<T>::~Tree() {
    destroy(root_); //call recursive helper function
}

template <typename T>
size_t Tree<T>::size() const {
    return size_;
}

template <typename T>
void Tree<T>::destroy(Node*& root) {
    if (root != nullptr) { // recur until we get to a leaf node
        destroy(root->left);
        destroy(root->right);
        delete root;
        root = nullptr;
    }
}

template <typename T>
void Tree<T>::insert(T elem) {    
    Node* elemNode = new Node(elem);
    // empty tree case
    if(root == nullptr) {
        // the root of the tree should always be black
        root = &elem;
    }
    ++size_;
}

template <typename T>
bool Tree<T>::exists(T& elem) {
    // call more general recursive helper function
    if (search(root_, elem) == nullptr) {
        return false;
    }
    return true;
}

template <typename T>
std::ostream& Tree<T>::print(std::ostream& out) const {
    printNodes(out, root_); // call recursive private helper function
    out << std::endl;
    return out;
}

template<typename T>
void Tree<T>::printNodes(std::ostream& out, Node *const root) const {
    if (root == nullptr) {
        out << "-";
    }
    else {
        out << "(";
        printNodes(out, root->left);
        out << ", " << root->value << ", ";
        printNodes(out, root->right);
        out << ")";
    }
}

// **********************
// Node Functions
// **********************

template <typename T>
Tree<T>::Node::Node(T elem):
    value(elem),
    left(nullptr),
    right(nullptr),
{
    // Nothing to do here
}