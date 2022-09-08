/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"
#include <FreeImage.h>
#include "Scene.h"

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "readfile.h" // prototypes for readfile.cpp  

void saveScreenshot(string fname, Scene scene) {
    int w = scene.cam.width;
    int h = scene.cam.height;
    int pix = w*h;
    BYTE *pixels = new BYTE[3*pix];	

    //TODO check this
    pixels = scene.render();


    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

    std::cout << "Saving screenshot: " << fname << "\n";
    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
    delete[] pixels;

}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions

int main(int argc, char* argv[]) {

  if (argc < 1) {cerr << "bruh" << endl;}
  FreeImage_Initialise();
  Scene scene;
  readfile(argv[1], &scene);
  saveScreenshot("poggers.png", scene);

  FreeImage_DeInitialise();
  return 0;
}
