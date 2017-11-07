#include "Header.h"
#include <iostream>
#include <string>
#include <array>

using namespace std;

void main() {

	Header head;

	head.setType(0, 1, 1);
	head.toggleBit(3, 3);
	head.toggleBit(3, 3);
	head.toggleBit(3, 3);
	head.toggleBit(3, 3);
	head.toggleBit(3, 3);
	head.toggleBit(3, 3);
}