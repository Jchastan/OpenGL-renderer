#pragma once
#include "Transform.h"
#include "Object.h"

class Intersection{
    public:

	Intersection() {hitDist = std::numeric_limits<float>::max();}
	Intersection(Ray ray, float dist, Object object, LocalGeo lg);

	float hitDist;
	LocalGeo lg;
	vec3 dir;
	Material material;

	vec3 shade;


};
