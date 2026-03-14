#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// a function to add (9 * corresponding rhs value) to the temporary checker in algorithm function
void totalAdder(vector<int> rhsVector, vector<int> varCol, int idx, int &total){

	total += rhsVector[varCol[idx]] * 9;
	
}

// a function to write error on output txt file when needed
void writeError(string outfilename){

	ofstream out(outfilename.c_str());
	if (out.is_open()){ // if we can open the file, write error
		out << "Error";
	}

	out.close();
}

// recursive algorithm to find correct answers
// var is vector that stores variables, varCol stores column index of each variable, firstIdx will iterate through indexes with recursion starting at 0
// total is res value with constants subtracted, rhsVector is vector of rhs values
bool solver(vector<int> &var, vector<int> varCol, int firstIdx, int total, vector<int> rhsVector){ 

	if (firstIdx!=var.size()){ // run until last index of var (the vector which stores variables)

		for (int ctr = 1; ctr < 10; ctr++){ // to check values 1-9

			int totalTemp = 0;
			for (int ctr2 = firstIdx + 1; ctr2 < var.size(); ctr2++){ // adding 9*corresponding coefficent (using totalAdder function) for variables in right hand side of current index
				totalAdder(rhsVector, varCol, ctr2, totalTemp);
			}
			totalAdder(rhsVector, varCol, firstIdx, totalTemp);

			if (totalTemp < total){ 
				continue; // skipping while temporary result is smaller than total
			}


			var[firstIdx] = ctr; // checking for values 1-9
			int newtotal = total - (var[firstIdx] * rhsVector[varCol[firstIdx]]); // making row of matrix - vector multiplication and subtracting the value from result

			// with this recursion, rightmost indexes will increase first, therefore finding smallest correct concatenated integer
			// 1 1 1 1 ... 1 will be the indexes, then it will increase from end -> 1 1 1 1 ... 2 / 1 1 1 1 ... 3/ ... / 1 1 2 9 ... 9 9 / 1 1 3 9 ... 9 9 etc
			if (solver(var, varCol, firstIdx+1, newtotal, rhsVector)){ // calling the function again, but for the next index 

				return true;
			}

		}

	}

	else if (total == 0){ // if function reaches correct answer return true
		return true;
	}
	return false; // when firstIdx reaches end of vector, returns false to stop calling function recursively for more indexes
}

