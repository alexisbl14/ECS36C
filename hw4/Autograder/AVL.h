#include <memory>
#include <string>
#include "json.hpp"

using namespace std;
class AVL;

class AVLNode {
 public:
    AVLNode(int key);
    AVLNode(int key, std::weak_ptr<AVLNode> parent);
    bool IsLeaf() const;
    bool IsMissingChild() const;
    bool HasLeftChild() const;
    bool HasRightChild() const;
    
    

 private:
    int key_;
    weak_ptr<AVLNode> parent_;
    shared_ptr<AVLNode> left_;
    shared_ptr<AVLNode> right_;
    int height_;
    int balanceFactor_;

    friend AVL;
}; //class AVLNode

class AVL {
 public:
    AVL();
    void Insert(int key);
    bool Find(int key) const;
    //void right_rotation();
    //void left_rotation();
    string JSON() const;
    size_t size() const;
    bool empty() const;
    int getHeight(std::shared_ptr<AVLNode> &temp) const;
    int balanceFactor(shared_ptr<AVLNode> &temp) const;
    void updateHeightAndBF(shared_ptr<AVLNode> &node);

 private:
    shared_ptr<AVLNode> root_;
    size_t size_;
    shared_ptr<AVLNode> rightRotate(shared_ptr<AVLNode> &node);
    shared_ptr<AVLNode> leftRotate(shared_ptr<AVLNode> &node);
    shared_ptr<AVLNode> rightLeftRotate(shared_ptr<AVLNode> &node);
    shared_ptr<AVLNode> leftRightRotate(shared_ptr<AVLNode> &node);
    shared_ptr<AVLNode> Balance(shared_ptr<AVLNode> &node);
    void check_balance(shared_ptr<AVLNode> &node);

    friend AVLNode;
    
}; //class AVL
