#include "Camera.h"

Camera::Camera(vec3 eye, vec3 center, vec3 up) {
    this->pos = eye;
    w = glm::normalize(eye - center);
    u = glm::normalize(glm::cross(up, w));
    v = glm::cross(w,u);

    this->buildRayArray();

}

void Camera::buildRayArray() {

    vector<vector<Ray>> bruh;
    float w = (float)this->width;
    float h = (float)this->height;
    for (float i = 0; i < height; i++){
    vector<Ray> subBruh;
	for (float j = 0; j < width; j++){
	    float a = ((j - (w/2.0f)/(w/2.0f))*(tan(fovy * aspect)));
	    float b = ((i - (h/2.0f)/(h/2.0f))*(tan(fovy)));
	    vec3 pixelPos = pos + a*u + b*v - w;
	    Ray newRay;
	    newRay.dir = glm::normalize(pixelPos - pos);
	    newRay.pos = pos;
	    subBruh.push_back(newRay);
	}
	bruh.push_back(subBruh);
    }
    rayArray = bruh;
}

Ray Camera::getRay(int i, int j) {
    return rayArray[i][j];
}

vec3 Camera::findColor(Intersection hit) {
    if (hit.hitDist == std::numeric_limits<float>::max()) {
	return vec3(0,0,0);
    }
    else {
	return hit.material.ambient;
    }
}
