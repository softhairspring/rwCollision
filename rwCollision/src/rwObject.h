/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef OBJECT_H
#define OBJECT_H

#include <list>
#include <vector>
#include "rwVector.h"
#include "rwAABB.h"
#include "rwDatatypes.h"
#include "rwSimplex.h"
#include "rwVertexMap.h"

using namespace std;

class rwObject;
class rwPhysicsSolver;
///definicja listy obiektów (wskazniki dla zachowania polimorfizmu)
typedef list<rwObject*> ObjectsList;

/**
* klasa bazowa dla wszelkich obiektów figur geometrycznych sceny

* mozliwe ze lepiej by³oby wy³¹czyæ czêœc gotowych metod dla wiêkszej elastycznoœci
**/
class rwObject{
private:

friend rwPhysicsSolver;

	bool ISCOLLIDING;
	bool ISSTATIC;
protected:

	rwAABB BB;
	rwVect3 position;
	rwVect3 vel_linear;
	vector<rwVect3> *points;
	VertexMap		*VertMap;
	rwMesh			*Mesh;
	Simplex* GJKLastValidSimplex;		//simplex z ostatniej kolizji(dla temporal coherence) 
	rwObject* GJKLastValidObj;
	rwVect3 PenetrationDepth;			//zmienna pomocnicza//glebokosc penetracji przy kolizji
public:

	rwObject(rwVect3 &newposition){
		position = newposition;
		BB.c = newposition;
		ISCOLLIDING	=false;
		ISSTATIC	=false;

		points = NULL;
		VertMap=NULL;
		GJKLastValidSimplex=NULL;
		GJKLastValidObj=NULL;
		PenetrationDepth.reset();
	}

	
	///inicjalizuje struktury i ³aduje punkty
	///przyjmuje vector punktów i mape sasiadów wierzcholkow
	bool init(vector<rwVect3> * pts, VertexMap *vm){
		if((points!=NULL) || (vm==NULL))
			return false;
		if(pts==NULL)
			return false;
		points = new vector<rwVect3>;
		assert(points);
		int size = pts->size();
		if( (size<=3) || (size> 66000) )
			return false;
		for (vector<rwVect3>::iterator it = pts->begin(); it!=pts->end(); ++it) {
			points->push_back( *it );
		}
		
		VertMap = vm;

		updateAABB();
		return true;
	}

	///inicjalizuje struktury i ³aduje punkty
	///przyjmuje vector punktów
	bool init(vector<rwVect3> * pts){
		if((points!=NULL))
			return false;
		if(pts==NULL)
			return false;
		points = new vector<rwVect3>;
		assert(points);
		int size = pts->size();
		if( (size<=3) || (size> 66000) )
			return false;
		for (vector<rwVect3>::iterator it = pts->begin(); it!=pts->end(); ++it) {
			points->push_back( *it );
		}

		updateAABB();
		return true;
	}


	///inicjalizuje struktury i ³aduje punkty
	///przyjmuje tablice wszystkich liczb tworzacych punkty (3 * punkt)
	bool init(double *pts, int count){
		if(points!=NULL)
			return false;
		if(pts==NULL)
			return false;
		if( (count<=9) || (count> 66000) )
			return false;
		points = new vector<rwVect3>;
		points->reserve(count);

		assert(points);
		for(int i=0;i<count;i+=3){
			points->push_back( rwVect3((float)*(pts+i),(float)*(pts+i+1),(float)*(pts+i+2)) );
		}
		
		updateAABB();
		return true;
	}


	///oblicza punkt œrodkowy dla punktów
	///w zalozeniu punkty maja byæ relatywne wiec funkcja ne jest potrzebna
	///w praktyce
	virtual void updateCenterPoint(){
		int count=0;
		rwVect3 center;
		for (vector<rwVect3>::iterator it = points->begin(); it!=points->end(); ++it) {
			center += *(it);
			count++;
		}
		center = center / (float)count;
		this->setPosition(center);
	}


	///
	vector<rwVect3> * getConvexHull(){
		return points;}


	///
	vector<rwVect3> & getConvexHullRef(){
		return *points;}


	///zwraca kopie listy punktów w przestrzeni sceny zamiast lokalnych z 0 w centrum
	vector<rwVect3> getAbsoluteConvexHull(){
		vector<rwVect3> o;
		vector<rwVect3>::iterator it = points->begin();
		while(it != points->end() ){
				o.push_back( position + (*it) );
			it++;
		}
		return o;
	}


