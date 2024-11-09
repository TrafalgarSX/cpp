#ifndef _binary_tree_h_
#define _binary_tree_h_

#include <memory>

#if 0
class TreeNode{
public:
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(int value);
    ~TreeNode();

private:
    int value;
};
#endif

#if 0
class TreeNode{
public:
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
    std::shared_ptr<TreeNode> parent;

    TreeNode(int value);
    ~TreeNode();

private:
    int value;
};
#endif

#if 0
class TreeNode{
public:
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
    std::weak_ptr<TreeNode> parent;

    TreeNode(int value);
    ~TreeNode();

private:
    int value;
};
#endif

class TreeNode{
public:
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    TreeNode* parent;

    TreeNode(int value);
    ~TreeNode();

private:
    int value;
};


#endif