#ifndef MAIN_PRESENTATION
#define MAIN_PRESENTATION

#include <windows.h>		
#include "gl.h"
#include "glu.h"
#include <stdio.h>	
#include<cstdlib>
//#include <stdlib.h>
#include <time.h>


#include "rwCollision.h"
#include "rwCommon.h"
#include "RenderManager.h"
#include "ObjectLoader.h"
#include "ActorManager.h"
#include "Mersenne/MersenneTwister.h"
using namespace std;

///zmienna PROFILER w³¹cza zapisywanie danych do pliku .db o prêdkosci wykonywania wybranych fragmentów kodu
//#define PROFILER

#define _GAMESTATE GameStateControler::getState()
//dane windows
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
	//static TCHAR lpszAppName[] = TEXT( "API Windows" );

bool	keys[256];			// Array Used For The Keyboard Routine
bool	mousekeys[3];
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	blend;				// Blending ON/OFF



//dane openGL
const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;


GLuint	filter;				// Which Filter To Use
GLuint	texture[3];			// Storage For 3 Textures

int		GLheight=768;//600;
int		GLwidth =1024;//800;
RECT screensize;

float cameraposition = -25.0f;
int fps=0;		//mierzy klatki/sek wyswietlane
int simfps=0;	//do mierzenia predkosci symulacji
bool RANDOMIZEVELOCITYFLAG=false; //flaga wywolania zmiany predkosci
bool ISRENDERACTORS=true; //wyswietlanie obiektow
//dane rwCommon
rwFrameTimer FT;
//dane rwCollision:
rwScene scene;
rwGJK	GJKtest;

//
RenderManager RM;
ObjectLoader  OL;
ActorManager  AM( &scene );


vector<Actor> actors;
rwObject *actor=NULL;	//obiekt ktorego bedziemy poruszac


//temp
float poschange = 0.4f; //do zmiany polozenia temp

int MAX_ierations=0;	//do automatycznego zamykania aplikacji po testach
int iteriationsaccu=0;
int testargument = 1;
bool addBorders();
bool test1(int dsquare);
bool test2box();
bool test2boxnc();
bool testnr1(int dsquare);
bool testnr2(int dsquare);
bool testnr4(int dsquare);
///prosty struct uzywany do zatrzymywania klatek i zwalniania animacji
struct FrameLimiter {

	bool LOCKSIMULATION;	//zatrzymaj symulacje
	bool SKIPONE;			//przeskocz jedna klatke
	bool ISSIMULATIONLOCKED;
	int stopframes;			//liczba klatek do przeskoczenia

	FrameLimiter(){
		LOCKSIMULATION=false;
		SKIPONE=false;
		ISSIMULATIONLOCKED=false;
		stopframes=0;
	}

}	FL;


///prosty kontroler
///poruszajacy sie po sferze wokol O
struct CameraControl {
	//rwVect3 Position;
	float xrot;
	float yrot;
float DampingRadius;
float DampingX;
float DampingY;
public:
	float DistRadius;
	
	//float zrot;

	CameraControl(){
		//Position.setX( 0.0f );
		//Position.setY( 0.0f );
		//Position.setZ( -25.0f );
		xrot=0;
		yrot=0;
		DistRadius= -25.0f;
		DampingRadius=0.0f;
		DampingX=0.0f;
		DampingY=0.0f;
	}


	void distance(bool forworback){
		if(forworback)
			DampingRadius=DampingRadius-(0.3f);
		else
			DampingRadius=DampingRadius+(0.3f);
		
	}
	void rotateHor(bool leftorright){
		if(leftorright)
			DampingY+=2;
		else
			DampingY-=2;
	}
	void rotateVert(bool leftorright){
		if(leftorright)
			DampingX+=(1.0f+DampingX);
		else
			DampingX-=(1.0f+DampingX);
	}

	void updateByDamping(){
		DistRadius = DistRadius + DampingRadius;
		DampingRadius*=0.5f;

		yrot = yrot + DampingY;
		DampingY*=0.5f;
		xrot = xrot + DampingX;
		DampingX*=0.5f;
	}
} camera;

void readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}



