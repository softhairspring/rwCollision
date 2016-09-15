/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <vector>
#include "rwObject.h"

using namespace std;

/**
Wieloœcian
**/
class rwPolyhedron : public rwObject{

	

public://

	///
	rwPolyhedron()
		: rwObject(rwVect3(0.0f,0.0f,0.0f) )
	{
	points=NULL;
	}
	///
	rwPolyhedron( rwVect3 &pos)
		: rwObject(pos)
	{
	points=NULL;
	
	}
	

	//
	//		overrided
	//
	///
	rwObject& toObject(){
		return static_cast<rwObject>(*this);
	}

	///
	
	///
	///zwraca punkt pomocniczy dla algorytmu GJK
	///zwracany jest najdalszy punkt w kierunku wektora dir
	///sprawdza iloczyn skalarny kazdego punktu
	virtual rwVect3 getSupportPoint(rwVect3 &dir){
		if( ! isVertexMapAvaible())
			__super::getSupportPoint(dir);

		rwVect3 max = points->front();
		rwVect3 max2 = (*points)[(points->size()/2)];

		int id=0;
		float scalar = max.dot(dir);	
		if(max2.dot(dir) > scalar){		//wstepna optymalizacja
			scalar = max2.dot(dir);
			id = (points->size()/2);
		}

		bool neighbour=true;
		int testedscal=0;
		int newid = 0;

		while(neighbour){
			newid=0;
			for(int i=0;i<VertMap[id].size();i++){
				testedscal = points->at(VertMap->at(id)[i]).dot(dir);
				if( testedscal > scalar){
					max = points->at(VertMap->at(id)[i]);			//nowy wierzcholek
					newid = id;
				}
			}
			if(newid==0)
				neighbour=false;	
			else
				id=newid;
		}

        return max;
	}

	
};




#endif