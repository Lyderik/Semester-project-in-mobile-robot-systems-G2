#include <iostream>
#include <string>
#include "File.h"

void main()
{
	File testFile;
	testFile.loadFileFromPath("D:\\Feature Films\\(1982) Blade Runner\\Blade Runner.mkv");
	char* bytes = testFile.readAllBytes();
	testFile.endReadFile();

	testFile.startWriteFile("D:\\Feature Films\\(1982) Blade Runner\\Blade Runner Copy.mkv");
	testFile.writeAllToFile(bytes);
	testFile.endWriteFile();
}