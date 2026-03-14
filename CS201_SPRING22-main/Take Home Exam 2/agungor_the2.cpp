#include <iostream>
#include <string>

using namespace std;

bool inputCheck(float iCweight, float iCspeed, float iCminute);
void getInput(string gIusername);
float calculateMET(float cMspeed, string exercise);
void computeResults( float weight, float goal, float speedwalk, float speedrun, float speedcycle, float minuteswalk, float minutesrun, float minutescycle);
void displayResults(float diff, float total, float weight, float walkingmet, float runningmet, float cyclemet, float calwalking, float calrunning, float calcycle);

void getInput(string gIusername){
	float gIweight;
	float speedwalk;
	float minuteswalk;
	float speedrun;
	float minutesrun;
	float speedcycle;
	float minutescycle;
	float caloriegoal;

	cout << "Welcome " << gIusername << ", please enter your weight(kg): "; 
	cin >> gIweight;
	cout << gIusername << ", please enter speed(km/h) and minutes spent in a week for the activities below." << endl;
	cout << "Walking: ";
	cin >> speedwalk >> minuteswalk;
	cout << "Running: ";
	cin >> speedrun >> minutesrun;
	cout << "Cycling: ";
	cin >> speedcycle >> minutescycle;
	cout << gIusername << ", please enter your weekly calorie burn goal: ";
	cin >> caloriegoal;

	if (inputCheck(gIweight, speedwalk, 100)){

		if (inputCheck(gIweight, speedrun, 100)){

			if (inputCheck(gIweight, speedcycle, 100)){

				if (inputCheck(gIweight, speedwalk, minuteswalk)){

					if (inputCheck(gIweight, speedrun, minutesrun)){

						if (inputCheck(gIweight, speedcycle, minutescycle)){

							computeResults(gIweight, caloriegoal, speedwalk, speedrun, speedcycle, minuteswalk, minutesrun, minutescycle);



						}
					}
				}
			}
		}
	}



}

bool inputCheck(float iCweight, float iCspeed, float iCminute){
	bool checkeric;


	if (iCweight < 30){
		cout << "Weight out of range!" << endl;
		
		checkeric= false; 
	}

	else{
		if (iCspeed < 4 || iCspeed > 30){
			cout << "Speed out of range!" << endl;
			
			checkeric= false;

		}
		else{
			if (iCminute < 0 || iCminute > 2000){
				cout << "Minute out of range!" << endl;
				
				checkeric=false;
			}
			else{

				return true;
			}
		}
	}

	return checkeric;
}

float calculateMET(float cMspeed, string exercise){
	float MET;

	if (exercise == "Walking"){

		if (cMspeed >= 4 && cMspeed < 6.5){
			MET=4;
			
		}

		if (cMspeed >= 6.5){
			MET = 6.5;
			
		}
	}

	if (exercise == "Running"){

		if (cMspeed >= 4 && cMspeed < 11){
			MET = 9;
			
		}

		if (cMspeed >= 11){
			MET = 12.5;
			
		}
	}

	if (exercise == "Cycling"){

		if (cMspeed >= 4 && cMspeed < 20){
			MET = 6;
			
		}

		if (cMspeed >= 20 && cMspeed < 25){
			MET = 8;
			
		}

		if (cMspeed >= 25){
			MET = 10.5;
			
				
		}
	}

	return MET;
}

void computeResults( float weight, float goal, float speedwalk, float speedrun, float speedcycle, float minuteswalk, float minutesrun, float minutescycle){

	float walkingmet;
	float runningmet;
	float cyclemet;
	float calwalking;
	float calrunning;
	float calcycle;
	float diff;
	float total;

	walkingmet = calculateMET(speedwalk, "Walking");
	calwalking = ((walkingmet * weight)/60) * minuteswalk;
	


	runningmet = calculateMET(speedrun, "Running");
	calrunning = ((runningmet * weight)/60) * minutesrun;
	


	cyclemet = calculateMET(speedcycle, "Cycling");
	calcycle = ((cyclemet * weight)/60) * minutescycle;
	

	total = calwalking + calrunning + calcycle;
	

	diff = goal - total;

	displayResults(diff, total, weight, walkingmet, runningmet, cyclemet, calwalking, calrunning, calcycle);



}

void displayResults(float diff, float total, float weight, float walkingmet, float runningmet, float cyclemet, float calwalking, float calrunning, float calcycle){

	float morewalk;
	float morerun;
	float morecycle;

	cout << "From walking, you burned " << calwalking << " calories." << endl;
	cout << "From running, you burned " << calrunning << " calories." << endl;
	cout << "From cycling, you burned " << calcycle << " calories." << endl;
	cout << "You burned " << total << " calories." << endl;

	if (diff == 0){
		cout << "Congratulations! You have reached your goal!" << endl;
		
	}
	
	if (diff < 0){
		cout << "You have surpassed your goal! You can eat something worth " << -(diff) << " calories :)" << endl;
		
	}

	if (diff > 0){
		morewalk = diff / ((walkingmet / 60) * weight);
		morerun = diff / ((runningmet / 60) * weight);
		morecycle = diff / ((cyclemet / 60) * weight);


		cout << "You did not reach your goal by " << diff << " calories." << endl;
		cout << "You need to walk " << morewalk << " minutes more to reach your goal or," << endl;
		cout << "You need to run " << morerun << " minutes more to reach your goal or," << endl;
		cout << "You need to do cycling " << morecycle << " minutes more to reach your goal." << endl;
		
	}
}

int main(){
	string mainusername;
	string mainusername2;

	cout << "Please enter your name: ";
	cin >> mainusername;

	getInput(mainusername);

	cout << endl;
	cout << "Please enter your name: ";
	cin >> mainusername;

	getInput(mainusername);

	return 0;
}





// Ahmet Alperen Gungor 28847