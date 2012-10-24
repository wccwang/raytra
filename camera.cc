#include"camera.h"
#include<iostream>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<utility>

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

ray* camera::genRay(float xx, float yy){
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

myVector camera::calculatePixel(ray* rayPtr, int recurse_limit, 
	const vector<surface*> &surfaceSet, const vector<pLight*>& pointLights, 
	aLight *alPtr, vector<pLight>& lightSpls, vector<myVector>& lightDirs, int num_spls){
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
	myVector Ia = alPtr->getColorVector()/num_spls;
	myVector La = kd*Ia;
	myVector result = La;
	
	// point light
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
			float length = li.getNorm();
			
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
			result/=(length*length);
		}
	}
	// Area light samples
	for(size_t i=0; i<lightSpls.size(); ++i){
		// check shadow
		myVector shadVec = lightSpls[i].getPos() - data->itrPt;
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
			myVector I = lightSpls[i].getColorVector();
			myVector Ld = kd*I;
			myVector li = lightSpls[i].getPos() - data->itrPt;
			float length = li.getNorm();
			
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
			// normalize light intensity
			float cos1 = dot1/(data->n.getNorm()*li.getNorm());
			float dot3 = lightDirs[i].dot(li*-1.0f);
			float cos2 = dot3/(lightDirs[i].getNorm()*li.getNorm());
			result+=Ld+Ls;
			result*=(cos1*cos2);
			result/=(length*length);
		}
	}
	
	if(km[0]==0 && km[1]==0 && km[2]==0)
		return result;
	else{
		myVector reflecDir = data->n*(2.0f*data->n.dot(data->v)) - data->v;
		myPoint reflecPt = data->itrPt + reflecDir*0.05f;
		ray* reflecRay = new ray(reflecPt, reflecDir);
		return result+km*calculatePixel(reflecRay, recurse_limit-1, surfaceSet, pointLights, alPtr, lightSpls, lightDirs, num_spls);
	}
	
}

void shuffle(vector<pair<float,float> > &s){
	size_t n = s.size();
	for(size_t i=0; i<n; ++i){
		size_t j = i+rand()%(n-i);
		pair<float, float> tmp = s[i];
		s[i]=s[j];
		s[j] = tmp;
	}
	return;
}

void camera::renderScene(vector<surface*> &surfaceSet, const vector<pLight*>& pointLights, 
	aLight *alPtr, vector<sLight *> &areaLights, const int N1, const int N2){
	
	srand(time(0));
	int recurse_limit = 5;
	float scale = (float)RAND_MAX+1.0f;
	int num_samplepts = N1*N1;
	int num_shadows = N2*N2;
	
	for(int j=0; j<ph; ++j){
		for(int i=0; i<pw; ++i){
			Rgba &px = pixelBuffer[j][i];
			myVector result(0.0f, 0.0f, 0.0f);
			vector<pair<float,float> > r;
			vector<pair<float,float> > s;
			//Generate pixel samples
			for(int p=0; p<N1; ++p){
				for(int q=0; q<N1; ++q){
					r.push_back(make_pair((p+(float)rand()/scale)/N1, (q+(float)rand()/scale)/N1));
				}
			}
			
			for(int k=0; k<num_samplepts; ++k){
				ray* currentRay = genRay((float)i-0.5f+r[k].first , (float)j-0.5f+r[k].second); 
				// shadow ray samples
				for(int p=0; p<N2; ++p){
					for(int q=0; q<N2; ++q){
						s.push_back(make_pair((p+(float)rand()/scale)/N2, (q+(float)rand()/scale)/N2));
					}
				}
				shuffle(s);
				
				for(int m=0; m<num_shadows; ++m){
					vector<pLight> lightSpls;
					vector<myVector> lightDirs;
					for(size_t al=0; al<areaLights.size(); ++al){
						lightSpls.push_back(areaLights[al]->getSample(s[m].first, s[m].second));
						lightDirs.push_back(areaLights[al]->getDir());
					}
					result += calculatePixel(currentRay, recurse_limit, surfaceSet, pointLights, alPtr, lightSpls, lightDirs, num_samplepts);
				}
			}
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