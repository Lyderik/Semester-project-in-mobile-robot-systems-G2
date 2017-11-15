#include "Header.h"

using namespace std;

void main() {

	Header head;

	/*head.setType(0, 1, 1);
	head.toggleBit(1, 1);
	head.toggleBit(1, 4);
	head.toggleBit(1, 5);
	head.toggleBit(1, 2);
	head.toggleBit(1, 4);*/
		//head.setDest(5);
	//head.setSource(10);
	head.setControl(0, 1, 0, 1, 0, 0, 0, 1);
	head.convertArray();
	head.addArray();
	head.splitArray();
	cout << head.getstart() << endl;
}


