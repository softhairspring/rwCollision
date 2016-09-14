#ifndef RWPHYSICSSOLVER_H
#define RWPHYSICSSOLVER_H

#include "rwObjects.h"
#include "Mersenne/MersenneTwister.h"
#include "rwRKIntegrator.h"


/**
* klasa odpowiedzialna za ingerowanie w obiekty - ich ruch i zachowanie
* w tym miejscu mo¿na zaimplementowaæ oczekiwane zachowanie w odpowiedzi na kolizje.
**/
class rwPhysicsSolver {

	rwRKIntegrator Integrator;
	bool ISGRAVITATION;
	MTRand rand;

public:

	rwPhysicsSolver(){
		ISGRAVITATION = true;
	}
	/**
	* oblicza przesuniêcie obiektu dla nastepnej klatki
	**/
	rwVect3& getNewPosition(rwVect3& pos,rwVect3& vel,float& timestep){
		pos += vel*timestep;
		return pos;
	}


	//simplest mockup - movement only along velocity vector
	bool step(ObjectsList &list, float timestep){
		rwObject * o;
		rwVect3 v;
		ObjectsList::iterator it = list.begin();
		while(it != list.end() ){
			o = *it;
			v = o->vel_linear;
			//dont move static object
			if (o->ISSTATIC){
				it++;
				continue;
			}
			///brak kolizji
			if( ! o->isColliding() ) {
				//o->setPosition (  o->position + ( o->vel_linear * timestep ) );
				State s;
				s.x = o->position;
				s.v = o->vel_linear;
				Integrator.integrate(s, 0, timestep);
				o->position=s.x;
				o->vel_linear = s.v;

			}
			else{///kolizja
				
				//o->setLinearVelocity(  -  o->vel_linear + o->vel_linear*0.008f);	//male spowalnianie
				//o->setLinearVelocity( - o->vel_linear );
				
				//old
				//o->setPosition( o->position + (o->getLinearVelocity()*timestep) );
				//RK4
				State s;
				s.x = o->position;
				s.v = -o->vel_linear;
				Integrator.integrate(s, 0, timestep);
				// minus
				o->position = s.x;
				o->vel_linear =  s.v;
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