#include "file.h"

std::string File::readStr(const int& filenum, const int& linenum) {
	std::ifstream file;
	std::string returnString = " ";
	size_t find = 0;

	setPath(filenum);

	file.open(_filepath.c_str());

	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
	}

	file.close();

	find = returnString.find('-');

	returnString.erase(0, find + 2);

	return returnString;
}

std::string File::readStr(const int& linenum) {
	std::ifstream file;
	std::string returnString = " ";
	size_t find = 0;

	file.open(_filepath.c_str());

	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
	}

	file.close();

	find = returnString.find('-');

	returnString.erase(0, find + 2);

	return returnString;
}

u16string File::readUnicodeStr(const int& linenum) {
	std::wifstream file(_filepath, std::ios::binary);
	std::wstring wstr = L"";
	size_t find = 0;
	file.imbue(std::locale(file.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	
	for (int i = 0; i < linenum; i++) {
		std::getline(file, wstr);
	}

	file.close();

	find = wstr.find('-');

	wstr.erase(0, find + 2);
	
	std::wcout << wstr << std::endl;

	u16string utext(wstr.begin(), wstr.end());

	return utext;
}

int File::readInt(const int& filenum, const int& linenum) {
	std::ifstream file;
	std::string returnString;
	size_t find = 0;
	int fileInt = 0;

	setPath(filenum);

	file.open(_filepath.c_str());

	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
	}

	file.close();

	find = returnString.find('-');

	returnString.erase(0, find + 2);

	std::istringstream iss(returnString);

	iss >> fileInt;

	return fileInt;
}

int File::readInt(const int& linenum) {
	std::ifstream file;
	std::string returnString;
	size_t find = 0;
	int fileInt = 0;

	file.open(_filepath.c_str());

	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
	}

	file.close();

	find = returnString.find('-');

	returnString.erase(0, find + 2);

	std::istringstream iss(returnString);

	iss >> fileInt;

	return fileInt;
}

float File::readFloat(const int& linenum) {
	std::ifstream file;
	std::string returnString;
	size_t find = 0;
	float fileFloat = 0;

	file.open(_filepath.c_str());

	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
	}

	file.close();

	find = returnString.find('-');

	returnString.erase(0, find + 2);

	std::istringstream iss(returnString);

	iss >> fileFloat;

	return fileFloat;
}

/*
void File::writeInt(const int& num, const int& linenum) {
	std::fstream file;
	std::string returnString = "";
	std::string writeString = "";
	std::string totalString = "";
	std::string value = std::to_string(num) + "\n";
	size_t find = 0;
	size_t size = 0;
	
	file.open(_filepath.c_str(), std::fstream::in | std::fstream::out);


	int i = 0;
	while (!file.eof()) {
		std::getline(file, returnString);
		std::cout << "Next line: " << returnString << std::endl;

		if (i < linenum - 1) {
			size += returnString.length() + 2;
			std::cout << "Size: " << size << std::endl;
		}
		else if (i == linenum - 1) {
			writeString = returnString;
		}
		else if (i >= linenum) {
			totalString += returnString + "\n";
		}

		i++;
	}

	/*
	for (int i = 0; i < linenum; i++) {
		std::getline(file, returnString);
		std::cout << returnString << std::endl;
		if (i < linenum - 1) {
			size += returnString.length() + 2;
			totalString += returnString + "\n";
		}
	}

	std::cout << totalString << std::endl;

	find = writeString.find('-');

	std::cout << find << std::endl;

	file.seekp(size + find + 2);

	std::cout << size + find + 2 << std::endl;

	file.write(value.c_str(), value.length());

	file.close();
}
*/

void File::setPath(const int& filenum) {
	switch (filenum) {
	case OPTIONS_FILE:
		_filepath = "Data/options.txt";
		break;
	case PLAYER_FILE:
		_filepath = "Data/player.txt";
		break;
	}
}

void File::setPath(std::string filepath) {
	_filepath = filepath;
}