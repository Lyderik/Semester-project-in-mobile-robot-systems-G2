#pragma once
#include <iostream>
#include <string>
#include <array>
#include <bitset>
using namespace std;
typedef __int8 byte;

class Header
{
public:
	Header();
	~Header();
	void setBit(int, int);
	void clearBit(int, int);
	void setType(int, int, int);
	void toggleBit(int, int);
protected:
	bitset<8U> header[5];
};