#pragma once
#include "Transform.h"
#include "Intersection.h"



class Object {

    public:

	Object() {};
	virtual ~Object() {};

	//virtual bool Intersect(const Ray &ray, Object object);
	virtual void addTransform(mat4 transform){};
	virtual LocalGeo intersect(Ray ray) {
	    LocalGeo output;
	    output.pos = vec3(NULL,NULL,NULL);
	    output.normal = vec3(NULL,NULL,NULL);
	    return output;
	};




	bool isHitObj = false;
	LocalGeo vertnorms [3];
	vec3 verts [3];
	vec4 sphereData;
	//psudo I was given had a pointer but it made no sense so it's not a pointer now
	Material material;
	mat4 transmat;

};

class Sphere : public Object {

    public:

	Sphere(float x, float y, float z, float r) {
	    sphereData = vec4(x,y,z,r);
	};

	void addTransform(mat4 transform) {
	    transmat = transform;
	}
	
	LocalGeo intersect(Ray ray);
	virtual ~Sphere(){};

};

class Triangle : public Object {

    public:

	Triangle(vec3 v1, vec3 v2, vec3 v3) {

	    verts[0] = v1;
	    verts[1] = v2;
	    verts[2] = v3;
	};

	void addTransform(mat4 transform) {
	    for (int i = 0; i < 3; i++) {
		vec4 bruh = vec4(verts[i], 1);
		bruh = transform * bruh;
		verts[i] = vec3(bruh);
	    }
	    transmat = transform;
	}
	LocalGeo intersect(Ray ray);
	virtual ~Triangle(){};

};

class Trinormal : public Object {

    public:

	Trinormal(LocalGeo v1, LocalGeo v2, LocalGeo v3) {
	    vertnorms[0] = v1;
	    vertnorms[1] = v2;
	    vertnorms[2] = v3;
	};

	void addTransform(mat4 transform) {
	    for (int i = 0; i < 3; i++) {
		vec4 bruh1 = vec4(vertnorms[i].pos, 1);
		vec4 bruh2 = vec4(vertnorms[i].normal, 1);
		bruh1 = transform * bruh1;
		bruh2 = transform * bruh2;
		vertnorms[i].pos = vec3(bruh1);
		vertnorms[i].normal = vec3(bruh2);
	    }
	    transmat = transform;
	}
	LocalGeo intersect(Ray ray);
	virtual ~Trinormal(){};

};
