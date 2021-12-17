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
    return temp->height_;
}

int AVL::balanceFactor(shared_ptr<AVLNode> &temp) const{
	return (getHeight(temp->right_) - getHeight(temp->left_));
}

shared_ptr<AVLNode> AVL::leftRotate(shared_ptr<AVLNode> &node){
	shared_ptr<AVLNode> temp = node->right_;
	node->right_ = temp->left_;
	temp->left_ = node;
	if(node->right_) {
		node->right_->parent_ = node;
	}
	temp->parent_ = node->parent_;
	shared_ptr<AVLNode> parent = node->parent_.lock();
	if(parent != nullptr){
		if(node == parent->right_){
			parent->right_ = temp;
		}
		else if(node == parent->left_){
			parent->left_ = temp;
		}
	}
	node->parent_ = temp;
	updateHeightAndBF(node);
	return temp;
}

std::shared_ptr<AVLNode> AVL::rightRotate(std::shared_ptr<AVLNode> &node) {
    std::shared_ptr<AVLNode> temp = node->left_;
    node->left_ = temp->right_;
    temp->right_ = node;
    if(node->left_) {
        node->left_->parent_ = node;
    }
    temp->parent_ = node->parent_;
	shared_ptr<AVLNode> parent = node->parent_.lock();
    if(parent != nullptr) {
        if(node == parent->left_) {
            parent->left_ = temp;
        } else if(node == parent->right_) {
            parent->right_ = temp;
        }
    }
    node->parent_ = temp;
	updateHeightAndBF(node);
    return temp;
}

shared_ptr<AVLNode> AVL::leftRightRotate(shared_ptr<AVLNode> &node){
	node->left_ = leftRotate(node->left_);
	return rightRotate(node);
}

shared_ptr<AVLNode> AVL::rightLeftRotate(shared_ptr<AVLNode> &node){
	node->right_ = rightRotate(node->right_);
	return leftRotate(node);
}

void AVL::check_balance(shared_ptr<AVLNode> &node){
	if (node->balanceFactor_ > 1 || node->balanceFactor_ < -1){
		node = Balance(node);
	}
	if (node->parent_.lock() == nullptr){
		return;
	}
	auto parent = node->parent_.lock();
	check_balance(parent);
}

shared_ptr<AVLNode> AVL::Balance(shared_ptr<AVLNode> &parent){
	shared_ptr<AVLNode> parent_left = parent->left_;
	if(parent->balanceFactor_ < -1){
        if(parent_left->balanceFactor_ <= 0){
			parent = rightRotate(parent);
		}
		else{
			parent_left = leftRotate(parent_left);
			parent->left_ = parent_left;
			parent = rightRotate(parent);
		}
    }
	shared_ptr<AVLNode> parent_right = parent->right_;
    if (parent->balanceFactor_ > 1){
		if(parent->right_->balanceFactor_ >= 0){
			parent = leftRotate(parent);
		}
		else{
			parent_right = rightRotate(parent_right);
			parent->right_ = parent_right;
			parent = leftRotate(parent);
		}
    }
	auto new_parent = parent->parent_.lock();
    if(new_parent == nullptr){
		root_ = parent;
	}
	return parent;
}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = make_shared<AVLNode>(key);
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
		updateHeightAndBF(lastNode->left_);
	} 
	else {
		lastNode->right_ = make_shared<AVLNode>(key, lastNode);
		child = lastNode->right_;
		updateHeightAndBF(lastNode->right_);
	}
	size_++;
	check_balance(lastNode);
	return;
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
	result["height"] = root_->height_;
	result["size"] = size_;
	return result.dump(2) + "\n";
}