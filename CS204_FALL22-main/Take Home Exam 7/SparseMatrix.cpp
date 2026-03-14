#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
using namespace std;

#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(const string &file_name) {
    ifstream fin(file_name);
    if (fin.fail()) {
        throw "File couldn't be opened";
    }
    // actual head_
    head_ = nullptr;
    // head_ of current row
    row_node *curr_head = nullptr;
    // row counter for error checking
    int row = 0;
    fin >> num_rows_ >> num_columns_;
    if (fin.fail()){
        throw "Invalid number of rows or number of columns";
    }
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        int col = 0, val;
        stringstream ss(line);
        // if this is the first row
        if (head_ == nullptr) {
            head_ = curr_head = new row_node;
        } else {
            curr_head->down = new row_node;
            curr_head = curr_head->down;
        }
        // pointer at last cell added in this row
        cell_node *curr_cell = nullptr;
        while (ss >> val) {
            if (val != 0) {
                cell_node *new_cell = new cell_node;
                new_cell->value = val;
                new_cell->column = col;
                // if first cell in this row
                if (curr_cell == nullptr)
                    curr_head->cell = curr_cell = new_cell;
                else {
                    curr_cell->right = new_cell;
                    curr_cell->right->left = curr_cell;
                    curr_cell = curr_cell->right;
                }
            }
            col++;
            if (col > num_columns_) {
                throw "Too many columns in row " + to_string(row);
            }
        }
        row++;
        if (row > num_rows_) {
            throw "Too many rows";
        }
        if (col < num_columns_) {
            throw "Not enough columns in row " + to_string(row);
        }
    }
    if (row < num_rows_) {
        throw "Not enough rows";
    }
}

SparseMatrix::~SparseMatrix() {
    while (head_ != nullptr) {
        cell_node *cell = head_->cell;
        while (cell != nullptr) {
            cell_node *t = cell->right;
            delete cell;
            cell = t;
        }
        row_node *t = head_->down;
        delete head_;
        head_ = t;
    }
}

ostream &operator<<(ostream &out, const SparseMatrix & rhs) {
    int rows = 0;
    row_node * head = rhs.head_;
    while (head != nullptr) {
        // To know how many zeros to print between cells
        int col = -1;
        cell_node *cell = head->cell;
        while (cell != nullptr) {
            col++;
            // until we get to this cell's column, print zeros
            while (col < cell->column) {
                out << "0 ";
                col++;
            }
            out << cell->value << " ";
            col = cell->column;
            cell = cell->right;
        }
        // If there are more zeros after the last cell in the row,
        // print them
        while (col < rhs.num_columns_ - 1) {
            out << "0 ";
            col++;
        }
        out << endl;
        head = head->down;
        rows++;
    }
    // If there are rows with no cells at all, print zeros for them
    while (rows < rhs.num_rows_) {
        for (int i = 0; i < rhs.num_columns_; i++) {
            out << "0 ";
        }
        out << endl;
        rows++;
    }
    return out;
}

SparseMatrix::SparseMatrix(SparseMatrix&& rhs) {
    head_ = rhs.head_;
    rhs.head_ = nullptr;
    num_rows_ = rhs.num_rows_;
    num_columns_ = rhs.num_columns_;
}

SparseMatrix::SparseMatrix(row_node *head, int num_rows, int num_columns): head_(head), num_rows_(num_rows), num_columns_(num_columns) {}

SparseMatrix::SparseMatrix(const SparseMatrix &rhs): head_(nullptr), num_columns_(rhs.num_columns_), num_rows_(rhs.num_rows_) {
    copy(rhs, head_, num_rows_, num_columns_);
}

SparseMatrix &SparseMatrix::operator=(const SparseMatrix & rhs) {
    if (&rhs != this)
        copy(rhs, head_, num_rows_, num_columns_);
    return *this;
}

