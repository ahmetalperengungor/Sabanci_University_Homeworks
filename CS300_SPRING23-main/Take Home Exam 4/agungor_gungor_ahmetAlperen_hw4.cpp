#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <vector>

using namespace std;
using namespace std::chrono;

// Ahmet Alperen Gungor - 28847

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
					cout << "The given contact full name already exists in the database: ";
					cout<< fullName<<endl;
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
			cout<<"Name not found!"<< endl;
		}
	}

	//helper function of search for recursive search, but does not print the outputs to the console for finding time
	void NONPRINTsearchHelper(o* temp, string str, bool&answer){
		
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
			//cout<< tempFirstName << " " << tempLastName << " " << temp->number << " " <<temp->location << endl;
		}

		NONPRINTsearchHelper(temp->left, str, answer);
		NONPRINTsearchHelper(temp->right, str, answer);

	}

	//searching for the given input
	void NONPRINTsearch(string str){
		o* temp = root;
		bool answer = false;
		NONPRINTsearchHelper(temp,str,answer);
		/*if(answer==false){
			cout<<"Name not found!"<< endl;
		}*/
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

//node that has personal contact information and next pointer, used to create hash tables with seperate chaining
struct hNode{
	string firstName;
	string lastName;
	string number;
	string location;
	string connectedName;
	hNode* next;
	hNode(){next=nullptr;}
	hNode(string inputFirstName, string inputLastName, string inputNumber, string inputLocation):firstName(inputFirstName), lastName(inputLastName), number(inputNumber), location(inputLocation), next(nullptr){
		connectedName = inputFirstName + ' ' + inputLastName;
	}
};


// hash function, taken from textbook, chapter 5.2 figure 5.4 that uses each word in the string to calculate key
int hashFunction(const string& key, int tableSize){
	int hashVal = 0;
	for (int ctr = 0; ctr<key.length(); ctr++){
		hashVal = 37* hashVal + key[ctr];
	}
	hashVal%=tableSize;
	if(hashVal<0){
		hashVal+=tableSize;
	}
	return hashVal;
}

// implementation of hash table, using seperate chaining as collision method
template <typename o>
class HT{
public:
	int currentSize;
	int tableSize;
	o** arr;

	HT(int inTableSize = 3);
	float loadFactor();	
	void rehash();
	void insert(string inputFirstName, string inputLastName, string inputNumber, string inputLocation);
	string remove(string inputFullName);
	string search(string inputFullName);
};

// constructor
template <typename o>
HT<o>::HT(int inTableSize){
	currentSize = 0;
	tableSize = inTableSize;
	arr = new o* [inTableSize];
	for (int ctr = 0; ctr < tableSize; ctr++){
		arr[ctr] = nullptr; // empty array
	}
} 

// returns load factor value, which is the proportion of current number of elements to max number of elements
template <typename o>
float HT<o>::loadFactor(){
	return (float(currentSize+1)/float(tableSize));
}

// rehashing function in order to make sure load factor is lower than 0.9
template <typename o>
void HT<o>::rehash(){
	int prevTableSize = tableSize; // prevTableSize keeps track of table size
	tableSize = prevTableSize*2; // update the current table size as the double of the previous table size
	o** tempArr = arr; // tempArr keeps track of the old array
	arr = new o* [tableSize]; // update the current array as a new array with the doubled capacity

	cout<<"rehashed..."<<endl;
    cout<<"previous table size: "<<prevTableSize<<", new table size: " <<tableSize<< ", current unique item count, " <<currentSize+1 << endl;
	cout<<"current load factor: "<< loadFactor()<<endl;
    

	for (int ctr = 0; ctr < tableSize; ctr++){
		arr[ctr] = nullptr; // empty array
	}

	for (int ctr2 = 0; ctr2 < prevTableSize; ctr2++){
		o* currentRowHead = tempArr[ctr2];
		while(currentRowHead!=nullptr){ // currentRowHead starts from the given array of mapped key position and adds personal contact information of the current key, moves until nullptr is reached
			insert(currentRowHead->firstName, currentRowHead->lastName, currentRowHead->number, currentRowHead->location);
			currentRowHead = currentRowHead->next;
		}
	}
	delete[] tempArr;
	return;
}

// insertion function for contacts personal information, creates a hash node for that 
template <typename o>
void HT<o>::insert(string inputFirstName, string inputLastName, string inputNumber, string inputLocation){

	while(loadFactor()>=0.75){ // even though the upper limit is 0.9, ensuring load factor stays 0.75 at max increased the speed of my code, therefore using this value
		rehash();
	}
	string inputConnectedName = inputFirstName + ' ' + inputLastName;
	o* inputNode = new o(inputFirstName, inputLastName, inputNumber, inputLocation);
	int mappedIndex = hashFunction(inputConnectedName, tableSize); // where the current given input will be placed
	if(arr[mappedIndex]==nullptr){ // if the position for the wanted key if empty
		arr[mappedIndex] = inputNode; // adding the current node that has the user contact information
	}
	else{
		inputNode->next = arr[mappedIndex];
		arr[mappedIndex] = inputNode;
	}
	return;
}

// deleting a given contact information from the hash table with a given firstname and lastname
template <typename o>
string HT<o>::remove(string inputFullName){
	string consoleOutput = "Name not found!";
	int mappedIndex = hashFunction(inputFullName, tableSize); // the key value of a given fora given first and last name
	o* currentRowHead = arr[mappedIndex];
	if(currentRowHead->connectedName == inputFullName){ // input is found at the beginning of a given key index
		if(currentRowHead->next!=nullptr){ // there exists other values for a given key index
			arr[mappedIndex] = currentRowHead->next;
			delete currentRowHead;
			consoleOutput = "Deleted Successfully";
			return consoleOutput;
		}
		else{ // there does not exist any other value for the given key index
			arr[mappedIndex] = nullptr;
			delete currentRowHead;
			consoleOutput = "Deleted Successfully";
			return consoleOutput;
		}
	}
	else{
		o* prevRowHead = currentRowHead; // will be coming from behind, in order to connect the node before deletion to connect next node
		currentRowHead = currentRowHead->next; // will iterate over the nodes to find if requested node exists in list, if so deletes it
		while(currentRowHead!=nullptr){
			if(currentRowHead->connectedName == inputFullName){
				prevRowHead->next = currentRowHead->next;
				delete currentRowHead;
				consoleOutput = "Deleted Successfully";
				return consoleOutput;
			}
			else{
				prevRowHead = currentRowHead;
				currentRowHead = currentRowHead->next;
			}

		}
	}
	return consoleOutput;
}

// searches for a given contact information
template <typename o>
string HT<o>::search(string inputFullName){

	int mappedIndex = hashFunction(inputFullName, tableSize); // the key value of a given fora given first and last name
	o* currentRowHead = arr[mappedIndex];	
	while(currentRowHead!=nullptr){
		if (currentRowHead->connectedName==inputFullName){
			return inputFullName + ' ' + currentRowHead->number + ' ' + currentRowHead->location;
		}
		currentRowHead = currentRowHead->next;
	}
	string consoleOutput = "Name not found!";
	return consoleOutput;
}




int main(){
	string filename;
	cout<< "Enter the file name: ";
	cin>>filename;
    ifstream myfile(filename);
    string line;
    BST<Node> binaryTree;
	HT<hNode> hashTable;
	auto bstTime = 0.0;
	auto htTime = 0.0;

	if (myfile.is_open()){
		while(getline(myfile,line)){
			stringstream ss(line);
			string word;
			int wordCount = 0;
			string info[4];
			while (ss >> word){
				for (char& c : word){ // turning input into uppercase
					c = toupper(c);
				}
				info[wordCount] = word;
				wordCount++;
			}

			auto start = std::chrono::high_resolution_clock::now();
			binaryTree.insert(info[0], info[1], info[2], info[3]);
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			bstTime+=BSTTime.count();

			start = std::chrono::high_resolution_clock::now();
			hashTable.insert(info[0], info[1], info[2], info[3]);
			hashTable.currentSize+=1;
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			htTime+=HTTime.count();

			
		}
		myfile.close();
	}
	// even though this was not wanted in the pdf, added just in case it was missing, this part compares the speed of bst and ht creation from the file input
    cout << "Loading the phonebook into a BST." << endl;
    cout << "Phonebook creation in BST took " << bstTime << " nanoseconds .  .  ." << endl;   
    cout << "Loading the phonebook into an Hash Table" << endl;
    cout << "Phonebook creation in Hash Table took " << htTime << " nanoseconds .  .  ." << endl;
	cout << endl;
	cout<<"After preprocessing, the contact count is "<<hashTable.currentSize<<". Current load ratio is "<<hashTable.loadFactor()<<endl;
	cout << endl;
	bool check = true;
	while (check==true){
        cout << "Choose which action to perform from 1 to 6:" << endl;
        cout << "1 - Search a phonebook contact" << endl;
        cout << "2 - Adding a phonebook contact" << endl;
        cout << "3 - Deleting a phonebook contact" << endl;
        cout << "4 - Press 4 to exit" << endl;
	
		int action;
		cin >> action;
		if(action == 1){ // if user chooses to search for a contact in the phonebook
			cout<<endl;
			cout<< "Enter name to search for: "; 
			string inputWanted;
			getline(cin>>ws, inputWanted);
			cout<<endl;
			for (int i = 0; i < inputWanted.length(); i++)
			{
				if (isalpha(inputWanted.at(i)))
				{
					inputWanted.at(i) = toupper(inputWanted.at(i));
				}
			}
            cout << "Searching an item in the phonebook (BST) . . ." << endl;
            cout << "Phonebook: Searching for: (" << inputWanted << ")" << endl;
            cout << "====================================" << endl;
			auto start = std::chrono::high_resolution_clock::now();
			int k = 500;
			for(int i = 0; i<k;i++){
				binaryTree.NONPRINTsearch(inputWanted); //this search does not output any value
			}
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			binaryTree.search(inputWanted);
			cout << endl;
            cout << "BST Search Time: " << BSTTime.count()/k << " nanoseconds " << endl;
			cout << endl;
            cout << "Searching an item in the phonebook (Hash Table) . . ." << endl;
            cout << "Phonebook: Searching for: (" << inputWanted << ")" << endl;
            cout << "====================================" << endl;
			start = std::chrono::high_resolution_clock::now();
			for(int i=0; i<k; i++){
				hashTable.search(inputWanted);
			}
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			cout <<hashTable.search(inputWanted)<<endl;
			cout<<endl;
            cout << "Hashtable Search Time: " << HTTime.count()/k << " nanoseconds " << endl;
            cout<<"Speed Up: "<<float(BSTTime.count())/float(HTTime.count())  << endl;
			cout<<endl;
		}
		else if(action ==2){ // if user chooses to add a contact to phonebook
            string input;
            string inputFirstName;
            string inputLastName;
            string inputNumber;
            string inputLocation;
			cout<<endl;
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
			auto start = std::chrono::high_resolution_clock::now();
			binaryTree.insert(inputFirstName, inputLastName, inputNumber, inputLocation);
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			cout << "Contact has been added successfully to the BST" << endl;
			cout << endl;
            cout << "Adding an item to the phonebook (Hashtable) . . ." << endl;
            cout << "====================================" << endl;
			start = std::chrono::high_resolution_clock::now();
			hashTable.insert(inputFirstName, inputLastName, inputNumber, inputLocation);
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
            cout << "Contact has been added successfully to the Hashtable" << endl;
            cout << endl;
            cout << "Adding a contact to the Binary Tree took " << BSTTime.count() << " nanoseconds .  .  ." << endl;
            cout << "Adding a contact to the Hashtable took " << HTTime.count() << " nanoseconds .  .  ." << endl;
            cout<<"Speed Up: "<<float(BSTTime.count())/float(HTTime.count())  << endl;
			cout << endl;
		}
		else if(action==3){ // if user wants to delete a contact from the phonebook
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

			auto start = std::chrono::high_resolution_clock::now();
			binaryTree.remove(name);
			auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			start = std::chrono::high_resolution_clock::now();
			hashTable.remove(name);
			auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
            cout << "Deleting from the Binary Tree took " << BSTTime.count() << " nanoseconds .  .  ." << endl;
            cout << "Deleting from Hashtable took " << HTTime.count() << " nanoseconds .  .  ." << endl;
            cout << endl;
		}
		else if(action==4){
			cout<< "Exiting..."<<endl;
			check = false;
		}
	}

	return 0;
}