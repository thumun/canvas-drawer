#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{

   // would be cool to add jitter or something 

   // display of maurer rose
   Canvas drawer(800, 800);
   drawer.background(0, 0, 0);
   drawer.maurerRose(6, 71,  PointAndColor(400, 400, Pixel{210, 185, 255}));
   // drawer.maurerRose(3, 47,  PointAndColor(400, 400, Pixel{0, 0, 0}));

   drawer.save("rose.png");

   // ----------------------------------------------------------------

   // want to put clouds in the background 

   // ~All-seeing~
   // Canvas drawer(800, 800);
   // drawer.background(1, 1, 1);

   // // Image img;
   // // img.load("../img/cloud.png");
   // // drawer.alphaBlend(img, 0.5); // how to easily have img func in canvas class?

   // // why is there a repetition --> half circle still visible 
   // drawer.triCircle(PointAndColor(400, 400, Pixel{174, 4, 204}), Pixel{54,1,63}, 1000, 300);
   // drawer.stylizedCircle(PointAndColor(400, 400, Pixel{1, 1, 1}), Pixel{54,1,63}, 1000, 300);
   // drawer.save("eyeball.png");

   // ----------------------------------------------------------------

   // maybe blend w/ bg ? 

   // Polka Dots 
   // stole palette from here:https://www.schemecolor.com/pastel-color-tones.php 
   // Canvas drawer(800, 800);

   // vector<Pixel> colors; 
   // colors.push_back(Pixel{224, 187, 228});
   // colors.push_back(Pixel{149, 125, 173});
   // colors.push_back(Pixel{210, 145, 188});
   // colors.push_back(Pixel{254, 200, 216});
   // colors.push_back(Pixel{255, 223, 211});
   // colors.push_back(Pixel{254, 200, 216});
   // colors.push_back(Pixel{210, 145, 188});
   // colors.push_back(Pixel{149, 125, 173});
   // colors.push_back(Pixel{224, 187, 228});

   // int counter = 0; 

   // for (int i = 0; i < 800; i+=100){
   //    for (int j = 0; j < 800; j+=100){
   //       drawer.triCircle(PointAndColor(i, j, colors[counter]), colors[counter+1], 360, 50);
   //       counter += 1;  
   //    }
   //    counter = 0; 
   // }

   // drawer.save("pokatest2.png");

}

