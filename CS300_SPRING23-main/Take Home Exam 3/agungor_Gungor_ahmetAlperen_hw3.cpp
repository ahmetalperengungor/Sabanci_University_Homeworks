#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Ahmet Alperen Gungor - 28847

struct Node{ //node that has personal contact information, and features of node that is used in both avl and bst
	string firstName;
	string lastName;
	string number;
	string location;
	string connectedName; // fullname with space between first and last name
	Node* left;
	Node* right;
	int depth;
	Node(){
		left=nullptr;
		right=nullptr;
		depth = 1;
	}
};

// below is for BST implementation
template <typename o>
class BST{
public:
	o* root;

	BST(){
		root = nullptr;
	}

	int maxDepth(o* node){ // a recursive function to find the height of the given node - using the term depth since started like that but then realized height is probably more suitable
		if(node == nullptr){
			return 0;
		}
		else{
			int leftSubtreeDepth = maxDepth(node->left);
			int rightSubtreeDepth = maxDepth(node->right);
			if(leftSubtreeDepth<rightSubtreeDepth){ // if right is larger, use it
				return(rightSubtreeDepth+1);
			}
			else{ // if left is larger (or equal), use it
				return(leftSubtreeDepth+1);
			}
		}
	}

	o* getRoot(){ // currently not needed since root is not a private member of class, but added if user wants the root to be a private member
		return root;
	}

	//inserting a new node to the bst in the correct position
	void insert(string inputFirstName, string inputLastName, string inputNumber, string inputLocation){

		if(root!=nullptr){
			string fullName = inputFirstName + " " + inputLastName;
			o* temp = root;
			while(temp!=nullptr){
				string tempConnectedName = temp->firstName + ' ' + temp->lastName;

				if(fullName<tempConnectedName){// new node is smaller than the current iterated node, thus need to go left
					if(temp->left!=nullptr){
						temp = temp->left;
					}
					else{
						o* newNode = new o();
						newNode->firstName = inputFirstName;
						newNode->lastName = inputLastName;
						newNode->number = inputNumber;
						newNode->location = inputLocation;
						newNode->connectedName = fullName;
						newNode->depth = 0;
						temp->left = newNode;
						temp = nullptr;
					
					}
				}
				else if(tempConnectedName<fullName){ // new node is bigger than the current iterated node, thus need to go right
					if(temp->right!=nullptr){
						temp = temp->right;
					}
					else{
						o* newNode = new o();
						newNode->firstName = inputFirstName;
						newNode->lastName = inputLastName;
						newNode->number = inputNumber;
						newNode->location = inputLocation;
						newNode->connectedName = fullName;
						newNode->depth = 0;
						temp->right = newNode;
						temp = nullptr;

					}
				}
				else{
					cout << "The given contact full name already exists in the database";
					temp = nullptr;
				}
			}
		}
		else{ // checking if the tree is empty currently
			o* newNode = new o();
			newNode->firstName = inputFirstName;
			newNode->lastName = inputLastName;
			newNode->number = inputNumber;
			newNode->location = inputLocation;
			newNode->depth = 0;
			newNode->connectedName = inputFirstName + ' ' + inputLastName;
			root = newNode;
			root->depth = 0;
			return;	
		}
	}


	// helper function of remove that utilizes recursion to find requested information
	o* removeHelper(o* node, string inputFullName){
		if(node==nullptr){
			return nullptr;
		}
		string tempFullName = node->firstName + ' ' + node->lastName;
		if(tempFullName<inputFullName){
			node->right = removeHelper(node->right, inputFullName);
		}
		else if(inputFullName<tempFullName){
			node->left = removeHelper(node->left, inputFullName);
		}
		else{
			if(node->left == nullptr && node->right == nullptr){
				return nullptr;
			}
			else if (node->left == nullptr){
				o* temp = node->right;
				delete node;
				return temp;
			}
			else if(node->right == nullptr){
				o* temp = node->left;
				delete node;
				return temp;
			}
			o* temp2 = node->right;
			while(temp2!=nullptr && temp2->left !=nullptr){
				temp2 = temp2->left;
			}
			o* subTreeNode = temp2;
			node->firstName = subTreeNode->firstName;
			node->lastName = subTreeNode->lastName;
			node->number = subTreeNode->number;
			node->location = subTreeNode->location;

		}
		return node;
	}



	// deleting the given node in bst, utilizing the helper function in order to freely use recursion 
	void remove(string inputFullName){
		removeHelper(root,inputFullName);
	}

