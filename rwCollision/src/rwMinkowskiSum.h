/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef RWMINKOWSKISUM_H
#define RWMINKOWSKISUM_H

#include <vector>
#include "rwVector.h"

using namespace std;

/**
* operacje na CSO dwóch bry³
* klasa nie tworzy ró¿nicy ani sumy minkowskiego tylko zwraca punkty z CSO na ¿¹danie.
* nazwa jest myl¹ca( jest to implementacja ró¿nicy minkowskiego)
**/
class rwMinkowskiSum {

	static vector<rwVect3>::iterator it;

	bool ISGRAVITATION;

	/// najdalszy punkt w zadanym kierunku
	/// O(n)
	static rwVect3 MaxPointAlongDirection(vector<rwVect3>& shape, rwVect3 direction)
        {
            rwVect3 max = shape.front();
			vector<rwVect3>::iterator it = shape.begin();
			while(it != shape.end())
            {
                if (max.dot(direction) < (*it).dot(direction))
                    max = (*it);
				it++;
            }

            return max;
        }
	/// najblizszy punkt w zadanym kierunku
	/// O(n)
	static rwVect3 MinPointAlongDirection(vector<rwVect3>& shape, rwVect3 direction)
        {
            rwVect3 max = shape.front();
			vector<rwVect3>::iterator it = shape.begin();
			while(it != shape.end())
            {
                if (max.dot(direction) < (*it).dot(direction))
                    max = (*it);
				it++;
            }

            return max;
        }

public:

	rwMinkowskiSum(){
	}

	///najdalszy punkt w zadanym kierunku w sumie minkowskiego dwóch zbiorów
	///O(2n)
	static rwVect3 MaxPointAlongDir(vector<rwVect3> &shape1, vector<rwVect3> &shape2, rwVect3 direction)
        {
            return MaxPointAlongDirection(shape1, direction) - MaxPointAlongDirection(shape2, direction.getNegative());
        }
	///najblizszy punkt w kierunku punktu zerowego w sumie minkowskiego dwóch zbiorów
	///O(2n)
	static rwVect3 MinPointToOrigin(vector<rwVect3> &shape1, vector<rwVect3> &shape2, rwVect3 direction)
        {
            return MinPointAlongDirection(shape1, direction) - MaxPointAlongDirection(shape2, direction.getNegative());
        }

    

};

#endif