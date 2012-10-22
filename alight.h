#ifndef ALIGHT_H_
#define ALIGHT_H_

class aLight{
	public:
		aLight(){
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
		}
		void setALight(float r, float g, float b){
			color[0] = r;
			color[1] = g;
			color[2] = b;
		}
		myVector getColorVector(){
			float r= color[0];
			float g= color[1];
			float b= color[2];
			myVector retVec(r, g, b);
			return retVec;
		}
	private:
		float color[3];
};

#endif