	//deleting the given node in bst
	void removeR(string fullName){

		o* temp = root; // represents the wanted node
		o* temp2 = root; // will be the parent of wanted node
		while(temp!=nullptr){
			if(temp->connectedName==fullName){
				break;
			}
			temp2 = temp;
			if(fullName<temp->connectedName){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
			
		}
		if(temp->left==nullptr&&temp->right==nullptr){ //node tbd does not have child
			delete temp;
		}
		else if(temp->left == nullptr&& temp->right!=nullptr){ // node tbd has child on only right
			if(temp2->left == temp){ // if node tbd was left child of its parent
				temp2->left = temp->right;
			}
			else{ // if node tbd was right child of its parent
				temp2->right = temp->right;
			}
			delete temp;
		}
		else if(temp->left!=nullptr&&temp->right==nullptr){ // node tbd has child on only left
			if(temp2->left == temp){ // if node tbd was left child of its parent
				temp2->left = temp->left;
			}
			else{ // if node tbd was right child of its parent
				temp2->right = temp->left;
			}
			delete temp;
		}
		else{ // node tbd has both childs
			o* rightTemp = temp->right; 
			o* rightTemp2 = nullptr; // parent of rightTemp
			while(rightTemp->left!=nullptr){
				rightTemp2 = rightTemp;
				rightTemp = rightTemp->left;
			}
			if(rightTemp2==nullptr){
				temp->firstName = rightTemp->firstName;
				temp->lastName = rightTemp->lastName;
				temp->number = rightTemp->number;
				temp->location = rightTemp->location;
				temp->connectedName = rightTemp->connectedName;
				temp->depth = rightTemp->depth;
				temp->right = rightTemp->right;
			}
			else{
				rightTemp2->left = rightTemp->right;
			}
			delete rightTemp;
		}
	}

	//helper function of search for recursive search
	void searchHelper(o* temp, string str, bool&answer){
		
		if(temp==nullptr){
			return;
		}

		string tempFullName = temp->firstName + ' ' + temp->lastName;
		if(tempFullName.find(str) == 0){

			answer = true;
			string tempFirstName = temp->firstName;
			string tempLastName = temp->lastName;
			
			for(int ctr = 0; ctr< tempFirstName.length();ctr++){
				tempFirstName.at(ctr) = toupper(tempFirstName.at(ctr));
			}
			for(int ctr2 = 0; ctr2< tempLastName.length(); ctr2++){
				tempLastName.at(ctr2) = toupper(tempLastName.at(ctr2));
			}
			cout<< tempFirstName << " " << tempLastName << " " << temp->number << " " <<temp->location << endl;
		}

		searchHelper(temp->left, str, answer);
		searchHelper(temp->right, str, answer);

	}

	//searching for the given input
	void search(string str){
		o* temp = root;
		bool answer = false;
		searchHelper(temp,str,answer);
		if(answer==false){
			cout<<"Phonebook contact not found"<< endl;
		}
	}

	//helper function of inorderPrint for recursive inorder travel
	void inorderPrintHelper(o* temp, ofstream& fname){
		if(temp == nullptr){
			return;
		}

		inorderPrintHelper(temp->left, fname);
		fname << temp->connectedName << " " << temp->number << " " << temp->location << endl;
		inorderPrintHelper(temp->right, fname);
	}

	//printing the phonebook in inorder
	void inorderPrint(){
		ofstream fileName("phonebookInOrderBST.txt");
		inorderPrintHelper(root, fileName);
		fileName.close();
	}

	//helper function of preorderPrint for recursive preorder travel
	void preorderPrintHelper(o* temp, ofstream& fname){
		if(temp==nullptr){
			return;
		}
		fname<< temp->connectedName << " " << temp->number << " " << temp->location << endl;
		preorderPrintHelper(temp->left, fname);
		preorderPrintHelper(temp->right, fname);
	}

	//printing the phonebook in preorder
	void preorderPrint(){
		ofstream filename("phonebookPreOrderBST.txt");
		preorderPrintHelper(root,filename);
		filename.close();
	}

	//helper function to draw the tree into the file
	//right childs have __ left childs have --
	//root has __
	void drawTreeHelper(const string& pre, o* temp, bool isRight, ofstream& fname){
		if(temp!=nullptr){
			fname<<pre<<(isRight ? "|__" : "|--")<<temp->connectedName<<endl;
			drawTreeHelper(pre+(isRight ? "   " : "|  "), temp->left, false, fname);
			drawTreeHelper(pre+(isRight ? "   " : "|  "), temp->right, true, fname);
		}
	}

	//drawing the tree 
	void drawTree(){
		ofstream filename("phonebookTreeBST.txt");
		drawTreeHelper("", root, true, filename);
		filename.close();
	}

};

// AVL implementation starts here


template <typename o>
class AVL{
public:
	o* root;

	AVL(){
		root = nullptr;
	}

	int depth(o* node){ // for finding height of avl tree
	//i started using depth in the bst, even though it is height I am using depth as the variable name, sorry for the confusion
		if (node==nullptr)
		{
			return 0;
		}
		return node->depth; 
	}

	int balanceNumber(o* node){// for finding the balance number
		if(node==nullptr){
			return 0;
		}
		return depth(node->left) - depth(node->right); // calculating the difference between right and left subtrees to find balance number
	}

	o* rightRotate(o* y){ // right direction rotation
		if(y!=nullptr && y->left!=nullptr){
			o* x = y->left;
			o* temp = x->right;
			x->right = y;
			y->left = temp;
			y->depth = max(depth(y->left), depth(y->right)) + 1; // updating the heights, as I said earlier sorry for the confusion of variable names
			x->depth = max(depth(x->left), depth(x->right)) + 1;
			return x;
		}
		return y;
	}

