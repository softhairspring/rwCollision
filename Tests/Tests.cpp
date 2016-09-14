// Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "targetver.h"

#include "math.h"
#include <stdio.h>
#include <string>
#include <iostream>

 // test.cpp
  #include <UnitTest++.h>


#include "rwCollision.h"
#include "ObjectLoader.h"

using namespace std;
using namespace rwFigures;

#define ERRORSTABLESIZE 128







//
TEST(MyPlayground){


}

// rwVect3
TEST(rwVector){
	rwVect3 a(10.0f,20.0f,30.0f);
	rwVect3 b(1.0f,2.0f,3.0f);
	rwVect3 c;

	c=a+b;
	CHECK_EQUAL(c[0], 11);
	c=a-b;
	CHECK_EQUAL(c[0], 9);
	c=a*3;
	CHECK_EQUAL(c[1], 60);
	c=a/2;
	CHECK_EQUAL(c[1], 10);

	c=a;
	c+=b;
	CHECK_EQUAL(c[0], 11);
	c=a;
	c-=b;
	CHECK_EQUAL(c[2], 27);
	c=a;
	c*=3;
	CHECK_EQUAL(c[1], 60);
	c=a;
	c/=2;
	CHECK_EQUAL(c[1], 10);
	
	c.setX(-1.0f);
	CHECK_EQUAL(c.getX(), c[0]);

	CHECK_EQUAL(sqrt(a.dot(a)), a.length());
	CHECK_EQUAL(a.dot(a), a.length2());

	rwVect3 aa(10.0f,20.0f,30.0f);
	rwVect3 bb(0.0f,-4.0f,-2.0f);
	rwVect3 cc(1.0f,2.0f,3.0f);

	CHECK_EQUAL(-320, aa.dotTriple(bb,cc));
	
}
// AABBS
TEST(rwAABBtrue){
	///sprawdza czy zachodzi kolizja
	rwAABB A;
	rwAABB B;
	rwVect3 rn(1.0f,1.0f,1.0f);
	rwVect3 rn2(1.0f,1.0f,1.0f);
	rwVect3 center(0.5f,1.5f,0.0f);
	A.r = rn;
	B.r = rn2;
	B.c = center;

	
	CHECK_EQUAL(true, B.checkCollision(A) );

	//test getExpandedAABB
	B.c =  rwVect3(8.0f, 8.0f,4.0f);
	rwAABB C = B.getExpandedAABB( rwVect3(12.0f,-12.0f,2.0f) );
	rwVect3 checkexpandcenter(10.0f,-2.0f,3.0f);
	rwVect3 checkexpandbox =	  rwVect3(2.0f,10.0f,1.0f) +B.r;
	CHECK_EQUAL(checkexpandcenter[0], C.c[0] );
	CHECK_EQUAL(checkexpandcenter[1], C.c[1] );
	CHECK_EQUAL(checkexpandcenter[2], C.c[2] );
	CHECK_EQUAL(checkexpandbox[0], C.r[0] );
	CHECK_EQUAL(checkexpandbox[1], C.r[1] );
	CHECK_EQUAL(checkexpandbox[2], C.r[2] );

	//test kierunku rozciagania aabb
	rwAABB D;
	D.r = rwVect3(10.0f,10.0f,0.0f) ;
	D.c = rwVect3(0.0f,0.0f,0.0f) ;
	rwAABB E = D.getExpandedAABB(   rwVect3( -10.0f,-20.0f,0.0f) );
	rwVect3 checkexpandbox2(15.0f,20.0f,0.0f);

	CHECK_EQUAL(checkexpandbox2[0], E.r[0] );
	CHECK_EQUAL(checkexpandbox2[1], E.r[1] );
	CHECK_EQUAL(checkexpandbox2[2], E.r[2] );
	;

}
//
TEST(rwAABBfalse){
	///sprawdza czy nie zachodzi kolizja
	rwAABB A;
	rwAABB B;
	rwVect3 rn(1.0f,1.0f,1.0f);
	rwVect3 rn2(1.0f,1.0f,1.0f);
	rwVect3 center(2.5f,1.5f,1.5f);
	A.r = rn;
	B.r = rn2;
	B.c = center;

	CHECK_EQUAL(false, B.checkCollision(A) );

	///sprawdza poprawnosc tworzenia AABB
	rwAABB C;
	vector<rwVect3> cube1 ;
	cube1.push_back( rwVect3(6.0f, 0, 0));
	cube1.push_back( rwVect3(3.0f, 4, 1));
	cube1.push_back( rwVect3(0.0f, 1, -5));
	CHECK_EQUAL(true, C.computeAABB( cube1 ) );
	CHECK_EQUAL( 5 , C.r[2]);

}
//
TEST(rwObject){


	rwVect3 zero(0.0f,0.0f,0.0f);
	rwObject oa(zero);
	int count = 3;
	double pts[12]= { 5, -5, 10,
					 -5, 5, -10,
					 2, -2, 1,
					 2, -2, 1
	};
	CHECK_EQUAL(true, oa.init(pts,12) );
	oa.setLinearVelocity(rwVect3(5.0f, -5.0f, 10.0f) );

	float ratio = oa.getVelocityToSizeRatio( 1.0f );
	//predkosc rowna najdalszym punktom czyli polowie dlugosci obiektu
	CHECK_EQUAL(0.5f, ratio );
}
//
TEST(rwPolyhedron){
}

