/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RWAABB_H
#define RWAABB_H

#include <vector>
#include "rwVector.h"
#include "rwMath.h"

using namespace std;

/**
*	Axis aligned bounding box used for erly collision detection

*	Figura ograaniczajaca prawdziwy obiekt
**/
struct rwAABB {
	/// punkt œrodka
	rwVect3 c; 
	/// promieñ/po³owy d³ugoœci krawêdzi pude³ka
    rwVect3 r; 
	///czy koliduje
	bool ISCOLLIDING;
	///ustala wymiary BB na podstawie punktow
	bool computeAABB(vector<rwVect3> &v){
		//rwVect3 maxv;
		vector<rwVect3>::iterator it = v.begin();
		while(it != v.end() ){
			if( rwMath::absolute( (*it)[0] ) > r[0] ) 
				r.setX (rwMath::absolute((*it)[0] ));
			if( rwMath::absolute( (*it)[1] ) > r[1] ) 
				r.setY (rwMath::absolute((*it)[1] ));
			if( rwMath::absolute( (*it)[2] ) > r[2] ) 
				r.setZ (rwMath::absolute((*it)[2] ));
			it++;
		}
		return validate();
	}

	///poprawienie pozycji
	void setPosition(rwVect3 &pos){
		c = pos;
	}
	void setHalfRadius(rwVect3 &radius){
		r = radius;
	}
	//bool computeAABB(double *lista, int &count){
	//	return false;
	//}

	///sprawdza czy dwa AABB siê przecinaj¹
	bool checkCollision(rwAABB & box)
	{
		if (rwMath::absolute(c[0] - box.c[0]) > (r[0] + box.r[0])) return false;
		if (rwMath::absolute(c[1] - box.c[1]) > (r[1] + box.r[1])) return false;
		if (rwMath::absolute(c[2] - box.c[2]) > (r[2] + box.r[2])) return false;
		return true;
	}

	///sprawdzenie poprawnosci wymiarów (czy zostaly zainicjalizowane)
	bool validate(){
		if( 
			(r[0]==0.0f)&&
			(r[1]==0.0f)&&
			(r[2]==0.0f)
			)
			return false;
		return true;
	}

	///zwraca nowy AABB ze srodkiem pomiedzy aktualn¹ pozycj¹
	///a podana pozycj¹ secondposition i nowymi wymiarami pokrywaj¹cymi
	///obiekt bêd¹cy w dwóch pozycjach jednoczeœnie
	rwAABB getExpandedAABB(rwVect3 &secondposition){
		rwAABB nBB;
		rwVect3 half = (secondposition-c)*0.5;
		nBB.c		 = c    + half;
		nBB.r		 = half.absolute() + r; 
		return nBB;
	}

	///zwraca najwieksz¹ wartoœæ z wszystkich ograniczajacych osi
	float getMaxBound(){
		if(r[0] > r[1])
			if(r[0] >r[2])
				return r[0];
		if(r[1] > r[2])
			return r[1];
		return r[2];
	}
	void setCollidingFlag(bool collide)	{ ISCOLLIDING=collide;}
	bool isColliding()					{return ISCOLLIDING;}
};




#endif