	o* leftRotate(o* x){// left direction rotation
		if(x!=nullptr && x->right!=nullptr){
			o* y = x->right;
			o* temp = y->left;
			y->left = x;
			x->right = temp;
			x->depth = max(depth(x->left), depth(x->right)) + 1; // updating the heights
			y->depth = max(depth(y->left), depth(y->right)) + 1;
			return y;
		}
		return x;
	}

	void towardsLeft(o* &ccc) const{
		rightRotate(ccc->left);
		leftRotate(ccc);
	}

	void towardsRight(o* &c) const{
		leftRotate(c->right);
		rightRotate(c);
	}

	o* insertHelperAVL(o* node, string inputFirstName, string inputLastName, string inputNumber, string inputLocation){ // helper function to recursively add given information into avl tree
		string fullName = inputFirstName + ' ' + inputLastName;

		if(node==nullptr){
			o* newNode = new o();
			newNode->firstName = inputFirstName;
			newNode->lastName = inputLastName;
			newNode->number = inputNumber;
			newNode->location = inputLocation;
			newNode->connectedName = fullName;
			newNode->depth = 1;			
			return newNode;
		}

		string tempFullName = node->firstName + ' ' + node->lastName;

		if(fullName<tempFullName){ // if the current node is bigger than input
			node->left = insertHelperAVL(node->left, inputFirstName, inputLastName, inputNumber, inputLocation);
		}
		else if(tempFullName<fullName){ // if the current node is smaller than input
			node->right = insertHelperAVL(node->right, inputFirstName, inputLastName, inputNumber, inputLocation);
		}
		else{
			return node;
		}
		node->depth = max(depth(node->left), depth(node->right)) + 1; // updating the height of parent
		int balanceCheck = balanceNumber(node); // checking the balance of the parent to be sure about balance issues, if unbalanced following cases can be made

		if(balanceCheck>1 && fullName < (node->left->firstName + ' ' + node->left->lastName)){
			return rightRotate(node); // left and left unbalance, therefore rotating towards right to balance
		}
		if(balanceCheck<-1 && fullName > (node->right->firstName + ' ' + node->right->lastName)){
			return leftRotate(node); // right and right unbalance, therefore rotating towards left to balance
		}
		if(balanceCheck>1 && fullName > (node->left->firstName + ' ' + node->left->lastName)){
			node->left = leftRotate(node->left);
			return rightRotate(node); // left and right unbalance, therefore need to first leftrotate the left child then rightrotate to balance
		}
		if(balanceCheck<-1 && fullName<(node->right->firstName + ' ' + node->right->lastName)){
			node->right = rightRotate(node->right);
			return leftRotate(node); // right and left unbalance, therefore need to first rightrotate the right child then leftrotate to balance
		}
		return node;
	}

	void insertAVL(string inputFirstName, string inputLastName, string inputNumber, string inputLocation){ // main function to insert, calling the helper to recursively complete addition of new information to the avl tree
		root = insertHelperAVL(root, inputFirstName, inputLastName, inputNumber, inputLocation);
	}

	void searchHelperAVL(o* root, string inputFullName, bool &found){ // helper function of searchAVL to recursively look for the given fullname
		
		if (root == nullptr)
		{
			return;
		}
		string connectedName = root->firstName + ' ' + root->lastName;
		if (connectedName.find(inputFullName) == 0)
		{
			found = true;
			string name = root->firstName;
			string surname = root->lastName;
			cout << name << ' ' << surname << ' ' << root->number << ' ' << root->location << endl;
		}
		searchHelperAVL(root->left, inputFullName, found);
		searchHelperAVL(root->right, inputFullName, found);
	}

	void searchAVL(string inputFullName){ //main function to search for a given phonebook contact information
		
		bool isFound = false;
		searchHelperAVL(root, inputFullName, isFound);
		if(isFound==false){
			cout<< "Phonebook contact not found" << endl;
		}
	}


	void inorderPrintHelperAVL(o* node, ofstream& fname){ // helper function of inorder print to recursively print the avl tree inorder order
		if(node == nullptr){
			return;
		}
		// calling inorder order recursion
		inorderPrintHelperAVL(node->left, fname); 
		fname << node->firstName << " " << node->lastName << " " << node->number << " " << node->location << endl;
		inorderPrintHelperAVL(node->right, fname);
	}

	void inorderPrintAVL(){ // main function to inorder print, calling the helper to recursively complete printing the avl tree
		ofstream filename("phonebookInOrderAVL.txt");
		inorderPrintHelperAVL(root, filename);
		filename.close();
	}

	void preorderPrintHelperAVL(o* node, ofstream& fname){ // helper function of preorder print to recursively print the avl tree inorder order
		if(node==nullptr){
			return;
		}
		// callin preorder order recursion
		fname<< node->firstName << " " << node->lastName << " " << node->number << " " << node->location << endl;
		preorderPrintHelperAVL(node->left, fname);
		preorderPrintHelperAVL(node->right, fname);
	}

