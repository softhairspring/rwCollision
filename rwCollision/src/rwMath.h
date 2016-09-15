/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RWMATH_H
#define RWMATH_H

#include <cmath>
#include <cstdlib>
#include <cfloat>

//operacje pomocnicze 
//g³ównie okreœlaj¹ce znak liczby
class rwMath{
public:
///zwraca znak		TODO:obsluga zera
static bool signb(int val)		{if(val>0) return true; return false;}
static bool signb(float val)	{if(val>0) return true; return false;}
static int signi(int val)		{if(val>0) return 1; return -1;}
static int signi(float val)		{if(val>0) return 1; return -1;}
///zwraca wartosc absolutn¹
static int absolute(int val)	{if(val<0) return val* -1; return val;}
static float absolute(float val){if(val<0) return val* -1; return val;}
///zwraca prawde jesli liczby maja ten sam znak
static bool signCompare(float a,float b) {if( (a>0)&&(b>0)||(a<0)&&(b<0) )return true; return false;}

};

#endif



