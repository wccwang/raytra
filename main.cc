#include<vector>
#include<iostream>
#include"sphere.h"
#include"readscene.h"

using namespace std;

int main (int argc, char *argv[])
{
	if (argc != 3) {
        cerr << "usage: raytra scenefilename outputfilename" << endl;
        return -1;
	}
    
	char* inputscene = argv[1];
	char* outputImage = argv[2];
	camera cam1;
	vector<surface*> surfaceSet;
	vector<pLight *> pointLights;
	aLight *alPtr = new aLight;
	material *mtrPtr = new material;
	
	parseSceneFile (inputscene, cam1, surfaceSet, pointLights, alPtr, mtrPtr);
	
	cam1.renderScene(surfaceSet, pointLights, alPtr);

	cam1.writeImage (outputImage);
	
	return 0;
}