	void preorderPrintAVL(){// main function to preorder print, calling the helper to recursively complete printing the avl tree
		ofstream filename("phonebookPreOrderAVL.txt");
		preorderPrintHelperAVL(root,filename);
		filename.close();
	}

	void drawHelperAVL(const string& pre, o* node, bool isRight, ofstream& fname){ // helper function of drawing avl tree to recursively draw each row
		if(node!=nullptr){
			fname<< pre << (isRight ? "|__" : "|--") << node->firstName << " " << node->lastName << endl;
			drawHelperAVL(pre + (isRight ? "   " : "|  "), node->left, false, fname);
			drawHelperAVL(pre + (isRight ? "   " : "|  "), node->right, true, fname);
		}
	}

	void drawAVL(){ // main function to draw avl tree into file
		ofstream filename("phonebookTreeAVL.txt");
		drawHelperAVL("", root, true, filename);
		filename.close();
	}

	o* minValueAVL(o* node){ // finding the minimum value in the avl tree
		o* temp = node;
		while(temp!=nullptr && temp->left !=nullptr){
			temp = temp->left;
		}
		return temp;
	}

	o* removeHelperAVL(o *&node, string input){ // helper function of remove to delete the node with the given phonebook information
		if(node == nullptr){
			return node;
		}
		string tempFullName = node->firstName + ' ' + node->lastName;
		if (input<tempFullName){ // if the wanted name is alphabetically lower than the current node, go towards left
			node->left = removeHelperAVL(node->left, input);
		}
		else if (tempFullName<input){ // if the wanted name is alphabetically higher than the current node, go towards right
			node->right = removeHelperAVL(node->right, input);
		}
		else{ // if equal, than the wanted node is found on the avl tree
			if((node->right == nullptr) || (node->left == nullptr)){
				o *temp = node->left ? node->left : node->right;

				// No child case
				if (temp == nullptr)
				{
					temp = node;
					node = nullptr;
				}
				else // there is only 1 child              
					*node = *temp; 
								   
				delete temp;


			}
			else{// there are 2 children
				o* temp = minValueAVL(node->right); // minimum value of the right subtrees child will be changed with the current node to be removed
				node->firstName = temp->firstName;
				node->lastName = temp->lastName;
				node->number = temp->number;
				node->location = temp->location;
				node->right = removeHelperAVL(node->right, (temp->firstName + " " + temp->lastName));
			}
		}
		if(node==nullptr){
			return node;
		}
		else{
			// same process done in the insert function, in order the balance the avl tree based on the unbalanced situation
			node->depth = max(depth(node->left), depth(node->right)) + 1;
			int balanceCheck = balanceNumber(node);
			if(balanceCheck>1 && balanceNumber(node->left) >= 0){
				return rightRotate(node);
			}
			else if(balanceCheck<-1 && balanceNumber(node->right)<= 0){
				return leftRotate(node);
			}
			else if(balanceCheck>1 && balanceNumber(node->left)<0){
				leftRotate(node->left);
				return rightRotate(node);
			}
			else if (balanceCheck<-1 && balanceNumber(node->right) > 0){
				rightRotate(node->right);
				return leftRotate(node);
			}
		}
		return node;
	}

	void removeAVL(string inputFullName){ // main function to remove a given node with contact information from the avl tree
		root = removeHelperAVL(root, inputFullName);
	}

