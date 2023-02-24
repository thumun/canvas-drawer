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
   verticies.clear(); 
}

void Canvas::save(const std::string &filename)
{
   m_img.save(filename);
}

// look back at - may have mem error
void Canvas::alphablend(Image &otherImg, float alpha)
{
   m_img = m_img.alphaBlend(otherImg, alpha);
}

void Canvas::crop(int x, int y, int w, int h)
{
   m_img = m_img.subimage(x, y, w, h);
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
   else if 
      (m_currentType == PrimitiveType::TRIANGLES && verticies.size() % 3 == 0)
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

      float t = sqrt(pow(a.x - x, 2) + pow(a.y - y, 2)) /
                sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));

      Pixel newColor;
      newColor.r = a.px.r * (1 - t) + b.px.r * (t);
      newColor.g = a.px.g * (1 - t) + b.px.g * (t);
      newColor.b = a.px.b * (1 - t) + b.px.b * (t);

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

      float t = sqrt(pow(a.x - x, 2) + pow(a.y - y, 2)) /
                sqrt(pow(b.x - a.x, 2) +
                     pow(b.y - a.y, 2));

      Pixel newColor;
      newColor.r = a.px.r * (1 - t) + b.px.r * (t);
      newColor.g = a.px.g * (1 - t) + b.px.g * (t);
      newColor.b = a.px.b * (1 - t) + b.px.b * (t);

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

            if ((alpha > 0 || implicitAlpha * 
            implicitLinewithFloat(-1.1, -2.3, b, c) > 0) && (beta > 0 
            || implicitBeta * implicitLinewithFloat(-1.1, -2.3, c, a) > 0) 
            && (gamma > 0 
               || implicitGamma * implicitLinewithFloat(-1.1, -2.3, a, b) > 0))
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

float Canvas::implicitLine(PointAndColor input, PointAndColor p1, 
                           PointAndColor p2)
{
   return ((float)((p1.y - p2.y) * input.x) 
            + (float)((p2.x - p1.x) * input.y) 
            + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
}

float Canvas::implicitLinewithFloat(float inputX, float inputY, 
                                    PointAndColor p1, PointAndColor p2)
{
   return ((float)((float)(p1.y - p2.y) * inputX) 
            + (float)((float)(p2.x - p1.x) * inputY) 
            + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
}

void Canvas::makePoint(PointAndColor a)
{
   // use set to make the point
   m_img.set(a.x, a.y, a.px);
}

void Canvas::triCircle(PointAndColor center, Pixel outer, int resolution, 
                        float radius)
{

   // resolution = how many triangles are drawn
   // 1 = 4 (one for each quadrant)

   // using unit circle to split circle

   // dividing circle into parts (360/resolution)
   float numTriangles = 360.0 / resolution;

   for (int i = 0; i < resolution; i++)
   {

      PointAndColor a = PointAndColor(center.x, center.y, center.px);
      PointAndColor b = PointAndColor(center.x + radius * cos(i * numTriangles), 
                           center.y + radius * sin(i * numTriangles), outer);
      PointAndColor c = 
               PointAndColor(center.x + radius * cos((i + 1) * numTriangles), 
                  center.y + radius * sin((i + 1) * numTriangles), outer);

      makeTriangle(a, b, c);
   }
}

// supports two colors 
void 
Canvas::maurerRose(int petals, int degrees, PointAndColor center, Pixel outer)
{

   verticies.clear(); // just in case 

   int n = petals;
   int d = degrees;

   float k = 0;
   float r = 0;
   int x = 0;
   int y = 0;

   for (int theta = 0; theta < 361; theta++)
   {
      k = theta * d * M_PI / 180;
      r = 300 * sin(n * k);
      x = (int)(r * cos(k));
      y = (int)(r * sin(k));

      verticies.push_back(PointAndColor(x + center.x, y + center.y, center.px));
   }

   // cycling through verticies made aboce to draw lines btwn them
   for (int i = 0; i < verticies.size() - 1; i++)
   {
      PointAndColor other = 
               PointAndColor(verticies[i + 1].x, verticies[i + 1].y, outer);
      bresenham(verticies[i], other);
   }
}

void Canvas::makeRectangle(PointAndColor center, int width, int height)
{

   int minX = center.x - width / 2;
   int maxX = center.x + width / 2;
   int minY = center.y - height / 2;
   int maxY = center.y + height / 2;

   PointAndColor a = PointAndColor(minX, minY, center.px);
   PointAndColor b = PointAndColor(minX, maxY, center.px);
   PointAndColor c = PointAndColor(maxX, maxY, center.px);
   PointAndColor d = PointAndColor(maxX, minY, center.px);

   makeTriangle(a, c, b);
   makeTriangle(a, d, c);
}

void Canvas::stylizedCircle(PointAndColor center, 
                           Pixel outer, int resolution, float radius)
{

   verticies.clear();

   float numTriangles = 360.0 / resolution;

   for (int i = 0; i < resolution; i++)
   {

      // finding verticies via "unit circle"
      // one vertex at center point 
      // other two along circle 

      PointAndColor a = PointAndColor(center.x, center.y, center.px);
      PointAndColor b = PointAndColor(center.x + radius * cos(i * numTriangles), 
                           center.y + radius * sin(i * numTriangles), outer);
      PointAndColor c = 
               PointAndColor(center.x + radius * cos((i + 1) * numTriangles), 
                  center.y + radius * sin((i + 1) * numTriangles), outer);

      verticies.push_back(a);
      verticies.push_back(b);
      verticies.push_back(c);
   }

   // cycles through verticies to construct triangles 
   for (int i = 0; i < verticies.size(); i += 3)
   {
      bresenham(verticies[i], verticies[i + 1]);
      bresenham(verticies[i], verticies[i + 2]);
      bresenham(verticies[i + 1], verticies[i + 2]);
   }
}

void Canvas::makeRandomPoints(Pixel cl, int width, int height)
{

   int random = 0;

   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
      {
         random = rand() % 100;
         if (random <= 1)  // lowered density to look like stars in sky 
         {
            makePoint(PointAndColor(i, j, cl));
         }
      }
   }
}