void SparseMatrix::copy(const SparseMatrix& rhs, row_node *&head, int &num_rows, int &num_columns) {
    num_rows = rhs.num_rows_;
    num_columns = rhs.num_columns_;
    if (rhs.num_rows_ == 0 || rhs.num_columns_ == 0) return;
    row_node* curr = head = new row_node;
    row_node* other_head = rhs.head_;
    while(other_head!= nullptr){
        cell_node* rhs_cell = other_head->cell;
        cell_node* my_cell = nullptr;
        while (rhs_cell!=nullptr){
            cell_node * new_cell = new cell_node(rhs_cell->column, rhs_cell->value);
            if (my_cell == nullptr) my_cell = curr->cell = new_cell;
            else{
                my_cell->right = new_cell;
                my_cell->right->left = my_cell;
                my_cell = my_cell->right;
            }
            rhs_cell = rhs_cell->right;
        }
        if (other_head->down!=nullptr){
            curr->down = new row_node;
            curr = curr->down;
        }
        other_head = other_head->down;
    }
}


// a struct of arguments to pass for threads
struct arg{
public:
	row_node* head; // head of lhs matrix
	row_node* rhsHead; // head of rhs matrix
	int lhsNumCol; // num of columns in lhs matrix
	int rhsNumCol; // num of columns in rhs matrix
	int ctr1; // current row in lhs that we are evaluating
	row_node* mTemp; // node which will be modified for the current row
};

// same function as second take home to find corresponding value row/col
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

// a function that treads run on using parameters from argument struct
void rowCreator(arg argarg){
	row_node* lHead = argarg.head; // temp head for lhs matrix
	row_node* rHead = argarg.rhsHead; // temp head for rhs matrix
	cell_node* temp2mul; // iterator which will iterate over cell nodes of current row
	bool checker = true; // will be used to check if any cell node has added or not
	for (int ctr1 = 0; ctr1 < argarg.lhsNumCol; ctr1++){
		int sum = 0;
		for (int ctr2 = 0; ctr2< argarg.lhsNumCol; ctr2++){
			int lhsValue = rowColFinder(lHead, argarg.ctr1, ctr2); // find the value of corresponding element in lhs matrix
			int rhsValue = rowColFinder(rHead, ctr2, ctr1); // find the value of corresponding element in rhs matrix
			sum += lhsValue * rhsValue;
		}
		if (checker && sum!=0){ // if their multiplication is not 0 and no cell node added yet
			argarg.mTemp->cell = new cell_node;
			temp2mul = argarg.mTemp->cell;
			temp2mul->column = ctr1;
			temp2mul->value = sum;
			checker = false;
		}
		else if (sum!=0){ // if their multiplication is not 0 and at least 1 cell node added previously to current row 
			temp2mul->right = new cell_node;
			temp2mul->right->left = temp2mul;
			temp2mul = temp2mul->right;
			temp2mul->column = ctr1;
			temp2mul->value = sum;
		}
	}
}



SparseMatrix SparseMatrix::mat_mul(const SparseMatrix& rhs) const{
	row_node* mHead = new row_node; // head of the mul matrix
	row_node* mTemp = mHead; // will iterate through mul matrix
	row_node* lTemp = head_; // iterates throught row nodes of left matrix
	row_node* rTemp = rhs.head_; // iterates through row nodes of right matrix
	vector<thread> threads;

	arg argarg;
	for (int ctr1 = 0; ctr1< num_rows_; ctr1++){ // iterate for each row of lhs

		// setting arguments in order to pass with threads
		argarg.ctr1=ctr1;
		argarg.head=head_;
		argarg.lhsNumCol=num_columns_;
		argarg.mTemp=mTemp;
		argarg.rhsHead=rhs.head_;
		argarg.rhsNumCol=rhs.num_columns_;

		//add treads to the vector of threads
		threads.push_back(thread(&rowCreator,argarg));
		if (ctr1!=num_rows_-1){ // if not last row, continue adding row nodes and iterate for new rows in next loop
			mTemp->down = new row_node;
			mTemp = mTemp->down;
		}

		
	}

	for (int ctr1 = 0; ctr1< num_rows_; ctr1++){
		threads[ctr1].join();
	}

	// create the final sparesematrix using calcuations from previous steps
	SparseMatrix result(mHead, num_rows_, rhs.num_columns_);
	return result;
}