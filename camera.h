#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include <ImfRgbaFile.h>
#include <ImfArray.h>

#include "basicmath.h"
#include "ray.h"
#include "plight.h"
#include "alight.h"
#include "slight.h"
#include "surface.h"
#include "sphere.h"

using namespace Imf;

class camera{
	public:
		void init(myPoint p, myVector D, float fl, float Iw, float Ih, int nx, int ny);
		ray* genRay(float, float);
		myVector calculatePixel(ray*, int, const std::vector<surface*>&, 
			const std::vector<pLight*>&, aLight*, 
			std::vector<pLight>&, std::vector<myVector>&, int);
		void renderScene(std::vector<surface*>&, const std::vector<pLight*>&, 
			aLight*, std::vector<sLight *>&, const int, const int);
		void writeImage (const char fileName[]);
		friend void printCamera(camera *);
	private:
		myPoint eye;
		float d;
		myVector u;
		myVector v;
		myVector w;
		float iw;
		float ih;
		int pw;
		int ph;
	
		Array2D<Rgba> pixelBuffer;
};

#endif