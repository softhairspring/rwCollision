/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef ACTOR_H
#define ACTOR_H
#include "rwCollision.h"

class ActorManager;

/**
3d mesh entity in scene
**/
class Actor {

	friend ActorManager;
	rwVect3 Color;


protected:
	rwObject	*object;
	rwMesh		*mesh;

public:

	Actor(){
		object = NULL;
		mesh = NULL;
		Color = rwVect3(0.1f, 0.1f, 0.4f);
	}
	~Actor(){
		if (object)
			;
		if (mesh)
			delete(mesh);
	}

	bool init(rwObject *obj, rwMesh *m){
		if (obj == NULL || m == NULL)
			return NULL;
		object = obj;
		mesh = m;
		return true;
	}

	void applyImpulse(rwVect3 force){
		object->applyImpulse(force);
	}

	// akcesory
	rwObject	*getCollisionShape()	{ return object; }
	rwMesh		*getMesh()				{ return mesh; }
	rwVect3		getPosition()			{ return object->getPosition(); }
	void setColor(rwVect3 color){Color=color;}
	rwVect3 getColor(){return Color;}

	//
	void setStatic(bool isstatic) { object->setStaticFlag(isstatic) ; }
};


#endif