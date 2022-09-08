// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
    float degreesys = glm::radians(degrees);
    vec3 axis1 = glm::normalize(axis);
    mat3 bruh = mat3(cos(degreesys)) + (1 - cos(degreesys))*glm::outerProduct(axis1,axis1) +
    sin(degreesys)*mat3(0,axis1.z,-1*axis1.y,-1*axis1.z,0,axis1.x,axis1.y,-1*axis1.x,0);
    return bruh;
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
    eye = rotate(degrees, up)*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
    mat3 bruh = mat3(rotate(degrees, glm::normalize(glm::cross(eye,up))));
    eye = bruh*eye;
    up = bruh*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
    vec3 c = glm::normalize(eye - center);
    vec3 b = glm::normalize(up);
    vec3 a = glm::cross(b,c);

    mat4 bruh = mat4(1);
    bruh[3] = bruh[3] - vec4(eye, 0);
    bruh = (mat4(a.x,b.x,c.x,0,a.y,b.y,c.y,0,a.z,b.z,c.z,0,0,0,0,1))*bruh;

    return bruh;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar){
    mat4 ret;
    fovy = 0.5*glm::radians(fovy);
    float tmp = 1/(aspect*tan(fovy));
    ret[0] = vec4(tmp, 0, 0, 0);
    tmp = 1/(tan(fovy));
    ret[1] = vec4(0, tmp, 0, 0);
    tmp = -(zFar + zNear)/(zFar - zNear);
    ret[2] = vec4(0, 0, tmp, -1);
    tmp = -(2*zFar*zNear)/(zFar - zNear);
    ret[3] = vec4(0, 0, tmp, 0);

    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
    mat4 ret;
    ret = mat4(sx,0,0,0,0,sy,0,0,0,0,sz,0,0,0,0,1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
    mat4 ret;
    ret = mat4(1,0,0,0,0,1,0,0,0,0,1,0,tx,ty,tz,1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform(){

}

Transform::~Transform(){

}
