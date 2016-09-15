/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef OBJECTLOADER_C
#define OBJECTLOADER_C


#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <strstream>
#include "ObjectLoader.h"
#include "rwMesh.h"


#define _DEG2RAD 0.0174532925

using namespace std;


/**
* Pobiera dane na temat obiektu 3D z pliku typu .x (DirectX)
* wczytuje punkty oraz indeksy tworz¹ce œciany(trójk¹ty) obiektu
* 
**/
ObjectFileData* ObjectLoader::getMesh(std::string &path){

fs = FM.getDxFile(path);
if(fs==NULL)
	return NULL;

ObjectFileData *OFD;
vector<rwVect3> *pts;
rwMesh *mesh ;

rwVect3 v;
int nrofmeshes=0;
int nrofvertices=0;
int nr=0;
size_t strpos[3];


std::string o="";
//std::string n;

while(true){
	if(o.find("{") !=std::string::npos )
	{
		if(fs->eof())		//koniec pliku
			break;
		///poierz ilosc punktow
		getline(*fs,o);
		nr = o.find_first_of(";");
		if(nr==string::npos)		//blad skladni
			break;
		nrofvertices = (int)this->str2dbl( o.substr( 0,nr) );
		if(nrofvertices < 3)			// za malo punktow
			continue;

		///pobierz punkty bry³y
		pts = new vector<rwVect3>;
		

		for(int i=0;i<nrofvertices;++i){
			getline(*fs,o);
			strpos[0]= o.find_first_of	(";");
			strpos[1]= o.find			(";",strpos[0]+1);
			strpos[2]= o.find			(";",strpos[1]+1);
			rwVect3 v;
			v.setX(  (float)str2dbl ( o.substr(0, strpos[0]) )  );
			v.setY(  (float)str2dbl ( o.substr(strpos[0]+1, strpos[1]-strpos[0]-1 ) )  );
			v.setZ(  (float)str2dbl ( o.substr(strpos[1]+1, strpos[2]-strpos[1]-1 ) )  );
			pts->push_back( v );

			
		}

		///pobierz mape siatki
		mesh= new rwMesh();

		getline(*fs,o);
		nr = o.find_first_of(";");
		if(nr==string::npos)		//blad skladni
			break;
		nrofmeshes = (int)this->str2dbl( o.substr( 0,nr) );
		if(nrofmeshes < 3)			// za malo punktow
			continue;

		for(int i=0;i<nrofmeshes;++i){
			getline(*fs,o);
			if( o[0] != '3' )		//nie ma trójk¹ta!
				return NULL;
			strpos[0]= o.find			(",", 2);
			strpos[1]= o.find			(",",strpos[0]+1);
			strpos[2]= o.find			(";",3);
			rwFace face;
			face.a =(int)str2dbl ( o.substr(2, strpos[0]-2) );
			face.b =(int)str2dbl ( o.substr(strpos[0]+1, strpos[1]-strpos[0]-1 ) );
			face.c =(int)str2dbl ( o.substr(strpos[1]+1, strpos[2]-strpos[1]-1 ) );
			mesh->addFace( face );
		}

		///tworzenie mapy wierzcholkow
		VertexMap *VM = new VertexMap();	//do mapowania sasiednich wierzcholkow

		

		for(int i=0;i<nrofvertices;i++){
			VertexNeighbours VN;
			VM->push_back(VN);

			rwFace f;
			f = mesh->resetIterator();
			while(!mesh->isEnd() ){
				if( (f.a==i)||(f.b==i)||(f.c==i)){	//vert in face
					if(f.a!=i)
						(*VM)[i].push_back(f.a);
					if(f.b!=i)
						(*VM)[i].push_back(f.b);
					if(f.c!=i)
						(*VM)[i].push_back(f.c);
				}
				f = mesh->getNext();
			}
		}

		///usun dublujace sie indeksy sasiadujacych wierzcholkow
		int size=0;;
		for(int i=0;i<nrofvertices;i++){
				size = (VM->at(i)).size();
				for(int v=0;v<size;v++)
					for(int n=v;n<size;n++)
						if((*VM)[i][v]==(*VM)[i][n]){
							(*VM)[i].erase((*VM)[i].begin()+n);
							size=size-1;
						}
		}


		OFD = new ObjectFileData();
		OFD->points = pts;
		OFD->mesh = mesh;
		OFD->vertexmap = VM;
		return OFD; //true ret
	}
	else
	{
		getline(*fs,o) ;
	}
	if(o.find("}") !=std::string::npos )
		return NULL;
}
	return NULL;

}






/**
*konwersja stringu z Unicode na Ascii
*innymi s³owy usuwanie pustych bajtów z ci¹gu
**/
string  ObjectLoader::wideToAscii( string &majtki)
{

	std::string o;
	int count=0;
	for(int i=0;i < (int)majtki.length(); i++){
		if( ((int)majtki.at(i)) > 0){
			o.insert(count,&majtki.at(i));
			++count;
		}
	}
	o.insert(count,"\n");
	return o;
}




double ObjectLoader::str2dbl(std::string &s){
	char * cstr;
	double d=0;
	if(s.empty())
		return 0x0;
	cstr = new char [s.size()+1];
	strcpy_s (cstr, s.size()+1, s.c_str());
	return strtod( cstr , NULL);
}
 


#endif


