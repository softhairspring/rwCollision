/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
* Copyright (c) 2007-2016 Robert Widziszewski
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef RENDER_H
#define RENDER_H


#include "rwCollision.h"
#include <vector>
#include <map>

//#include "BasicTerrainGLListsbyType.h"
using namespace std;

struct b2AABB;

//typedef std::map <TerrainType, GLuint> TerrainMap;
//typedef std::map <ActorType, GLuint> ActorMap;
//typedef std::map <ItemType, GLuint> ItemMap;
//typedef std::map <BulletType, GLuint> BulletMap;


/**
* Klasa renderuj¹ca podstawowe obiekty
* zaklada sie ze pozycja bezwzgledna jest ustawiana na wyzszej warstwie tak samo jak k¹t obrotu
**/
class Render
{

	

public:

	//TerrainMap terrainMap;
	//ActorMap	actorMap;
	//ItemMap	itemMap;
	//BulletMap bulletMap;


	bool loadBasicGLLists();
	//void DrawBullets(std::vector<Bullet*> &v);

	///rysuje poligon (grupe lini jedna po drugiej)
	void DrawPolygon(rwVect3&  pos, vector<rwVect3>* vertices);

	void DrawBox(rwVect3&  pos, rwVect3& halfsize);

	void DrawTriangle(rwVect3&pos, rwVect3& a, rwVect3& b, rwVect3& c );
	void DrawTriangle(             rwVect3& a, rwVect3& b, rwVect3& c );

	void DrawTriangleLines(rwVect3&pos, rwVect3& a, rwVect3& b, rwVect3& c );
	/*
	

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount);

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawXForm(const b2XForm& xf);

	void DrawBulletCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	
	*/
};
/*
void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

//void DrawString(int x, int y, const char* string, ...);
void DrawAABB(b2AABB* aabb, const b2Color& color);
*/
#endif
