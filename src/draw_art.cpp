#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{

   // would be cool to add jitter or something 

   // display of maurer rose
   // Canvas drawer(800, 800);
   // drawer.background(0, 0, 0);
   // drawer.maurerRose(6, 71,  PointAndColor(400, 400, Pixel{52, 235, 164}), Pixel{235, 52, 213});
   // drawer.maurerRose(5, 97,  PointAndColor(0, 0, Pixel{245, 39, 12}), Pixel{52, 158, 235});
   // drawer.maurerRose(5, 97,  PointAndColor(0, 800, Pixel{245, 39, 12}), Pixel{52, 158, 235});
   // drawer.maurerRose(5, 97,  PointAndColor(800, 0, Pixel{245, 39, 12}), Pixel{52, 158, 235});
   // drawer.maurerRose(5, 97,  PointAndColor(800, 800, Pixel{245, 39, 12}), Pixel{52, 158, 235});


   // drawer.save("rose.png");

   // ----------------------------------------------------------------

   // ~All-seeing~
   // Canvas drawer(800, 800);
   // drawer.background(255, 255, 255);

   // Image img;
   // img.load("../img/cloud.png"); // img problem (?) 
   // drawer.alphablend(img, 0.5); 

   // // why is there a repetition --> half circle still visible 
   // drawer.triCircle(PointAndColor(400, 400, Pixel{245, 171, 12}), Pixel{12, 206, 245}, 1000, 300);
   // drawer.stylizedCircle(PointAndColor(400, 400, Pixel{1, 1, 1}), Pixel{255, 230, 153}, 1000, 300);
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

   // ----------------------------------------------------------------

   // flag of Kurdistan 

   // Canvas drawer(800, 500);
   // drawer.makeRectangle(PointAndColor(400, 50, Pixel{245, 39, 12}), 805, 200);
   // drawer.makeRectangle(PointAndColor(400, 450, Pixel{41, 107, 46}), 805, 200);
   // drawer.triCircle(PointAndColor(400, 250, Pixel{245, 183, 69}), Pixel{245, 183, 69}, 40, 150);
   // drawer.save("kurdistan.png");

}

