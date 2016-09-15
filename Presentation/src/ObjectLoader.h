/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H
#include "rwCollision.h"
#include "FileManager.h"
//#include <list>

using namespace std;


/**
**/
struct ObjectFileData {
	vector<rwVect3> *points;
	rwMesh			*mesh;
	VertexMap		*vertexmap;
};

/**
DirectX file parser, loads all mesh data.

Klasa odpowiedzialna za ³adowanie obiektów z pliku do programu.
**/
class ObjectLoader : public FileManager{

	FileManager FM;
	ifstream *fs;

	string  wideToAscii( string &majtki);
	double str2dbl(std::string &s);


public:

	ObjectFileData* getMesh(std::string &path);
	

};




#endif
