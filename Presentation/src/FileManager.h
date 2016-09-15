/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <string>

using namespace std;


/***
Used to load directx files.
*/
class FileManager {
protected:

	ifstream *fs;

	//
	ifstream* getFileStream(char *f);

public:
	FileManager();
	~FileManager();

//
	ifstream* getDxFile(std::string* path);
	ifstream* getDxFile(std::string &path);
	

};





#endif

