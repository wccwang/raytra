#ifndef READSCENE_H
#define READSCENE_H

#include<string>
#include<vector>
#include"camera.h"
#include"surface.h"
#include"plight.h"
#include"alight.h"
#include"slight.h"
#include"material.h"

using namespace std;

float getTokenAsFloat (string inString, int whichToken);
void parseSceneFile (char *filnam, camera &cam1, vector<surface*> &surfaceSet, vector<pLight*>& , aLight*, material*, vector<sLight*>&);

#endif