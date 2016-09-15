#ifndef RWPRIMITIVEINTERSECTION_H
#define RWPRIMITIVEINTERSECTION_H

//#include <vector>
#include "rwMath.h"
#include "rwVector.h"
//#include "rwAABB.h"

using namespace std;


/**
* Klasa z testami przeci�� prymityw�w jak linie tr�jk�ty kwadraty ko�a
* rozwijana w miare potrzeby
**/
class rwPrimitiveIntersection {

	static float denom;
	static rwVect3 line;
	static rwVect3 pa;
	static rwVect3 pb ;
	static rwVect3 pc;
	static rwVect3 m;
public:


	/**
	* sprawdza czy lini przecina tr�jk�t
	* zwraca u,v,w kt�re s� parametrami barycentrycznymi pozycji punktu przeci�cia
	**/
	static bool intersectLineTriangle(rwVect3 linestart,rwVect3 lineend, 
							rwVect3 tra,rwVect3 trb,rwVect3 trc, 
							float &u,float &v,float &w ){

		rwVect3 line = lineend - linestart;
		rwVect3 pa = tra - linestart;
		rwVect3 pb = trb - linestart;
		rwVect3 pc = trc - linestart;
		// sprawdzanie skalar�w punktu poczatkowego lini z 
		// ilocznem wektorowym trojkatow z pkt lini i dwoch pkt trojkata
		rwVect3 m = line.cross(pc);
		u = pb.dot( m); 
		v = -pa.dot( m); 
		if (!rwMath::signCompare(u, v)) return false;
		w = line.dotTriple( pb, pa);
		if (!rwMath::signCompare(u, w)) return false;

		//licz wartosci barycentryczne
		denom = 1.0f / (u + v + w);
		u *= denom;
		v *= denom;
		w *= denom;
		return true;
	}

};

#endif