#include "sparse_matrix_2d_linked_list.h"
#include <fstream>
#include <sstream>
#include <iostream>

// returns the value of specific row and column in the sparse matrix
int rowColFinder(row_node* head, int row, int col){
	row_node* temp = head;

	int answer = 0; // if cannot find the cell node with specific row and col values, return 0

	for (int ctr1 = 0; ctr1< row; ctr1++){ // go to wanted row
		if (temp->down!=nullptr){
			temp = temp->down;
		}
	
	}

	cell_node* temp2 = temp->cell;

	while(temp2){ // search whole row until finding wanted column, if cant find then 0 will be returned
		if (temp2->column == col){
			answer = temp2->value;
			break;

		}
		temp2 = temp2->right;
	}

	return answer;

}



row_node* read_matrix(const string& filename, int& num_rows, int& num_columns){
	
	ifstream matrix; // txt file of matrixs
	matrix.open(filename.c_str());
	string matrixLines; // using this for only reading first row of txt file
	string matrixLines2; // using this for rest of the txt file while reading rows

	// to read num of rows and cols from file
	getline(matrix,matrixLines);
	stringstream ss(matrixLines);
	string numRowStr, numColStr; 
	ss >> numRowStr >> numColStr;
	num_rows = stoi(numRowStr);
	num_columns = stoi(numColStr);

	row_node* head = new row_node; // first row node
	row_node* temp = head; // a temporary pointer which points to row nodes while ctr1 iterates, starting from the head node

	for (int ctr1 = 0; ctr1 < num_rows; ctr1++){
		
		getline(matrix, matrixLines2); // reading line by line while ctr1 iterates
		stringstream ss2(matrixLines2); 
		string valuesStr; // values in matrix input as a string object
		int valuesInt; // values in matrix input as int object, will be converted from valuesStr using stoi

		temp->cell = nullptr;

		cell_node* temp2 = temp->cell; // a temporary pointer which points to cell nodes while loop iterates over values of columns
		bool checker = true;
		int columnCounter = 0;
		while (ss2 >> valuesStr){ // iterate for each column in the row
			valuesInt = stoi(valuesStr);
			if (valuesInt != 0){
				if (checker){ // to check if this is the first cell node to be created
					temp->cell = new cell_node; // create the first cell node and connect current row node's cell pointer to this cell node
					temp2 = temp->cell; // connect cell node iterator to current cell node
					temp2->value = valuesInt; 
					temp2->column = columnCounter;
					checker = false;
				}
				else{
					temp2->right = new cell_node; // creating a new cell node and connecting it to previous node's right
					temp2->right->left = temp2; // connecting new nodes left to previous node
					temp2 = temp2->right; // updating temp2, it now points to newly created cell node

					temp2->column = columnCounter;
					temp2->value = valuesInt;

				}
			}
			
			columnCounter+=1;
		}

		if (ctr1!=num_rows-1){
			temp->down = new row_node;
			temp = temp->down;
		}
	}
	return head;
}


void print_matrix(row_node* head, int num_rows, int num_columns){

	row_node* temp = head; // iterates through row nodes
	int difference; // difference between column values of linked nodes

	for (int ctr1 = 0; ctr1 < num_rows; ctr1++){
		cell_node* temp2 = temp->cell; // iterates through cell nodes for each row

		if (temp->cell == nullptr){ // if row only consists of 0s
			cout << "0";
			for (int ctr5 = 0; ctr5<num_columns-1; ctr5++){
				cout << " 0";
			}
		}

		else{

			if (temp2->column!=0){ // if matrix starts with 0s, first print them until reaching the first node
				cout << "0";
				for (int ctr6 = 0; ctr6<(temp2->column)-1; ctr6++){
					cout << " 0";
				}
				cout << " ";
			}
			
			bool checker = true;
			while(checker){
				cout << temp2->value;
				if(temp2->right==nullptr){ // if temp reached last node
					if(temp2->column != num_columns-1){ // if there are 0s after the last node
						for(int ctr4 = 0; ctr4< (num_columns -1 ) - temp2->column; ctr4++){
							cout << " 0"; // print them
						}
					}
					checker = false;
				}
				else{
					difference = temp2->right->column - temp2->column;
					if (difference!=1){ // if there are 0 values between nodes
						cout << " 0";
						for (int ctr2 = 0; ctr2 < difference-2; ctr2++){
							cout << " 0";
						}
						cout << " ";
					}
					else{
						cout << " ";
					}
				}
				temp2 = temp2->right;
			}
		}
			cout << endl;
			temp = temp->down;
	}
}



void delete_matrix(row_node* head){
	row_node* temp = head; // iterates through row nodes
	
	while (temp){
		row_node* tempNext = temp->down; // points next row node while temp iterates
		cell_node* temp2 = temp->cell; // iterates through cell nodes for each row

		while (temp2){
			cell_node* temp2Next = temp2->right; // points next cell node while temp2 iterates
			delete temp2;
			temp2 = temp2Next;
		}
		delete temp;
		temp = tempNext;
	}
}

