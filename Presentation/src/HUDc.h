/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef HUD_H
#define HUD_H

#include "gl\gl.h"			// Header File For The OpenGL32 Library
#include "gl\glu.h"			// Header File For The GLu32 Library


/***

Head Up Display used to show basic info like fps, or some other variables
*/


// //
extern GLuint	base;				// Base Display List For The Font Set
//GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
//GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring






GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}



/**
Klasa odpowiedzialna za wyœwietlanie elementów 2D ortogonalnie takich jak tekst
**/
class HUDc{

	int w;
	int h;
	int topborderheight;
	int lineheight;
public:	

	void step(){
	}


	HUDc(){
		topborderheight=25;
		lineheight = 17;
		w=640;
		h=480;
	}
	~HUDc(){
	}

	void updateScreenSize(int width,int height){
		w=width;
		h=height;
	}
	void drawHUD(float fps, float sps, int count, char* actioninfo){
		glPushMatrix();
		//glTranslatef(0,h-topborderheight,0);
		glTranslatef(0,80,0);
		glRasterPos2f(1,-lineheight);
		glColor3f( 0.0, 0.0, 0.0 );
		glPrint("fps: %f",fps);

		glTranslatef(0,-lineheight,0);
		glRasterPos2f(1,-lineheight);
		glPrint("sps: %f",sps);

		glTranslatef(0,-lineheight,0);
		glRasterPos2f(1,-lineheight);
		glPrint("objects: %d",count);

		glTranslatef(0,-lineheight,0);
		glRasterPos2f(1,-lineheight);
		glColor3f( 0.1, 0.1, 1 );
		glPrint(actioninfo);


		glPopMatrix();
	}


	void drawDebug(int milisec, int ridetime, int sec, int testy, double acceleration){

				glPushMatrix();
				glTranslatef(0,0,1);
				glColor3f( 1, 1, 1 );
				glRasterPos2f(1,1);
				glPrint("m-sec: %d \tFPS: %d",milisec,ridetime);

				//glTranslatef(0,0,1);
				glColor3f( 1, 1, 1 );
				//glRasterPos2f(1,1);
				glPrint("\tm-X %d mY%d accel:%f",sec, testy, acceleration);
				glPopMatrix();
	}

	
};





#endif
