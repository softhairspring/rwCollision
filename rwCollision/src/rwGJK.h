#ifndef GJK_H
#define GJK_H

#include <list>
#include <vector>
#include "rwGlobals.h"
#include "rwObjects.h"
#include "rwMath.h"
#include "rwPrimitiveIntersection.h"
#include "rwMinkowskiSum.h"
#include "rwDebugDataCollector.h"
#include "rwPhysicsSolver.h"
using namespace std;



//typedef vector<rwVect3> Simplex;

//nie u¿ywane
class rwGJKSimplexPoint{
	rwVect3 P;//punkt minkowsksiego
	rwVect3 A;//punkty A i B ktore tworz¹ P
	rwVect3 B;
} ;


//nie u¿ywane
class todo_rwGJKSimplex : public vector<rwGJKSimplexPoint> {
public:

	rwVect3 direction;
};
    ///tattatata


//nie u¿ywane
struct rwCollisionResult{
	Simplex s;
};







/**
implementacja algorytmu GJK
**/
class rwGJK {
private:
		static const int MaxIterations = 20;
		static const int MaxRecursion  = 10;

		rwPhysicsSolver * PS;

		vector<rwVect3> shape1;
		vector<rwVect3> shape2;
		///simplex
		Simplex simplex ;
		///kierunek szukania
		rwVect3 direction;
		///zmienne pomocnicze
		rwVect3 ipoint;
		
		rwVect3 D;
		rwVect3 C;
        rwVect3 B;
		rwVect3 A;

