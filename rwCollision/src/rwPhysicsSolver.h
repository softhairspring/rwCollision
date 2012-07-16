#ifndef RWPHYSICSSOLVER_H
#define RWPHYSICSSOLVER_H

#include "rwObjects.h"
#include "Mersenne/MersenneTwister.h"


/**
* klasa odpowiedzialna za ingerowanie w obiekty - ich ruch i zachowanie
* w tym miejscu mo�na zaimplementowa� oczekiwane zachowanie w odpowiedzi na kolizje.
**/
class rwPhysicsSolver {


	bool ISGRAVITATION;
	MTRand rand;
public:

	rwPhysicsSolver(){
		ISGRAVITATION = true;
	}
	/**
	* oblicza przesuni�cie obiektu dla nastepnej klatki
	**/
	rwVect3& getNewPosition(rwVect3& pos,rwVect3& vel,float& timestep){
		pos += vel*timestep;
		return pos;
	}

	bool step(ObjectsList &list, float timestep){
		rwObject * o;
		rwVect3 v;
		ObjectsList::iterator it = list.begin();
		while(it != list.end() ){
			o = *it;
			v = o->vel_linear;
			///brak kolizji
			if( ! o->isColliding() ) {
				o->setPosition (  o->position + ( o->vel_linear * timestep ) );
			}
			else{///kolizja
				
				//o->setLinearVelocity(  -  o->vel_linear + o->vel_linear*0.008f);	//male spowalnianie
				o->setLinearVelocity( - o->vel_linear );
				
				o->setPosition( o->position + (o->getLinearVelocity()*timestep) );

			}
			///grawitacja
			//if(ISGRAVITATION)
			//	if( ! o->ISSTATIC)
			//		if(o->vel_linear[1] > -10)
			//			o->vel_linear.setY( o->vel_linear[1] - 0.05 );
			it++;
		}
		return false;
	}

};

#endif