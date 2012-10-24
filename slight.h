#ifndef SLIGHT_H_
#define SLIGHT_H_

#include "basicmath.h"
#include "plight.h"

class sLight{
	public:
		sLight(myPoint pt, myVector DIR, myVector U, float len, myVector rgb)
		:position(pt), dir(DIR), u(U), length(len), color(rgb){
			dir = dir.getUnitVec();
			u = u.getUnitVec();
			v = dir.cross(u);	
		}
		pLight getSample(float ru, float rv){
			myVector displace = u*((ru-0.5f)*length) + v*((rv-0.5f)*length);
			myPoint result = position+displace;
			return pLight(result, color);
		}
		myVector getDir(){return dir;}
		myVector getColorVector(){return color;}
		myPoint getPos(){return position;}
	private:
		myPoint position;
		myVector dir;
		myVector u;
		myVector v;
		float length;
		myVector color;
};

#endif