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