row_node* add_matrices(row_node* left, row_node* right, int num_rows, int num_columns){
	row_node* lTemp = left; // iterates through row nodes of left matrix
	row_node* rTemp = right; // iterates through row nodes of right matrix
	row_node* sHead = new row_node;
	row_node* sTemp = sHead; // will iterate through sum matrix, currently points to nullptr, cell nodes will be added
	
	for (int ctr1 = 0; ctr1 < num_rows; ctr1++){
		
		cell_node* temp2Left = lTemp->cell; // iterates through cell nodes of left matrix for each row
		cell_node* temp2Right = rTemp->cell; // iterates through cell nodes of right matrix for each row
		cell_node* temp2Sum; // adds summation of right and left matrix for each row while iterating and creating nodes
		bool checker = true;

		for (int ctr2 = 0; ctr2 < num_columns; ctr2++){
			
			int sum = 0;
					
			// to check whether left matrix or matrix reached its end, if so rest of the values for them are 0
			// this way it prevents temp2 pointers to try pointing nullptr's right
			if (temp2Left == nullptr && temp2Right == nullptr){
				sum = 0;

			}

			else if (temp2Left == nullptr && temp2Right !=nullptr){
				sum = temp2Right->value;
				temp2Right = temp2Right->right;
			}

			else if (temp2Left != nullptr && temp2Right == nullptr){
				sum = temp2Left->value;
				temp2Left = temp2Left->right;
			}

			else{

				if (ctr2 == temp2Left->column && ctr2 == temp2Right->column){ // if there aren't 0 values for the current column
					sum = temp2Left->value + temp2Right->value;
					// both cell node iterates can go to the next node 
					temp2Left = temp2Left->right;
					temp2Right = temp2Right->right;
					
				}
				else if (ctr2 == temp2Left->column && ctr2 !=temp2Right->column){ // if right matrix's value is 0 
					sum = temp2Left->value;
					// only left cell node can go to the next node
					temp2Left = temp2Left->right;
					
				}
				else if (ctr2 != temp2Left->column && ctr2 == temp2Right->column){ // if left matrix's value is 0
					sum = temp2Right->value;
					// only right cell node can go to the next node
					temp2Right = temp2Right->right;
					
				}
				else{ // if both are 0
					sum = 0;
					// none of the cell node iterators cannot go to the next node since we havent reached their columns yet
					
				}
			}

			
			if (checker && sum !=0){ // to check if this is the first time cell node is created for sum matrix for the current row
				
				sTemp->cell = new cell_node;
				temp2Sum = sTemp->cell;
				temp2Sum->column = ctr2;
				temp2Sum->value = sum;
				checker = false;
				
			}
			else if (sum!=0){ // first cell node has already created, will create new cell nodes if sum is not 0
			
				temp2Sum->right = new cell_node;
				temp2Sum->right->left = temp2Sum;
				temp2Sum = temp2Sum->right;
				temp2Sum->column = ctr2;
				temp2Sum->value = sum;
				
			}

		}

		lTemp = lTemp->down;
		rTemp = rTemp->down;
		
		if (ctr1!=num_rows-1){
			sTemp->down = new row_node;
			sTemp = sTemp->down;
		}

	}

	return sHead;

}



bool is_symmetric(row_node* head, int num_rows, int num_columns){

	if (num_rows!=num_columns){ // if not square matrix, then not symmetric
		return false;
	}

	for (int ctr1 = 0; ctr1 < num_rows; ctr1++){
		for (int ctr2 = 0; ctr2 < num_columns; ctr2++){
			if (rowColFinder(head,ctr1,ctr2) != rowColFinder(head,ctr2,ctr1)){ // using the function I created to find row and col values of a sparse matrix
				return false;
			}
		}
	}
	return true;
}


row_node* transpose(row_node* head, int num_rows, int num_columns){
	row_node* tHead = new row_node; // a copy of the original sparse matrix, will be transformed into transposed version
	row_node* tTemp = tHead; // will iterate and create row nodes

	for (int ctr1 = 0; ctr1 < num_columns; ctr1++){ // iterating num columns times first since transpose of a matrix has swapped number of rows and columns
		// rest of the process is same as read_matrix function
		tTemp->cell = nullptr;
		cell_node* tTemp2 = tTemp->cell;
		bool checker = true;

		for (int ctr2 = 0; ctr2 < num_rows; ctr2++){
			int currentValue = rowColFinder(head,ctr2,ctr1);
			if (currentValue !=0){
				if (checker){
					tTemp->cell = new cell_node;
					tTemp2 = tTemp->cell;
					tTemp2->value = currentValue;
					tTemp2->column = ctr2;
					checker = false;

				}
				else{
					tTemp2->right = new cell_node;
					tTemp2->right->left = tTemp2;
					tTemp2 = tTemp2->right;

					tTemp2->column = ctr2;
					tTemp2->value = currentValue;

				}

			}
		}

		if (ctr1!=num_columns-1){
			tTemp->down = new row_node;
			tTemp = tTemp->down;
		}

	}

	return tHead;

}




//Ahmet Alperen Gungor - 28847