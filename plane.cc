#include "plane.h"

bool plane::intersect(ray* rayPtr, int sf_id){
	myPoint rayOrigin = rayPtr->getOrigin();
	myPoint origin(0.0f, 0.0f, 0.0f);
	myVector P0 = rayOrigin - origin;
	myVector rayDir = rayPtr->getDir();
	
	float denominator = rayDir.dot(normal);
	if(denominator==0) return false;
	else{
		float t = (P0.dot(normal) + d)/denominator;
		t*=-1.0f;
		if(t<0) return false;
		myPoint itrPt = rayOrigin + (rayDir*t);
		myVector n(normal);
		myVector v = rayDir * (-1.0f);
		intersection* data = new intersection(sf_id, t, n, v,  itrPt);
		rayPtr->addIntersectData(data);
		return true;
	}
}