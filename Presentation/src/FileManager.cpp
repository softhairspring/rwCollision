#ifndef FILEMANAGER_C
#define FILEMANAGER_C

#include <string>
#include <fstream>
#include "FileManager.h"

FileManager::FileManager(){
	fs = NULL;
}
FileManager::~FileManager(){
	if(fs!=NULL)
		;
}
/**
* zwraca stream do pliku *.x
**/
ifstream* FileManager::getDxFile(std::string* path){

	if(path->find_last_of(".x",1)==std::string::npos)
		return NULL;
	int size = path->size();
	char *cf = new char[size+1];
	path->copy( cf , size+1,0);//size pos
	return getFileStream( cf );
}
ifstream* FileManager::getDxFile(std::string &path){

	if(path.find_last_of(".x")==std::string::npos)
		return NULL;

	int size = path.size();
	char *cf = new char[size+1];
	path.copy( cf , size,0);//size pos
	*(cf+size)='\0';
	return getFileStream( cf);
}

/**
*otworzenie pliku i zwrocenie jego bufora
**/
ifstream* FileManager::getFileStream(char *f){
		if(f==NULL)
			return NULL;
	fs=NULL;
	fs=new ifstream();
	fs->open (f, ifstream::in);
	
	if ( fs->good())
		return fs;
	//if( ! fs->bad() )
	//	if( ! fs->fail() )
	//		if( ! fs->eof() )
	//			;
	return NULL;
}


#endif