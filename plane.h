#ifndef PLANE_H_
#define PLANE_H_
#include "surface.h"

class plane : public surface{
	public:
		plane(myVector vec, float D): normal(vec), d(D){}
		bool intersect(ray* , int);
	private:
		myVector normal;
		float d;
};
#endif