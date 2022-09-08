/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h" 
#include "Scene.h"

using namespace std;
#include "readfile.h"

int numused = 0;
int numobjects = 0;
int numvertex = 0;
int numvertexnormal = 0;

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat* values) 
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename, Scene* scene) 
{
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    cout << filename << endl;
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if (!((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))) {
	    }
	    else {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                int i; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "directional") {
                    if (numused == 4096) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {
			    Light light;
			    light.lightvec = vec3(values[0], values[1], values[2]);
			    light.colorvec = vec3(values[3], values[4], values[5]);
			    light.isPoint = false;
			    scene->lights.push_back(light);
                            ++numused; 
                        }
                    }
                }
                else if (cmd == "point") {
                    if (numused == 4096) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {
			    Light light;
			    light.lightvec = vec3(values[0], values[1], values[2]);
			    light.colorvec = vec3(values[3], values[4], values[5]);
			    light.isPoint = true;
			    scene->lights.push_back(light);
                            ++numused; 
                        }
                    }
                }

                else if (cmd == "maxverts") {
                    }
                else if (cmd == "maxvertnorms") {
                    }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
			scene->vertices.push_back(vec3(values[0],values[1],values[2]));
			numvertex++;
                    }
		}
                else if (cmd == "vertexnormal") {
                    validinput = readvals(s, 6, values); // colors 
                    if (validinput) {
			LocalGeo tmp;
			tmp.pos = vec3(values[0],values[1],values[2]);
			tmp.normal = vec3(values[3],values[4],values[5]);
			scene->vertexnormals.push_back(tmp);
			numvertexnormal++;
                    }
		}

                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
			scene->material.ambient = vec3(values[0],values[1],values[2]);
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
			scene->material.diffuse = vec3(values[0],values[1],values[2]);
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
			scene->material.specular = vec3(values[0],values[1],values[2]);
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
			scene->material.emission = vec3(values[0],values[1],values[2]);
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
			scene->material.shininess = values[0];
                    }
                } else if (cmd == "size") {
		    cout << "bruh" << endl;
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        scene->cam.width = (int) values[0];
			scene->cam.height = (int) values[1]; 
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {

			scene->cam = Camera(vec3(values[0],values[1],values[2]),
					    vec3(values[3],values[4],values[5]),
					    vec3(values[6],values[7],values[8]));

			
			/*
			memcpy(&eyeinit, &values[0], sizeof(float)*3);
			memcpy(&center, &values[3], sizeof(float)*3);
			memcpy(&upinit, &values[6], sizeof(float)*3);
			*/
			//scene->cam.up = Transform::upvector(upinit, eyeinit);
			scene->cam.fovy = glm::radians(values[9]);
			scene->cam.aspect = scene->cam.width/scene->cam.height;
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly. 
                        // Set eyeinit upinit center fovy in variables.h 

                    }
                }

                // I've left the code for loading objects in the skeleton, so 
                // you can get a sense of how this works.  
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere" || cmd == "tri" || cmd == "trinormal") {
                    if (numobjects == 4096) { // No more objects 
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                    } else {
                        validinput = readvals(s, 3, values); 
                        bool validishinput = readvals(s, 4, values); 
                        if (validinput || validishinput) {
			    int thisIsWhyJavaIsBetter = 5;
			    if(cmd == "sphere") {thisIsWhyJavaIsBetter = 0;}
			    if (cmd == "tri") {thisIsWhyJavaIsBetter = 0;}
			    if (cmd == "trinormal") {thisIsWhyJavaIsBetter = 0;}
			    switch(thisIsWhyJavaIsBetter) {
				case 0:
				    scene->objects.push_back(Sphere(values[0], values[1], values[2], values[3]));
				    //make sphere
				    break;
				case 1:
				    scene->objects.push_back(Triangle(scene->vertices[values[0]],
								      scene->vertices[values[1]],
								      scene->vertices[values[2]]));
				    //make tri
				    break;
				case 2:
				    scene->objects.push_back(Trinormal(scene->vertexnormals[values[0]],
								       scene->vertexnormals[values[1]],
								       scene->vertexnormals[values[2]]));
				    //make trinormal
				    break;
			    }
                            Object* obj = &(scene->objects[numobjects]); 

                            // Set the object's light properties
                            obj->material = scene->material; 

                            // Set the object's transform
                            obj->addTransform(transfstack.top()); 

			    ++numobjects; 
                        }
                    }
                }

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
			mat4 tmp = Transform::translate(values[0],values[1],values[2]);
			rightmultiply(tmp, transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
			mat4 tmp = Transform::scale(values[0],values[1],values[2]);
			rightmultiply(tmp, transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {
			vec3 tmp = vec3(values[0],values[1],values[2]);
			mat4 rmat4;
			mat3 rmat3 = Transform::rotate(values[3],tmp);
			rmat4[0] = vec4(rmat3[0], 0);
			rmat4[1] = vec4(rmat3[1], 0);
			rmat4[2] = vec4(rmat3[2], 0);
			rmat4[3] = vec4(0,0,0,1);
			rightmultiply(rmat4, transfstack);
                        // values[0..2] are the axis, values[3] is the angle.  
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.  
                        // Note that rotate returns a mat3. 
                        // Also keep in mind what order your matrix is!

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
