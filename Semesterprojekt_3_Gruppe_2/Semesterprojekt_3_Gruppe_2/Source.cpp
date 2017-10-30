#include <iostream>
#include <string>
#include "File.h"

void main()
{
	File testFile;
	testFile.loadFileFromPath("C:\\Users\\aszel\\Desktop\\msyh.ttc");
	char* bytes = testFile.readAllBytes();
	testFile.endReadFile();

	testFile.startWriteFile("C:\\Users\\aszel\\Desktop\\car.ttc");
	testFile.writeAllToFile(bytes);
	testFile.endWriteFile();
}