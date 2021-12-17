#include "AVL.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"

using namespace std;

AVLNode::AVLNode(int key) :
    key_(key),
    parent_(weak_ptr<AVLNode>()),
    left_(nullptr),
    right_(nullptr),
	height_(0),
	balanceFactor_(0) {}

AVLNode::AVLNode(int key, weak_ptr<AVLNode> parent) :
    key_(key),
    parent_(parent),
    left_(nullptr),
    right_(nullptr),
	height_(0),
	balanceFactor_(0) {}

bool AVLNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
	return right_ != nullptr;
}

void AVL::updateHeightAndBF(shared_ptr<AVLNode> &node){
	int nullheight = -1;
	if(node->left_ && node->right_){
		node->height_ = max(node->left_->height_, node->right_->height_) + 1;
		node->balanceFactor_ = node->right_->height_ - node->left_->height_;
	}
	else if(node->left_ && node->right_ == nullptr){
		node->height_ = node->left_->height_ + 1;
		node->balanceFactor_ = nullheight - node->left_->height_;
	}
	else if(node->left_ == nullptr && node->right_){
		node->height_ = node->right_->height_ + 1;
		node->balanceFactor_ = node->right_->height_ - nullheight;
	}
	else if(node->left_ == nullptr && node->right_ == nullptr){
		node->height_ = 0;
		node->balanceFactor_ = nullheight - nullheight;
	}
	auto parent = node->parent_.lock();
	if (parent != nullptr){
		updateHeightAndBF(parent);
	}
}



AVL::AVL() : root_(nullptr), size_(0) {}

