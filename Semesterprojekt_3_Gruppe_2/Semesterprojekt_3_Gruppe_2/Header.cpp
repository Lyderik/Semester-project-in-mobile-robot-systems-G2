#include "Header.h"

Header::Header()
{
	header[0] = 00000000;
	header[1] = 00000000;
	header[2] = 00000000;
	header[3] = 00000000;
	header[4] = 00000000;
}


void Header::setBit(int nr, int s)
{
	//nr er det array nummer det skal settes i
	//s er den posítion i 8 bit værdien der skal sættes
	header[nr] = header[nr].set(s, true);
}

void Header::clearBit(int nr, int s)
{
	//nr er det array nummer det skal settes i
	//s er den posítion i 8 bit værdien der skal cleares
	header[nr] = header[nr].reset(s);
}

void Header::setType(int one, int two, int three)
{
	if (one == 1)
	{
		setBit(2, 2);
	}
	else if (one == 0)
	{
		clearBit(2, 2);
	}
	if (two == 1)
	{
		setBit(2, 1);
	}
	else if (two == 0)
	{
		clearBit(2, 1);
	}
	if (three == 1)
	{
		setBit(2, 0);
	}
	else if (three == 0)
	{
		clearBit(2, 0);
	}

	cout << header[2] << endl;
}

void Header::toggleBit(int nr, int s)
{
	//nr er det array nummer det skal settes i.
	//s er den posítion i 8 bit værdien der skal toggles.
	header[nr] = header[nr].flip(s);
	cout << header[3] << endl;
}

Header::~Header()
{
}
