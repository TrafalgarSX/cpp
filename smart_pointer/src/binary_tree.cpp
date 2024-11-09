#include <memory>
#include <iostream>

#include <binary_tree.h>


TreeNode::TreeNode(int value) : value(value) {
    std::cout << "TreeNode constructor called value is :" << value << std::endl;
}

TreeNode::~TreeNode() {
    std::cout << "TreeNode destructor called value is :" << value << std::endl;
}