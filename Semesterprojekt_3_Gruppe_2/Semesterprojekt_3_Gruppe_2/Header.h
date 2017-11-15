#pragma once
#include <iostream>
#include <string>
#include <array>
#include <bitset>
using namespace std;

class Header
{
public:
	Header();
	~Header();
	void setBit(int, int);
	void clearBit(int, int);
	void setType(int, int, int);
	void toggleBit(int, int);
	void setDest(char);
	void setSource(char);
	void setControl(int, int, int, int, int, int, int, int);
	void addArray();
	void convertArray();
	void splitArray();
	bool getFlag(int nr, int s);
	bool getstart();
protected:
	bitset<8U> header[5];
	bitset<8U> newHeader[5];
	char data[1];
	char newData[1];
	char headerChar[5];
	char newHeaderChar[5];
	char frame[6];
	char newFrame[6];
};