//	rwPrimitiveIntersection
TEST(rwPrimitiveIntersection){

	//test lini z trojkatem
	rwVect3 la( 13.0f,-11.0f, -3.0f);
	rwVect3 lb(-13.0f, 10.0f, 4.0f);
	rwVect3 tra(-10.0f,0.0f,0.0f);
	rwVect3 trb(5.0f,0.0f,5.0f);
	rwVect3 trc(5.0f,0.0f,-5.0f);
	float v,u,w;
	bool inters = rwPrimitiveIntersection::intersectLineTriangle(la,lb,tra,trb,trc,v,u,w);

	CHECK_EQUAL(true, inters);
}
//
//
TEST(GJK001test2d){
vector<rwVect3> cube2 ;
cube2.push_back(rwVect3(0, 3, 0));
cube2.push_back( rwVect3(0, 1, 0));
cube2.push_back( rwVect3(3, 1, 0));

vector<rwVect3> cube1 ;
cube1.push_back( rwVect3(6.0f, 1, 0));
cube1.push_back( rwVect3(3.0f, 1, 0));
cube1.push_back( rwVect3(6.0f, 3, 0));
rwGJK gjk;
//bool intersect = gjk.BodiesIntersect(cube1, cube2); //returns true in this case
//CHECK_EQUAL(intersect,true);
}
//	
//	testy obiektow zwyklych niekolidujacych
//
TEST(GJK002test2d){
vector<rwVect3> cube2 ;
cube2.push_back(rwVect3(0, 3, 0));
cube2.push_back( rwVect3(0, 1, 0));
cube2.push_back( rwVect3(3, 1, 0));

vector<rwVect3> cube1 ;
cube1.push_back( rwVect3(16.0f, 1, 0));
cube1.push_back( rwVect3(13.0f, 1, 0));
cube1.push_back( rwVect3(16.0f, 3, 0));
rwGJK gjk;
//bool intersect = gjk.BodiesIntersect(cube1, cube2); //returns true in this case
//CHECK_EQUAL(intersect,false);
}
//
//	testy obiektow zwyklych kolidujacych 
//
TEST(GJK101test){
vector<rwVect3> cube1 ;
cube1.push_back(rwVect3(0, 0, 0));
cube1.push_back( rwVect3(1, 0, 0));
cube1.push_back( rwVect3(1, 1, 0));
cube1.push_back( rwVect3(0, 1, 0));
cube1.push_back( rwVect3(0, 0, -1));
cube1.push_back( rwVect3(1, 0, -1));
cube1.push_back( rwVect3(1, 1, -1));
cube1.push_back( rwVect3(0, 1, -1));

vector<rwVect3> cube2 ;
cube2.push_back( rwVect3(0.5f, 0, 0));
cube2.push_back( rwVect3(1.5f, 0, 0));
cube2.push_back( rwVect3(1.5f, 1, 0));
cube2.push_back( rwVect3(0.5f, 1, 0));
cube2.push_back( rwVect3(0.5f, 0, -1));
cube2.push_back( rwVect3(1.5f, 0, -1));
cube2.push_back( rwVect3(1.5f, 1, -1));
cube2.push_back( rwVect3(0.5f, 1, -1));
rwGJK gjk;
//bool intersect = gjk.BodiesIntersect(cube1, cube2); //returns true in this case
//CHECK_EQUAL(intersect,true);
}
/*
TEST(GJK102test){
bool initOK1=false;
	bool initOK2=false;
	rwScene scene1;
	rwPolyhedron poly1;
	initOK1 = poly1.init(rwFigures::cube01_s3_nXnYnZ,rwFigures::cube01_s3_nXnYnZnr);
	rwPolyhedron poly2;
	initOK2 = poly2.init(rwFigures::cube01_s3_nXnYnZ_pair,rwFigures::cube01_s3_nXnYnZ_pairnr);
	CHECK_EQUAL(initOK1, true);
	CHECK_EQUAL(initOK2, true);
	scene1.addObject( poly1 );
	scene1.addObject( poly2 );
	rwGJK gjk;
	//bool gjktest = gjk.BodiesIntersect(poly1.getConvexHullRef(),poly2.getConvexHullRef());
	//CHECK_EQUAL(gjktest, true);
}
*/
//
//	testy obiektow rozciagnietych
//