	///
	///zwraca liste punktów w przestrzeni sceny wraz z dodanymi nowymi punktami
	///dla nowej zadanej pozycji
	vector<rwVect3>* addAbsoluteConvexHullAtPosition(vector<rwVect3> * addTo, rwVect3 &atPosition){
		vector<rwVect3>::iterator it = points->begin();
		while(it != points->end() ){
				addTo->push_back( atPosition + (*it) );
			it++;
		}
		return addTo;
	}







	virtual void step() {
	//
	}


		///
	///zwraca punkt pomocniczy dla algorytmu GJK
	///zwracany jest najdalszy punkt w kierunku wektora dir
	///metoda Hill-Climbing (log n?)
	///sprawdza iloczyn skalarny kazdego punktu
	virtual rwVect3 getSupportPoint(rwVect3 &dir){
		if( ! isVertexMapAvaible())
			getSupportPointLinear(dir);

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

	
	///
	///zwraca punkt pomocniczy dla algorytmu GJK
	///zwracany jest najdalszy punkt w kierunku wektora dir
	///metoda podstawowa o(n)
	///sprawdza iloczyn skalarny kazdego punktu
	virtual rwVect3 getSupportPointLinear(rwVect3 &dir){
		rwVect3 max = points->front();
		float scalar =0;
		vector<rwVect3>::iterator it = points->begin();
		while(it != points->end()){
			if (scalar < (*it).dot(dir)){
                 max = (*it);
				 scalar = max.dot(dir);
			}
			it++;
        }
        return max;
	}


	/// najdalszy punkt w zadanym kierunku
	/// O(n)
	virtual rwVect3 maxPointAlongDirection(rwVect3 &direction)
        {
            rwVect3 max = points->front();
			vector<rwVect3>::iterator it = points->begin();
			while(it != points->end())
            {
                if (max.dot(direction) < (*it).dot(direction))
                    max = (*it);
				it++;
            }

            return max;
        }

	///zwraca stosunek predkosci do wielkoœci obiektu
	///zwróci 2 jeœli przebyta droga w ciagu timestep bedzie dwa razy wieksza
	///od rozmiaru(najwiêkszej œrednicy) figury 
	float getVelocityToSizeRatio(const float &timestep){
		return ( vel_linear.length() / (2*BB.r.length()) ) * timestep;
	}
	
	///__nieuzywane deprecated
	virtual rwObject& toObject(){	return *this;}
	
	//fizyka
	void applyImpulse(rwVect3 &impulse) { vel_linear+= impulse; }

	//accesory
	rwVect3 getPosition()			{ return position;}
	void	setPosition(rwVect3 & pos){position = pos; BB.c = pos;}
	rwVect3 getLinearVelocity()		{return vel_linear;}
	void	setLinearVelocity(rwVect3 & vel){vel_linear = vel;}
	rwAABB& getAABB()				{return BB;}
	void	setAABB(rwAABB & bb)	{BB = bb;}
	rwMesh* getMesh()				{ return Mesh;}
	void	setMesh(rwMesh* mesh)	{Mesh = mesh;}
	void	updateAABB()			{BB.computeAABB( *points );}
	vector<rwVect3> * getVertexList(){ return points;}
	void	setCollidingDistance(rwVect3& dist){PenetrationDepth =  dist;}
	rwVect3& getCollidingDistance()	{return PenetrationDepth;}
	void	resetCollidingDistance(){PenetrationDepth.reset(); }

	//flagi
	void setCollidingFlag(bool collide)	{ 
		ISCOLLIDING=collide;
	}
	bool isColliding()					{return ISCOLLIDING;}
	void setStaticFlag(bool isstatic)	{ ISSTATIC=isstatic;}
	bool isStatic()					{return ISSTATIC;}

	//dane pomocnicze dla GJK do optymalizacji
	rwObject* getGJKLastObj(){ return GJKLastValidObj;};
	Simplex* getGJKLastSimplex(){return GJKLastValidSimplex;}
	void setGJKLastData(Simplex &s, rwObject* o){
		if(GJKLastValidSimplex!=NULL) 
			delete(GJKLastValidSimplex); 
		GJKLastValidSimplex = &s;
		GJKLastValidObj = 0;
	}

	bool isVertexMapAvaible(){ if(VertMap==NULL) return false; return true; }
};

#endif