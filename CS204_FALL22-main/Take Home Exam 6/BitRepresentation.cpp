#ifndef _BITREPRESENTATION_CPP
#define _BITREPRESENTATION_CPP
// in the representation it is advised that we exclude the implementation cpp from the project, but my compiler does 
// not work when I do it, therefore added it to the project but also made a header guard for implementation cpp too

// in addition, my compiler cannot create vectors using curly brackets under the name non-aggregates cannot be with initializer list

#include "BitRepresentation.h"


// constructor that sets the data T as input number
template <typename T>
BitRepresentation<T>::BitRepresentation(T number) : data(number){
}

// constructor that takes a vector of bools and sets data accordingly
template <typename T>
BitRepresentation<T>::BitRepresentation(vector<bool> vec){
	data = 0;

	// if size of data is smaller than input vector, only take size of data number of bools from vec
	if (sizeof(T)*8 < vec.size()){
		for (int ctr = vec.size() - sizeof(T)*8; ctr< vec.size(); ctr++){
			if (vec[ctr] == true){
				data += 1;
			}
			if (ctr != vec.size()-1){
				data = data << 1;
			}
		}
	}
	// if not, rest of the data will be filled with 0's
	else{
		for (int ctr2 = 0; ctr2< vec.size(); ctr2++){
			if (vec[ctr2] == true){
				data += 1;
			}
			if (ctr2 != vec.size()-1){
				data = data << 1;
			}
		}
	}
}

template <typename T>
string BitRepresentation<T>::getBinary(){
	string str="";
	
	for (int ctr = 0; ctr<sizeof(T)*8; ctr++){
		int currentBit = (data>>ctr) & 1; // if bit on current iteration is 1, then will add 1 to string, if 0 then 0 will be added
		str = to_string(currentBit) + str; // converting to string
	}
	return str;
}

template <typename T>
string BitRepresentation<T>::getHexadecimal(){
	string str = "";
	char digStr[] = "0123456789abcdef"; // indexes of this array represents corresponding hexadecimal numbers
	T tempVal = data;
	int outCtr = 0;
	for (int ctr = 0; ctr < sizeof(T)*2; ctr++){ // iterating size*2 times instead of 8 because inside my code iterates 4 times manually
		
		int currentNum = 0;
		currentNum = (tempVal & 1);
		tempVal = tempVal >> 1;
		currentNum += (tempVal & 1)*2; // this number increments by power of 2 to match correct index
		tempVal = tempVal >> 1;
		currentNum += (tempVal & 1)*4;
		tempVal = tempVal >> 1;
		currentNum += (tempVal & 1)*8;

		tempVal = tempVal >> 1;
		str = digStr[currentNum] + str;

		outCtr++;
	}
	return str;
}

template <typename T>
void BitRepresentation<T>::unsetBit(unsigned int location){
	if (sizeof(T)*8<location){ // location should not be larger than size
		return;
	}
	unsigned int targetBit = 1 << location;
	data &= ~(targetBit); 
}

template <typename T>
void BitRepresentation<T>::setBit(unsigned int location){
	if (sizeof(T)*8<location){ // location should not be larger than size
		return;
	}
	unsigned int targetBit = 1 << location;
	data |= (targetBit);
}

template <typename T>
bool BitRepresentation<T>::isBitSet(unsigned int location){
	if (sizeof(T)*8<location){ // location should not be larger than size
		return false;
	}
	return (data >> location) & 1;
}

template <typename T>
T BitRepresentation<T>::getNumber(){
	return data;
}


#endif 