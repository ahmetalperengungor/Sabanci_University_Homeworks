#ifndef _BITREPRESENTATION_H
#define _BITREPRESENTATION_H         
			
  
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// declaration of BitRepresentation file
template <typename T>
class BitRepresentation{
private:
	T data;
public:
	BitRepresentation(T number);
	BitRepresentation(vector<bool>);
	string getBinary();
	string getHexadecimal();
	void setBit(unsigned int location);
	void unsetBit(unsigned int location);
	bool isBitSet(unsigned int location);
	T getNumber();

};

 
#include "BitRepresentation.cpp"

#endif