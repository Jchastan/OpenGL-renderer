# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal; 
in vec4 myvertex; 

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess. 
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

        vec4 retval = lambert + phong ; 
        return retval ;            
}   

void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor;
	
	vec3 normal = mat3(transpose(inverse(modelview))) * mynormal;
	vec4 vertex = modelview * myvertex;

	const vec3 eyepos = vec3(0,0,0);
	vec3 mypos = vertex.xyz/vertex.w;
	vec3 eyedirn = normalize(eyepos - mypos);
	normal = normalize(normal);


	int i;
	for (i = 0; i < numused; i++) {
	    if (lightposn[i].w != 0) {
		vec3 position = lightposn[i].xyz / lightposn[i].w;
		vec3 direction = normalize(position - mypos);
		vec3 half = normalize(direction + eyedirn);
		finalcolor = finalcolor + ComputeLight(direction, lightcolor[i], normal, half, diffuse, specular, shininess);
	    }
	    else {
		vec3 direction = normalize(lightposn[i].xyz);
		vec3 half = normalize(direction + eyedirn);
		finalcolor = finalcolor + ComputeLight(direction, lightcolor[i], normal, half, diffuse, specular, shininess);
	    }
	}

	finalcolor = finalcolor + ambient + emission;

        fragColor = finalcolor; 
    } else {
        fragColor = vec4(color, 1.0f); 
    }
}
