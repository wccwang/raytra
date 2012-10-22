#ifndef PLIGHT_H_
#define PLIGHT_H_

#include "basicmath.h"

class pLight{
	public:
		pLight(){
			position = myPoint(0.0f, 0.0f, 0.0f);
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			intensity = 0.0f;
		}
		void setPLight(myPoint pt, float r, float g, float b);
		myVector getColorVector();
		myPoint getPos(){return position;}
	private:
		myPoint position;
		float color[3];
		float intensity;
};
#endif