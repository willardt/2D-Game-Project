#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <locale>
#include <codecvt>

#ifndef FILE_H
#define FILE_H

#define OPTIONS_FILE 0
#define PLAYER_FILE 1

typedef std::basic_string<uint16_t, std::char_traits<uint16_t>, std::allocator<uint16_t> > u16string;

class File {
private:
	std::string _filepath;
public:
	std::string readStr(const int& filenum, const int& linenum);
	std::string readStr(const int& linenum);
	u16string readUnicodeStr(const int& linenum);
	void writeStr(const int& filenum, const int& linenum);
	void writeStr(const int& linenum);
	int readInt(const int& filenum, const int& linenum);
	int readInt(const int& linenum);
	void writeInt(const int& filenum, const int& linenum);
	void writeInt(const int& linenum);
	float readFloat(const int& linenum);
	void setPath(const int& filenum);
	void setPath(std::string filename);
};

#endif