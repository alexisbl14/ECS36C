#include <algorithm>
#include <memory>
#include <vector>
#include "json.hpp"

using namespace std;

class Node {
    public:
        int data;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> parent;
    public:
        Node() : data(0) {}
        explicit Node(int d) : data(d) {}
};

class Avl {
    private:
        size_t current_size;
        std::shared_ptr<Node> root;
    private:
        int height(std::shared_ptr<Node> node);
        std::shared_ptr<Node> get_min(std::shared_ptr<Node> node);
        std::shared_ptr<Node> inorder_successor(std::shared_ptr<Node> node);
        void add_helper(std::shared_ptr<Node> parent, std::shared_ptr<Node> node);
        std::shared_ptr<Node> find_helper(std::shared_ptr<Node> parent_node, std::shared_ptr<Node> node);
        std::shared_ptr<Node> find_helper(std::shared_ptr<Node> parent_node, int data);
        void destroy_helper(std::shared_ptr<Node>& node);
        void transplant(std::shared_ptr<Node> node, std::shared_ptr<Node> second_node);
        std::shared_ptr<Node> left_rotate(std::shared_ptr<Node> node);
        std::shared_ptr<Node> right_rotate(std::shared_ptr<Node> node);
        std::shared_ptr<Node> left_right_rotate(std::shared_ptr<Node> node);
        std::shared_ptr<Node> right_left_rotate(std::shared_ptr<Node> node);
        void check_balance(std::shared_ptr<Node> node);
        std::shared_ptr<Node> rebalance(std::shared_ptr<Node> node);
        void traverse_inorder_helper(std::shared_ptr<Node> node, std::vector<int>& out);
        void traverse_preorder_helper(std::shared_ptr<Node> node, std::vector<int>& out);
        void traverse_postorder_helper(std::shared_ptr<Node> node, std::vector<int>& out);
    public:
        Avl() : current_size(0) {}
        void add(std::shared_ptr<Node> node);
        void add(int data);
        std::shared_ptr<Node> find(std::shared_ptr<Node> node);
        std::shared_ptr<Node> find(int data);
        void remove(std::shared_ptr<Node> node);
        void remove(int data);
        void destroy();
        void traverse_inorder(std::vector<int>& out);
        void traverse_preorder(std::vector<int>& out);
        void traverse_postorder(std::vector<int>& out);
        string JSON() const;
        inline size_t size() const {
            return current_size;
        }
        inline bool empty() const {
            return !(static_cast<bool>(current_size));
        }
};

int Avl::height(std::shared_ptr<Node> node) {
    if(!node) {
        return 0;
    }
    return std::max(height(node->left), height(node->right)) + 1;
}

std::shared_ptr<Node> Avl::get_min(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node->right;
    while(temp->left) {
        temp = temp->left;
    }
    return temp;
}

void Avl::remove(std::shared_ptr<Node> node) {
    if(!node->left) {
        transplant(node, node->right);
        check_balance(node);
    } else if(!node->right) {
        transplant(node, node->left);
        check_balance(node);
    } else {
        std::shared_ptr<Node> successor = inorder_successor(node);
        if(successor->parent != node) {
            transplant(successor, successor->right);
            check_balance(successor->parent);
            successor->right = node->right;
            if(successor->right) {
                successor->right->parent = successor;
            }
        }
        transplant(node, successor);
        successor->left = node->left;
        if(successor->left) {
            successor->left->parent = successor;
        }
        successor->parent = node->parent;
        check_balance(successor);
    }
}

void Avl::remove(int data) {
    std::shared_ptr<Node> node = find(data);
    std::shared_ptr<Node> successor = 0;
    if(!node) {
        return;
    }
    if(!node->left) {
        transplant(node, node->right);
        check_balance(node);
    } else if(!node->right) {
        transplant(node, node->left);
        check_balance(node);
    } else {
        successor = inorder_successor(node);
        if(successor->parent != node) {
            transplant(successor, successor->right);
            check_balance(successor->parent);
            successor->right = node->right;
            if(successor->right) {
                successor->right->parent = successor;
            }
        }
        transplant(node, successor);
        successor->left = node->left;
        if(successor->left) {
            successor->left->parent = successor;
        }
        successor->parent = node->parent;
        check_balance(successor);
    }
}

std::shared_ptr<Node> Avl::find_helper(std::shared_ptr<Node> parent_node, std::shared_ptr<Node> node) {
    if(!parent_node || parent_node->data == node->data) {
        return parent_node;
    }
    if(node->data > parent_node->data) {
        return find_helper(parent_node->right, node);
    } else if(node->data < parent_node->data) {
        return find_helper(parent_node->left, node);
    }
    return 0;
}

std::shared_ptr<Node> Avl::find_helper(std::shared_ptr<Node> parent_node, int data) {
    if(!parent_node || data == parent_node->data) {
        return parent_node;
    }
    if(data > parent_node->data) {
        return find_helper(parent_node->right, data);
    } else if(data < parent_node->data) {
        return find_helper(parent_node->left, data);
    }
    return 0;
}

std::shared_ptr<Node> Avl::find(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = root;
    return find_helper(temp, node);
}

std::shared_ptr<Node> Avl::find(int data) {
    std::shared_ptr<Node> temp = root;
    return find_helper(temp, data);
}

void Avl::destroy() {
    root.reset();
}

