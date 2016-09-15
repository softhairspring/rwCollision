/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef VECTOR_CPP
#define VECTOR_CPP
#include "rwVector.h"


//
// operatory
//
	rwVect3 operator+(const rwVect3& v1, const rwVect3& v2) {
		return rwVect3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
	}

	rwVect3	operator-(const rwVect3& v1, const rwVect3& v2){
		return rwVect3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
	}
	
	rwVect3 operator-(const rwVect3& v){
		return rwVect3(-v[0], -v[1], -v[2]);
	}
	rwVect3 operator*(const rwVect3& v, const float& s)
	{
		return rwVect3(v.pt[0] * s, v.pt[1] * s, v.pt[2] * s);
	}
	
	rwVect3	operator*(const float& s, const rwVect3& v)
	{ 
		return v * s; 
	}
	
	rwVect3	operator/(const rwVect3& v, const float& s)
	{
		assert(s != 0.0f);
		return v * (1.0f / s);
	}
	//rwVect3	operator/(const rwVect3& v, const rwVect3& z)
	//{
		//assert(s != 0.0f);
		//return rwVect3(v.pt[0] / z[0], v.pt[1] / z[1], v.pt[2] / z[2]);
	//}


#endif