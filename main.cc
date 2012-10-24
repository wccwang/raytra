#include<vector>
#include<iostream>
#include<cstdlib>
#include"sphere.h"
#include"readscene.h"

using namespace std;

int main (int argc, char *argv[])
{
	if (argc < 3) {
        cerr << "usage: raytra scenefilename outputfilename" << endl;
        return -1;
	}
    
	char* inputscene = argv[1];
	char* outputImage = argv[2];
	int num_samples = 3;
	int num_shadows =  1;
	if (argc == 4){
		num_samples = atoi(argv[3]);
		num_shadows =  atoi(argv[3]);
	}
	else if(argc == 5){
		num_samples = atoi(argv[3]);
		num_shadows =  atoi(argv[4]);
	}
	camera cam1;
	vector<surface*> surfaceSet;
	vector<pLight *> pointLights;
	vector<sLight *> areaLights;
	aLight *alPtr = new aLight;
	material *mtrPtr = new material;
	
	parseSceneFile (inputscene, cam1, surfaceSet, pointLights, alPtr, mtrPtr, areaLights);
	
	cam1.renderScene(surfaceSet, pointLights, alPtr, areaLights, num_samples, num_shadows);

	cam1.writeImage (outputImage);
	
	return 0;
}