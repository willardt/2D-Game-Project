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
	std::vector<std::string> _data;
	std::vector<u16string> _udata;

	void _setPath(const int& filenum);
	void _setPath(std::string filename);
public:
	void read();
	//*path - filepath
	void read(std::string path);

	void uread();
	//*path - filepath
	void uread(std::string path);

	// *line - file line
	int getInt(int line);
	// *line - file line
	float getFloat(int line);
	// *line - file line
	std::string getStr(int line);
	// *line - file line
	u16string getU16(int line);

	int getSize();

	//Checks if file exists
	bool exists(std::string path);
};

#endif