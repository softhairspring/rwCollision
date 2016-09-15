/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RWFACE_H
#define RWFACE_H

#include <vector>

using namespace std;


struct rwFace{
	int a;
	int b;
	int c;
};

/**
* 3d object's mesh

* klasa opisujaca siatke bry³y
* rwFace to numery punktów figury
**/
class rwMesh{
	
	bool ISEND;
	vector<rwFace> faces;
	vector<rwFace>::iterator it;
	rwFace f;
public:
	rwMesh(){
		it = faces.begin();
	}

	void addFace(rwFace &f){
		faces.push_back( f );
	}

	rwFace resetIterator(){
		it = faces.begin();
		f = *it;
		ISEND=false;
		return f;
	}
	rwFace getNext(){
		if( (it) != faces.end()){
			it++;
			if((it) == faces.end() ){
				ISEND=true;
				return faces[0];
			}
		}
		f = *it;
		return f;
	}
	bool isLast(){ 
		if( (it+1) != faces.end())
			return false;
		return true;
	}
	bool isEnd(){ 
		return ISEND;
	}
	int size(){ return faces.size(); }
};


#endif