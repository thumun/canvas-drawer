#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{

   Canvas drawer(100, 100);
   drawer.background(60,35,49);
   drawer.save("test.png"); 
   // drawer.background(0,0,0);

   // drawer.begin(LINES);

   // drawer.color(255,0,0);
   // drawer.vertex(0,0);
   // drawer.vertex(100,0);

   // drawer.color(0,255,0);
   // drawer.vertex(0, 0);
   // drawer.vertex(0,100);

   // drawer.end();

   // Canvas drawer(640, 380);
   // your code here
}