	o* getRoot(){ // unnecessary since currently the root is not private, but added in case user requests private root
		return root;
	}
};




int main(){
	string filename;
	cout<< "Please enter the contact file name: ";
	cin>>filename;
    ifstream myfile(filename);
    string line;
    BST<Node> binaryTree;
	AVL<Node> avlTree;
	double bstTime = 0.0;
	double avlTime = 0.0;

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            stringstream ss(line);
            string word;
            int wordCount = 0;
            string info[4];
            while (ss >> word)
            {
				for (char& c : word){ // turning input into uppercase
					c = toupper(c);
				}
                info[wordCount] = word;
                wordCount++;
            }

			
            high_resolution_clock::time_point bst_start = high_resolution_clock::now();
            binaryTree.insert(info[0], info[1], info[2], info[3]);
            high_resolution_clock::time_point bst_end = high_resolution_clock::now();
            microseconds duration = duration_cast<microseconds>(bst_end - bst_start);
            bstTime += duration.count();

            high_resolution_clock::time_point avl_start = high_resolution_clock::now();
            avlTree.insertAVL(info[0], info[1], info[2], info[3]);
            high_resolution_clock::time_point avl_end = high_resolution_clock::now();
            microseconds duration_avl = duration_cast<microseconds>(avl_end - avl_start);
            avlTime += duration_avl.count();

        }
        myfile.close();
    }

    cout << "Loading the phonebook into a BST." << endl;
    cout << "Phonebook creation in BST took " << bstTime << " microseconds .  .  ." << endl;
	Node* rootBst = binaryTree.root;
	int leftBstDepth = binaryTree.maxDepth(rootBst->left); // using another function inside bst class to determine the height
	int rightBstDepth = binaryTree.maxDepth(rootBst->right);
	Node* rootAvl = avlTree.root;

    if (leftBstDepth - rightBstDepth != 1 && leftBstDepth - rightBstDepth != -1 &&leftBstDepth - rightBstDepth!=0)
    {
        cout << "The tree is not balanced" << endl;
    }
    else
    {
        cout << "The tree is balanced" << endl;
    }
    cout << "The heights of BST are for left: " << leftBstDepth << " and right: " << rightBstDepth << endl;
    cout << "Loading the phonebook into an AVL" << endl;
    cout << "Phonebook creation in AVL took " << avlTime << " microseconds .  .  ." << endl;
    cout << "The tree is balanced" << endl;
    cout << "The heights of AVL are for left: " << rootAvl->left->depth << " and right: " << rootAvl->right->depth << endl;
    cout << endl;

	bool check = true;
	while(check==true){
        cout << "Choose which action to perform from 1 to 6:" << endl;
        cout << "1 - Search a phonebook contact" << endl;
        cout << "2 - Adding a phonebook contact" << endl;
        cout << "3 - Deleting a phonebook contact" << endl;
        cout << "4 - Print the phonebook to a file(inorder)\n    Print the phonebook to a file(preorder)" << endl;
        cout << "5 - Draw the phonebook as a Tree to a file" << endl;
        cout << "6 - Press 6 to exit" << endl;

		int option;
		cin>>option;
		if(option==1){ // if user wants to search a contact in the phonebook
            string inputWanted;
			cout<<endl;
            cout << "Search for a contact: ";
            getline(cin >> ws, inputWanted);
			cout<<endl;
            bool newWord = true;

			for (int i = 0; i < inputWanted.length(); i++)
			{
				if (isalpha(inputWanted.at(i)))
				{
					inputWanted.at(i) = toupper(inputWanted.at(i));
				}
			}


            cout << "Searching an item in the phonebook (BST) . . ." << endl;
			cout<< endl;
            cout << "Phonebook: Searching for: (" << inputWanted << ")" << endl;

            cout << "====================================" << endl;
            high_resolution_clock::time_point bst_search_start = high_resolution_clock::now();
            binaryTree.search(inputWanted);
            high_resolution_clock::time_point bst_search_end = high_resolution_clock::now();
            nanoseconds duration_bst_search = duration_cast<nanoseconds>(bst_search_end - bst_search_start);
            cout << endl;
            cout << "Searching an item in the phonebook (AVL) . . ." << endl;
            cout << "Phonebook: Searching for: (" << inputWanted << ")" << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point avl_search_start = high_resolution_clock::now();
            avlTree.searchAVL(inputWanted);
            high_resolution_clock::time_point avl_search_end = high_resolution_clock::now();
            nanoseconds duration_avl_search = duration_cast<nanoseconds>(avl_search_end - avl_search_start);
            cout<<endl;
            cout << "The search in BST took " << duration_bst_search.count() << " nanoseconds " << endl;
            cout << "The search in AVL took " << duration_avl_search.count() << " nanoseconds " << endl;
		}
		else if(option == 2){ // if user wants to add a phonebook contact
            string input;
            string inputFirstName;
            string inputLastName;
            string inputNumber;
            string inputLocation;
            cout << "Enter the information of the contact to be added:" << endl;
            cout << "Name:";
            getline(cin >> ws, input);
            istringstream iss(input);
            iss >> inputFirstName >> inputLastName;

			for (char& c : inputFirstName){ // turning input firstname into uppercase
				c = toupper(c);
			}

			for (char& c : inputLastName){ // turning input lastname into uppercase
				c = toupper(c);
			}

            cout << endl;
            cout << "Tel: ";
            cin >> inputNumber;
            cout << endl;
            cout << "City: ";
            cin >> inputLocation;
            cout << endl;
            cout << "Adding an item to the phonebook BST . . ." << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point bst_insertion_start = high_resolution_clock::now();
            binaryTree.insert(inputFirstName, inputLastName, inputNumber, inputLocation);
            high_resolution_clock::time_point bst_insertion_end = high_resolution_clock::now();
            nanoseconds duration_bst_insertion = duration_cast<nanoseconds>(bst_insertion_end - bst_insertion_start);
			cout << "Contact has been added successfully to the BST" << endl;
			cout << endl;

            cout << "Adding an item to the phonebook (AVL) . . ." << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point avl_insertion_start = high_resolution_clock::now();
            avlTree.insertAVL(inputFirstName, inputLastName, inputNumber, inputLocation);
            high_resolution_clock::time_point avl_insertion_end = high_resolution_clock::now();
            nanoseconds duration_avl_insertion = duration_cast<nanoseconds>(avl_insertion_end - avl_insertion_start);
            cout << "Contact has been added successfully to the AVL Tree" << endl;
            cout << endl;
            cout << "Adding a contact to the Binary Tree took " << duration_bst_insertion.count() << " nanoseconds .  .  ." << endl;
            cout << "Adding a contact to the AVL Tree took " << duration_avl_insertion.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
		}
		else if(option == 3){ // if user wants to delete a contact from the phonebook
            cout << "Deleting an item from the phonebook . . ." << endl;
            cout << "Enter the fullname of the contact to be deleted: ";
            string name;
            getline(cin >> ws, name);
            bool newWord = true;

			for (int i = 0; i < name.length(); i++)
			{
				if (isalpha(name.at(i)))
				{
					name.at(i) = toupper(name.at(i));
				}
			}

            cout << endl;
			
			
            high_resolution_clock::time_point bst_deletion_start = high_resolution_clock::now();
            binaryTree.remove(name);
            high_resolution_clock::time_point bst_deletion_end = high_resolution_clock::now();
            nanoseconds duration_bst_deletion = duration_cast<nanoseconds>(bst_deletion_end - bst_deletion_start);
			
            high_resolution_clock::time_point avl_deletion_start = high_resolution_clock::now();
            avlTree.removeAVL(name);
            high_resolution_clock::time_point avl_deletion_end = high_resolution_clock::now();
            nanoseconds duration_avl_deletion = duration_cast<nanoseconds>(avl_deletion_end - avl_deletion_start);
            cout << "Deleted succcessfully. . ." << endl;
            cout << endl;
            cout << "Deleting from the Binary Tree took " << duration_bst_deletion.count() << " nanoseconds .  .  ." << endl;
            cout << "Deleting from AVL Tree took " << duration_avl_deletion.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
		}
		else if (option == 4){ // if user wants to print inorder and preorder
            high_resolution_clock::time_point bst_writing_i_start = high_resolution_clock::now();
            binaryTree.inorderPrint();
            high_resolution_clock::time_point bst_writing_i_end = high_resolution_clock::now();
            nanoseconds duration_bst_writing_i = duration_cast<nanoseconds>(bst_writing_i_end - bst_writing_i_start);
            high_resolution_clock::time_point bst_writing_p_start = high_resolution_clock::now();
            binaryTree.preorderPrint();
            high_resolution_clock::time_point bst_writing_p_end = high_resolution_clock::now();
            nanoseconds duration_bst_writing_p = duration_cast<nanoseconds>(bst_writing_p_end - bst_writing_p_start);
            high_resolution_clock::time_point avl_writing_i_start = high_resolution_clock::now();
            avlTree.inorderPrintAVL();
            high_resolution_clock::time_point avl_writing_i_end = high_resolution_clock::now();
            nanoseconds duration_avl_writing_i = duration_cast<nanoseconds>(avl_writing_i_end - avl_writing_i_start);
            high_resolution_clock::time_point avl_writing_p_start = high_resolution_clock::now();
            avlTree.preorderPrintAVL();
            high_resolution_clock::time_point avl_writing_p_end = high_resolution_clock::now();
            nanoseconds duration_avl_writing_p = duration_cast<nanoseconds>(avl_writing_p_end - avl_writing_p_start);
            
            cout << "Printing in order to file from the Binary Tree took " << duration_bst_writing_i.count() << " nanoseconds .  .  ." << endl;
            cout << "Printing in order to file from AVL Tree took " << duration_avl_writing_i.count() << " nanoseconds .  .  ." << endl;
            cout << "Printing pre order to file from the Binary Tree took " << duration_bst_writing_p.count() << " nanoseconds .  .  ." << endl;
            cout << "Printing pre order to file from AVL Tree took " << duration_avl_writing_p.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
			cout<<endl;
            
		}
		else if (option == 5){// if user wants to draw the trees into file
            high_resolution_clock::time_point bst_drawing_start = high_resolution_clock::now();
            binaryTree.drawTree();
            high_resolution_clock::time_point bst_drawing_end = high_resolution_clock::now();
            nanoseconds duration_drawing_bst = duration_cast<nanoseconds>(bst_drawing_end - bst_drawing_start);
            high_resolution_clock::time_point avl_drawing_start = high_resolution_clock::now();
            avlTree.drawAVL();
            high_resolution_clock::time_point avl_drawing_end = high_resolution_clock::now();
            nanoseconds duration_drawing_avl = duration_cast<nanoseconds>(avl_drawing_end - avl_drawing_start);
            cout<<  endl;
            cout << "Drawing tree to file from the Binary Tree took " << duration_drawing_bst.count() << " nanoseconds .  .  ." << endl;
            cout << "Drawing tree to file from AVL Tree took " << duration_drawing_avl.count() << " nanoseconds .  .  ." << endl;
		}
		else if (option == 6){
			cout << "Exiting..."<<endl;
			check = false;
		}
	}

	return 0;
}








