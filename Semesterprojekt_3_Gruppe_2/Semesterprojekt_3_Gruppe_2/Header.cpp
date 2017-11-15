#include "Header.h"

Header::Header()
{
	header[0] = 00000000;
	header[1] = 00000000;
	header[2] = 00000000;
	header[3] = 00000000;
	header[4] = 00000000;
	data[0]   = 11111111;
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

void Header::toggleBit(int nr, int s)
{
	//nr er det array nummer det skal settes i.
	//s er den posítion i 8 bit værdien der skal toggles.
	header[nr] = header[nr].flip(s);
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
}

void Header::setDest(char d)
{
	//tager en char som argument, og sætter det i det slot i arrayet der svarer til destination adressen, lig med den char.
	header[1] = d;
}

void Header::setSource(char s)
{
	//tager en char som argument, og sætter det i det slot i arrayet der svarer til source adressen, lig med den char.
	header[2] = s;
}

void Header::setControl(int o, int t, int th, int f, int fi, int s, int se, int e)
{
	//Tager individuelle bits til at sætte hvert enkelt felt i control slottet i arrayet.
	header[3] = 00000000;
	if (o == 1)
	{
		setBit(3, 7);
	}
	if (t == 1)
	{
		setBit(3, 6);
	}
	if (th == 1)
	{
		setBit(3, 5);
	}
	if (f == 1)
	{
		setBit(3, 4);
	}
	if (fi == 1)
	{
		setBit(3, 3);
	}
	if (s == 1)
	{
		setBit(3, 2);
	}
	if (se == 1)
	{
		setBit(3, 1);
	}
	if (e == 1)
	{
		setBit(3, 0);
	}
}

void Header::convertArray()
{
	//converterer et bitset array til et array af chars.
	for (int i = 0; i != 5; i++)
	{
		headerChar[i] = header[i].to_ulong();
	}
}

void Header::addArray()
{
	//ligger dataen og headeren sammen i et char frame array, som kan sendes videre.
	int i = 0;
	while (i!=4)
	{
		frame[i] = headerChar[i];
		i++;
	}
	int j = 0;
	while (i!=4+1)
	{
		frame[i] = data[j];
		i++;
		j++;
	}
}

void Header::splitArray()
{
		//skiller framen ad i header & data.
	int i = 0;
	int j = 0;
	int k = 0;
	while (j!=4)
	{
		newHeaderChar[i] = newFrame[j];
		i++;
		j++;
	}
	while (j!=5)
	{
		newData[k] = newFrame[j];
		k++;
		j++;
	}
	
	for (int l = 0; l != 5; l++)
	{
		//converterer char header array til bitset array.

		newHeader[l] = newHeaderChar[l];
	}
}

bool Header::getstart()
{
		//tager bitten på positionen hvor startflaget er, og laver det til en char

		char bit = newHeader[3].to_string().at(0);

		//tjekker om flaget er et, og retunerer sandt eller falsk.

		if (bit == '1')
		{
			return true;
		}
			return false;
}

Header::~Header()
{
}