#ifndef SPHERE_H_
#define SPHERE_H_

#include "surface.h"
#include "ray.h"

class sphere : public surface{
public:
	sphere(float x, float y, float z, float r):origin(myPoint(x,y,z)), radius(r){}
	bool intersect(ray* , int);
private:	
	myPoint origin;	
	float radius;
};
#endif