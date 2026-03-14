#include "activity_bst.h"
#include <iostream>



// using 2 functions provided in the homework pdf
string fill_with_zeros(int num_zeros, const string& input){
	int zeros_to_add = num_zeros - input.length();
	if (zeros_to_add < 0) zeros_to_add = 0;
	return string(zeros_to_add, '0') + input;
}
string number_to_time(int number){
	return fill_with_zeros(2, to_string(number/100))+":"+ fill_with_zeros(2,
		to_string(number%100));
}


//a function that resets the root to its original position, used for recursive iterations to not lose initial position of the root
void rootReseter(tree_node* &root){
	while(root->parent != nullptr){
		root = root->parent;
	}
}

// a function that checks whether time input is valid or not
bool timeChecker(int time){
	if (time > 2359 || time%100 > 59 || time < 0){
		return false;
	}
	return true;
}

// lowerBound is -1 initially
// finds the highest value lower than or equal to time, which is the starting time of that action
// if no lowerBound is found, then its value remains -1, which means free activity in the code
void lowerTimeFinder (tree_node* head, int time, int& lowerBound, string& act){ 
	tree_node* temp = head; // temp traverses the tree

	while (temp!=nullptr){
		if (temp->time == time){
			lowerBound = temp->time;
			act = temp->activity;
			break;
		}
		else if (temp->time < time){
			lowerBound = temp->time;
			act = temp->activity;
			temp = temp->right;
		}
		else{
			temp = temp->left;
		}
	}
}

//upperBound is -1 initially
//finds the lowest value higher than time, which is the ending time of that action
// if no upperBound is found, then its value remains -1, which means activity lasts until 00.00 in the code
void upperTimeFinder (tree_node* head, int time, int& upperBound){
	tree_node* temp = head; // temp traverses the tree
	
	while (temp!=nullptr){

		if (temp->time > time){
			upperBound = temp->time;
			temp = temp->left;
		}
		else{
			temp = temp->right;
		}
	}
}


// a function that finds every occurence of a specific activity and prints time intervals of it, used in print_single_activity
// had to create a new function for this instead of directly writing to print_single_activity
// because function needs to check " 3 free " case without going into recursion (because if tree is fully empty, need to check whether root == nullptr but last iteration of recursion will be root == nullptr as well even if initially tree was not empty)
// therefore cannot call print_single_activity function recursively and check " 3 free" at the same time
// print_single_activity checks " 3 free " cases, then calls treeTraverse recursively to complete for the rest of the cases
void treeTraverse(tree_node* root, const string& activity){
	tree_node* temp = root; // temp traverses the tree

	if (temp!= nullptr){

		treeTraverse(temp->left, activity); 



		if (temp->activity == activity){ // if current node's activity is wanted
			int up = 0; 
			tree_node* copyRoot = root;
			rootReseter(copyRoot); // using this in order not to lose initial position of root
			upperTimeFinder(copyRoot, temp->time, up); // temp-> time is the starting point of that activity, upperTimeFinder finds the end point of that activity for that period (which is why initial position of root is important)
			cout << "Time period: [" << number_to_time(temp->time) << " - " << number_to_time(up) << "]: " << activity << endl;
			
		}

		treeTraverse(temp->right, activity);
	}


}




void add_activity(tree_node *&root, int time, const string &activity){

	if (!timeChecker(time)){
		cout << "ERROR! Could not add activity \'" << activity << "\' due to illegal time value" << endl;

	}

	else{
		
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

		cout << "Added activity \'"<< activity<< "\' at "<< number_to_time(time) << endl; 	

	}
}



void print_activity(tree_node *root, int time){

	if (!timeChecker(time)){
		cout << "ERROR! Could not print activity at specific time due to illegal time" << endl;
	}

	else{
		int startingPoint = -1;
		string act;
		lowerTimeFinder(root, time, startingPoint, act);

		if (startingPoint == -1){ // if startingPoint is not changed in the lowerTimeFinder function, then it is free activity 
			act = "free";
			startingPoint = 0;
		}
		cout << "Time: " << number_to_time(time) << "," << " Activity: " << act << endl;
	}
}



void print_activity_and_duration(tree_node *root, int time){

	if (!timeChecker(time)){
		cout << "ERROR! Could not print activity at specific time due to illegal time" << endl;
	}

	else{
		int startingPoint = -1;
		int endPoint = -1;
		string act;

		lowerTimeFinder(root, time, startingPoint, act);

		if (startingPoint == -1){ // if startingPoint is not changed in the lowerTimeFinder function, then it is free activity
			act = "free";
			startingPoint = 0;
		}

		upperTimeFinder(root, time, endPoint);

		if (endPoint == -1){ // if endPoint is not changed in the upperTimeFinder function, then it ends at 00.00
			endPoint = 0;
		}

		cout << "Time period: [" << number_to_time(startingPoint) << " - " << number_to_time(endPoint) << "]: " << act << endl;
		
	}

	

}


void print_single_activity(tree_node *root, const string &activity){
	// both of these variables are for checking "3 free" case, creating a new recursive function instead of using recursion in this one is also because of that case, as explained at treeTraverse function
	tree_node* temp2 = root;
	int firstTaskTime;

	if (root == nullptr){ // if there is no value in the tree
		cout << "Time period: [00:00 - 00:00]: free" << endl; // then user did not enter any activity, it is free whole day
	}

	else{

		// this part is checking if there is any activity starting from 00.00, if not then it is free activity until first task starts
		if (activity == "free"){
		
			while (temp2->left != nullptr){
				temp2 = temp2->left;
			}

			firstTaskTime = temp2->time;
			
			if (firstTaskTime != 0){
				cout << "Time period: [" << number_to_time(0) << " - " << number_to_time(firstTaskTime) << "]: " << "free" << endl;
			}
		}



		treeTraverse(root, activity); // recursive function to print cases except for fully empty tree case, which is printed above

	}


}


void print_all_activities(tree_node *root){
	tree_node* temp = root;
	// Inorder Traversal to print time intervals and corresponding activities in order
	if (temp){
		
		print_all_activities(temp->left);
		cout << "[" << number_to_time(temp->time) << "] - " << temp->activity << endl;
		print_all_activities(temp->right);
	}




}

void delete_tree(tree_node *root){

	if (root != nullptr){
		
    delete_tree(root->left);
    delete_tree(root->right);
    
    delete root;

	}

}

// Ahmet Alperen Gungor - 28847