int main(){

	string lhsfilename, rhsfilename, resfilename, outfilename;
	
	// taking txt files' names as inputs from user, then opening them
	cout << "Enter LHS matrix filename: ";
	cin >> lhsfilename;
	ifstream lhs;
	lhs.open(lhsfilename.c_str());
	cout << "Enter RHS matrix filename: ";
	cin >> rhsfilename;
	ifstream rhs;
	rhs.open(rhsfilename.c_str());
	cout << "Enter RES filename: ";
	cin >> resfilename;
	ifstream res;
	res.open(resfilename.c_str());
	cout << "Enter output filename: ";
	cin >> outfilename;
	ofstream out; 
	out.open(outfilename.c_str());

	if (lhs.fail() || rhs.fail() || res.fail() || out.fail()){
		writeError(outfilename); // invalid input, therefore writing error to output txt
		cout << "Error" << "\n"; // invalid input, therefore printing error to console
		return 0;
	}

	int lhsRowCount, lhsColCount; 
	int rhsRowCount, rhsColCount; 
	int resRowCount, resColCount; 

	lhs >> lhsRowCount >> lhsColCount; // row and column count of lhs matrix (taken from first row of lhs file)
	rhs >> rhsRowCount >> rhsColCount; // row and column count of rhs matrix (taken from first row of rhs file)
	res >> resRowCount >> resColCount; // row and column count of res matrix (taken from first row of res file)


	vector<vector<int>> lhsMatrix(lhsRowCount, vector<int>(lhsColCount)); // matrix representation of lhs
	for (int ctr1=0; ctr1<lhsRowCount; ctr1++){ // idx for rows
		for (int ctr2 = 0; ctr2<lhsColCount; ctr2++){ // idx for cols
			lhs >> lhsMatrix[ctr1][ctr2]; 
			if (lhsMatrix[ctr1][ctr2]!= -1 && (lhsMatrix[ctr1][ctr2]>9 || lhsMatrix[ctr1][ctr2] <1)){ // checking validity of input
				writeError(outfilename); // invalid input, therefore writing error to output txt
				cout << "Error" << "\n"; // invalid input, therefore printing error to console
				return 0;
			}
		}
	}

	
	vector<int> rhsVector(rhsRowCount); // vector representation of rhs
	for(int ctr=0; ctr<rhsRowCount; ctr++){ // idx for rows
		rhs >> rhsVector[ctr];
		if(rhsVector[ctr] >9 || rhsVector[ctr] <1){ // checking validity of input
			writeError(outfilename); // invalid input, therefore writing error to output txt
			cout << "Error" << "\n"; // invalid input, therefore printing error to console
			return 0;
		}
	}

	
	vector<int> resVector(resRowCount); // vector representation of res
	for(int ctr=0; ctr<resRowCount; ctr++){ // idx for rows
		res >> resVector[ctr];
		if(resVector[ctr] <1){ // checking validity of input
			writeError(outfilename); // invalid input, therefore writing error to output txt
			cout << "Error" << "\n"; // invalid input, therefore printing error to console
			return 0;
		}
	}
		

	// checking if there are more than 1 columns in rhs file and if row size is correct
	string rhsLine;
	rhs.seekg (0, ios::beg); 
	getline(rhs, rhsLine); // removing first line
	int rhsRowChecker = 0; // to count num of rows
	while (!rhs.eof()){
		int checker = 0;
		getline(rhs, rhsLine);
		stringstream ss(rhsLine);
		int temp;
		while (ss>>temp){
			checker++;
			if (checker!=1){
				writeError(outfilename); // invalid input, therefore writing error to output txt
				cout << "Error" << "\n"; // invalid input, therefore printing error to console
				return 0;
			}
		}
		rhsRowChecker++;
	}
	
	// checking if there are more than 1 columns in res file and if row count is correct
	string resLine;
	res.seekg (0, ios::beg); 
	getline(res, resLine); // removing first line
	int resRowChecker = 0; // to count num of rows
	while (!res.eof()){
		int checker = 0;
		getline(res, resLine);
		stringstream ss(resLine);
		int temp;
		while (ss>>temp){
			checker++;
			if (checker!=1){
				writeError(outfilename); // invalid input, therefore writing error to output txt
				cout << "Error" << "\n"; // invalid input, therefore printing error to console
				return 0;
			}
		}
		resRowChecker++;
	}

	// checking if column and row count for lhs is correct
	string lhsLine;
	lhs.seekg (0, ios::beg); 
	getline(lhs, lhsLine); // removing first line
	int lhsRowChecker = 0; // to count num of rows
	while (!lhs.eof()){
		int checker = 0;
		getline(lhs, lhsLine);
		stringstream ss(lhsLine);
		int temp;
		while (ss>>temp){
			checker++;
		}
		if (checker!=lhsColCount){
				writeError(outfilename); // invalid input, therefore writing error to output txt
				cout << "Error" << "\n"; // invalid input, therefore printing error to console
				return 0;
			}
		lhsRowChecker++;
	}
	
	if (rhsRowChecker!=rhsRowCount || resRowChecker!=resRowCount || lhsRowChecker!=lhsRowCount){ // validity of num of rows and cols
		writeError(outfilename); // invalid input, therefore writing error to output txt
		cout << "Error" << "\n"; // invalid input, therefore printing error to console
		return 0;
	}


	// these 3 vectors have same indexes for each unknown in a row
	vector<int> var; // a vector for unknowns in a row
	vector<int> varRow; // row idx of each unknown
	vector<int> varCol; // column idx of each unknown
	for (int ctr = 0; ctr < lhsRowCount; ctr++){ // running through rows of lhs
		int multiplied; 
		int total; // total is corresponding value at res
		total = resVector[ctr]; 
		for (int ctr2 = 0; ctr2 < lhsMatrix[ctr].size(); ctr2++){ // running through cols of lhs
			multiplied = lhsMatrix[ctr][ctr2] * rhsVector[ctr2]; // multiplying lhs with rhs
			if (multiplied < 0){
				var.push_back(-multiplied);
				varRow.push_back(ctr);
				varCol.push_back(ctr2);
			}
			else{
				total -= multiplied; // subtracting constant values (non -1 ones)
			}
			
		}

		// calling the algorithm function
		solver(var,varCol,0,total,rhsVector);


		// loop to write solution for current row in "row column answer" format to output txt file
		for (int ctr = 0; ctr<var.size(); ctr++){
			out << varRow[ctr] << " " << varCol[ctr] << " " << var[ctr] << "\n";
		}

		// clearing answer vectors for the next row
		var.clear();
		varRow.clear();
		varCol.clear();
	}

	// closing the txt files
	lhs.close();
	rhs.close();
	res.close();
	out.close();

	return 0;

	// AHMET ALPEREN GUNGOR - 28847 
}