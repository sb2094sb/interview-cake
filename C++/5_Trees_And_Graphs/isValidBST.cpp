#include <iostream>
#include <memory>
#include <stack>

// C++11 lest unit testing framework
#include "../lest.hpp"

using namespace std;

class BinaryTreeNode
{
public:
    int value_;
    BinaryTreeNode* left_;
    BinaryTreeNode* right_;

    BinaryTreeNode(int value) :
        value_(value),
        left_(nullptr),
        right_(nullptr)
    {
    }

    ~BinaryTreeNode()
    {
        delete left_;
        delete right_;
    }

    BinaryTreeNode* insertLeft(int value)
    {
        this->left_ = new BinaryTreeNode(value);
        return this->left_;
    }

    BinaryTreeNode* insertRight(int value)
    {
        this->right_ = new BinaryTreeNode(value);
        return this->right_;
    }
};

class NodeBounds{
public:
    const BinaryTreeNode* node_;
    int lowerBound_;
    int upperBound_;
    
    NodeBounds(const BinaryTreeNode* node, int lowerBound = numeric_limits<int>::min(),
    int upperBound = numeric_limits<int>::max()) :
    node_(node),
    lowerBound_(lowerBound),
    upperBound_(upperBound)
    {
    }

};

bool isBinarySearchTree(const BinaryTreeNode* root){
    stack<NodeBounds> nodeAndBoundsStack;
    nodeAndBoundsStack.push(NodeBounds(root));
    
    while(!nodeAndBoundsStack.empty()){
        const NodeBounds nodeBounds = nodeAndBoundsStack.top();
        nodeAndBoundsStack.pop();
        int nodeValue = nodeBounds.node_->value_;
        
        // check to see if false
        if(nodeValue <= nodeBounds.lowerBound_ || nodeValue >= nodeBounds.upperBound_){
            return false;
        }
        
        // populate the stack
        if(nodeBounds.node_->left_ != nullptr){
            nodeAndBoundsStack.push(NodeBounds(nodeBounds.node_->left_, nodeBounds.lowerBound_, nodeValue));
        }
        if(nodeBounds.node_->right_ != nullptr){
            nodeAndBoundsStack.push(NodeBounds(nodeBounds.node_->right_, nodeValue, nodeBounds.upperBound_));
        }
        
    }
    
    return true;
}


// using recursion
// bool isBinarySearchTree(const BinaryTreeNode* rootNode, const int lowerBound = numeric_limits<int>::min(), 
// const int upperBound = numeric_limits<int>::max())
// {
//     if(rootNode == nullptr){
//         return true;
//     }
    
//     if(rootNode->value_ >= upperBound || rootNode->value_ <= lowerBound){
//         return false;
//     }
    
//     return isBinarySearchTree(rootNode->left_, lowerBound, rootNode->value_)
//         && isBinarySearchTree(rootNode->right_, rootNode->value_, upperBound);
// }


// tests

const lest::test tests[] = {
    {CASE("Valid full tree") {
        auto root = make_unique<BinaryTreeNode>(50);
        root->insertLeft(30)->insertLeft(10);
        root->insertRight(70)->insertRight(80);
        root->left_->insertRight(40);
        root->right_->insertLeft(60);
        EXPECT(isBinarySearchTree(root.get()) == true);
    }},
    {CASE("Both subtrees valid") {
        auto root = make_unique<BinaryTreeNode>(50);
        root->insertLeft(30)->insertLeft(20);
        root->insertRight(80)->insertRight(90);
        root->left_->insertRight(60);
        root->right_->insertLeft(70);
        EXPECT(isBinarySearchTree(root.get()) == false);
    }},
    {CASE("Descending linked list") {
        auto root = make_unique<BinaryTreeNode>(50);
        root->insertLeft(40)->insertLeft(30)->insertLeft(20)->insertLeft(10);
        EXPECT(isBinarySearchTree(root.get()) == true);
    }},
    {CASE("Out of order linked list") {
        auto root = make_unique<BinaryTreeNode>(50);
        root->insertRight(70)->insertRight(60)->insertRight(80);
        EXPECT(isBinarySearchTree(root.get()) == false);
    }},
    {CASE("One node tree") {
        auto root = make_unique<BinaryTreeNode>(50);
        EXPECT(isBinarySearchTree(root.get()) == true);
    }}
};

int main(int argc, char** argv)
{
    return lest::run(tests, argc, argv);
}