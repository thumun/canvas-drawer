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

void Canvas::save(const std::string &filename)
{
   m_img.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
   m_currentType = type;
   verticies.clear(); // in case type is changed before previous shape is drawn
}

void Canvas::end()
{

   if (m_currentType == PrimitiveType::LINES && verticies.size() % 2 == 0)
   {
      for (int i = 0; i < verticies.size(); i += 2)
      {
         bresenham(verticies[i], verticies[i + 1]);
      }
   }
   else if (m_currentType == PrimitiveType::TRIANGLES && verticies.size() % 3 == 0)
   {
      for (int i = 0; i < verticies.size(); i += 3)
      {
         makeTriangle(verticies[i], verticies[i + 1], verticies[i + 2]);
      }
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

   if (abs(H) < abs(W))
   {
      if (a.x > b.x)
      {
         drawlineLow(b, a);
      }
      else
      {
         drawlineLow(a, b);
      }
   }
   else
   {
      if (a.y > b.y)
      {
         drawlineHigh(b, a);
      }
      else
      {
         drawlineHigh(a, b);
      }
   }
}

void Canvas::drawlineHigh(PointAndColor a, PointAndColor b)
{
   int x = a.x;
   int W = b.x - a.x;
   int H = b.y - a.y;
   int dx = 1; // what is this

   if (W < 0)
   {
      dx = -1;
      W = -1 * W;
   }

   int F = 2 * W - H;

   for (int y = a.y; y <= b.y; y++)
   {

      float t = sqrt(pow(verticies[0].x - x, 2) + pow(verticies[0].y - y, 2)) / sqrt(pow(verticies[1].x - verticies[0].x, 2) + pow(verticies[1].y - verticies[0].y, 2));

      Pixel newColor;
      newColor.r = verticies[0].px.r * (1 - t) + verticies[1].px.r * (t);
      newColor.g = verticies[0].px.g * (1 - t) + verticies[1].px.g * (t);
      newColor.b = verticies[0].px.b * (1 - t) + verticies[1].px.b * (t);

      m_img.set(y, x, newColor);

      if (F > 0)
      {
         x += dx;
         F += 2 * (W - H);
      }
      else
      {
         F += 2 * W;
      }
   }
}

void Canvas::drawlineLow(PointAndColor a, PointAndColor b)
{
   int y = a.y;
   int W = b.x - a.x;
   int H = b.y - a.y;
   int dy = 1;
   if (H < 0)
   {
      dy = -1;
      H = -1 * H;
   }

   int F = 2 * H - W;
   for (int x = a.x; x <= b.x; x++)
   {

      float t = sqrt(pow(verticies[0].x - x, 2) + pow(verticies[0].y - y, 2)) / sqrt(pow(verticies[1].x - verticies[0].x, 2) + pow(verticies[1].y - verticies[0].y, 2));

      Pixel newColor;
      newColor.r = verticies[0].px.r * (1 - t) + verticies[1].px.r * (t);
      newColor.g = verticies[0].px.g * (1 - t) + verticies[1].px.g * (t);
      newColor.b = verticies[0].px.b * (1 - t) + verticies[1].px.b * (t);

      m_img.set(y, x, newColor);

      if (F > 0)
      {
         y += dy;
         F += 2 * (H - W);
      }
      else
      {
         F += 2 * H;
      }
   }
}

/*
void Canvas::makeTriangle(PointAndColor a, PointAndColor b, PointAndColor c)
{
   int ymin = min(a.y, b.y);
   ymin = min(ymin, c.y);

   int ymax = max(a.y, b.y);
   ymax = max(ymax, c.y);

   int xmin = min(a.x, b.x);
   xmin = min(xmin, c.x);

   int xmax = max(a.x, b.x);
   xmax = max(xmax, c.x);

   for (int y = ymin; y < ymax; y++)
   {
      for (int x = xmin; x < xmax; x++)
      {

         PointAndColor pt(x, y, m_currColor);

         float alpha = implicitLine(pt, b, c) / implicitLine(a, b, c);
         float beta = implicitLine(pt, c, a) / implicitLine(b, c, a);
         float gamma = implicitLine(pt, a, b) / implicitLine(c, a, b);

         if (alpha > 0 && beta > 0 && gamma > 0)
         {
            Pixel newColor;

            newColor.r = a.px.r * alpha + b.px.r * beta + c.px.r * gamma;
            newColor.g = a.px.g * alpha + b.px.g * beta + c.px.g * gamma;
            newColor.b = a.px.b * alpha + b.px.b * beta + c.px.b * gamma;

            m_img.set(y, x, newColor);
         }
      }
   }
}
*/

void Canvas::makeTriangle(PointAndColor a, PointAndColor b, PointAndColor c)
{
   int ymin = min(a.y, b.y);
   ymin = min(ymin, c.y);

   int ymax = max(a.y, b.y);
   ymax = max(ymax, c.y);

   int xmin = min(a.x, b.x);
   xmin = min(xmin, c.x);

   int xmax = max(a.x, b.x);
   xmax = max(xmax, c.x);

   float implicitAlpha = implicitLine(a, b, c);
   float implicitBeta = implicitLine(b, c, a);
   float implicitGamma = implicitLine(c, a, b);

   for (int y = ymin; y < ymax; y++)
   {
      for (int x = xmin; x < xmax; x++)
      {

         PointAndColor pt(x, y, m_currColor);

         float alpha = implicitLine(pt, b, c) / implicitAlpha;
         float beta = implicitLine(pt, c, a) / implicitBeta;
         float gamma = implicitLine(pt, a, b) / implicitGamma;

         if (alpha >= 0 && beta >= 0 && gamma >= 0)
         {

            // PointAndColor offscreen(-1, -1, m_currColor);

            if ((alpha > 0 || implicitAlpha*implicitLinewithFloat(-1.2,-1.2,b,c) > 0) 
                  && (beta > 0 || implicitBeta*implicitLinewithFloat(-1.2,-1.2,c,a) > 0)
                  && (gamma > 0 || implicitGamma*implicitLinewithFloat(-1.2,-1.2,a,b) > 0))
            {
               Pixel newColor;

               newColor.r = a.px.r * alpha + b.px.r * beta + c.px.r * gamma;
               newColor.g = a.px.g * alpha + b.px.g * beta + c.px.g * gamma;
               newColor.b = a.px.b * alpha + b.px.b * beta + c.px.b * gamma;

               m_img.set(y, x, newColor);
            }
         }
      }
   }
}


float Canvas::implicitLine(PointAndColor input, PointAndColor p1, PointAndColor p2)
{
   return ((float)((p1.y - p2.y) * input.x) + (float)((p2.x - p1.x) * input.y) + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
}

float Canvas::implicitLinewithFloat(float inputX, float inputY, PointAndColor p1, PointAndColor p2)
{
   return ((float)((float)(p1.y - p2.y) * inputX) + (float)((float)(p2.x - p1.x) * inputY) + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
}
