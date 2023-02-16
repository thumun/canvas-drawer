#include "canvas.h"
#include <cassert>

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : m_img(w, h)
{
   m_width = w; 
   m_height = h; 
 
}

Canvas::~Canvas()
{

}

void Canvas::save(const std::string& filename)
{
   m_img.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
   m_currentType = type; 
   verticies.clear();   // in case type is changed before previous shape is drawn
}

void Canvas::end()
{
   // draw everything as lines
   // when drawing lines --> need to take into account color ratio to create blend
   // if it's a triangle --> fill algorithm 
   if (m_currentType == PrimitiveType::LINES && verticies.size() == 2){
      // call my implicit line equ 
      bresenham(verticies[0], verticies[1]);
   } 
   else if (m_currentType == PrimitiveType::TRIANGLES && verticies.size() == 3){
      // call my draw triangle equ 
   }
}

void Canvas::vertex(int x, int y)
{
   verticies.push_back(PointAndColor(x, y, m_currColor));
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   // m_var that holds curr color 
   m_currColor.r = r;
   m_currColor.g = g;
   m_currColor.b = b;

}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   Pixel bg;
   bg.r = r;
   bg.g = g; 
   bg.b = b; 

   m_img.fill(bg);
  
}

// seems like my axes are swapped, is it supposed to be that way 
// fix: swap a.x and b.x 
// a.y and b.y 
void Canvas::bresenham(PointAndColor a, PointAndColor b)
{
   int W = b.x - a.x;
   int H = b.y - a.y;

   if (abs(H) < abs(W)){
      if (a.x > b.x) {
         drawlineLow(b, a);
      } else {
         drawlineLow(a, b);
      }

   } else {
      if (a.y > b.y){
         drawlineHigh(b, a);
      } else {
         drawlineHigh(a, b);
      }
   }  
}

// gotta test this !!
void Canvas::drawlineHigh(PointAndColor a, PointAndColor b)
{
   int x = a.x;
   int W = b.x - a.x;
   int H = b.y - a.y;
   int dx = 1; // what is this 

   if (W < 0){
      dx = -1; 
      W = -1*W;
   }

   int F = 2*W - H; 

   for (int y = a.y; y <= b.y; y++){

      float t = sqrt(pow(verticies[0].x-x, 2) + pow(verticies[0].y-y, 2))
         /sqrt(pow(verticies[1].x-verticies[0].x, 2) 
         + pow(verticies[1].y-verticies[0].y, 2));

      Pixel newColor; 
      newColor.r = verticies[0].px.r*(1-t) + verticies[1].px.r*(t);
      newColor.g = verticies[0].px.g*(1-t) + verticies[1].px.g*(t);
      newColor.b = verticies[0].px.b*(1-t) + verticies[1].px.b*(t);

      m_img.set(y, x, newColor);

      if (F > 0){
         x += dx; 
         F += 2*(W-H);
      } else {
         F += 2*W;
      }
   }
}

void Canvas::drawlineLow(PointAndColor a, PointAndColor b)
{
   int y = a.y;
   int W = b.x - a.x; 
   int H = b.y - a.y; 
   int dy = 1;
   if (H < 0){
      dy = -1;
      H = -1*H;
   }
  
   int F = 2*H - W;
   for (int x = a.x; x <= b.x; x++){

      float t = sqrt(pow(verticies[0].x-x, 2) + pow(verticies[0].y-y, 2))
               /sqrt(pow(verticies[1].x-verticies[0].x, 2) 
               + pow(verticies[1].y-verticies[0].y, 2));

      Pixel newColor; 
      newColor.r = verticies[0].px.r*(1-t) + verticies[1].px.r*(t);
      newColor.g = verticies[0].px.g*(1-t) + verticies[1].px.g*(t);
      newColor.b = verticies[0].px.b*(1-t) + verticies[1].px.b*(t);

      m_img.set(y, x, newColor); 
      
      if (F > 0){
         y+=dy;
         F += 2*(H-W);
      } else {
         F += 2*H;
      }
   }   
}


