#ifndef RWDEBUGDATACOLLECTOR_H
#define RWDEBUGDATACOLLECTOR_H

#include <vector>
#include "rwVector.h"
#include "rwAABB.h"

using namespace std;

typedef vector<rwVect3> Simplex;

struct CollisionInfo {
	vector<Simplex> simplexarray;
	vector<rwAABB> bbarray;
	rwVect3 dir;
};

/**
Zbiera dane o kolizjach GJK, aby je póŸniej wyietliæ na ekranie w wersji Debug
operacje bardzo kosztowne obliczniowo
u¿ywane przy œledzeniu generowania simplexów
**/
class rwDebugDataColletor {

	
public:
	static CollisionInfo CI;

	static void updateObjects(){}
	static void updateCollisionInfo(Simplex &s, rwVect3 d ){
	CI.simplexarray.push_back( s );
	CI.dir = d;

	
	}

	//zeruje zmienne
	static void clearCollisionInfo(){
		CI.simplexarray.clear();
		CI.bbarray.clear();
		CI.dir = rwVect3(0.0f,0.0f,0.0f);
	}
	CollisionInfo& getCollisionInfo(){ return CI;}
	/// ; O

};

#endif