#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "student.h"

using namespace std;	


void SelectSort(vector<Student> & a){ // a is point vector, a2 is namevector, a3 is lastname vector, a4 is id vector

    int j, k, minIndex, numElts = a.size(), n, ctr3, ender;
	double temp;
	string temp2, temp3; 
    for(k=0; k < numElts - 1; k++){
        minIndex = k;             
        for(j=k+1; j < numElts; j++)
            if (a[j].getPoints() < a[minIndex].getPoints())
               minIndex = j;     

		// swapping points
        temp = a[k].getPoints();         
        a[k].setPoints(a[minIndex].getPoints());
        a[minIndex].setPoints(temp);

		// swapping full name
		temp2 = a[k].getFirstName() + " " + a[k].getLastName();        
		a[k].setName(a[minIndex].getFirstName() + " " + a[minIndex].getLastName());
        a[minIndex].setName(temp2);

		// swapping ID
		temp3 = a[k].getId();       
		a[k].setId(a[minIndex].getId());
        a[minIndex].setId(temp3);


    }

	reverse(a.begin(), a.end()); // it was ascending order, now descending

	int ctrnum = numElts-1;

	while (ctrnum>0){ // this is for deleting students with -1 points (they are not in vector)
		if (a[ctrnum].getPoints() == -1){
			a.pop_back();
		}
		ctrnum--;
	}

	double temptemp;
	string temptemp2, temptemp3;
	string l,r;

	for (n=0; n<a.size()-1; n++){ // this is for checking lastnames and sorting accordingly when points are equal
		if (a[n].getPoints() == a[n+1].getPoints()){
			ender = 0;

			l = a[n].getLastName();
			r = a[n+1].getLastName();
			ctr3=0;
			while (ctr3<l.size() && ender == 0){
				if (l[ctr3] < r[ctr3]){
					ender +=1;
				}
				else if (l[ctr3] > r[ctr3]){
					// swapping points
					temptemp=a[n].getPoints();
					a[n].setPoints(a[n+1].getPoints());
					a[n+1].setPoints(temptemp);

					// swapping full name
					temptemp2=a[n].getFirstName() + " " + a[n].getLastName();    
					a[n].setName(a[n+1].getFirstName() + " " + a[n+1].getLastName());
					a[n+1].setName(temptemp2);

					// swapping ID
					temptemp3=a[n].getId();
					a[n].setId(a[n+1].getId());
					a[n+1].setId(temptemp3);

					ender += 1;
				}
				else if (l[ctr3] == r[ctr3]){
					ctr3++;
				}

			}
			
		}

	}


	double temptemptemp;
	int ctrctr = 0;

	while(ctrctr<a.size()){ // adding 1 back since started with -1 (to delete students that dont have any info on grade text, since they all can be 0 not to confuse with those students)
		temptemptemp = a[ctrctr].getPoints();
		temptemptemp +=1;
		a[ctrctr].setPoints(temptemptemp);
		ctrctr++;
	}



}

Student::Student(){

	string firstName = " ";
	string lastName = " ";
	string ID = " "; 
	double points = -1; 
	double midtermWeight = 0;
	double finalWeight = 0;
	double hwWeight = 0;

}

void Student::setId(string cc){

	ID = cc;

}

string Student::getId() const{

	return ID;

}

void Student::setName(string ccfullname){

	int spacelast = ccfullname.rfind(" ");
	string ccname = ccfullname.substr(0, spacelast);
	string cclastname = ccfullname.substr(spacelast+1);

	firstName = ccname;
	lastName = cclastname;

}

string Student::getFirstName() const{

	return firstName;

}

string Student::getLastName() const{

	return lastName;

}

void Student::setPoints(double ccpoint){

	points = ccpoint;

}

double Student::getPoints() const{

	return points;

}

void Student::setExamWeights(double cchw, double ccmid, double ccfinal){

	hwWeight = cchw;
	midtermWeight = ccmid;
	finalWeight = ccfinal;

}

