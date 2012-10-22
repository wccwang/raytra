#include<cmath>
#include"sphere.h"

bool sphere::intersect(ray* rayPtr, int sf_id){
	myPoint rayOrigin = rayPtr->getOrigin();
	myVector rayDir = rayPtr->getDir();
	myVector vec1 = rayOrigin - this->origin; //P0-O
	float A = rayDir.dot(vec1);
	float B = rayDir.dot(rayDir);
	float C = vec1.dot(vec1)-radius*radius;
	if(A*A-B*C>=0){
		float t1 = (-A - sqrt(A*A-B*C))/B;
		float t2 = (-A + sqrt(A*A-B*C))/B;
		if(t1<0 || t2<0) return false;
		float t = (t1<t2)? t1:t2;
		myPoint itrPt = rayOrigin+(rayDir * t); //intersect point = P0+td
		
		myVector vec3 = itrPt - this->origin;
		myVector n(vec3.getUnitVec());
		myVector v = rayDir * (-1.0f);
		intersection* data = new intersection(sf_id, t, n, v,  itrPt);
		rayPtr->addIntersectData(data);
		return true;
	}
	else return false;
}