// 
//tworzenie czcionki dla OpenGL w formie bitmapy
//
GLuint	base;				// Base Display List For The Font Set
GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

GLvoid BuildFont(GLvoid)								
{
	HFONT	font;										
	HFONT	oldfont;									

	base = glGenLists(96);								//96 znaków

	font = CreateFont(	-20,							// Height
						0,								// Width
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           
	wglUseFontBitmaps(hDC, 32, 96, base);				// budowa 96 znaków wielkosci 32
	SelectObject(hDC, oldfont);							
	DeleteObject(font);									
}

//zniszczenie elementow czcionki
GLvoid KillFont(GLvoid)									
{
	glDeleteLists(base, 96);							
}

// 
//zmienia rozmiar okna
//
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,400.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									

}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.05f, 0.15f, 0.1f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LEQUAL);//GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	BuildFont();
	///

	return TRUE;										// Initialization Went OK
}





//
//					DRAW
//
//g³ówna funkcja rysuj¹ca zawartoœæ okna
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glMatrixMode(GL_MODELVIEW);
				

	

	camera.updateByDamping();
	//kamera:
	glTranslatef(0.0f,0.0f,camera.DistRadius);
	glRotatef(camera.xrot,1.0f,0.0f,0.0f);
	glRotatef(camera.yrot,0.0f,1.0f,0.0f);
	
	

	glColor3b(1,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-50.0f,0.0f,0.0f);
	glVertex3f(50.0f,0.0f,0.0f);
	glEnd();


	RM.renderZeroPoint();
	if(ISRENDERACTORS)
		RM.renderActors( AM.getActorsRef(), true );
#ifdef DEBUG
	//RM.renderCollisionInfo();
#endif
	
	/// 2D
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, GLwidth, 0, GLheight,-1,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
	///2D in
			RM.renderHUD(fps, simfps, AM.getActorsCount());
	///2D out
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);

	//profiler
#ifdef PROFILER
	glEnable(GL_TEXTURE_2D);
	//ProfileDraw();
	glDisable( GL_TEXTURE_2D );
#endif
	return TRUE;										
}

//
//niszczy okno
//
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Uwolnienie DC i RC nieudane!","BLAD ZAMKNIECIA",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Uwalnienie kontekstu renderuj¹cego nieudane!","BLAD ZAMKNIECIA",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Uwoknienie kontekstu urzadzenia nieudane.","BLAD ZAMKNIECIA",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"nie udalo sie uwolnic hWnd.","BLAD ZAMKNIECIA",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Nie udalo sie wyrejetrowac klasy OGL.","BLAD ZAMKNIECIA",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/**
fukcja tworzy okno z wykorzystaniem OpenGL
**/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Fullscreen Flag
	BOOL ShowCursorFlag=1;
	ShowCursor(ShowCursorFlag);			//cursor

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Nie udalo sie zarejestrowac klasy okna!","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"Rzadana rozdielczosc nie jest obslugiwana przez Twoja karte graficzna. Urzyc trybu okienkowego?","okno",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Teraz nast¹pi zamkniecie aplikacji..","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",						// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Blad tworzenia okna!","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		(BYTE)bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Inicjalizacja GL nieudana!","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}