TEST(GJK201){
	bool initOK1=false;
	bool initOK2=false;
	rwScene scene1;
	rwPolyhedron poly1;
	initOK1 = poly1.init(ccd_cube_extr01,ccd_cube_extr01nr);
	rwPolyhedron poly2;
	initOK2 = poly2.init(ccd_cube_extr02,ccd_cube_extr02nr);
	
	scene1.addObject( poly1 );
	scene1.addObject( poly2 );
	rwGJK gjk;
	bool gjktest = gjk.BodiesIntersect(&poly1,&poly2, 0.01666f, true);

	CHECK_EQUAL(gjktest, true);
}



 TEST(BasicCollisionDetectionAlgorithm){
	 /* 
	bool initOK1b=false;
	bool initOK2b=false;
	rwScene scene1b;
	ObjectLoader OL;

	std::string s("data/ico1-1.x");

	ObjectFileData *ofd = OL.getMesh( s );
				if(ofd==NULL)
					return;

	rwPolyhedron *shape1 = new rwPolyhedron( rwVect3(0,0,0) );
	shape1->init( ofd->points);
	shape1->setMesh( ofd->mesh );		//redundantne ale uzywane przez basic coll solv.
	scene1b.addObject( *shape1 );

	ofd = OL.getMesh( s );
				if(ofd==NULL)
					return;
	rwPolyhedron *shape2 = new rwPolyhedron( rwVect3(0.8f,0,0) );
	shape2->init( ofd->points);
	shape2->setMesh( ofd->mesh );		//redundantne ale uzywane przez basic coll solv.
	scene1b.addObject( *shape2 );


	scene1b.assignBasicCollisionDetector();
	scene1b.step(0.16f);
	

	CHECK_EQUAL(shape1->isColliding(),true);
	CHECK_EQUAL(shape2->isColliding(),true);

*/
 }

int main( int argc, char* argv[] )

{

    int pass =  UnitTest::RunAllTests();

	//gdy sie nie udalo
	if( pass){
		char in;
		std::cin >> in;
	}

return 0;
}




/*
{
    // six ways to detect and report the same error:
    BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error
    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error
    if( add( 2,2 ) != 4 )
        BOOST_ERROR( "Ouch..." );          // #3 continues on error
    if( add( 2,2 ) != 4 )
        BOOST_FAIL( "Ouch..." );           // #4 throws on error
    if( add( 2,2 ) != 4 ) throw "Oops..."; // #5 throws on error

    return add( 2, 2 ) == 4 ? 0 : 1;       // #6 returns error code
}

*/