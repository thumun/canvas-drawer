#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Canvas drawer(800, 800);
   // drawer.maurerRose(6, 71);
   // Pixel testPx; 
   // testPx.r = 210/255; 
   // testPx.g = 185/255; 
   // testPx.b = 255/255; 

   drawer.stylizedCircle(PointAndColor(400, 400, Pixel{210, 185, 255}), 1000, 100);
   drawer.save("test.png");

   // your code here
}

