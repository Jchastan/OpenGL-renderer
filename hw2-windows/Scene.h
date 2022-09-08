#include "Camera.h"
#include "Intersection.h"
#include "Object.h"
#include <vector>

using namespace std;
typedef struct Light {

    bool isPoint;
    vec3 lightvec;
    vec3 colorvec;
    //attenuation const linear quadratic?
    
} Light;

class Scene {

    public:

	Scene() {
	    material.ambient = vec3(0.2,0.2,0.2);
	};
	Intersection intersect(Ray ray);
	unsigned char* render();
	vector<Object> objects;
	vector<vec3> vertices;
	vector<LocalGeo> vertexnormals;
	Camera cam = Camera();
	vector<Light> lights;
	Material material;
	
};