		rwVect3 AO;
        rwVect3 AB;
        rwVect3 AC;
        rwVect3 AD;


/// okresla nowy kierunek szukania i uaktualnia simplex
bool UpdateSimplexAndDirection()
        {
			vector<rwVect3>::iterator it;
			int s_size = simplex.size();
            //linia
            if ( s_size == 2)
            {
				it = simplex.begin();
                B = *(it); it++;
				A = *(it);
				
                rwVect3 AB = B - A;
                rwVect3 AO = -A;

				//
                //if (AB.dot(AO) > 0)
                //{
                //direction = AB.cross(AO).cross(AB);
				//
				direction = -(AB+AO);
				//
                //}
                //else
                //{
                //    direction = AO;
                //}
            }
            //trójkat
            else if ( s_size == 3)
            {
				it = simplex.begin();
				C = *(it); it++;
                B = *(it); it++;
				A = *(it);
                
                
                AO = -A;

                AB = B - A;
                AC = C - A;
                rwVect3 ABC = AB.cross(AC);
				//
                if (ABC.cross(AC).dot(AO) > 0)
                {
                    if (AC.dot(AO) > 0)
                    {
						simplex.erase(simplex.begin()+1);
                        direction = AC.cross(AO).cross(AC);
                    }
                    else
                        if (AB.dot(AO) > 0)
                        {
							simplex.erase(simplex.begin());
                            direction = AB.cross(AO).cross(AB);
                        }
                        else
                        {
							simplex.erase(simplex.begin());
							simplex.erase(simplex.begin());
                            direction = AO;
                        }
                }
                else
                {	//
                    if (AB.cross(ABC).dot(AO) > 0)
                    {
                        if (AB.dot(AO) > 0)
                        {
							simplex.erase(simplex.begin());
                            direction = AB.cross(AO).cross(AB);
                        }
                        else
                        {
							simplex.erase(simplex.begin());
							simplex.erase(simplex.begin());
                            direction = AO;
                        }
                    }
                    else
                    {	//
                        if (ABC.dot(AO) > 0)
                        {
                            direction = ABC;
                        }
                        else
                        {
                            simplex.clear();
                            simplex.push_back(B);
                            simplex.push_back(C);
                            simplex.push_back(A);

                            direction = -ABC;
                        }
                    }
                }
            }
            //tetrahedron
            else if (s_size == 4)
            {
				it = simplex.begin();
				
				D = *(it);it++;
                C = *(it);it++;
				B = *(it);it++;
                A = *(it);

				AO =   - *(it);
                AB = B - *(it);
                AC = C - *(it);
                AD = D - *(it);
				
                rwVect3 ABC = AB.cross(AC);
                rwVect3 ACD = AC.cross(AD);
                rwVect3 ADB = AD.cross(AB);
                //test stron plaszczyzn
				int BOnACD = rwMath::signi(ACD.dot(AB));
				int COnADB = rwMath::signi(ADB.dot(AC));
				int DOnABC = rwMath::signi(ABC.dot(AD));

                //sprawdzamy czy zero(punkt) jest na tej samej stronie kazdej z plaszczyzn
				int ABsameAsOrigin = rwMath::signi(ACD.dot(AO)) == rwMath::signi(BOnACD);
				int ACsameAsOrigin = rwMath::signi(ADB.dot(AO)) == rwMath::signi(COnADB);
				int ADsameAsOrigin = rwMath::signi(ABC.dot(AO)) == rwMath::signi(DOnABC);

				//jest w tetrah.
				if (ABsameAsOrigin && ACsameAsOrigin && ADsameAsOrigin)
                {
                    return true;
                }   
				//w przypadku zera poza jedna z plaszczyzn
				//usun punkt i sprubuj ponownie
				else if (!ABsameAsOrigin)
                {
					//B jest najdalszym punktem - usun go, ustaw kierunek na B i wroc do trojkata
                    simplex.erase(simplex.begin()+2);
                    direction = ACD * (float)-BOnACD;                   
                }

                else if (!ACsameAsOrigin)
                {
                    //C jest najdalszym punktem - usun go, ustaw kierunek na C
                    simplex.erase(simplex.begin()+1);
                    direction = ADB * (float)-COnADB;
                }

                else //if (!ADsameAsOrigin)
                {
                    simplex.erase(simplex.begin());
                    direction = ABC * (float)-DOnABC;
                }

                //powrót do trójkata
                return UpdateSimplexAndDirection();
              
            }

			//brak kolizji w tej iteracji
            return false;
        }


		

//
public:
/**
		* Oblicza czy wystepuje kolizja dla 
		* dwoch zbiorow punktow tworzacych figure geometryczna
**/
bool BodiesIntersect(rwObject *o1, rwObject*o2, float timestep, bool CCD)
        {
			assert(o1);
			assert(o2);
			shape1 = o1->getAbsoluteConvexHull();
			shape2 = o2->getAbsoluteConvexHull();
			//jesli uzywamy continuous coll. detection to dodaj nowe punkty(rozci¹ganie objêtoœci)
			if ( CCD ){
				float t1= o1->getVelocityToSizeRatio( timestep );
				float t2 = o2->getVelocityToSizeRatio(timestep ) ;

				if( o1->getVelocityToSizeRatio(timestep) >minimum_sweep_for_extrude*timestep ){
					rwVect3 pos1 = PS->getNewPosition(o1->getPosition(),o1->getLinearVelocity(),timestep);
					o1->addAbsoluteConvexHullAtPosition( &shape1, pos1 );
				}
				if( o2->getVelocityToSizeRatio(timestep) >minimum_sweep_for_extrude*timestep ){
					rwVect3 pos2 = PS->getNewPosition(o2->getPosition(),o2->getLinearVelocity(),timestep);
					o2->addAbsoluteConvexHullAtPosition( &shape2, pos2 );
				}
			}


			//closest points

			//


			simplex.clear();
            //bierzemy roznice dla dwoch dowolnych punktow dwoch figur
			direction = shape1.front() - shape2.front();
			rwVect3 S = rwMinkowskiSum::MaxPointAlongDir(shape1, shape2, direction);
            direction = -S;
			simplex.push_back(S);
			
			//glowna petla GJK
            for (int i = 0; i < MaxIterations; i++)
            {
				//assert ( i<15 ) ;
				rwVect3 A = rwMinkowskiSum::MaxPointAlongDir(shape1, shape2, direction);
				if (A.dot( direction) < 0)
                {
					//TODO: SZUKANIE NAJBLIZSZEGO PUNKTU MIMO BRAKU KOLIZJI
                    return false;
                }

                simplex.push_back(A);

				///przekaz informacje do wyswietlenia
#ifdef DEBUG
				updateDebugInfo();
#endif
                if (UpdateSimplexAndDirection())
                {

					//temporal coherence
					//o1->setGJKLastData( simplex , o2);

                    return true; //nastapila kolizja
                }
				
            }

            return false;
        }

void updateDebugInfo(){
#ifdef DEBUG
	rwDebugDataColletor::CI.dir = direction;
	rwDebugDataColletor::CI.simplexarray.push_back( simplex );
#endif
}


//zwraca glebokosc penetracji // wartosc poprawna tylko w wypadku kolizji
//nie u¿ywane
rwVect3 getPenetrationDepth(){
	rwVect3 out;
	int s = simplex.size()-1;
	out = simplex[s];
	s--;
	while(s>0){
		if(simplex[s].length2() < out.length2())
			out = simplex[s];
		s--;
	}
	return out;
}
};




#endif














