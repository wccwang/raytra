#ifndef SURFACE_H_
#define SURFACE_H_

#include "basicmath.h"
#include "material.h"
#include "ray.h"
#include "plight.h"

class surface{
	public:
		virtual bool intersect(ray* , int) = 0;
		void setMaterial(material *mtrPtr){mtr = *mtrPtr;}
		material getMaterial(){return mtr;}
		virtual ~surface(){}
	private:
		material mtr;
};

#endif