/**
obsluga zdarzeñ systemowych
obs³uga wejœcia u¿ytkownika(klawiatura, mysz)
*/
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{

bool Lbttdown=false;

	switch (uMsg)									// Check For Windows Messages
	{
		case WM_LBUTTONDOWN: // k0
			{
				Lbttdown=true;
				return 0;
			}
		case WM_LBUTTONUP:{	//	k0
				Lbttdown=false;
				return 0;
			}

		
		case WM_RBUTTONDOWN:{	//	k1
			
			
			return 0;
		}
		case WM_RBUTTONUP:{		//	k1
			mousekeys[0]=false;
			
			return 0;
		}

		case WM_MOUSEMOVE : //	k4
		{


			if (wParam & MK_RBUTTON) //ruszanie myszy z przyciskiem
			{

			}
			return 0;
		}

		case WM_MBUTTONDOWN:	//		k2	srodkowy myszy
			{
				camera.distance(false);
				return 0;	
		}
		case WM_MBUTTONUP:		//		k2	srodkowy myszy
		{
			camera.distance(true);
				return 0;	
		}


		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			
			keys[wParam] = TRUE;					// If So, Mark It As TRUE

			
			//usun wszystkie obiekty
			if (keys['P'])
			{
				AM.clearAll();
			}
			//kamera:
			if (wParam == VK_LEFT)
			{
				camera.rotateHor(true);
			}
			if (wParam == VK_RIGHT)
			{
				camera.rotateHor(false);
			}
			if (wParam == VK_UP)
			{
				camera.rotateVert(true);
			}
			if (wParam == VK_DOWN)
			{
				camera.rotateVert(false);
			}

			if (keys['S'])
			{
				if(testargument<5)
					testargument++;
#ifndef DEBUG
				char c;
				itoa(testargument, &c, 10);
				RM.setACtionInfo(&c);
#endif
			}
			if (keys['X'])
			{
				if(testargument>1)
					testargument--;
#ifndef DEBUG
				char cc;
				itoa(testargument, &cc, 10);
				RM.setACtionInfo(&cc);
				//RM.setACtionInfo("zmniejszono argument");
#endif		
			}
			

			if (keys['A'])//oddalanie kamery
			{
				camera.distance(false);
			}
			if (keys['Z'])//przyblizanie kamery
			{
				camera.distance(true);
			}
			if (keys['B'])
			{
				scene.assignBasicCollisionDetector();
				RM.setACtionInfo("Podstawowy wykrywacz kolizji");
			}
			if (keys['G'])
			{
				scene.assignGJKCollisionDetector();
				RM.setACtionInfo("Wykrywacz kolizji GJK");
			}
			if (keys['C'])
			{
				if(scene.isContinuousCollisionDetectionOn())
					scene.setContinuousCollisionDetection(false);
				else
					scene.setContinuousCollisionDetection(true);

				if(scene.isContinuousCollisionDetectionOn())
					RM.setACtionInfo("CCD is ON");
				else
					RM.setACtionInfo("CCD is OFF");
			}
			if (keys['N'])
			{
				if(scene.isBroadPhase())
					scene.setBroadPhase(false);
				else
					scene.setBroadPhase(true);

				if(scene.isBroadPhase())
					RM.setACtionInfo("BroadPhase is ON");
				else
					RM.setACtionInfo("BroadPhase is OFF");
			}
			//aktor do testow;
			if (keys['1'])
			{
				testnr1(testargument);
			}
			if (keys['2'])
			{
				testnr2(testargument);
			}
			if (keys['3'])
			{
				;
			}
			if (keys['4'])
			{
				testnr4(testargument);
			}
			if (keys['5'])
			{
				testnr2(5);
			}

			if (keys['8'])
			{
				//test2box();
			}
			if (keys['0'])
			{
				addBorders();
			}
			if (keys['9'])
			{
				 test2boxnc();
			}
			if (keys['Y'])
			{
				if(actor)
				actor->applyImpulse(rwVect3(0.0f,300.f,0.0f) );
			}
			if (keys['R'])//losowanie predkosci i kierunku ruchu ze zwiekszeniem predkosci ruchu
			{
				RANDOMIZEVELOCITYFLAG=true;
				//MAX_ierations=650;
			}
			if (keys['T'])//wylaczenie programu po 650 iteracjach
			{
				MAX_ierations=650;
			}
			if (keys['D'])//losowanie predkosci i kierunku ruchu ze zwiekszeniem predkosci ruchu
			{
				if(ISRENDERACTORS)
					ISRENDERACTORS=false;
				else
					ISRENDERACTORS=true;
			}



			//inne
			if (keys['N'])//odblokowanie symulacji
			{
				FL.LOCKSIMULATION=false;
			}
			if (keys['M'])//klatkowanie symulacji
			{
				FL.LOCKSIMULATION=true;
				FL.SKIPONE=true;
			}
		
	
			
			///

			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
		
			return 0;								// Jump Back
		}

		case WM_SIZING:
		{
				return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//
//			WinMain
//
int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// tryb ekranu
	if (MessageBox(NULL,"Uruchomic w trybie pelnoekranowym?", "FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// tworzenie okna
	if (!CreateGLWindow("Prezentacja rwCollision",GLwidth,GLheight,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	//zmienne do kontroli czasu i synchronizacji klatek logiki (nie renderingu)
	double timestep= 1.0/60.0;	//60 symulacji na sekunde
	double lastframetime=1;		//czas calej ostatniej klatki wykonanej
	double framestarttime=0;	//do przechowania czasu na starcie klatki
	double currentframetime=0;	//do buff
	double frameaccumulator =0;	//do trzymania pozostalego czasu iteracji logiki
///
//
///
	



	//init
	scene.init();
	RM.init(GLwidth,GLheight);
	scene.setContinuousCollisionDetection(false);

	//assert(actor.init(rwFigures::ccd_cube_extr04,rwFigures::ccd_cube_extr04nr) );
	//assert(actor2.init(rwFigures::cube2sphere30_sc3_sd3,rwFigures::cube2sphere30_sc3_sd3nr) );

	//rwObject * o;
	//Actor    * a;


		

	

/// / / / 
//
/// / / /





int fpsaccumulator=0;
long seconds =0;
int simfpsaccumulator=0;

#ifdef PROFILER
	ProfileInit();
	StartTimeLoggingToFile();
#endif
	FT.start();
	FT.startSecondsCounting();


	//%%%%%%%%%%%%%%%%%%%%%
	//g³ówna pêtla programu
	while(!done)									//Main loop
	{

		if(FT.getSecondsCount()> seconds)	//do wyswietlania predkosci animacji
		{
			seconds = FT.getSecondsCount();
			fps = fpsaccumulator;
			simfps = simfpsaccumulator;
			fpsaccumulator=0;
			simfpsaccumulator=0;
		}
		
#ifdef PROFILER
		//ProfileBegin("all");
		//ProfileBegin( "GL");
#endif
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// obs³uga zdarzeñ
		{
			if (msg.message==WM_QUIT)				// wyjscie?
			{
				done=TRUE;			
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	//w tym miejscu jest wywo³anie rysowania
		{
				done=TRUE;							// quit
		}
		fpsaccumulator++;
		///
		SwapBuffers(hDC);

		//
		//		klatka logiki i fizyki
		//
		framestarttime = FT.getMiliSecondsCount();
		lastframetime = (framestarttime - currentframetime)*0.001;
		if(lastframetime > 0.02)
			lastframetime = 0.017;
		currentframetime = framestarttime;
		frameaccumulator += lastframetime;
		while( (frameaccumulator > timestep) )
		{
			frameaccumulator -= timestep;

			

#ifdef PROFILER
			ProfileBegin( "simulation");
#endif
			//czy nie zatrzymalismy symulacji?
			if( (FL.stopframes == 0) && ! FL.LOCKSIMULATION ){
#ifdef DEBUG
			rwDebugDataColletor::clearCollisionInfo();
#endif
				//
			if(RANDOMIZEVELOCITYFLAG){
				poschange=1200.0f;
				AM.randomizeVelocities( poschange );
				RM.setACtionInfo("zwiekszono predkosc");
				RANDOMIZEVELOCITYFLAG=false;
			}
				scene.step(timestep);
				simfpsaccumulator++;
				if(MAX_ierations!=0){
					iteriationsaccu++;
					if(iteriationsaccu>(MAX_ierations-1))
						done=true;
				}
			}
			else{
				if(FL.SKIPONE){
#ifdef DEBUG
			rwDebugDataColletor::clearCollisionInfo();
#endif
					//
					scene.step(timestep);
					FL.SKIPONE=false;
					simfpsaccumulator++;
					if(MAX_ierations!=0){
						iteriationsaccu++;
						if(iteriationsaccu>(MAX_ierations-1))
						done=true;
					}
				}else
				if(FL.stopframes > 0){
					FL.stopframes--;
					//FL.ISSIMULATIONLOCKED=true;
				}	
			}
#ifdef PROFILER
			ProfileEnd( "simulation");
#endif
		}
#ifdef PROFILER		
		//ProfileEnd("all");
		ProfileDumpOutputToBuffer();
#endif

	
	}//endwhile


	KillGLWindow();	


	
	return (msg.wParam);								
}

//fukcja dodaj¹ca œciany szeœcianu(pomieszczenie)
bool addBorders(){

	Actor    * a=NULL;
// dodaj obiekt podloza na dole
	string s1("data/ccd-box-enclosing-1.x");
	string s2("data/ccd-box-enclosing-2.x");
	string s3("data/ccd-box-enclosing-3.x");
	string s4("data/ccd-box-enclosing-4.x");
	string s5("data/ccd-box-enclosing-5.x");
	string s6("data/ccd-box-enclosing-6.x");
	string s;
	rwVect3 pos_zero;
	//rwVect3 pos_ground(0.0f,-20.0f,0.0f);
	for(int i=0;i<6;i++){
		switch(i){
			case 0: s = s1; break;
				case 1: s = s2; break;
					case 2: s = s3; break;
						case 3: s = s4; break;
							case 4: s = s5; break;
								case 5: s = s6; break;
		}
		switch(i){
			case 0: pos_zero = rwVect3(0.0f,0.0f,-136.2f); break;
			case 1: pos_zero = rwVect3(0.0f,0.0f, 136.2f); break;
			case 2: pos_zero = rwVect3(-136.2f,0.0f,0.0f); break;
			case 3: pos_zero = rwVect3(136.2f,0.0f,0.0f); break;
			case 4: pos_zero = rwVect3(0.0f,-136.2f,0.0f); break;
			case 5: pos_zero = rwVect3(0.0f,136.2f,0.0f); break;
		}
		ObjectFileData *ofd = OL.getMesh( s );
		assert(ofd);
		a = AM.getNewActor(pos_zero,ofd->points,ofd->mesh );
		assert( a );
		a->setColor(rwVect3(0.8f,0.6f,0.6f ));
		a->setStatic( true );
		delete(ofd);
	}
	return true;
}

///dsquare^3 obiekty
bool test1(int dsquare){
	MTRand rand;
	Actor *a=NULL;
	int q=dsquare;
	float poschange = 250/q;
	float rightborderpos = 250/2;
	string s("data/ico1-10.x");
	for(		int r=0;r<1;r++){
		for(	int c=0;c<q;c++){
			for(int d=0;d<q;d++)
			{
				a=NULL;
				ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

				a = AM.getNewActor(rwVect3(
					-rightborderpos+ (poschange*(r+1)) ,
					-rightborderpos+ (poschange*(c+1)) ,
					-rightborderpos+ (poschange*(d+1)) ),
					ofd->points,ofd->mesh );
				assert( a );
				if(a==NULL)
					return false;
				//a->getCollisionShape()->applyImpulse( rwVect3(0,0,0));
				delete(ofd);
			}
		}
	}
return true;
}


///test prostych obiektów w ilosci 25*x
bool testnr1(int dsquare){
	
	MTRand rand;
	Actor *a=NULL;
	int countall=0;
	int q=dsquare;
	if(q==0)
		q=1;
	float poschange = 250/10;
	float poschangeq=250/q;
	float rightborderpos = 250/2;
	string s("data/ico1-3.x");

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;


	for(		int r=0;r<q;r++){
		for(	int c=0;c<10;c++){
			for(int d=0;d<10;d++)
			{
				countall++;
				a=NULL;
				

				a = AM.getNewActor(rwVect3(
					-135+ (poschangeq*(r+1)) ,
					-135+ (poschangeq*(c+1)) ,
					-135+ (poschangeq*(d+1)) ),
					ofd->points,ofd->mesh );
				assert( a );
				if(a==NULL)
					return false;
				//a->getCollisionShape()->applyImpulse( rwVect3(0,0,0));
				
			}
		}
	}
	delete(ofd);
return true;
}

///test dla zlozonych obiektow, gdzie dsquare oznacza "subdywizje" siatki obiektu 50szt.
bool testnr2(int dsquare){
	
	FL.LOCKSIMULATION=true;

	MTRand rand;
	Actor *a=NULL;
	int q=dsquare;
	if(q==0)
		q=1;
	float poschange = 250/5;
	float poschangeq = 250/q;
	float rightborderpos = 245/2;
	string s= ("data/ico1-10.x");
	switch(q){
		case(1): s= ("data/ico1-10.x");break;
			case(2): s= ("data/ico2-10.x");break;
				case(3): s= ("data/ico3-10.x");break;
					case(4): s= ("data/ico4-10.x");break;
						case(5): s= ("data/ico5-10.x");break;
							case(6): s= ("data/ico6-10.x");break;
								case(7): s= ("data/ico7-10.x");break;
									case(8): s= ("data/ico8-10.x");break;
	}

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

	for(		int r=0;r<4;r++){
		for(	int c=0;c<5;c++){
			for(int d=0;d<5;d++)
			{
				a=NULL;
				

				a = AM.getNewActor(rwVect3(
					-rightborderpos+ (poschange*(r+1)) ,
					-rightborderpos+ (poschange*(c+1)) ,
					-rightborderpos+ (poschange*(d+1)) ),
					ofd->points,ofd->mesh );
				assert( a );
				if(a==NULL)
					return false;
				//a->getCollisionShape()->applyImpulse( rwVect3(0,0,0));
				
			}
		}
	}

	FL.LOCKSIMULATION=false;
	delete(ofd);

return true;
}


///test prostych obiektów o roznych predkosciach dsquare*10
bool testnr4(int dsquare){
	MTRand rand;
	Actor *a=NULL;
	int countall=0;
	int q=dsquare*20;
	if(q==0)
		q=1;
	float poschange = 250/5;
	float poschangeq=250/5;
	float rightborderpos = 250/2;
	string s("data/ico1-3.x");

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;


	for(		int r=0;r<4;r++){
		for(	int c=0;c<5;c++){
			for(int d=0;d<5;d++)
			{
				countall++;
				a=NULL;
				

				a = AM.getNewActor(rwVect3(
					-135+ (poschangeq*(r+1)) ,
					-135+ (poschangeq*(c+1)) ,
					-135+ (poschangeq*(d+1)) ),
					ofd->points,ofd->mesh );
				assert( a );
				int m1=1,m2=1,m3=1;
				rand.seed();
				if(rand.rand53() < 0.5)
					m1=-1;
				rand.seed();
				if(rand.rand53() < 0.5)
					m2=-1;
				rand.seed();
				if(rand.rand53() < 0.5)
					m3=-1;

				rwVect3 vel = rwVect3(
					rand.rand(q)+(q/2)*m1,
					rand.rand(q)+(q/2)*m2,
					rand.rand(q)+(q/2)*m3);
				a->getCollisionShape()->applyImpulse( vel);
				if(a==NULL)
					return false;
				//a->getCollisionShape()->applyImpulse( rwVect3(0,0,0));
				
			}
		}
	}
	delete(ofd);
return true;
}


bool test2box(){
	Actor *a=NULL;
	string s("data/box.x");

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

	a = AM.getNewActor(rwVect3(0,0,0),ofd->points,ofd->mesh );

	ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

	a = AM.getNewActor(rwVect3(0.5f,0.5f,0.5f),ofd->points,ofd->mesh );
	//a = AM.getNewActor(rwVect3(3.0f,3.0f,3.0f),ofd->points,ofd->mesh );
	

return true;
}


bool test2boxnc(){
	Actor *a=NULL;
	string s("data/box.x");

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

	a = AM.getNewActor(rwVect3(0,0,0),ofd->points,ofd->mesh );

	ofd = OL.getMesh( s );
				if(ofd==NULL)
					return false;

	//a = AM.getNewActor(rwVect3(0.5f,0.5f,0.5f),ofd->points,ofd->mesh );
	a = AM.getNewActor(rwVect3(3.0f,3.0f,3.0f),ofd->points,ofd->mesh );
	

return true;
}

//todo
void shootMesh(){
	/*
	Actor *a = NULL;
	string s("data/ico1-3.x");
	ObjectFileData *ofd = OL.getMesh(s);
	if (ofd == NULL)
		return;
	a = AM.getNewActor(rwVect3(0, 0, 0), ofd->points, ofd->mesh);
	*/
	//a->applyImpulse(camera.)

}
#endif