#ifndef COLLISION_DETECTORGJK_H
#define COLLISION_DETECTORGJK_H

#include <queue>
#include "rwGlobals.h"
#include "rwObjects.h"
#include "rwGJK.h"
using namespace std;


/*
Klasa koordynuj¹ca szukanie kolizji miedzy obiektami wykorzystuj¹c algorytm GJK. 
Jest odpwiedzialna za sterowanie testami fazy wstêpnej i w¹skiej.
Algorytmy wykrywania kolizji znajduja sie w innych klasach
Klasa nie powinna modyfikowac obiektow bezposrednio - tylko przez inne klasy
**/
class rwCollisionDetectorGJK: public rwCollisionDetector {
	
	

	rwPhysicsSolver PS;
	rwGJK gjk;
	queue<rwObject*> broad_colliding_pairs;


	//local
	rwAABB extrBB1, extrBB2;

	//temp
	rwVect3 newpos1, newpos2;
	vector<rwVect3> vect1, vect2;

	///zeruje flagi kolizji obiektów dla jasnoœci
	void setFlagsToZero(){
		ObjectsList::iterator it = Objects.begin();
		while(it != Objects.end() ){
			(*it)->setCollidingFlag(false);
			(*it)->getAABB().setCollidingFlag(false);
			(*it)->resetCollidingDistance();
			it++;
		}
	}
public:

	
	/**
	 wykrycie wstepnych kolizji BBox'ów
	 z uzyciem extrude dla szybkich obiektow
	 **/
	virtual bool solveBroad(){
		if(! this->isBroadPhase())
		{
			return skipBroad();
		}

		if(this->Objects.empty())
			return false;

		setFlagsToZero();
		
		ObjectsList::iterator l1 = Objects.begin();
		ObjectsList::iterator l2 = l1;
		l2++;
		while(l2 != Objects.end() ){
			while(l2 != Objects.end() ){

				if( isCCD() ){
					//czy stosowac extrapolacje
					//jesli tak to zastosuj rozciagniete AABB
					if( (*l1)->getVelocityToSizeRatio(timestep) >minimum_sweep_for_extrude*timestep ){
						rwVect3 extrpos = PS.getNewPosition(
							(*l1)->getPosition(),
							(*l1)->getLinearVelocity(),
							timestep
						);
						 extrBB1 = (*l1)->getAABB().getExpandedAABB(extrpos);
#ifdef DEBUG
						rwDebugDataColletor::CI.bbarray.push_back( extrBB1 );
#endif
					}else{
						extrBB1 = (*l1)->getAABB();
					}

					if( (*l2)->getVelocityToSizeRatio(timestep) >minimum_sweep_for_extrude*timestep ){
						rwVect3 extrpos = PS.getNewPosition(
							(*l2)->getPosition(),
							(*l2)->getLinearVelocity(),
							timestep
						);
						 extrBB2 = (*l2)->getAABB().getExpandedAABB(extrpos);
#ifdef DEBUG
						rwDebugDataColletor::CI.bbarray.push_back( extrBB2 );
#endif
					}else{
						extrBB2 = (*l2)->getAABB();
					}
					//sprawdz kolizje CCD
					if(  extrBB1.checkCollision( extrBB2 )  ){//koliduje
						broad_colliding_pairs.push(*l1);		//wstaw do kolejki do narrow
						broad_colliding_pairs.push(*l2);
						(*l1)->getAABB().setCollidingFlag(true);
						(*l2)->getAABB().setCollidingFlag(true);
					}
				}
				else
				{
					//dla braku CCD:
					if(  (*l1)->getAABB().checkCollision( (*l2)->getAABB() )  ){//koliduje
						broad_colliding_pairs.push(*l1);		//wstaw do kolejki do narrow
						broad_colliding_pairs.push(*l2);
						(*l1)->getAABB().setCollidingFlag(true);
						(*l2)->getAABB().setCollidingFlag(true);
					}

				}

				
				l2++;
			}
			l1++;
			l2=l1;
			l2++;
		}
		return true;
	}
	/**
	wykrycie dokladnej kolizji obiektów które zosta³y wykryte w fazie broad
	jako koliduj¹ce
	**/
	virtual bool solveNarrow(){
		bool boom =false;
		rwVect3 buff;
		while( ! broad_colliding_pairs.empty() ){
			//   ;
			rwObject *p1 = broad_colliding_pairs.front();
			broad_colliding_pairs.pop();
			rwObject *p2 = broad_colliding_pairs.front();
			broad_colliding_pairs.pop();
			assert(p1 && p2);
			//dodaj przesuniete punkty do bazowych punktów

			


			//exact phase GJK
			bool boom = gjk.BodiesIntersect( p1,p2, timestep, isCCD() );
			if(boom){
				/*
				buff= -gjk.getPenetrationDepth();
				//dla obiektow nieruchomych ignoruj i ustaw pelna penetracje przeciwnemu obiektowi
				if(p1->isStatic() && ( ! p2->isStatic()))
					p2->setCollidingDistance(buff);
				else if(p2->isStatic()&& ( ! p1->isStatic()))
					p1->setCollidingDistance(buff );
				else
				{	//w innym wypadku po polowie wektora penetracji dwom obiektom
					int test  = buff.length2();
					if(buff.length2()!=0){
						p1->setCollidingDistance(buff/2 );
						p2->setCollidingDistance(buff/2 );
					}
				}
				*/
				p1->setCollidingFlag(true);
				p2->setCollidingFlag(true);
			}
		}

		return true;
	}


	/**
	 oznaczenie wszystkich obiektow do testów narrowphase
	 **/
	virtual bool skipBroad(){
		if(this->Objects.empty())
			return false;

		setFlagsToZero();
		
		ObjectsList::iterator l1 = Objects.begin();
		ObjectsList::iterator l2 = l1;
		l2++;
		while(l2 != Objects.end() ){
			while(l2 != Objects.end() ){
						broad_colliding_pairs.push(*l1);		//wstaw do kolejki do narrow
						broad_colliding_pairs.push(*l2);
						(*l1)->getAABB().setCollidingFlag(true);
						(*l2)->getAABB().setCollidingFlag(true);
				l2++;
			}
			l1++;
			l2=l1;
			l2++;
		}
		return true;
	}
};




#endif