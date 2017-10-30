#include <string>
#include <fstream>
#include <vector>
#pragma once

class File
{
public:
	File();
	void loadFileFromPath(std::string _fileInPath);
	char readNextByte();
	char * readNextBytes(int length);
	char* readAllBytes();
	void endReadFile();

	void startWriteFile(std::string _outputFilePath);
	void writeByteToFile(char _byte);
	void writeAllToFile(char * _buffer);
	void writeAllToFile(char * _buffer, int length);
	void endWriteFile();
	~File();
private:
	std::ifstream inputStream;
	std::ofstream outputStream;
	std::string fileInPath;
	int byteCount = 0;
};

