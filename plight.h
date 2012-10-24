#ifndef PLIGHT_H_
#define PLIGHT_H_

#include "basicmath.h"

class pLight{
	public:
		pLight(){
			position = myPoint(0.0f, 0.0f, 0.0f);
			color = myVector(0.0f, 0.0f, 0.0f);
			//intensity = 0.0f;
		}
		pLight(myPoint pt, myVector rgb):position(pt), color(rgb){}
		myVector getColorVector(){return color;}
		myPoint getPos(){return position;}
	private:
		myPoint position;
		myVector color;
		//float intensity;
};

#endif
