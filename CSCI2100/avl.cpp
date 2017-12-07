#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
using namespace std;


// Node in AVL Tree
class Node
{
public:
	int value;
	int height;
	Node * left, *right;

	Node(int value) {
		this->value = value;
		this->left = NULL;
		this->right = NULL;
		this->height = 0;
	}
};


// AVL Tree
class AVLTree 
{
public:
	Node * root;

	AVLTree(){
		this->root = NULL;
	}

	~AVLTree(){
		delete [] root;
	}

	int height(Node * node) {
		if (node == NULL) {
			return 0;
		} else {
			return node->height;
		}
	}
	// insert a node into the tree
	void insert(int value) {
		this->root = this->insert(this->root, value);
	}
	// find the predecessor given the target value
	int predecessor(int value) {
		return this->predecessor(this->root, value);
	}
	// rotation rules for reference
	void printRotationRules() {
		cout << "----------------------------------------------------------------------------" << endl;
		cout << "|      Rotations      |                      Conditions                    |" << endl;
		cout << "----------------------------------------------------------------------------" << endl;
		cout << "| Left Rotation       | Insert a left child to the left tree of a node     |" << endl;
		cout << "| Right Rotation      | Insert a right child to the right tree of a node   |" << endl;
		cout << "| Left Right Rotation | Insert a right child to the left tree of a node    |" << endl;
		cout << "| Right Left Rotation | Insert a left child to the right tree of a node    |" << endl;
		cout << "----------------------------------------------------------------------------" << endl;
	}

private:
	int predecessor(Node * node, int value) {
		// empty tree
		if (node == NULL) {
			return -1;
		}
		// search to left subtree
		if (node->value > value) {
			return this->predecessor(node->left, value);
		} 
		// find the node with same value
		else if (node->value == value) {
			return value;
		} 
		// search to right subtree
		else {
			// right subtree has nodes with value smaller than target, continue
			if (this->minimum(node->right) <= value) {
				return this->predecessor(node->right, value);
			} 
			// all values in right subtree are larger than target, return current value
			else{
				return node->value;
			}
		}
	}
	// return the minumum value of a tree with node as root
	int minimum(Node * node) {
		if ( node == NULL ) {
			return INT_MAX;
		} else if (node->left == NULL) {
			return node->value;
		} else {
			return this->minimum(node->left);
		}
	}
	// return the balacne factor and [-1, 1] is the balance range
	// if value >= 2, imbalance tree with heavy left child
	// if value <= -2, imbalance tree with heavy right child
	int calculateBalance(Node * node) {
	    if (node == NULL) {
	        return 0;
	    }
	    return (this->height(node->left) - this->height(node->right));
	}

	// insert a node through a given root
	Node * insert(Node * node, int value) {
		// empty tree
	    if (node == NULL){
	        return new Node(value);
	    }
	    // node with same value exists
	    if (value == node->value) {
	    	return node;
	    } else if (value < node-> value) {
	        node->left = this->insert(node->left, value);
	    } else {
	        node->right = this->insert(node->right, value);
	    }
	    // update height of the node
	    node->height = 1 + max(height(node->left), height(node->right));
	    // calculate balance factor
	    int balance = this->calculateBalance(node);
	    // if imbalanced, consider all four conditions
	    // for details, refer void printRotationRules() 
	    // LL Case
	    if (balance > 1 && value < node->left->value)
	        return this->rightRotate(node);	 
	    // RR Case
	    if (balance < -1 && value > node->right->value)
	        return this->leftRotate(node);
	    // LR Case
	    if (balance > 1 && value > node->left->value) {
	        node->left = this->leftRotate(node->left);
	        return this->rightRotate(node);
	    }	 
	    // RL Case
	    if (balance < -1 && value < node->right->value) {
	        node->right = this->rightRotate(node->right);
	        return this->leftRotate(node);
	    }
	    return node;
	}
	// left rotation operations with node as root
	Node * leftRotate(Node * node){
		Node * rightChild = node->right;
    	node->right = rightChild->left;
    	rightChild->left = node;
	    node->height = max(this->height(node->left), this->height(node->right)) + 1;
	    rightChild->height = max(this->height(rightChild->left), this->height(rightChild->right)) + 1;
	    return rightChild;
	}
	// right rotation operations with node as root
	Node * rightRotate(Node * node) {
		Node * leftChild = node->left;
	    node->left = leftChild->right;
	    leftChild->right = node;
	    node->height = max(this->height(node->left), this->height(node->right)) + 1;
	    leftChild->height = max(this->height(leftChild->left), this->height(leftChild->right)) + 1;
	    return leftChild;
	}
};



int main(int argc, char * argv[]) {
	string inputFileName = "ops-half.txt";
	if (argc == 2) {
		inputFileName = argv[1];
	}
	AVLTree t = AVLTree();
	vector<int> result;
	ifstream fin(inputFileName);
    for(string line; getline(fin, line);) {
        istringstream istr(line);
        string opts; int value;
        istr >> opts >> value;
        if (opts == "ins") {
             t.insert(value);
        }
        if (opts == "qry") {
        	result.push_back(t.predecessor(value));
        }
    }
    fin.close();
	ofstream fout("output.txt");
	for (size_t i = 0; i != result.size(); ++i) {
		fout << result[i] << endl;
	}
	fout.close();
}



