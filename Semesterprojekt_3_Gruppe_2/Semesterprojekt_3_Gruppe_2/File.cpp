#include "File.h"
#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>



File::File()
{
}

//INPUT: Add error handling pls
void File::loadFileFromPath(std::string _fileInPath)
{
	inputStream.open(_fileInPath, std::ios::binary);
}

char File::readNextByte()
{
	return inputStream.get();
}

char * File::readNextBytes(int length)
{
	char * buffer = new char[length];
	for (int i = 0; i < length; i++)
	{
		buffer[i] = readNextByte();
	}
	return buffer;
}

char * File::readAllBytes()
{
	inputStream.seekg(0, inputStream.end);
	int length = inputStream.tellg();
	inputStream.seekg(0, inputStream.beg);

	std::vector<char> vBuffer;
	while (!inputStream.eof())
	{
		vBuffer.push_back(readNextByte());
		byteCount++;
		if (byteCount % 1000000 == 0)
		{
			std::cout << byteCount / 1000000 << "/" << length/1000000 << std::endl;
		}
	}

	char * buffer = new char[vBuffer.size()];
	memcpy(buffer, vBuffer.data(), vBuffer.size());

	return buffer;
}

void File::endReadFile()
{
	inputStream.close();
}

//OUTPUT: Add error handling pls
void File::startWriteFile(std::string _outputFilePath)
{
	outputStream.open(_outputFilePath, std::ios::binary);
}

void File::writeByteToFile(char _byte)
{
	outputStream.put(_byte);
}
void File::writeAllToFile(char * _buffer)
{
	for (int i = 0; i < byteCount - 1; i++)
	{
		writeByteToFile(_buffer[i]);
	}
}
void File::writeAllToFile(char * _buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		writeByteToFile(_buffer[i]);
	}
}

void File::endWriteFile()
{
	outputStream.close();
}

File::~File()
{
}
