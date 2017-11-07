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
	inputStream.open(_fileInPath, std::ios::binary); //Opens input stream
}

char File::readNextByte()
{
	return inputStream.get(); //Gets next byte from the input stream
}

char * File::readNextBytes(int length)
{
	char * buffer = new char[length];
	for (int i = 0; i < length; i++) //Reads n bytes and outputs to char buffer
	{
		buffer[i] = readNextByte();
	}
	return buffer;
}

char * File::readAllBytes() //Reads all bytes
{
	inputStream.seekg(0, inputStream.end);
	int length = inputStream.tellg();
	inputStream.seekg(0, inputStream.beg); //Determines the length in bytes of the file

	std::vector<char> vBuffer;
	while (!inputStream.eof()) //While not at last byte
	{
		vBuffer.push_back(readNextByte()); //Write bytes to buffer
		byteCount++;
		if (byteCount % 1000000 == 0)
		{
			double percentage = ((double)byteCount / (double)length) * 100.0;
			std::cout << byteCount / 1000000 << "/" << length/1000000 << " " << percentage << "%" << std::endl;
		}
	}

	char * buffer = new char[vBuffer.size()]; 
	memcpy(buffer, vBuffer.data(), vBuffer.size()); //Converts vector to char buffer

	return buffer;
}

void File::endReadFile()
{
	inputStream.close(); //Closes input stream 
}

//OUTPUT: Add error handling pls
void File::startWriteFile(std::string _outputFilePath)
{
	outputStream.open(_outputFilePath, std::ios::binary); //Opens output stream to filepath
}

void File::writeByteToFile(char _byte)
{
	outputStream.put(_byte); //Puts a byte into the ouput stream
}
void File::writeAllToFile(char * _buffer)
{
	for (int i = 0; i < byteCount - 1; i++) //Puts all the bytes from the buffer into the output stream
	{
		writeByteToFile(_buffer[i]);
	}
}
void File::writeAllToFile(char * _buffer, int length)
{
	for (int i = 0; i < length; i++) //Puts n bytes from the buffer into the output stream
	{
		writeByteToFile(_buffer[i]);
	}
}

void File::endWriteFile()
{
	outputStream.close(); //Closes the output stream
}

File::~File()
{
}