void Avl::destroy_helper(std::shared_ptr<Node>& node) {
}

void Avl::transplant(std::shared_ptr<Node> node, std::shared_ptr<Node> second_node) 
{
    if(!node->parent) {
        root = second_node;
    }
    if(node->parent && node->parent->right == node) {
        node->parent->right = second_node;
    } else if(node->parent && node->parent->left == node) {
        node->parent->left = second_node;
    }
    if(node->parent && second_node) {
        second_node->parent = node->parent;
    }
}

std::shared_ptr<Node> Avl::inorder_successor(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node;
    if(temp->right) {
        return get_min(temp);
    }
    std::shared_ptr<Node> parent = temp->parent;
    while(parent && temp == parent->right) {
        temp = parent;
        parent = parent->parent;
    }
    return parent;
}

std::shared_ptr<Node> Avl::left_rotate(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node->right;
    node->right = temp->left;
    temp->left = node;
    if(node->right) {
        node->right->parent = node;
    }
    temp->parent = node->parent;
    if(node->parent) {
        if(node == node->parent->left) {
            node->parent->left = temp;
        } else if(node == node->parent->right) {
            node->parent->right = temp;
        }
    }
    node->parent = temp;
    return temp;
}

std::shared_ptr<Node> Avl::right_rotate(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node->left;
    node->left = temp->right;
    temp->right = node;
    if(node->left) {
        node->left->parent = node;
    }
    temp->parent = node->parent;
    if(node->parent) {
        if(node == node->parent->left) {
            node->parent->left = temp;
        } else if(node == node->parent->right) {
            node->parent->right = temp;
        }
    }
    node->parent = temp;
    return temp;
}

std::shared_ptr<Node> Avl::left_right_rotate(std::shared_ptr<Node> node) {
    node->left = left_rotate(node->left);
    return right_rotate(node);

}

std::shared_ptr<Node> Avl::right_left_rotate(std::shared_ptr<Node> node) {
    node->right = right_rotate(node->right);
    return left_rotate(node);
}

void Avl::add_helper(std::shared_ptr<Node> parent, std::shared_ptr<Node> node) {
    if(node->data > parent->data) {
        if(!parent->right) {
            parent->right = node;
            node->parent = parent;
            ++current_size;
        } else {
            add_helper(parent->right, node);
        }
    } else if(node->data < parent->data) {
        if(!parent->left) {
            parent->left = node;
            node->parent = parent;
            ++current_size;
        } else {
            add_helper(parent->left, node);
        }

    } else {
        return;
    }
    check_balance(node);
    return;
}

void Avl::add(std::shared_ptr<Node> node) {
    if(!root) {
        root = node;
        return;
    }
    add_helper(root, node);
}

void Avl::add(int data) {
    if(!root) {
        root = std::make_shared<Node>(data);
        return;
    } 
    std::shared_ptr<Node> node = std::make_shared<Node>(data);
    add_helper(root, node);
}

void Avl::check_balance(std::shared_ptr<Node> node) {
    if(height(node->left) - height(node->right) > 1 || height(node->left) - height(node->right) < -1) {
        node = rebalance(node);
    } 
    if(!node->parent) {
        return;
    }
    check_balance(node->parent);
}

//void Avl::rebalance(std::shared_ptr<Node> node) {
std::shared_ptr<Node> Avl::rebalance(std::shared_ptr<Node> node) {
    if(height(node->left) - height(node->right) > 1) {
        if(height(node->left->left) >= height(node->left->right)) {
            node = right_rotate(node);
        } else {
            node = left_right_rotate(node);
        }

    }
    if(height(node->left) - height(node->right) < -1) {
        if(height(node->right->right) >= height(node->right->left)) {
            node = left_rotate(node);
        } else {
            node = right_left_rotate(node);
        }
    }
    if(!node->parent) {
        root = node;
    }
    return node;
}

void Avl::traverse_inorder(std::vector<int>& out) {
    std::shared_ptr<Node> node = root;
    traverse_inorder_helper(node, out);
}

void Avl::traverse_preorder(std::vector<int>& out) {
    std::shared_ptr<Node> node = root;
    traverse_preorder_helper(node, out);
}

void Avl::traverse_postorder(std::vector<int>& out) {
    std::shared_ptr<Node> node = root;
    traverse_postorder_helper(node, out);
}

void Avl::traverse_inorder_helper(std::shared_ptr<Node> node, std::vector<int>& out) {
    if(!node) {
        return;
    }
    traverse_inorder_helper(node->left, out);
    out.push_back(node->data);
    traverse_inorder_helper(node->right, out);
}

void Avl::traverse_preorder_helper(std::shared_ptr<Node> node, std::vector<int>& out) {
    if(!node) {
        return;
    }
    out.push_back(node->data);
    traverse_preorder_helper(node->left, out);
    traverse_preorder_helper(node->right, out);
}

void Avl::traverse_postorder_helper(std::shared_ptr<Node> node, std::vector<int>& out) {
    if(!node) {
        return;
    }
    traverse_postorder_helper(node->left, out);
    traverse_postorder_helper(node->right, out);
    out.push_back(node->data);
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
			} 
            else {
				result[key]["root"] = true;
			}
			result[key]["height"] = height(v);
			result[key]["balance factor"] = height(v->right) - height(v->left);
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}