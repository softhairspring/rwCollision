#ifndef COLLISION_DETECTORBASIC_H
#define COLLISION_DETECTORBASIC_H

#include <queue>
#include "rwGlobals.h"
#include "rwObjects.h"
#include "rwGJK.h"
using namespace std;



/*
NIEDOKONCZONA nieuzywana
Klasa wykrywajaca kolizje miedzy obiektami metod¹ naiwn¹.
Klasa nie powinna modyfikowac obiektow bezposrednio - tylko przez inne klasy
**/
class rwCollisionDetectorBasic: public rwCollisionDetector {
	
	

	rwPhysicsSolver PS;


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
	 brak fazy wstepnej - oznaczenie wszystkich obiektow do sprawdzenia wystapienia kolizji
	 **/
	virtual bool solveBroad(){
		if(this->Objects.empty())
			return false;

		setFlagsToZero();
		
		ObjectsList::iterator l1 = Objects.begin();
		while(l1 != Objects.end() ){
					(*l1)->getAABB().setCollidingFlag(false);
					l1++;
		}
		return true;
	}
	/**
	wykrycie dokladnej kolizji obiektów które zosta³y wykryte w fazie broad
	jako koliduj¹ce
	**/
	virtual bool solveNarrow(){

		setFlagsToZero();
		vector<bool> counter;

		ObjectsList::iterator l1 = Objects.begin();
		ObjectsList::iterator l2 = Objects.begin();
		rwObject *o1 = (*l1);
		rwObject *o2 = (*l2);
		vector<rwVect3> shape1 =o1->getAbsoluteConvexHull();
		vector<rwVect3> shape2 =o2->getAbsoluteConvexHull();

		bool NOCOLLISION=false;
		rwMesh * m=NULL;
		rwFace f;
		rwVect3 a,b,c,p;

		int vs1=0;	//ilosc pkt obiektow
		int vs2=0;
		int fs1=0;	//ilosc scian obiektow
		int fs2=0;
		int vertid=0;
		int faceid=0;
		int k=0;

		rwVect3 Ls, Le;
		float u,v,w;

		while(l1 != Objects.end() ){
			o1= (*l1);
			l1++;
			l2=l1;
			l1--;
			shape1 =o1->getAbsoluteConvexHull();
			while(l2 != Objects.end() ){
				o2=(*l2);
				shape2 =o2->getAbsoluteConvexHull();
				vs1 = shape1.size();
				vertid=0;
				counter.clear();
				//while l1 edge
				while(vertid < (vs1) ) {
					//while face l2
					faceid=0;
					fs2 = o2->getMesh()->size();
					m = o2->getMesh();
					f = m->resetIterator();
					a = shape2[f.a];
					b = shape2[f.b];
					c = shape2[f.c];

					rwVect3 n = (b-a).cross(c-a);
					while(faceid < fs2){
						p =  shape1[vertid];
						int scalar = n.dot( p-a );
						
						if(scalar > 0){
							counter.push_back(NOCOLLISION);//zlicza sciany za ktorymi jest wierzcholek
						}

//TODO:
						f = m->getNext();
						a = shape2[f.a];
						b = shape2[f.b];
						c = shape2[f.c];
						faceid++;
					}
					vertid++;
					if( counter.size() == fs2){//jest kolizja
						o1->setCollidingFlag(true);
						o2->setCollidingFlag(true);
						vertid=vs1;//wyjscie z petli wierzcholkow
					}
				}//

				
				//TODO:
				//while l2 edge
					//while face l2

					l2++;
			}
			l1++;
			
			
		}
		

		return true;
	}
};




#endif