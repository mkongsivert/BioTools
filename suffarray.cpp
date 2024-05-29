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
    lastchars_ = std::list<Marker*>(); // empty list to start

    // Ukkonen's Alg
    for (auto itr = sequence.begin(); itr!= sequence.end(); ++itr)
    {
        insert(*itr);
    }
}

Tree::~Tree() {
    destroy(root_); // call recursive helper function
    lastchars_.clear();
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

size_t Tree::cindex(char c) {
    switch (c)
    {
        case 'a': return 0;
        case 'c': return 1;
        case 'g': return 2;
        case 't': return 3;
        default:
            std::cout << "Character " << c << " not in alphebet";
            return 4;
    }
}


void Tree::insert(char elem) {
    // get index for char, quit if char is not in accepted alphabet
    size_t i = cindex(elem);

    // Extend existing suffixes by one character
    for (auto itr = lastchars_.begin(); itr != lastchars_.end(); ++itr)
    {
        std::string branchval = (*itr)->tnode_->value_;
        // If the char is at the end of the string
        if ((*itr)->tstring_+1 == branchval.size())
        {
            continue;
        }
        else
        {
            // If the next character is our desired character, advance the iterator
            if (branchval[(*itr)->tstring_] == elem)
            {
                ++((*itr)->tstring_);
            }
            else
            {
                size_t j = cindex(branchval[(*itr)->tstring_]); // index to put oldbranch under
                Node* oldbranch = new Node(branchval.substr(0,(*itr)->tstring_));
                std::string thisbranch;
                (*itr)->tnode_->value_ = branchval.substr((*itr)->tstring_);
                Node* newbranch = new Node(elem);
                oldbranch->children_[i] = newbranch;
                // TODO: shuffle branches around
            }
        }
        
    }

    // Add suffix starting at current index
    if (root_->children_[i] == nullptr) // char hasn't been seen yet
    {
        root_->children_[i] = &Node(elem);
    }
    Marker* topchar = new Marker(root_->children_[i], 0);
    lastchars_.push_back(topchar);
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
    // Constructor from char
}

Tree::Node::Node(std::string key):
    value_(key),
    children_({nullptr, nullptr, nullptr, nullptr})
{
    // Constructor from string
}

void Tree::Node::extend(char v)
{
    value_.push_back(v);
}

// **********************
// Marker Functions
// **********************

Tree::Marker::Marker(Node* ntrack, size_t strack):
    tnode_(ntrack),
    tstring_(strack)
{
    // Nothing to do here
}