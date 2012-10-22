#ifndef RAY_H_
#define RAY_H_

#include <iostream>
#include "basicmath.h"
#include "intersection.h"

class ray{
	public:
		ray(const myPoint &pt, const myVector &vec):origin(pt), dir(vec.getUnitVec()){}
		ray(const myPoint &pt1,const myPoint &pt2):origin(pt1), dir(pt1-pt2){
			dir = dir.getUnitVec();
		}
		void addIntersectData(intersection* data){
			intersectData.push_back(data);
		}
		intersection* getNearestData(){
			if(intersectData.empty()) 
				std::cerr << "No Intersection!" << std::endl;
			float tret = intersectData[0]->t;
			int k = 0;
			for(size_t i=1; i<intersectData.size(); i++){
				if(tret > intersectData[i]->t){
					tret = intersectData[i]->t;
					k = i;
				}
			}
			return intersectData[k];
		}
		bool detectBlock(float tmax){
			if(intersectData.empty())return false;
			else{
				for(size_t i=0; i<intersectData.size(); ++i){
					if(intersectData[i]->t<tmax)
						return true;
				}
				return false;
			}
		}
		myPoint getOrigin(){return origin;}
		myVector getDir(){return dir;}
	private:
		myPoint origin;
		myVector dir;
		std::vector<intersection*> intersectData;
};
#endif