double Student::getGrade(string cctype, double ccgrade) const{

	double cccalculated;

	if (cctype == "hw"){
		cccalculated = hwWeight * ccgrade;
	}

	if (cctype == "midterm"){
		cccalculated = midtermWeight * ccgrade;
	}

	if (cctype == "final"){
		cccalculated = finalWeight * ccgrade;
	}

	return cccalculated;

}

void Student::display(vector<Student> & ccv, int ccrank){
	
	cout << ccrank << ", " << ccv[ccrank-1].ID << ", " << ccv[ccrank-1].firstName << " " << ccv[ccrank-1].lastName << ", " << ccv[ccrank-1].points << endl;
}







int main(){

	ifstream input1; // file with id and names
	ifstream input2; // file with id and grades

	string filename1; // name of file with id and names ex students.txt
	string filename2; // name of file with id and grades ex grades.txt

	string lines1; 
	int lines1length;
	string lines2;
	
	int whatctr =0;
	string singleid;
	string singleid2;
	string singlepoint;
	int lastspace;
	int firstspace;

	string fullfullname;
	
	int starcounter = 0;
	int line2counter = 0;

	string assesment; // hw if homework, midterm if midterm, final if final

	string idstream;
	double pointstream;
	
	int ctr1;
	double var1;

	int wanted;

	int endcheck=0;

	
	vector<Student> csclass; 
	

	cout << "Please enter a filename for Students Grades Results: ";
	cin >> filename2;

	input2.open(filename2.c_str());
	
	if (input2.fail()){
		cout << "Can not find the requested file. Terminating application ..." << endl;
	}
	else{
		cout << "Please enter a filename for Students Names: ";
		cin >> filename1;
		input1.open(filename1.c_str());

		if (input1.fail()){
			cout << "Can not find the requested file. Terminating application ..." << endl;
		}
		else{
			while(getline(input1, lines1)){
				lastspace = lines1.rfind(" ");
				firstspace = lines1.find(" ");
				lines1length = lines1.length();
				singleid = lines1.substr(0, (firstspace)); // 8 digit number ex 00028847
				Student students;
				
				students.setId(singleid); // this student has a string ID

				fullfullname = lines1.substr(firstspace+1); 
				students.setName(fullfullname); // this student has a string name and string last name

				students.setExamWeights(0.1, 0.2, 0.3); // weight of each assessment

				students.setPoints(-1); // initializing all values to -1

				csclass.push_back(students); // adding individual students to a single vector

				
			}

			

			while(getline(input2,lines2)){
				if (lines2[0] == '*'){
					if (lines2[3] == 'H'){
						assesment = "hw";
					}
					else if (lines2[3] == 'M'){
						assesment = "midterm";
					}
					else if (lines2[3] == 'F'){
						assesment = "final";
					}
				}
				else if(lines2.empty()){
					ctr1 = 0;
				}

				else{
					
					istringstream lineStream(lines2);

					lineStream >> idstream >> pointstream; // idstream is string and id numbers, pointstream is double and point numbers

					ctr1 = 0;
					var1 = 0;
					
					while (ctr1<csclass.size()){
						if (csclass[ctr1].getId() == idstream){

							var1 += csclass[ctr1].getPoints(); // adding previous points that are already in vector
							var1 += csclass[ctr1].getGrade(assesment, pointstream); 
							csclass[ctr1].setPoints(var1); 

						
							
						}

						ctr1++;
						
					}

				}

			}

	
			SelectSort(csclass);

			cout << "Enter the rank you want to query (enter 0 to exit): "; 
			
			cin >> wanted;

			if (wanted == 0){
				cout << "Exiting..." << endl;
			}
			else{
				while (wanted!=0){
					if (wanted > 0 && wanted < csclass.size()+1){
						csclass[wanted-1].display(csclass, wanted);
						cout << "Enter the rank you want to query (enter 0 to exit): ";
						cin >> wanted;
					}
					else{
						cout << "Rank needs to be greater than 0 and smaller than " << csclass.size()+1 << "!" << endl;
						cout << "Enter the rank you want to query (enter 0 to exit): ";
						cin >> wanted;
					}

				}

				cout << "Exiting..." << endl;
			}

		}

	}

	return 0;
}


// Ahmet Alperen Gungor 28847