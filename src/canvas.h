/*-----------------------------------------------
 * Author: Neha Thumu
 * Date: 2/13/2023
 * Description:
 * This canvas class handles the various functions involved in drawing lines
 * and triangles. There is also the option to set a background color and to set
 * color(s) for lines and triangles. (multiple colors for a line/triangle will
 * have gradient)
 * To draw a line: you need two verticies.
 * To draw a triangle: you need three verticies.
 * End() will draw the line/triangles based on the verticies.
 * Begin() is the way to set if you want to draw a line or triangle.
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include <cmath>
#include "image.h"

namespace agl
{
   enum PrimitiveType
   {
      UNDEFINED,
      LINES,
      TRIANGLES,
      POINTS   
   };
   struct PointAndColor
   {
      int x;
      int y;
      Pixel px;

      PointAndColor(int _x, int _y, Pixel _px)
      {
         x = _x;
         y = _y;
         px = _px;
      }
   };
   class Canvas
   {
   public:
      Canvas(int w, int h);
      virtual ~Canvas();

      // Save to file
      void save(const std::string &filename);

      // allows alphablend from image.h 
      void alphablend(Image &otherImg, float alpha);

      // ports over subImage from image.h 
      void crop(int x, int y, int w, int h);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // https://en.wikipedia.org/wiki/Maurer_rose 
      // used above link for base formula to create maurer rose 
      void maurerRose(int petals, int degrees, PointAndColor center, Pixel outer);

      // creates circle from triangles :) 
      void triCircle(PointAndColor center, Pixel outer, int resolution, float radius);

      // circle from lines (lines are drawing edges of triangles)
      void stylizedCircle(PointAndColor center, Pixel outer, int resolution, float radius);

      // draws a rectangle based on center point, width, and height
      void makeRectangle(PointAndColor center, int width, int height);

   private:
      Image m_img;
      int m_width; 
      int m_height;
      Pixel m_currColor;
      PrimitiveType m_currentType;
      std::vector<PointAndColor> verticies;

      // draws line from a to b
      void bresenham(PointAndColor a, PointAndColor b);

      // handles case where line slope is positive 
      void drawlineHigh(PointAndColor a, PointAndColor b);

      // handles case where line slope is negative 
      void drawlineLow(PointAndColor a, PointAndColor b);

      // draws a triangle based on three vertices
      void makeTriangle(PointAndColor a, PointAndColor b, PointAndColor c);

      // the implicit line formula 
      // used in makeTriangle to calculate alpha, beta, and gamma
      float implicitLine(PointAndColor input, PointAndColor p1, PointAndColor p2);

      float implicitLinewithFloat(float inputx, float inputy, PointAndColor p1, PointAndColor p2);

      // coloring given vertex 
      void makePoint(PointAndColor a);
      
   };
}

#endif
