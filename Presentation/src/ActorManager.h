/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H
#include <list>

#include "Actor.h"

#include "rwCollision.h"
#include "Mersenne/MersenneTwister.h"

using namespace std;

typedef list<Actor*> ActorsList;

/**
Actor factory, alsokeeps track of all actors
**/
class ActorManager {

	rwScene *scene;

	ActorsList actors;

public:

	ActorManager(rwScene *Scene){
	scene = Scene;
	}
	~ActorManager(){

	}


	/**
	*
	**/
	Actor * getNewActor(rwVect3 &position, vector<rwVect3> *pts, rwMesh	*mesh) {
	if(pts==NULL || mesh==NULL)
		return NULL;
	rwPolyhedron *shape = new rwPolyhedron( position );
	shape->init( pts );
	shape->setMesh( mesh );		//redundantne ale uzywane przez basic coll solv.
	Actor *actor = new Actor();

	if( ! actor->init( shape , mesh) )
		return NULL;

	assert(scene);
	scene->addObject( *shape );
	actors.push_back( actor );

	return actor;
	}


	/**
	*
	**/
	Actor * getNewActor(rwVect3 &position, vector<rwVect3> *pts, rwMesh	*mesh, VertexMap *vm) {
	if(pts==NULL || mesh==NULL || vm==NULL)
		return NULL;
	rwPolyhedron *shape = new rwPolyhedron( position );
	shape->init( pts);
	shape->setMesh( mesh );		//redundantne ale uzywane przez basic coll solv.

	Actor *actor = new Actor();

	if( ! actor->init( shape , mesh) )
		return NULL;

	assert(scene);
	scene->addObject( *shape );
	actors.push_back( actor );

	return actor;
	}


	void clearAll(){
		scene->clear();
		actors.clear( );

	}

	ActorsList& getActorsRef(){
		return actors;
	}

	int getActorsCount(){
		return actors.size();
	}

	void randomizeVelocities( int avspeed){
		ActorsList::iterator it = actors.begin();
		MTRand rand;

		while(it!=actors.end() ){
			if((*it)->getCollisionShape()->isStatic()){
				it++;
				continue;
			}
			(*it)->
				getCollisionShape()->
					setLinearVelocity( 
						rwVect3(rand.randInt(avspeed)-avspeed*0.5,
								rand.randInt(avspeed)-avspeed*0.5,
								rand.randInt(avspeed)-avspeed*0.5
						)
					);
			it++;
		}
	}
};


#endif
