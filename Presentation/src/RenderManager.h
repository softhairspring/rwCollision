

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include <vector>
#include <list>
#include <map>

#include "gl\gl.h"	
#include "gl\glu.h"


#include "Render.h"
#include "rwCollision.h"
#include "ActorManager.h"
#include "HUDc.h"
#define PI 3.14159265
#define DEG2RAD 0.0174532925
#define NINEDEG2RAD 1.570796325
#define RAD     57.295779513082320876798154814105


typedef list<rwObject*> ObjectsList;

/**
Klasa zarz¹dzaj¹ca wyœwietlaniem grafiki OpenGL
**/
class RenderManager {


	Render R;
	HUDc HUD;

	int cursortexid;
	float cursorhsize;
	char *ActionInfo;

public:
	RenderManager(){

		//x
		cursortexid=0; 
		cursorhsize = 5;
	}

	void init(int screenwidth , int screenheight ){
		HUD.updateScreenSize(screenwidth,screenheight);
	}
	/*
	void renderObjects( ObjectsList & objects, bool drawBB ){
		GLuint glui=-1;
		rwObject * b=NULL;
		if( objects.empty() ) 
			return;
		unsigned int size = objects.size();

		
			

			ObjectsList::iterator  itb = objects.begin();
			while(itb != objects.end())
			{
					R.DrawPolygon( (*(itb))->getPosition(), (*(itb))->getConvexHull() );
					itb++;
			}
		

	// AABB
		if(drawBB){
				//glEnable(GL_BLEND);
			glColor3f(1.0f,1.0f,1.0f);
			ObjectsList::iterator it = objects.begin();
			while(it != objects.end())
			{
					R.DrawBox( (*(it))->getPosition(), (*(it))->getAABB().r );
					it++;
			}
		}
	}
	*/
	///wyswietla wszystkich aktorow(figury)
	void renderActors( ActorsList & actors, bool drawBB ){
		GLuint glui=-1;
		rwObject * b=NULL;
		Actor *a=NULL;
		if( actors.empty() ) 
			return;
		unsigned int size = actors.size();		

	// siatki
		ActorsList::iterator  itb = actors.begin();
		while(itb != actors.end())
			{	
					a=*(itb);
					renderMesh( a->getPosition(),
						a->getCollisionShape()->getConvexHull(),
						a->getMesh() ,
						a->getColor());
					itb++;
			}
		

	// AABB
		if(drawBB){
				//glEnable(GL_BLEND);
			glColor3f(1.0f,1.0f,1.0f);
			ActorsList::iterator it = actors.begin();
			while(it != actors.end())
			{
					R.DrawBox( (*(it))->getPosition(), (*(it))->getCollisionShape()->getAABB().r );
					it++;
			}
		}

	
	
	}

	//
	void renderHUD(int fps, int simfps, int count){
		//HUD
		HUD.drawHUD(fps,simfps,count,ActionInfo);
	}
	///renderuje siatke obiektu
	///(sciany)
	void renderMesh(rwVect3 &pos, vector<rwVect3> *pts, rwMesh	*mesh, rwVect3& color){
		assert(pts);assert(mesh);
		int i=0;
		//rwVect3 color(0.0f,0.6f,1.0f );

		
		rwFace f = mesh->resetIterator();

		glEnable(GL_BLEND);
			//glBlendFunc (GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc (GL_SRC_ALPHA, GL_ONE);


		while( ! mesh->isEnd()  ){

			
			glColor4f(  color[0],color[1],color[2],0.5f);
			//
			R.DrawTriangle		(pos, (*pts)[f.a],(*pts)[f.b],(*pts)[f.c] );
			
			glColor4f( color[0],color[1],color[2],1.0f);

			R.DrawTriangleLines (pos, (*pts)[f.a],(*pts)[f.b],(*pts)[f.c] );

			f = mesh->getNext();
			
		}
		glDisable(GL_BLEND);
	}

	void renderZeroPoint(){
		//glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);
		glBegin(GL_LINES);
		glVertex3f(-0.5f,0.0f,0.0f);
		glVertex3f( 0.5f,0.0f,0.0f);
		glVertex3f(0.0f,-0.5f,0.0f);
		glVertex3f(0.0f, 0.50f,0.0f);
		glVertex3f(0.0f,0.0f,-0.5f);
		glVertex3f(0.0f,0.0f, 0.5f);
		glEnd();
		//glPopMatrix();
	}

	//rysuje simplexy i wektory kierunku dla GJK
	void renderCollisionInfo(){
		glLineWidth(1.0f);

		float inc_color=0.0f;

		glColor3f(inc_color,0.7f,0.3f);
		renderVector(rwDebugDataColletor::CI.dir);
		
		int s= rwDebugDataColletor::CI.simplexarray.size();
		
		//simplex'y
		while(s>0){
			rwVect3 color(0.2f,inc_color,0.9f);
			//simplex
			int vects=0;
			int maxvects = rwDebugDataColletor::CI.simplexarray[s-1].size();
			if( maxvects == 4)
				drawSimplex(rwVect3(0.0f,0.0f,0.0f),rwDebugDataColletor::CI.simplexarray[s-1],color);

			//while(vects < maxvects){
			//	renderVector(rwDebugDataColletor::CI.simplexarray[s-1][vects]);
			//	vects++;
			//	inc_color+=0.08f;
			//}
			s--;
		}
		//BBox'y
		s= rwDebugDataColletor::CI.bbarray.size();
		while(s>0){
			R.DrawBox( rwDebugDataColletor::CI.bbarray[s-1].c, 
				rwDebugDataColletor::CI.bbarray[s-1].r );

			s--;
		}

	}

	void drawSimplex(rwVect3 &pos, Simplex &s, rwVect3 &color){
		glEnable(GL_BLEND);
		glBlendFunc (GL_ONE, GL_ONE);
		glColor4f( color[0]/3,color[1]/3,color[2]/3,0.1f);

		glPushMatrix();
		glTranslatef(pos[0],pos[1],pos[2]);
		R.DrawTriangle(s[0],s[1],s[2]);
		R.DrawTriangle(s[0],s[1],s[3]);
		R.DrawTriangle(s[1],s[2],s[3]);
		R.DrawTriangle(s[0],s[2],s[3]);

		glPopMatrix();

		glDisable(GL_BLEND);
	}
	void renderVector(rwVect3 &start, rwVect3 &v){
	}
	void renderVector(rwVect3 &v){
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f( v[0],v[1],v[2]);
		glEnd();
		glPopMatrix();
	}
	void renderDebug(){
	}

	void setACtionInfo( char *fmt){
		ActionInfo = fmt;
	}
	/*
	void renderTestpoint(b2Vec2 &point,b2Color& colorp){
		R.DrawCircle( point, 1.0f , colorp);
	}
	void renderTestpoint(const b2Vec2 &point,b2Color& colorp){
		R.DrawCircle( point, 0.3f , colorp);
	}
	void renderTestline(b2Vec2 s,b2Vec2 e){
		R.DrawSegment(s,e,b2Color(0.5f,0.2f,0.2f) );
	}
*/
	Render getRenderer(){
		return R;
	}

};





#endif
