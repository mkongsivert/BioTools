/**
 * \file treeset.cpp
 *
 * \brief implementation for the treeset
 * 
 * \note list lastchar_ is an empty list by default
 */

#include "suffarray.hpp"

#include <numeric>
#include <vector>
#include <list>

Tree::Tree() :
    root_(&Node('^'))
{
    // Nothing to do here
}

Tree::Tree(std::string sequence)
{
    // Start with an empty tree
    root_ = &Node('^');
    lastchar_ = std::list<std::string::iterator>(); // empty list to start

    // Ukkonen's Alg
    for (auto itr = sequence.begin(); itr!= sequence.end(); ++itr)
    {
        insert(*itr);
    }
}

Tree::~Tree() {
    destroy(root_); // call recursive helper function
    lastchar_.clear();
}

void Tree::destroy(Node*& root) {
    if (root != nullptr) { // recur until we get to a leaf node
        for (size_t i = 0; i != 3; ++i)
        {
            // TODO: test
            destroy(root->children_[i]);
        }
        delete root;
        root = nullptr;
    }
}

void Tree::insert(char elem) {
    // TODO: finish
    // get index for char, quit if char is not in accepted alphabet
    size_t i;
    switch (elem)
    {
        case 'a': i = 0;
        case 'c': i = 1;
        case 'g': i = 2;
        case 't': i = 3;
        default:
            std::cout << "Character " << elem << " not in alphebet";
            return;
    }

    // Extend existing suffixes by one character
    for (auto itr = lastchar_.begin(); itr != lastchar_.end(); ++itr)
    {
        // Note: *itr will be type std::string::iterator
        
    }

    // Add suffix starting at current index
    if (root_->children_[i] == nullptr) // char hasn't been seen yet
    {
        root_->children_[i] = &Node(elem);
    }
    lastchar_.push_back(root_->children_[i]->value_.begin());
}

std::ostream& Tree::print(std::ostream& out) const {
    printNodes(out, root_); // call recursive private helper function
    out << std::endl;
    return out;
}

void Tree::printNodes(std::ostream& out, Node *const root) const {
    // TODO: fix
    if (root == nullptr) {
        out << "-";
    }
    else {
        out << "(";
        for (size_t i = 0; i < 4; ++i)
        {
            printNodes(out, root->children_[i]);
            out << ", " << root->value_ << ", ";
        }
        out << ")";
    }
}

std::list<std::string> Tree::makesuffarray() {
    return std::list<std::string>(); //TODO: write traversal
}

// **********************
// Node Functions
// **********************

Tree::Node::Node(char key):
    value_(std::string(1,key)),
    children_({nullptr, nullptr, nullptr, nullptr})
{
    // Nothing to do here
}

void Tree::Node::extend(char v)
{
    value_.push_back(v);
}