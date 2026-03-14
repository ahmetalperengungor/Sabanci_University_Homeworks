#include "ActivityBST.h"
using namespace std;

// -------------helper functions either created by me or provided in pdf-------------
// a function given to us in pdf
string fill_with_zeros(int num_zeros, const string& input){
	int zeros_to_add = num_zeros - input.length();
	if (zeros_to_add < 0) zeros_to_add = 0;
	return string(zeros_to_add, '0') + input;
}

// a function given to us in pdf
string number_to_time(int number){
	return fill_with_zeros(2, to_string(number/100))+":"+ fill_with_zeros(2,
		to_string(number%100));
}

// a function given to us in pdf
bool timeChecker(int time){
	if (time > 2359 || time%100 > 59 || time < 0){
		return false;
	}
	return true;
}

// a recursive helper function for copy constructor (deep copy)
void ActivityBST::deepCopyHelper(tree_node* temp){
	if (temp != nullptr){ // if not null
		if (temp->left != nullptr){ // if temp has a node in left
			this->add(temp->left->time, temp->left->activity); // add this node's value to the current activity tree (the one contents are copied to)

			deepCopyHelper(temp->left);

		}
				
		if (temp->right != nullptr){ // if temp has a node in right
			this->add(temp->right->time, temp->right->activity); // add this node's time and activity to the current activity tree (lhs)

			deepCopyHelper(temp->right);

		}
	}
}

// an in-order recursive helper function for + operator
void ActivityBST::addOperatorHelper(tree_node* temp){

	if (temp != nullptr){ // if not null
		// in-order traverses tree
		addOperatorHelper(temp->left); 

		add(temp->time, temp->activity); // add this node's time and activity to the current activity tree (lhs)
	
		addOperatorHelper(temp->right);
	}
}

// a recursive helper function to delete binary tree
void destructorHelper(tree_node* root){
	if (root != nullptr){
		
    destructorHelper(root->left);
    destructorHelper(root->right);
    
    delete root;

	}
}

// a recursive helper function for << operator
void printActivityHelper(tree_node* root, ostream& os){
	tree_node* temp = root;
	int time;
	if (temp != nullptr){
		//in-order traverses tree, there prints from lowest time value to highest time value
		printActivityHelper(temp->left, os);

		time = temp->time;
		os << "[" << number_to_time(time) << "] - " << temp->activity << endl;

		printActivityHelper(temp->right, os);

	}
}

//-------------functions described in the document-------------
// default constructor
ActivityBST::ActivityBST(){
	root = nullptr;
}

// copy constructor (deep copy)
ActivityBST::ActivityBST(const ActivityBST& copied){
	tree_node* temp = copied.root; // root of rhs object assigned to temporary pointer
	root = new tree_node(temp->time, temp->activity); // new tree node with rhs's(copied) inital values are assigned to root of lhs object
	this->deepCopyHelper(temp); // calling recursive helper function of copy constructor
}

// destructor
ActivityBST::~ActivityBST(){
	destructorHelper(root); // calling recursive helper function of destructor
}

//add function which is exact same as take home 3 
void ActivityBST::add(int time, const string& activity){
	if (timeChecker(time)){
		tree_node* temp; // traverses the tree
		tree_node* newNode = new tree_node(time, activity);

		if (root==nullptr){ // if there isnt a node in the tree yet
			root = newNode;
		}
		else{
			temp = root; // start traversing from the root
			while (temp != nullptr){
				if (time < temp->time){
					if (temp->left){ // if there is a node in the left side
						temp = temp->left; // continue traversing there
					}
					else{ // if there isnt 
						temp->left = newNode; // newNode is inserted here
						newNode->parent = temp; 
						break;
					}

				}
				else{
					if (temp->right != nullptr){ // if there is a node in the right side
						temp = temp->right; // continue traversing there
					}
					else{ // if there isnt 
						temp->right = newNode; // newNode is inserted here
						newNode->parent = temp;
						break;
					}
				}
			}
		}
	}
}

// = operator
const ActivityBST& ActivityBST::operator = (const ActivityBST& rhs){
	if (this!=&rhs){
		destructorHelper(root); // deleting the lhs object first by calling recursive helper function of destructor
		tree_node* temp = rhs.root; 
		root = rhs.root; // assigning root of rhs to lhs object
		this->deepCopyHelper(temp); // calling recursive helper function of copy constructor to copy rhs values to lhs
	}
	return *this; // returning the final modified object
}

// + operator
ActivityBST ActivityBST::operator + (const ActivityBST& rhs) const{
	
	ActivityBST wanted; // wanted will be the final modified object, which will be created by combining lhs and rhs of +

	tree_node* lhsRoot = root;
	tree_node* rhsRoot = rhs.root;
	// lhs and rhs objects will be combined and added to wanted by calling the recursive helper function of + operator
	wanted.addOperatorHelper(lhsRoot); 
	wanted.addOperatorHelper(rhsRoot);
	return wanted; // returning wanted, which is combination of lhs and rhs
}

// += operator
const ActivityBST& ActivityBST::operator += (const ActivityBST& rhs){
	addOperatorHelper(rhs.root); // calling recursive helper function of + operator
	return *this; // returning combined value (lhs and rhs)
}

// << operator
ostream& operator << (ostream& os, const ActivityBST& rhs){
	printActivityHelper(rhs.root, os); // calling recursive helper function of << operator
	return os; // returning ostream object
}


// Ahmet Alperen Gungor - 28847