//HASH TABLE IMPLEMENTATION



hash_node:: hash_node(){
    next=nullptr;
}
hash_node::hash_node(string full_name_input,string phone_number_input,string city_input): full_name(full_name_input), phone_number(phone_number_input),city(city_input),next(nullptr){}

template<typename T> 
int hash_table<T>::hash_function(string key ){

    int hashing_idx;
    int sum = 0;
    int factor = 10;
    for( int i = 0; i <key.length(); i++ ){
        sum+=int(key[i]);

    }
    hashing_idx = sum % factor;
    return hashing_idx;
}
template<typename T>

float hash_table<T>::load_factor(){
    return float(num_of_elements+1)/float(capacity);
}

template<typename T >
void hash_table<T>::insert(string full_name_input,string phone_number_input,string city_input){
    while(load_factor()>0.9){
        this->rehash();
    }
    int idx=this->hash_function(full_name_input);
    T* adding= new T(full_name_input,phone_number_input,city_input);
    if(this->arr[idx]==nullptr){
        this->arr[idx]=adding;

    }
    else{
        adding->next = this->arr[idx];
        this->arr[idx]=adding;

}

return;
}

template<typename T>
string hash_table<T>::search(string key){
    int idx=this->hash_function(key);
    T* head_linked=this->arr[idx];
    while(head_linked!=nullptr){
        if(head_linked->full_name==key){
            return head_linked->full_name+" "+head_linked->phone_number+" "+head_linked->city;

        }
        head_linked=head_linked->next;

}
    return "Name not found!";
    
}

