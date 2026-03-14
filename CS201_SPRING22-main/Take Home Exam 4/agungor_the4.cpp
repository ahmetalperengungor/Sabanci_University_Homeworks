#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;	


void SelectSort(vector<double> & a, vector<string> & a2, vector<string> &a3, vector<string> &a4){ // a is point vector, a2 is namevector, a3 is lastname vector, a4 is id vector

    int j, k, minIndex, numElts = a.size(), n, ctr3, ender;
	double temp;
	string temp2, temp3, temp4; 
    for(k=0; k < numElts - 1; k++){
        minIndex = k;             
        for(j=k+1; j < numElts; j++)
            if (a[j] < a[minIndex])
               minIndex = j;     
        temp = a[k];         
        a[k] = a[minIndex];
        a[minIndex] = temp;

		temp2 = a2[k];         
        a2[k] = a2[minIndex];
        a2[minIndex] = temp2;

		temp3 = a3[k];         
        a3[k] = a3[minIndex];
        a3[minIndex] = temp3;

		temp4 = a4[k];         
        a4[k] = a4[minIndex];
        a4[minIndex] = temp4;

    }

	reverse(a.begin(), a.end());
	reverse(a2.begin(), a2.end());
	reverse(a3.begin(), a3.end());
	reverse(a4.begin(), a4.end());

	int ctrnum = numElts-1;

	while (ctrnum>0){ // this is for deleting dudes with -1 points (they are not in vector)
		if (a[ctrnum] == -1){
			a.pop_back();
		}
		ctrnum--;
	}

	double temptemp;
	string temptemp2, temptemp3, temptemp4;
	string l,r;

	for (n=0; n<a.size()-1; n++){ // this is for checking lastnames and sorting accordingly when points are equal
		if (a[n] == a[n+1]){
			ender = 0;

			l = a3[n];
			r = a3[n+1];
			ctr3=0;
			while (ctr3<l.size() && ender == 0){
				if (l[ctr3] < r[ctr3]){
					ender +=1;
				}
				else if (l[ctr3] > r[ctr3]){
					temptemp=a[n];
					a[n] = a[n+1];
					a[n+1] = temptemp;

					temptemp2=a2[n];
					a2[n] = a2[n+1];
					a2[n+1] = temptemp2;

					temptemp3=a3[n];
					a3[n] = a3[n+1];
					a3[n+1] = temptemp3;

					temptemp4=a4[n];
					a4[n] = a4[n+1];
					a4[n+1] = temptemp4;

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

	while(ctrctr<a.size()){ // adding 1 back since started with -1 (to delete dudes that dont have any info on grade text, since they all can be 0 not to confuse with those dudes)
		temptemptemp = a[ctrctr];
		temptemptemp +=1;
		a[ctrctr] = temptemptemp;
		ctrctr++;
	}



}


int main(){

	ifstream input1; // file with id and names
	ifstream input2; // file with id and grades

	string filename1; // name of file with id and names ex students.txt
	string filename2; // name of file with id and grades ex grades.txt

	string lines1; 
	int lines1length;
	string lines2;
	

	string singlefirstnames;
	string singlelastnames;
	string singleid;
	string singleid2;
	string singlepoint;
	int lastspace;
	int firstspace;
	

	int starcounter = 0;
	int line2counter = 0;
	double assesment =0.0; // 10 if homework, 20 if midterm, 30 if final

	string idstream;
	double pointstream;
	
	int ctr1;
	double var1;

	int wanted;

	int endcheck=0;

	vector<string> names;
	vector<string> lastnames;
	vector<string> id;
	

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
				singlefirstnames = lines1.substr(firstspace+1, (lastspace-firstspace-1)); // first names ex Ali Efe
				singlelastnames = lines1.substr(lastspace+1); // last names ex Gungor

				names.push_back(singlefirstnames);
				lastnames.push_back(singlelastnames);
				id.push_back(singleid);
			}

			vector<double> point (id.size(), -1);

			while(getline(input2,lines2)){
				if (lines2[0] == '*'){
					if (lines2[3] == 'H'){
						assesment = 10;
					}
					else if (lines2[3] == 'M'){
						assesment = 20;
					}
					else if (lines2[3] == 'F'){
						assesment = 30;
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
					
					while (ctr1<id.size()){
						if (id[ctr1] == idstream){
							var1 += point[ctr1];
							var1 += (assesment * pointstream) / 100;
							point[ctr1] = var1; // point vector has the points with the same index as id
							
						}

						ctr1++;
					}

				}

			}

			SelectSort(point, names, lastnames, id);

			cout << "Enter the rank you want to query (enter 0 to exit): "; 
			
			cin >> wanted;

			if (wanted == 0){
				cout << "Exiting..." << endl;
			}
			else{
				while (wanted!=0){
					if (wanted > 0 && wanted < point.size()+1){
						cout << wanted << ", " << id[wanted-1] << ", " << names[wanted-1] << " " << lastnames[wanted-1] << ", " << point[wanted-1] << endl;
						cout << "Enter the rank you want to query (enter 0 to exit): ";
						cin >> wanted;
					}
					else{
						cout << "Rank needs to be greater than 0 and smaller than " << point.size()+1 << "!" << endl;
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