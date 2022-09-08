#include "Scene.h"

unsigned char* Scene::render() {
    int height = this->cam.height;
    int width = this->cam.width;
    unsigned char* image = (unsigned char*)calloc(width*height*3, sizeof(unsigned char));
    for (int i=0; i<height; i++) {
	for (int j=0; j<width; j++) {
	    Ray ray = cam.getRay(i, j);
	    Intersection hit = this->intersect(ray);
	    vec3 color = cam.findColor(hit);
	    image[i*width*3+width*3] = color.x;
	    image[i*width*3+width*3+1] = color.y;
	    image[i*width*3+width*3+2] = color.z;
	}
    }
    return image;
}

Intersection::Intersection(Ray ray, float dist, Object obj, LocalGeo lg) {
    hitDist = dist;
    this->lg = lg;
    dir = ray.dir;
    material = obj.material;
}

Intersection Scene::intersect(Ray ray) {
    float mindist = std::numeric_limits<float>::max();
    Object hitobject;
    LocalGeo pog;
    for (Object bruh : this->objects) {
	LocalGeo tmp = bruh.intersect(ray);
	float t = glm::dot(pog.pos - ray.pos, ray.dir);
	if (t>0 && t<mindist) {
	    mindist = t;
	    hitobject = bruh;
	    hitobject.isHitObj = true;
	    pog = tmp;
	}
    }
    Intersection poggers(ray, mindist, hitobject, pog);
    if (hitobject.isHitObj) {
	return poggers;
    }
    else {
	Intersection bruh;
	return bruh;
    }

}


LocalGeo Triangle::intersect(Ray ray) {

    Triangle tri = *this;

    mat4 bruh = mat4(vec4(tri.verts[0],1), vec4(tri.verts[1],1), vec4(tri.verts[2],1), vec4(-ray.dir, 0));
    vec4 pogchamp = vec4(ray.pos,1);
    pogchamp = glm::inverse(bruh) * pogchamp;
    LocalGeo poggers;
    poggers.pos = ray.pos + pogchamp.w * ray.dir;
    poggers.normal = glm::cross(tri.verts[1] - tri.verts[0], tri.verts[2] - tri.verts[0]);
    if (pogchamp.x > 0 && pogchamp.y > 0 && pogchamp.z > 0) {
	return poggers;
    }
    else {
	LocalGeo bruh;
	bruh.pos = vec3(NULL,NULL,NULL);
	bruh.normal = vec3(NULL,NULL,NULL);
	return bruh;
    }
}

LocalGeo Sphere::intersect(Ray ray) {

    Sphere sphere = *this;

    mat4 inverse = glm::inverse(sphere.transmat);
    mat3 tmp = mat3(vec3(inverse[0]),vec3(inverse[1]),vec3(inverse[2]));

    vec3 d = tmp*(ray.dir);
    vec3 p = vec3(glm::inverse(sphere.transmat)*(vec4(ray.pos, 1)));

    float a = glm::dot(ray.dir,ray.dir);
    float b = glm::dot(2.0f*(p - vec3(sphere.sphereData)), d);
    float c = glm::dot(ray.pos - vec3(sphere.sphereData), ray.pos - vec3(sphere.sphereData)) -
		      (sphere.sphereData.w * sphere.sphereData.w);
    float determinate = (b*b) - 4.0*a*c;
    if (determinate > 0) {
	float t = (-1*b + (float)sqrt(determinate))/(2.0*a);
	if ((-1*b - (float)sqrt(determinate))/(2.0*a) < t && (-b - (float)sqrt(determinate))/(2.0*a) > 0) {
	    t = (-b - (float)sqrt(determinate))/(2.0*a);
	}
	vec3 point = p + (t*d);
	vec3 normal = point - vec3(sphere.sphereData);
	tmp = glm::transpose(mat3(vec3(transmat[0]), vec3(transmat[1]), vec3(transmat[2])));
	point = tmp*point;
	LocalGeo bruh;
	bruh.pos = point;
	bruh.normal = normal;
	return bruh;
    }

    LocalGeo bruh;
    bruh.pos = vec3(NULL,NULL,NULL);
    bruh.normal = vec3(NULL,NULL,NULL);
    return bruh;

}
LocalGeo Trinormal::intersect(Ray ray) {

    Trinormal trinormal = *this;

    mat4 bruh = mat4(vec4(trinormal.vertnorms[0].pos,1),
		     vec4(trinormal.vertnorms[1].pos,1),
		     vec4(trinormal.vertnorms[2].pos,1),
		     vec4(-(ray.dir), 0));
    vec4 pogchamp = vec4(ray.pos,1);
    pogchamp = glm::inverse(bruh) * pogchamp;
    LocalGeo poggers;
    poggers.pos = ray.pos + ray.dir*pogchamp.w;
    poggers.normal = glm::normalize(trinormal.vertnorms[0].normal * pogchamp.x + 
				    trinormal.vertnorms[1].normal * pogchamp.y +
				    trinormal.vertnorms[2].normal * pogchamp.z);
    if (pogchamp.x > 0 && pogchamp.y > 0 && pogchamp.z > 0) {
	return poggers;
    }
    else {
	LocalGeo bruh;
	bruh.pos = vec3(NULL,NULL,NULL);
	bruh.normal = vec3(NULL,NULL,NULL);
	return bruh;
    }

}
