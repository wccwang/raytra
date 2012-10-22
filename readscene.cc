#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include<vector>
#include<cstdlib>
#include"readscene.h"
#include"sphere.h"
#include"plane.h"
#include"triangle.h"

#include "obj_file_parser.h"

using namespace std;

//#define IM_DEBUGGING

// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
float getTokenAsFloat (string inString, int whichToken)
{
    
    float thisFloatVal = 0.;    // the return value
    
    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit (-1);
    }
    
    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char [inString.size () + 1];
    
    strcpy (cstr, inString.c_str());
    
    char *p = strtok (cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit (-1);
    }
    
    for (int i = 0; i < whichToken; i++) {
        p = strtok (0, " ");
        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!" << endl;
            exit (-1);
        }
    }
    
    thisFloatVal = atof (p);
    
    delete[] cstr;
    
    return thisFloatVal;
}


//
// read the scene file.
//
// You'll need a few globals (or add arguments to this function): for the
// list of surfaces (geometric objects like spheres, triangles, planes) and
// another for the lights. These can be of type std::vector. You'll also
// need a global (or other parameter) for the camera.
//
// This should be pretty self-explanatory: it reads through the lines, and
// using the first character figures out what kind of command it is. It
// then calls the "getTokenAsFloat" routine to pull out the needed values.
// NOTE: since different commands take different number of arguments, you
// must take care not to call getTokenAsFloat for an index that is beyond the
// end of the line!
//
// One tricky bit: when a material is read in, we want
// to keep a pointer to it so that for the next geometric object read in ,
// we can add that material to the object. In the code that follows, I use the
// variable "lastSurfaceLoaded" to do that, but the code is commented out since
// I don't know the class names you will be using.
//
// Very minimal error check here. You might improve it slightly, but we'll
// only use "correct" scene files.
//
//
void parseSceneFile (char *filnam, camera &cam1, vector<surface*> &surfaceSet, vector<pLight*>& pointLights,  aLight *alPtr, material *lastMaterialLoaded)
{
    
    ifstream inFile(filnam);    // open the file
    
    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }
    
    
    // Note: you'll have to keep track of whatever the last material
    // you loaded in was, so you can apply it to any geometry that gets loaded. 
    // So here, you'll have something like:
    //
    // myMaterialClass *lastMaterialLoaded = 0;  // 0 or maybe a default material?
    //
    // and each time you load in a new piece of geometry (sphere, triangle, plane)
    // you will set its material to lastMaterialLoaded.

    
    while (! inFile.eof ()) {   // go through every line in the file until finished
        string line;
        getline (inFile, line); // get the line
        //cout << line << endl;
        switch (line[0])  {     // we'll decide which command based on the first character
            
            //
            // geometry types:
            //
            // NOTE: whichever type of geo you load in, set its material to
            // be "lastMaterialLoaded"
            //
            case 's':
	    {
                // it's a sphere, load in the parameters
                
                float x, y, z, r;
                x = getTokenAsFloat (line, 1); 
                y = getTokenAsFloat (line, 2); 
                z = getTokenAsFloat (line, 3); 
                r = getTokenAsFloat (line, 4); 

                // build your sphere here from the parameters
                // i.e. you must call your sphere constructor and set its position
                // and radius from the above values. You must also put your new
                // sphere into the objects list (which can be global)
                // So something like;
		surface *ps = new sphere(x, y, z, r);   // make a new instance of your sphere class
		ps->setMaterial (lastMaterialLoaded);
		surfaceSet.push_back(ps);
                // ms->setMaterial (lastMaterialLoaded)
                // objectsList->push_back (ms);  // objectsList is a global std:vector<surface *> for example.
                
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
		}
                break;
                
            case 't':   // triangle
	    {
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                x1 = getTokenAsFloat (line, 1); 
                y1 = getTokenAsFloat (line, 2); 
                z1 = getTokenAsFloat (line, 3);
		x2 = getTokenAsFloat (line, 4); 
                y2 = getTokenAsFloat (line, 5); 
                z2 = getTokenAsFloat (line, 6); 
		x3 = getTokenAsFloat (line, 7); 
                y3 = getTokenAsFloat (line, 8); 
                z3 = getTokenAsFloat (line, 9); 
		myPoint p1(x1, y1, z1);
		myPoint p2(x2, y2, z2);
		myPoint p3(x3, y3, z3);
		surface *ps = new triangle(p1, p2, p3);
		ps->setMaterial (lastMaterialLoaded);
		surfaceSet.push_back(ps);
	    }
                break;
                
            case 'p':   // plane
	    {
		float nx, ny, nz, d;
                nx = getTokenAsFloat (line, 1); 
                ny = getTokenAsFloat (line, 2); 
                nz = getTokenAsFloat (line, 3); 
                d = getTokenAsFloat (line, 4);
		myVector normal(nx,ny,nz);
		surface *ps = new plane(normal.getUnitVec(), d);
		ps->setMaterial (lastMaterialLoaded);
		surfaceSet.push_back(ps);
	    }
                break;
                
            //
            // camera:
            //
            case 'c':   // camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
	    {
		float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
                x = getTokenAsFloat (line, 1); 
                y = getTokenAsFloat (line, 2); 
                z = getTokenAsFloat (line, 3);
		myPoint pos(x, y, z);
		vx = getTokenAsFloat (line, 4); 
                vy = getTokenAsFloat (line, 5); 
                vz = getTokenAsFloat (line, 6);
		myVector dir(vx, vy, vz);
		d = getTokenAsFloat (line, 7); 
                iw = getTokenAsFloat (line, 8); 
                ih = getTokenAsFloat (line, 9);
		pw = getTokenAsFloat (line, 10); 
                ph = getTokenAsFloat (line, 11);
		cam1.init(pos, dir, d, iw, ih, (int)pw, (int)ph);
		#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a camera with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << vx<< " "<<vy << " "<< vz  << endl;
		#endif
	    }
                break;
                
            //
            // lights:
            //
            case 'l':   // light
                
                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p':   // point light
		    {
			float x, y ,z ,r ,g, b;
			x = getTokenAsFloat (line, 2); 
			y = getTokenAsFloat (line, 3); 
			z = getTokenAsFloat (line, 4);
			r = getTokenAsFloat (line, 5); 
			g = getTokenAsFloat (line, 6); 
			b = getTokenAsFloat (line, 7);
			myPoint pos(x,y,z);
			pLight* plPtr = new pLight;
			plPtr->setPLight(pos, r, g, b);
			pointLights.push_back(plPtr);
			//cout << x << " "<< y << " "<< z << " "<< r << " "<<g << " "<<b<< endl;
		    }
                        break;
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
		    {
			float r, g ,b;
			r = getTokenAsFloat (line, 2); 
			g = getTokenAsFloat (line, 3); 
			b= getTokenAsFloat (line, 4);
			alPtr->setALight(r, g, b);
		    }
                        break;
                        
                }
                
                break;
            
            //
            // materials:
            //
            case 'm':   // material
	    {   
		// the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
                //
		float dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                dr = getTokenAsFloat (line, 1); 
                dg = getTokenAsFloat (line, 2); 
                db = getTokenAsFloat (line, 3);
		sr = getTokenAsFloat (line, 4); 
                sg = getTokenAsFloat (line, 5); 
                sb = getTokenAsFloat (line, 6);
		r = getTokenAsFloat (line, 7); 
                ir = getTokenAsFloat (line, 8); 
                ig = getTokenAsFloat (line, 9);
		ib = getTokenAsFloat (line, 10);
		lastMaterialLoaded->setMaterial(dr, dg, db, sr, sg, sb, r, ir, ig, ib);
            }    
		break;

            
            case '/':
                // don't do anything, it's a comment
                break;
                

            //
            // options
            //
            case 'w':   // read triangle obj file
	    {
		size_t index = 1;
		string filename;
		while(line[index]==' ') index++;
		while(index<line.size()&&line[index]!=' '){
			filename+=line[index];
			index++;
		}
		//cout << filename << endl;
		vector<int> tris;
		vector<float> verts;
		read_wavefront_file(filename.c_str(), tris, verts);
		// create triangles
		size_t triIndex = 0;
		while(triIndex<tris.size()){
			int ver1 = tris[triIndex];
			int ver2 = tris[triIndex+1];
			int ver3 = tris[triIndex+2];
			myPoint p1(verts[ver1*3], verts[ver1*3+1], verts[ver1*3+2]);
			myPoint p2(verts[ver2*3], verts[ver2*3+1], verts[ver2*3+2]);
			myPoint p3(verts[ver3*3], verts[ver3*3+1], verts[ver3*3+2]);
			
			surface* ps = new triangle(p1, p2, p3);
			ps->setMaterial(lastMaterialLoaded);
			surfaceSet.push_back(ps);
			triIndex+=3;
		}
	    }
                break;
        }
        
    }
}

//
// the main just makes sure that there is an argument passed, which is
// supposed to be the scenefile.
//
/*
int main (int argc, char *argv[])
{
  
    if (argc != 2) {
        cerr << "useage: raytra scenefilename" << endl;
        return -1;
    }
    
    parseSceneFile (argv[1]);
    
    return 0;
}
*/