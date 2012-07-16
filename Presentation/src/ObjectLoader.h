#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H
#include "rwCollision.h"
#include "FileManager.h"
//#include <list>

using namespace std;

struct ObjectFileData {
	vector<rwVect3> *points;
	rwMesh			*mesh;
	VertexMap		*vertexmap;
};

/**
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
