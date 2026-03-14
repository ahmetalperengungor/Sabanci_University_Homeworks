#include <iostream>
#include <string> 
using namespace std;

int main(){
	string name;
	float midterm_grade;
	float final_grade;
	float th1, th2, th3, th4;
	float avg_grade_to_pass;
	float weighted_exam_grade;
	float weighted_exam_avg;
	float take_home_avr;
	float ratio;
	float take_home_grade;
	float recitation_attendance;
	float participation;
	float course_numeric_grade;

	cout << "The purpose of this program is to calculate your CS201 grade. What is your name? ";
	cin >> name;
	cout << "Hello " << name << ", what are your midterm and final exam grades? ";
	cin >> midterm_grade >> final_grade;

	weighted_exam_grade = (0.33 * midterm_grade) + (0.35 * final_grade);
	weighted_exam_avg = weighted_exam_grade / 0.68;
	avg_grade_to_pass = (30.0 * 0.33 + 30.0 * 0.35) / 0.68;

	if (weighted_exam_avg < avg_grade_to_pass){
		cout << name << ", your weighted exam average is less than 30, you will fail the course!" << endl;
	}

	else {

		cout << "What are the grades of your 4 take-home exams? ";
		cin >> th1 >> th2 >> th3 >> th4;
		take_home_avr = (th1 + th2 + th3 + th4) / 4;
		ratio = take_home_avr / weighted_exam_avg;
		if (ratio <= 2){
			take_home_grade = take_home_avr;
			cout << "You'll get all the points earned from take-home exams!" << endl;
			cout << "How many days have you attended recitations? ";
			cin >> recitation_attendance;
			participation =  (recitation_attendance / 13) * 100;

			course_numeric_grade = (participation * 0.07) + (take_home_grade * 0.25) + (midterm_grade * 0.33) + (final_grade * 0.35);
			
			cout << name << ", your grade for CS201 is: " << course_numeric_grade << endl;
		}

		else if (ratio > 2 && ratio < 3){
			take_home_grade = take_home_avr * (3-ratio);
			cout << "You'll get " << take_home_grade << " from the points earned from take-home exams!" << endl;
			cout << "How many days have you attended recitations? ";
			cin >> recitation_attendance;
			participation =  (recitation_attendance / 13) * 100;

			course_numeric_grade = (participation * 0.07) + (take_home_grade * 0.25) + (midterm_grade * 0.33) + (final_grade * 0.35);
			
			cout << name << ", your grade for CS201 is: " << course_numeric_grade << endl;
		}

		else if (ratio >= 3){
			take_home_grade = 0;
			cout << "You'll get NONE of the points earned from take-home exams!" << endl;			cout << "How many days have you attended recitations? ";			cin >> recitation_attendance;			participation =  (recitation_attendance / 13) * 100;
			course_numeric_grade = (participation * 0.07) + (take_home_grade * 0.25) + (midterm_grade * 0.33) + (final_grade * 0.35);
			
			cout << name << ", your grade for CS201 is: " << course_numeric_grade << endl;
		}










	}







	// Ahmet Alperen GUNGOR 28847
	
















}