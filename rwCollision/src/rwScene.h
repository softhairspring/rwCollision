/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RWSCENE_H
#define RWSCENE_H

#include <list>
#include "rwObjects.h"
#include "rwCollisionDetector.h"
#include "rwCollisionDetectorGJK.h"
#include "rwCollisionDetectorBasic.h"

#include "rwDefinedFigures.h"
#include "rwDebugDataCollector.h"
#include "rwPhysicsSolver.h"

using namespace std;

///definicja listy obiektów (wskazniki dla zachowania polimorfizmu)
typedef list<rwObject*> ObjectsList;

/**
* Klasa zarzadzajaca obiektami symulacji
* i przechowywujaca je 
**/
class rwScene {

ObjectsList Objects;
rwCollisionDetector *CD;
rwPhysicsSolver		*PS;
bool ISBROAD;


private:


	///tymczasowa metoda ruszajaca obiektami
	void stepPhysics(){

	}
public:
	///konstruktor
	rwScene(){
		CD=NULL;
		PS=NULL;
		ISBROAD=true;
	}
	///inicjalizacja musi byc wywolana przed uzyciem klasy
	bool init(){
		CD = new rwCollisionDetectorGJK();
		//CD = new rwCollisionDetectorBasic();
		PS = new rwPhysicsSolver();
		if(CD && PS)
			return true;
		return false;
	}


	bool assignPhysicsSolver(rwPhysicsSolver * ps){ PS = ps; return false;}

	bool assignBasicCollisionDetector() 
	{ 
		if(CD)
			delete(CD);
		CD = new rwCollisionDetectorBasic();
		if(CD)
			return true;
		return false;
	}
	bool assignGJKCollisionDetector() 
	{ 
		if(CD)
			delete(CD);
		CD = new rwCollisionDetectorGJK();
		if(CD)
			return true;
		return false;
	}

	///ustawienie wlasnego/innego wykrywacza kolizji (na przyszlosc)
	bool assignCollisionDetector(rwCollisionDetector *cd){
		if(cd==NULL)
			return false;
		if(CD!=NULL)
			delete(CD);
		CD = cd;
		return true;
	}
	///dodaje do sceny obiekt 3D
	bool addObject( rwObject& addme){
		Objects.push_back(&addme);
		return true;
	}
	///del? ptr zamiast
	///dodaje do sceny obiekt 3D (polyhedron)
	bool addObject(rwPolyhedron& addme){
		Objects.push_back( &addme );
		return true;
	}
	///metoda do usuwania obiektu ze swiata i z programu
	void releaseObject(rwObject * killme){}

	//
	///TODO: sprawdz czy sa wycieki ~!
	void clear(){
		Objects.clear();
	}
	///iteracja symulacji sceny
	///timestep - krok czasowy podany jako czesc sekundy (np. 0.1)
	///zamiast tego mozna uzyc solveCollision i	solvePhysics //
	void step(float timestep){
		solveCollision	(timestep);
		solvePhysics	(timestep);
	}
	
	///solveCollision i solvePhysics sa rozbiciem metody step, zeby moc dodac inne wywolania miedzy metodami.
	void solveCollision(float& timestep){
		assert((timestep>0)&&(timestep<1.5f) );
		assert(CD);
		assert(PS);

		CD->updateObjectsList( Objects, timestep );
		
		CD->solveBroad();
		CD->solveNarrow();
	}

	void solvePhysics(float& timestep){
		
		PS->step(Objects, timestep);
	}

	ObjectsList & getObjects(){ return Objects;}

	inline void setContinuousCollisionDetection(bool onoff) { CD->setCCD( onoff) ; } 
	inline bool isContinuousCollisionDetectionOn(){return CD->isCCD(); }

	virtual void setBroadPhase(bool broad)	{CD->setBroadPhase( broad);}
	virtual bool isBroadPhase()			{ return CD->isBroadPhase();}
};




#endif