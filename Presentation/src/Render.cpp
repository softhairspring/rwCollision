/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RENDER_CPP
#define RENDER_CPP

#include "Render.h"
#include <map>

#include <windows.h>
#include "gl\gl.h"			// Header File For The OpenGL32 Library
#include "gl\glu.h"			// Header File For The GLu32 Library

#include <cstdio>
#include <cstdarg>

#include "rwCollision.h"

using namespace std;

bool Render::loadBasicGLLists()
{
	return true;
}

void Render::DrawPolygon (rwVect3&  pos, vector<rwVect3>* vertices)
{
	assert(vertices);
	if(vertices==NULL)
		return;
	if(vertices->empty())
		return;
	vector<rwVect3>::iterator it = vertices->begin();
	rwVect3 v;

	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	glBegin(GL_LINE_LOOP);
	while(it != vertices->end())
        {
			v=*(it);
			glVertex3f(v[0],v[1],v[2] );
			it++;
		}
	glEnd();
	glPopMatrix();
	
}

void Render::DrawBox(rwVect3&  pos, rwVect3& halfsize){

	glEnable(GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	glColor4f( 0.4f,0.44f,0.4f,0.05f);

	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	glBegin(GL_LINE_LOOP);
	//front
	glVertex3f(-halfsize[0], -halfsize[1],halfsize[2]);
	glVertex3f(halfsize[0], -halfsize[1],halfsize[2]);
	glVertex3f(halfsize[0], halfsize[1],halfsize[2]);
	glVertex3f(-halfsize[0], halfsize[1],halfsize[2]);
	glVertex3f(-halfsize[0], -halfsize[1],halfsize[2]);
	//back
	glVertex3f(-halfsize[0], -halfsize[1],-halfsize[2]);
	glVertex3f(halfsize[0], -halfsize[1],-halfsize[2]);
	glVertex3f(halfsize[0], halfsize[1],-halfsize[2]);
	glVertex3f(-halfsize[0], halfsize[1],-halfsize[2]);
	glVertex3f(-halfsize[0], -halfsize[1],-halfsize[2]);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	/*
	glEnable(GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	glColor4f( 0.4f,0.44f,0.4f,0.05f);

	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	glBegin(GL_QUADS);
	//front z
	glVertex3f(-halfsize[0], -halfsize[1],halfsize[2]);
	glVertex3f(halfsize[0], -halfsize[1],halfsize[2]);
	glVertex3f(halfsize[0], halfsize[1],halfsize[2]);
	glVertex3f(-halfsize[0], halfsize[1],halfsize[2]);
	//back -z
	glVertex3f(-halfsize[0], -halfsize[1],-halfsize[2]);
	glVertex3f(halfsize[0], -halfsize[1],-halfsize[2]);
	glVertex3f(halfsize[0], halfsize[1],-halfsize[2]);
	glVertex3f(-halfsize[0], halfsize[1],-halfsize[2]);

	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	*/
}

void Render::DrawTriangle(rwVect3&pos, rwVect3& a, rwVect3& b, rwVect3& c ){
	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	DrawTriangle(a,b,c);
	glPopMatrix();
}
void Render::DrawTriangle(rwVect3& a, rwVect3& b, rwVect3& c ){
	glBegin(GL_TRIANGLES);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
	glEnd();
}

void Render::DrawTriangleLines(rwVect3&pos, rwVect3& a, rwVect3& b, rwVect3& c ){
	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	glBegin(GL_LINE_LOOP);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
	glEnd();
	glPopMatrix();
}

/*

void Render::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_BLEND);

	//glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();

}

void Render::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
}

void Render::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	///glEnable(GL_BLEND);
	///glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, 1.0f);
	
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	///glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(p.x, p.y);
	glEnd();
}

void Render::DrawBulletCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	//glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	//glDisable(GL_BLEND);

	theta = 0.0f;
	//glColor4f(color.r, color.g, color.b, 1.0f);

}

void Render::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void Render::DrawXForm(const b2XForm& xf)
{
	b2Vec2 p1 = xf.position, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();
}




void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
	glPointSize(1.0f);
}

void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	glEnd();
}*/



#endif