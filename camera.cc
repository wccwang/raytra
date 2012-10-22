#include"camera.h"
#include<iostream>
#include<cmath>
#include<algorithm>

using namespace std;

void camera::init(myPoint p, myVector D, float fl, float Iw, float Ih, int nx, int ny){
	eye = p;
	d = fl;
	iw = Iw;
	ih = Ih;
	pw = nx;
	ph = ny;
	u = myVector(-D._z, 0.0f, D._x);
	v = myVector(u._y*D._z-u._z*D._y, u._z*D._x-u._x*D._z, u._x*D._y-u._y*D._x);
	w = myVector(-D._x, -D._y, -D._z);
	u = u.getUnitVec();
	v = v.getUnitVec();
	w = w.getUnitVec();
	pixelBuffer.resizeErase(ph, pw);
}

ray* camera::genRay(int xx, int yy){
	float cu = -iw/2.0f+iw*(xx+0.5f)/pw;
	float cv = -ih/2.0f+ih*(ph-yy+0.5f)/ph;
	float dirX= -d*w._x+cu*u._x+cv*v._x;
	float dirY= -d*w._y+cu*u._y+cv*v._y;
	float dirZ= -d*w._z+cu*u._z+cv*v._z;
	myVector dir(dirX, dirY, dirZ);
	dir = dir.getUnitVec();
	ray* retRay = new ray(eye, dir);
	return retRay;
}

myVector camera::calculatePixel(ray* rayPtr, int recurse_limit, vector<surface*> &surfaceSet, const vector<pLight*>& pointLights, aLight *alPtr){
	// check for intersection
	if(recurse_limit==0) return myVector(0.0f, 0.0f, 0.0f);
		
	bool isHit = false;
	for(size_t k=0; k<surfaceSet.size(); ++k){
		if(surfaceSet[k]->intersect(rayPtr, k))
			isHit = true;
	}
	if(!isHit)
		return myVector(0.0f, 0.0f, 0.0f);
	
	// Get the intersection data from the nearest surface
	intersection* data = rayPtr->getNearestData();
	surface* target = surfaceSet[data->sf_id];
	myVector kd = target->getMaterial().getDiffuse();
	myVector ks = target->getMaterial().getSpecular();
	myVector km = target->getMaterial().getIdealSpec();
	
	// ambient light
	myVector Ia = alPtr->getColorVector();
	myVector La = kd*Ia;
	myVector result = La;
	
	for(size_t i=0; i<pointLights.size(); ++i){
		// check shadow
		myVector shadVec = pointLights[i]->getPos() - data->itrPt;
		float tmax =  shadVec.getNorm();
		shadVec = shadVec.getUnitVec();
		ray* shadRay = new ray(data->itrPt+shadVec*0.05f, shadVec);
		bool isblocked = false;
		for(int j=0; j<(int)surfaceSet.size(); ++j){
			if(j!=data->sf_id)
				surfaceSet[j]->intersect(shadRay, j);
		}
		if(shadRay->detectBlock(tmax))
			isblocked = true;
		
		if(!isblocked){
			// diffuse light
			myVector I = pointLights[i]->getColorVector();
			myVector Ld = kd*I;
			myVector li = pointLights[i]->getPos() - data->itrPt;
			li = li.getUnitVec();
			float dot1 = data->n.dot(li);
			Ld*=max(dot1,0.0f);
			// specular light
			myVector Ls = ks*I;
			myVector h = data->v+li;
			h = h.getUnitVec();
			float dot2 = data->n.dot(h);
			float p = target->getMaterial().getPhongExp();
			Ls*=max(0.0f, pow(dot2, p));
			
			result+=Ld+Ls;
		}
	}
	
	if(km[0]==0 && km[1]==0 && km[2]==0)
		return result;
	else{
		myVector reflecDir = data->n*(2.0f*data->n.dot(data->v)) - data->v;
		myPoint reflecPt = data->itrPt + reflecDir*0.05f;
		ray* reflecRay = new ray(reflecPt, reflecDir);
		return result+km*calculatePixel(reflecRay, recurse_limit-1, surfaceSet, pointLights, alPtr);
	}
	
}

void camera::renderScene(vector<surface*> &surfaceSet, const vector<pLight*>& pointLights, aLight *alPtr){
	for(int j=0; j<ph; ++j){
		for(int i=0; i<pw; ++i){
			ray* currentRay = genRay(i, j);
			Rgba &px = pixelBuffer[j][i];
			int recurse_limit = 5;
			myVector result = calculatePixel(currentRay, recurse_limit, surfaceSet, pointLights, alPtr);
			px.r = result[0];
			px.g = result[1];
			px.b = result[2];
			px.a = 1;
		}
	}
}

void camera::writeImage (const char fileName[]){  
    RgbaOutputFile file (fileName, pw, ph, WRITE_RGBA);
    file.setFrameBuffer (&pixelBuffer[0][0], 1, pw);
    file.writePixels (ph);
}

void printCamera(camera *cam){
	if(cam != NULL){
		cout  << cam->d<< endl; 
		printVector(cam->u);
		printVector(cam->v);
		printVector(cam->w);
	}
	else
		std::cerr<<"no camera found"<<std::endl;
}