template<typename T>
string hash_table<T>::delete_contact(string key){
    int idx=this->hash_function(key);
    T* head_linked=this->arr[idx];
    if( head_linked->full_name==key){
        if(head_linked->next){
            this->arr[idx]=head_linked->next;
            delete head_linked;
            return "Deleted Successfully";
        }
        else{
            delete head_linked;
            this->arr[idx]=nullptr;
            return "Deleted Successfully";
        }
    }
    else{
    while(head_linked!=nullptr){
        if(head_linked->next){

        if(head_linked->next->full_name==key){
            if(head_linked->next->next){
                T* tmp=head_linked->next;
                T* tmp_2=head_linked->next->next;
                head_linked->next=tmp_2;
                delete tmp;
            }
            else{
                delete head_linked->next;
            }

            return "Deleted Successfully";
            
        

        }
        }
        head_linked=head_linked->next;

}
}
    return "Name not found!";

    
}


template<typename T>
void hash_table<T>::rehash(){
    int old_capacity = capacity;
    T** tmp=this->arr;
    this->capacity = old_capacity*2;
    this->arr= new T*[this->capacity];
    cout<<"previous table size: "<<old_capacity<<", new table size: " <<this->capacity<< ", current unique item count " <<this->num_of_elements+1 <<", current load factor: "<< this->load_factor()<<endl;
    cout<<"rehashed..."<<endl;
    for (int i = 0; i < this->capacity; i++)
        {
            arr[i] = nullptr;
        }
    for ( int j=0 ; j < old_capacity ; j++){
        T* current_node=tmp[j];
        
        while(current_node!=nullptr){
            this->insert(current_node->full_name,current_node->phone_number,current_node->city);
            current_node = current_node->next;
            
        }



    }
    delete[] tmp;
    return;

}





