#include"plight.h"

void pLight::setPLight(myPoint pt, float r, float g, float b){
	position = pt;
	color[0] = r;
	color[1] = g;
	color[2] = b;
	float max = color[0];
	for(int i=1; i<3; i++){
		if(color[i]>max)
			max = color[i];
	}
	for(int i=0; i<3; i++){
		color[i]/=max;
	}
	intensity = max;
	return;
}

myVector pLight::getColorVector(){
	float r= color[0]*intensity;
	float g= color[1]*intensity;
	float b= color[2]*intensity;
	myVector retVec(r, g, b);
	return retVec;
}