#include "Transform.h"
#include "Intersection.h"
#include <limits>
#include <vector>

using namespace std;

class Camera {
    public:

	Camera() {
	    fovy = 0;
	    aspect = 0;
	    width = 0;
	    height = 0;

	};
	Camera(vec3 eye, vec3 center, vec3 up);
	vec3 findColor(Intersection hit);
	Ray getRay(int i, int j);
	int width, height;
	vec3 pos, u, v, w;
	float fovy;
	float aspect;
	void buildRayArray();
	vector<vector<Ray>> rayArray;

};