int AVL::getHeight(shared_ptr<AVLNode> &temp) const{
    int h = -1;
    if (temp != nullptr){
        int l_height = getHeight(temp->left_);
        int r_height = getHeight(temp->right_);
        int max_height = max(l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

int AVL::balanceFactor(shared_ptr<AVLNode> &temp) const{
	return (getHeight(temp->right_) - getHeight(temp->left_));
}

shared_ptr<AVLNode> AVL::leftRotate(shared_ptr<AVLNode> &Node){
	shared_ptr<AVLNode> temp(nullptr);
	temp = Node->right_;
	Node->right_ = temp->left_;
	weak_ptr<AVLNode> Parent_Node = Node->parent_;
	shared_ptr<AVLNode> Parent_Node2 = Parent_Node.lock();
	if (Parent_Node2 != nullptr){
		temp->parent_ = Node->parent_;
	}
	else if(Parent_Node2 == nullptr){
		temp->parent_.reset();
	}
	temp->left_ = Node;
	return temp;
}

shared_ptr<AVLNode> AVL::rightRotate(shared_ptr<AVLNode> &Node){
	cout << "checkpoint 6a\n";
	shared_ptr<AVLNode> temp(nullptr);
	cout << "checkpoint 6b\n";
	temp = Node->left_;
	cout << "checkpoint 6c\n";
	Node->left_ = temp->right_;
	cout << "checkpoint 6d\n";
	weak_ptr<AVLNode> Parent_Node = Node->parent_;
	cout << "checkpoint 6e\n";
	shared_ptr<AVLNode> Parent_Node2 = Parent_Node.lock();
	cout << "checkpoint 6f\n";
	if (Parent_Node2 != nullptr){
		cout << "checkpoint 6g\n";
		temp->parent_ = Node->parent_;
		cout << "checkpoint 6h\n";
	}
	else if(Parent_Node2 == nullptr){
		cout << "checkpoint 6i\n";
		temp->parent_.reset();
		cout << "checkpoint 6j\n";
	}
	temp->right_ = Node;
	return temp;
}

/*shared_ptr<AVLNode> AVL::rightRotate(shared_ptr<AVLNode> &Node)
{	cout << "checkpoint 6a1\n";
    shared_ptr<AVLNode> temp(nullptr);
    shared_ptr<AVLNode> parent_s;
    temp = Node->left_;
	cout << "checkpoint 6a2\n";
    parent_s = Node->parent_.lock();
    weak_ptr<AVLNode> temp_w(temp);
    shared_ptr<AVLNode> grandson_node_s = Node->left_->left_;
    weak_ptr<AVLNode> grandson_node_w(grandson_node_s);
	cout << "checkpoint 6a\n";
    if ((temp != nullptr) && (grandson_node_s != nullptr)) 
    {    
        cout << "checkpoint 6b\n";
		weak_ptr<AVLNode> temp_w(temp);
        grandson_node_s->parent_ = temp_w;
        temp->left_ = grandson_node_s;
        Node->left_ = temp->right_;
        weak_ptr<AVLNode> Node_w(Node);
        Node->left_->parent_ = Node_w;
        temp->right_ = Node;
        Node->parent_ = temp_w; 
		cout << "checkpoint 6c\n";
        if (parent_s !=nullptr){
			cout << "checkpoint 6d\n";
            weak_ptr<AVLNode> parent_w(parent_s);
            temp->parent_ = parent_w;
			cout << "checkpoint 6e\n";
                if (temp->parent_.lock()->left_ == temp){
                    shared_ptr<AVLNode> g_parent = temp->parent_.lock();
                    g_parent->left_ = temp;
                }    
                else if (temp->parent_.lock()->right_ == temp){
                    shared_ptr<AVLNode> g_parent = temp->parent_.lock();
                    g_parent->right_ = temp;
                }     
        }
        else if (parent_s == nullptr) 
        {
            weak_ptr<AVLNode> parent_w(parent_s);
            temp->parent_.reset();
        }

    
    }
    if (temp){    
        return temp;}
    
}*/

void AVL::Balance(shared_ptr<AVLNode> &parent){
	shared_ptr<AVLNode> grandparent = parent->parent_.lock();
	if (grandparent == nullptr) return;
	cout << "checkpoint 2\n";
	if (grandparent->balanceFactor_ < -1 && parent->balanceFactor_ <= 0){
		cout << "checkpoint 3\n";
		rightRotate(grandparent);
	}
	else if(grandparent->balanceFactor_ > 1 && parent->balanceFactor_ >= 0){
		cout << "checkpoint 4\n";
		leftRotate(grandparent);
		cout << "checkpoint 4a\n";
	}
	else if(grandparent->balanceFactor_ > 1 && parent->balanceFactor_ < 0){
		cout << "checkpoint 5\n";
		grandparent->left_ = leftRotate(grandparent->left_);
		rightRotate(grandparent);
	}
	else if(grandparent->balanceFactor_ < -1 && parent->balanceFactor_ > 0){
		cout << "checkpoint 6\n";
		grandparent->right_ = rightRotate(grandparent->right_);
		cout << "checkpoint 7\n";
		leftRotate(grandparent);
	}
}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = make_shared<AVLNode>(key);
		//root_->height_ = 0;
		//root_->balanceFactor_ = 0;
		//root_->left_ = nullptr;
		//root_->right_ = nullptr;
		size_++;
		return;
	}
	shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	shared_ptr<AVLNode> child;
	if (key < lastNode->key_) {
		lastNode->left_ = make_shared<AVLNode>(key, lastNode);
		child = lastNode->left_;
		updateHeightAndBF(lastNode);
	} 
	else {
		lastNode->right_ = make_shared<AVLNode>(key, lastNode);
		child = lastNode->right_;
		updateHeightAndBF(lastNode);
	}
	size_++;

	shared_ptr<AVLNode> parent = lastNode;
	while(parent != nullptr){
		cout << "checkpoint 1\n";
		Balance(parent);
		cout << "checkpoint after balancing\n" << endl;
		updateHeightAndBF(parent);
		
		parent = (parent->parent_).lock();
		
	}
	return;
	//weak_ptr<AVLNode> grandparent_node = lastNode->parent_;
	/*cout << "checkpoint 1\n";
	shared_ptr<AVLNode> grandparent_node = (lastNode->parent_).lock();
	shared_ptr<AVLNode> parent_node = lastNode;
	cout <<"checkpoint2\n";
	if (grandparent_node == nullptr){
		return;
	}
	
	int balance = grandparent_node->balanceFactor_;
	//cout << "balance factor: " << balance << endl;
	//cout <<"checkpoint3\n";
	//cout << "grandparent_node2: " << grandparent_node2->key_ << endl;*/
	/*if (balance > 1 && key < grandparent_node2->left_->key_){
		rightRotate(grandparent_node2);
	}
	cout <<"checkpoint4\n";
	if (balance < -1 && key > grandparent_node2->right_->key_){
		leftRotate(grandparent_node2);
	}
	cout <<"checkpoint5\n";
	if(balance > 1 && key > grandparent_node2->left_->key_){
		grandparent_node2->left_ = leftRotate(grandparent_node2->left_);
		rightRotate(grandparent_node2);
	}
	cout <<"checkpoint6\n";
	if (balance < -1 && key < grandparent_node2->right_->key_){
		grandparent_node2->right_ = rightRotate(grandparent_node2->right_);
		leftRotate(grandparent_node2);
	}
	cout <<"checkpoint7\n";*/
}

size_t AVL::size() const {
    return size_;
}

bool AVL::empty() const {
    return size_ == 0;
}

bool AVL::Find(int key) const {
    shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			return true;
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

string AVL::JSON() const {
	nlohmann::json result;
	queue< shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			string key = to_string(v->key_);
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
			result[key]["height"] = v->height_;
			result[key]["balance factor"] = v->balanceFactor_;
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}