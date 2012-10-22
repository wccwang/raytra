#include"triangle.h"

bool triangle::intersect(ray* rayPtr, int sf_id){
	myPoint rayOrigin = rayPtr->getOrigin();
	myVector rayDir = rayPtr->getDir();
	float a = p1[0] - p2[0];
	float b = p1[1] - p2[1];
	float c = p1[2] - p2[2];
	float d = p1[0] - p3[0];
	float e = p1[1] - p3[1];
	float f = p1[2] - p3[2];
	float g = rayDir[0];
	float h = rayDir[1];
	float i = rayDir[2];
	float j = p1[0] - rayOrigin[0];
	float k = p1[1] - rayOrigin[1];
	float l = p1[2] - rayOrigin[2];
	float A = e*i - h*f;
	float B = g*f - d*i;
	float C = d*h - e*g;
	float D = a*k- j*b;
	float E = j*c- a*l;
	float F = b*l - k*c;
	float M = a*A+b*B+c*C;
	
	float t = -(f*D+e*E +d*F)/M;
	if(t<0) return false;
	
	float gamma = (i*D+h*E+g*F)/M;
	if(gamma<0 || gamma >1) return false;
	
	float beta = (j*A+k*B+l*C)/M;
	if(beta<0 || beta>(1-gamma)) return false;
	
	myPoint itrPt = rayOrigin+(rayDir * t);
	myVector v = rayDir * (-1.0f);
	intersection* data = new intersection(sf_id, t, normal, v,  itrPt);
	rayPtr->addIntersectData(data);
	return true;
}