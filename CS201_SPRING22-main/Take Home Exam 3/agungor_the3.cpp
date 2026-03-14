#include <iostream>
#include <string>

using namespace std;


bool onechecker(string input){
	bool check = true;
	int length = input.length();
	int truecount = 0;
	int k;
	char x;

	if (length==0){
		truecount+=1;
	}
	else{
		check = true;
	}

	for(k=0; k<length; k++){
		x = input[k];
		if (x<97 || x>122){
			truecount += 1;
		}
	}
	
	if (truecount!=0){
		check = false;
	}


	return check;
}
bool twochecker(string input, string sourceword){
	bool check = true;
	int length = input.length();
	int truecount = 0;
	int qcount = 0;
	int k;
	char x;
	int sourcelength= sourceword.length();


	if (length==0){
		truecount +=1;
	}

	else{
		if (length>sourcelength){
			truecount +=1;
		}
		else{
			check = true;
		}
	}

	for(k=0; k<length; k++){
		x = input[k];
		if (x>=97 && x<=122){
			check = true;
		}
		else if(x==63){
			check = true;
			qcount += 1;
		}

		else{
			truecount += 1;
		}
	}
	if (truecount!=0){
		check = false;
	}
	else{
		if (qcount>=length){
		check = false;
		}
		else{
			check = true;
		}	
		
	}
	

	return check;
}


int main(){
	
	string selection;
	string firstword1;
	string firstword2;
	string sourceword = "";
	string subcheck;
	string foundsub;
	int qchecker;
	int qchecker2;
	int searchlength;
	int sourcelength;
	int indexfinder;
	int d1;
	int d2;
	int qcounter;
	int anychecker;
	int longcounter;

	cout << "This program searches a search string in a source string with the option of wildcards." << endl;	cout << endl;	cout << "Example inputs with their meaning in parentheses:" << endl << "1 thislectureisawesome (to enter a new source string)" << endl << "2 lecture (to enter a new search string)" << endl << "2 t?r? (to enter a search string with wildcards)  " << endl << "3 (to exit the program)" << endl;	cout << endl;
	cout << "Enter your choice and string: ";
	cin >> selection;

	while (selection!="3"){
		if (selection!= "1" && selection!= "2"){
			cout << "Choice can be 1, 2 or 3! Try again." << endl;
			cout << endl;
		}

		else if (selection == "1"){
			cin >> firstword1;
			if (onechecker(firstword1)){
				sourceword = firstword1;
				cout << "Source word has been changed to " << "\"" << sourceword << "\"" << endl;
				cout << endl;
			}
			else{
				cout << "Wrong input format! Try again." << endl;				cout << endl;
			}
		}

		else if (selection == "2"){
			cin >> firstword2;
			if (sourceword == ""){
				cout << "Wrong input format! Try again." << endl;				cout << endl;
			}

			else{
				if (twochecker(firstword2, sourceword)){
					qchecker2 = 0;
					for (qchecker = 0; qchecker < firstword2.length(); qchecker++){
						if (firstword2.at(qchecker) == 63){
							qchecker2++;
						}
						else{
							qchecker2 +=0;
						}
					}

					if (qchecker2==0){ // there are no jokers
						searchlength = firstword2.length();
						indexfinder = sourceword.find(firstword2);
						if (indexfinder == string::npos){
							cout << "For the source word " << "\"" << sourceword << "\"" << " and search word " << "\"" << firstword2 << "\"" << ", no match has been found." << endl;
							cout << endl;
						}
						else{
							cout << "For the source word " << "\"" << sourceword << "\"" << " and search word " << "\"" << firstword2 << "\"" << "," << endl;
							while (indexfinder!= string::npos){
								cout << "\"" << firstword2 << "\"" << " has been found at index " << indexfinder << endl;
								indexfinder = sourceword.find(firstword2, indexfinder+1);
							}
							cout << endl;
						}

					}

					else{ // there are jokers
						searchlength = firstword2.length();
						sourcelength = sourceword.length();
						
						anychecker = 0;
						longcounter = 0;
						
						for (d1=0; d1<((sourcelength-searchlength)+1); d1++){
							subcheck = sourceword.substr(d1, searchlength);
							qcounter = 0;
							
							for (d2=0; d2<searchlength; d2++){
								if (subcheck[d2] == firstword2[d2]){
									qcounter += 1;
								}
								else if (firstword2[d2] == 63){
									qcounter += 1;
								}
								else{
									qcounter += 0;
								}
							}
							
							if (qcounter == searchlength){
								if (longcounter == 0){
									cout << "For the source word " << "\"" << sourceword << "\"" << " and search word " << "\"" << firstword2 << "\"" << "," << endl;
									longcounter += 1;
								}
								else{
									longcounter += 1;
								}
								foundsub = sourceword.substr(d1, searchlength);
								cout << "\"" << foundsub << "\"" << " has been found at index " << d1 << endl;
								anychecker +=1;
							}

							else{
								anychecker += 0;
							}
						}
						
						if (anychecker == 0){
							cout << "For the source word " << "\"" << sourceword << "\"" << " and search word " << "\"" << firstword2 << "\"" << ", no match has been found." << endl;
							cout << endl;
						}
						else{
							cout << endl;
						}
					}
				}
				else{ 
					cout << "Wrong input format! Try again." << endl;
					cout << endl;
				}
			}
		}
		else{
			qchecker2 += 0;
		}


		cout << "Enter your choice and string: ";
		cin >> selection;

	}
	
	cout << "See you!" << endl;
	
	
	return 0;
}