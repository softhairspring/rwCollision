//FileManager
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <string>

using namespace std;

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

