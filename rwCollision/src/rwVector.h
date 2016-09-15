#ifndef VECTOR_H
#define VECTOR_H

/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#include "rwMath.h"
#include <assert.h>




	

/**
* three dimensional vector
**/
class rwVect3 {
public:

	float pt[3];

	rwVect3(){
		pt[0]=(0);
		pt[1]=(0);
		pt[2]=(0); 
	}
	rwVect3(float x, float y, float z){
		pt[0]=(x);
		pt[1]=(y);
		pt[2]=(z); 
	}
	
	///wektor skalarny
	 inline float dot(const rwVect3& v) const
		{
			return pt[0] * v.pt[0] + pt[1] * v.pt[1] + pt[2] * v.pt[2];
		}
	///potrójny skalar
	 inline float dotTriple(const rwVect3& v,const rwVect3& u){
		float out = pt[0]*((v[1]*u[2])+(v[2]*u[1]))+ 
					pt[1]*((v[0]*u[2])+(v[2]*u[0]))+ 
					pt[2]*((v[0]*u[1])+(v[1]*u[0]));
		//float out = dot( v.cross( u ) );
		return out;
	}

	 inline rwVect3 cross(const rwVect3& v) const
		{
			return rwVect3( pt[1] * v[2] - pt[2] * v[1],
							pt[2] * v[0] - pt[0] * v[2],
							pt[0] * v[1] - pt[1] * v[0]);
		}

	 inline float length2() const
		{
			return dot(*this);
		}

	///dlugosc wektora
	inline float length() const
		{
			return sqrtf( length2 () );
		}

	float distance2(const rwVect3& v) const 
		{
			return (v - (*this)).length2();
		}

	float distance(const rwVect3& v) const 
		{
			return (v - *this).length();
		}
		
	rwVect3 normalize() 
		{
			return *this /= length();
		}
		
	rwVect3 normalized() const 
		{
			return (*this) / length();
		} 

	float angle(const rwVect3& v) const 
		{
			float s = sqrtf(length2() * v.length2());
			assert(s != (0.0));
			return acos(dot(v) / s);
		}

	///
	rwVect3 absolute(){
		return rwVect3( rwMath::absolute(pt[0]),
						rwMath::absolute(pt[1]),
						rwMath::absolute(pt[2])
			);
	}
	///odwraca wektor
	rwVect3& negate(){
		pt[0]=-pt[0];
		pt[1]=-pt[1];
		pt[2]=-pt[2]; 
		return *this;
	}
	///zwraca nowy wektor odwrocony
	rwVect3 getNegative(){
		rwVect3 npt(-pt[0],-pt[1],-pt[2] );
		return npt;
	}
	///zeruje wektor
	void reset(){
		pt[0]=0.0f;
		pt[1]=0.0f;
		pt[2]=0.0f;
	}

	

	inline float getX(){ return pt[0]; }
	inline float getY(){ return pt[1]; }
	inline float getZ(){ return pt[2]; }
	inline void setX(float x){ pt[0]=x; }
	inline void setY(float y){ pt[1]=y; }
	inline void setZ(float z){ pt[2]=z; }
//
// operatory
//
	friend rwVect3 operator+(const rwVect3& v1, const rwVect3& v2);
	friend rwVect3	operator-(const rwVect3& v1, const rwVect3& v2);
	friend rwVect3 operator-(const rwVect3& v);
	friend rwVect3 operator*(const rwVect3& v, const float& s);
	friend rwVect3	operator*(const float& s, const rwVect3& v);
	friend rwVect3	operator/(const rwVect3& v, const float& s);
	//error C2801: 'operator []' must be a non-static member
	//friend float operator[]( const rwVect3& v,const int& id);

	//float operator[]( const int& id);
	//error C2678: binary '[' : no operator found which 
	//takes a left-hand operand of type 'const rwVect3' (or there is no acceptable conversion)
	
	float operator[]( const int& id) const{
		return pt[id];
	}
	

	rwVect3& operator+=(const rwVect3& v)
		{
			pt[0] += v.pt[0]; pt[1] += v.pt[1]; pt[2] += v.pt[2];
			return *this;
		}
		
	rwVect3& operator-=(const rwVect3& v) 
		{
			pt[0] -= v.pt[0]; pt[1] -= v.pt[1]; pt[2] -= v.pt[2];
			return *this;
		}
		

	rwVect3& operator*=(const float &s)
		{
			pt[0] *= s; pt[1] *= s; pt[2] *= s;
			return *this;
		}
		
	rwVect3& operator/=(const float &s) 
		{
			assert(s != (0.0f));
			return *this *= (1.0f) / s;
		}
	bool operator==(const rwVect3& v) 
		{
			if( (pt[0] != v.pt[0])||
				(pt[1] != v.pt[1])||
				(pt[2] != v.pt[2]) 
				)
				return false;
			return true;
		}

};
//


	









#endif