int main()
{
    string file_name;
    cout << "Please enter the contact file name: ";
    cin >> file_name;
    ifstream myfile(file_name);
    string line;
    BinaryTree<binary_node> binary_tree;
    hash_table<hash_node> HASH_TABLE;
    double bst_time = 0.0;
    double avl_time = 0.0;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            stringstream ss(line);

            string word;
            int word_cnt = 0;
            string info[4];
            while (ss >> word)
            {

                info[word_cnt] = word;
                word_cnt++;
            }
            transform(info[0].begin(), info[0].end(), info[0].begin(), ::toupper);
            transform(info[1].begin(), info[1].end(), info[1].begin(), ::toupper);
            
            high_resolution_clock::time_point bst_start = high_resolution_clock::now();
            binary_tree.add_contact(info[0], info[1], info[2], info[3]);
            high_resolution_clock::time_point bst_end = high_resolution_clock::now();
            microseconds duration = duration_cast<microseconds>(bst_end - bst_start);
            bst_time += duration.count();
            high_resolution_clock::time_point avl_start = high_resolution_clock::now();
            HASH_TABLE.insert(info[0]+" "+ info[1], info[2], info[3]);
            HASH_TABLE.num_of_elements++;
            high_resolution_clock::time_point avl_end = high_resolution_clock::now();
            microseconds duration_avl = duration_cast<microseconds>(avl_end - avl_start);
            avl_time += duration_avl.count();
        }
        myfile.close();
    }
    cout << "Loading the phonebook into a BST." << endl;
    cout << "Phonebook creation in BST took " << bst_time << " microseconds .  .  ." << endl;
    
    cout << "Loading the phonebook into an Hash Table" << endl;
    cout << "Phonebook creation in Hash Table took " << avl_time << " microseconds .  .  ." << endl;


    cout << endl;
    cout<<"After preprocessing, the contact count is "<<HASH_TABLE.num_of_elements<<". Current load ratio is "<<HASH_TABLE.load_factor()<<endl;
    bool option_check = true;
    while (option_check)
    {
        cout << "Choose which action to perform from 1 to 6:" << endl;
        cout << "1 - Search a phonebook contact" << endl;
        cout << "2 - Adding a phonebook contact" << endl;
        cout << "3 - Deleting a phonebook contact" << endl;
        cout << "4 - Press 4 to exit" << endl;

        int option;
        cin >> option;
        if (option == 1)
        {
            string search_input;
            cout << "Search for a contact: ";
            getline(cin >> ws, search_input);
            bool new_word = true;
           
            transform(search_input.begin(), search_input.end(), search_input.begin(), ::toupper);

            cout << "Searching an item in the phonebook (BST) . . ." << endl;
            cout << "Phonebook: Searching for: (" << search_input << ")" << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point bst_search_start = high_resolution_clock::now();
            binary_tree.search_a_contact(search_input);
        
            high_resolution_clock::time_point bst_search_end = high_resolution_clock::now();
            nanoseconds duration_bst_search = duration_cast<nanoseconds>(bst_search_end - bst_search_start);
            cout<<endl;
            cout << "BST Search Time: " << duration_bst_search.count() << " nanoseconds " << endl;

            cout << endl;
            cout << "Searching an item in the phonebook (Hash Table) . . ." << endl;
            cout << "Phonebook: Searching for: (" << search_input << ")" << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point avl_search_start = high_resolution_clock::now();
            cout<<HASH_TABLE.search(search_input)<<endl;
            high_resolution_clock::time_point avl_search_end = high_resolution_clock::now();
            nanoseconds duration_avl_search = duration_cast<nanoseconds>(avl_search_end - avl_search_start);
            cout<<endl;
            cout << "Hashtable Search Time: " << duration_avl_search.count() << " nanoseconds " << endl;
            cout<<"Speed Up: "<<float(duration_bst_search.count())/float(duration_avl_search.count())  << endl;
        }
        else if (option == 2)
        {
            string input;
            string input_name;
            string input_surname;
            string input_number;
            string input_city;
            cout << "Enter the information of the contact to be added:" << endl;
            cout << "Name:";
            getline(cin >> ws, input);
            istringstream iss(input);
            iss >> input_name >> input_surname;
            cout << endl;
            transform(input_name.begin(), input_name.end(), input_name.begin(), ::toupper);
            transform(input_surname.begin(), input_surname.end(), input_surname.begin(), ::toupper);
            cout << "Tel: ";
            cin >> input_number;
            cout << endl;
            cout << "City: ";
            cin >> input_city;
            cout << endl;
            cout << "Adding an item to the phonebook BST . . ." << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point bst_insertion_start = high_resolution_clock::now();
            binary_tree.add_contact(input_name, input_surname, input_number, input_city);
            high_resolution_clock::time_point bst_insertion_end = high_resolution_clock::now();
            nanoseconds duration_bst_insertion = duration_cast<nanoseconds>(bst_insertion_end - bst_insertion_start);
            cout << "Contact has been added successfully to the BST" << endl;
            cout << endl;
            cout << "Adding an item to the phonebook (Hashtable) . . ." << endl;
            cout << "====================================" << endl;
            high_resolution_clock::time_point avl_insertion_start = high_resolution_clock::now();
            HASH_TABLE.insert(input_name+" "+input_surname,input_number,input_city);

            high_resolution_clock::time_point avl_insertion_end = high_resolution_clock::now();
            nanoseconds duration_avl_insertion = duration_cast<nanoseconds>(avl_insertion_end - avl_insertion_start);
            cout << "Contact has been added successfully to the Hashtable" << endl;
            cout << endl;
            cout << "Adding a contact to the Binary Tree took " << duration_bst_insertion.count() << " nanoseconds .  .  ." << endl;
            cout << "Adding a contact to the Hashtable took " << duration_avl_insertion.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
        }

        else if (option == 3)
        {
            cout << "Deleting an item from the phonebook . . ." << endl;
            cout << "Enter the fullname of the contact to be deleted: ";
            string name;
            getline(cin >> ws, name);
            transform(name.begin(), name.end(), name.begin(), ::toupper);

            
            cout << endl;

            high_resolution_clock::time_point bst_deletion_start = high_resolution_clock::now();
            binary_tree.delete_contact(name);
            high_resolution_clock::time_point bst_deletion_end = high_resolution_clock::now();
            nanoseconds duration_bst_deletion = duration_cast<nanoseconds>(bst_deletion_end - bst_deletion_start);
            high_resolution_clock::time_point avl_deletion_start = high_resolution_clock::now();
            HASH_TABLE.delete_contact(name);
            high_resolution_clock::time_point avl_deletion_end = high_resolution_clock::now();
            nanoseconds duration_avl_deletion = duration_cast<nanoseconds>(avl_deletion_end - avl_deletion_start);
            
            cout << "Deleting from the Binary Tree took " << duration_bst_deletion.count() << " nanoseconds .  .  ." << endl;
            cout << "Deleting from Hashtable took " << duration_avl_deletion.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
        }

      

        

        else if (option == 4)
        {
            cout<<"Exiting..."<<endl;
            option_check = false;
        }
    }

    return 0;
}