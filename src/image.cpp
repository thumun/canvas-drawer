// Copyright 2021, Aline Normoyle, alinen
/**
 * This is the img operations file for Assignment 03: Canvas-Drawer.
 *
 * (copied from A02)
 * It has various operations/filters that can be applied to images. 
 * The operations are: resize, fliphorizontal, subimage, replace, swirl, add
 * subtract, difference, inverse, multiply, screen, darkest, lightest, 
 * gammacorrect, alphablend, greyscale, sepia, gaussian blur, film grain, 
 * overlay, and pixelate.
 * 
 * There is also the ability to save and load images.  
 *
 * @author: Neha Thumu
 * @version: Feburary 16, 2023
 */

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <cmath>
#include <vector>

namespace agl
{

   // extension func since std::clamp not working
   float clamp(float val, float min, float max)
   {
      return val < min ? min : (val > max ? max : val);
   }

   Image::Image()
   {
      m_channels = 3;
      m_data = nullptr;
   }

   Image::Image(int width, int height)
   {
      m_width = width;
      m_height = height;
      m_channels = 3;
      // if (m_data != nullptr)
      // {
      //    delete m_data;
      // }
      m_data = new char[m_width * m_height * m_channels];
      for (int i = 0; i < m_width * m_height * m_channels; i++)
      {
         m_data[i] = -1;
      }
   }

   Image::Image(const Image &orig)
   {
      m_width = orig.m_width;
      m_height = orig.m_height;
      m_channels = orig.m_channels;
      // if (m_data != nullptr)
      // {
      //    delete m_data;
      // }
      m_data = new char[m_width * m_height * m_channels];
      for (int i = 0; i < m_width * m_height * m_channels; i++)
      {
         m_data[i] = orig.m_data[i];
      }
   }

   Image &Image::operator=(const Image &orig)
   {
      if (&orig == this)
      {
         return *this;
      }

      if (m_data != nullptr)
      {
         delete[] m_data;
      }

      m_width = orig.m_width;
      m_height = orig.m_height;
      m_channels = orig.m_channels;
      m_data = new char[m_width * m_height * m_channels];
      for (int i = 0; i < m_width * m_height * m_channels; i++)
      {
         m_data[i] = orig.m_data[i];
      }

      return *this;
   }

   Image::~Image()
   {
      if (m_data != nullptr)
      {
         delete[] m_data;
         m_data = nullptr;
      }
   }

   int Image::width() const
   {
      return m_width;
   }

   int Image::height() const
   {
      return m_height;
   }

   char *Image::data() const
   {
      return m_data;
   }

   void Image::set(int width, int height, unsigned char *data)
   {
   }

   bool Image::load(const std::string &filename, bool flip)
   {
      stbi_set_flip_vertically_on_load(flip);
      char *tempData = (char *)stbi_load(filename.c_str(),
                                         &m_width, &m_height, &m_channels, 3);

      if (m_data != nullptr)
      {
         delete[] m_data;
      }

      if (tempData != nullptr)
      {
         m_data = new char[m_width * m_height * m_channels];
         for (int i = 0; i < m_width * m_height * m_channels; i++)
         {
            m_data[i] = tempData[i];
         }
         stbi_image_free(tempData);
         return true;
      }

      m_data = nullptr;
      return false;
   }

   bool Image::save(const std::string &filename, bool flip) const
   {
      stbi_flip_vertically_on_write(flip);
      return stbi_write_png(filename.c_str(), m_width, m_height,
                            m_channels, m_data, m_width * m_channels);
   }

   Pixel Image::get(int row, int col) const
   {
      char red = m_data[m_channels * row * m_width + m_channels * col];
      char green = m_data[m_channels * row * m_width + m_channels * col + 1];
      char blue = m_data[m_channels * row * m_width + m_channels * col + 2];

      return 
         Pixel{(unsigned char)red, (unsigned char)green, (unsigned char)blue};
   }

   void Image::set(int row, int col, const Pixel &color)
   {
      if (row < m_height && col < m_width){
         m_data[m_channels * row * m_width + m_channels * col] = (char)color.r;
         m_data[m_channels * row * m_width + m_channels * col + 1] = (char)color.g;
         m_data[m_channels * row * m_width + m_channels * col + 2] = (char)color.b;
      }
   }

   Pixel Image::get(int i) const
   {
      return Pixel{0, 0, 0};
   }

   void Image::set(int i, const Pixel &c)
   {
   }

   Image Image::resize(int w, int h) const
   {

      Image result(w, h);

      float widthProp;
      float heightProp;
      int origX;
      int origY;

      for (int i = 0; i < h; i++)
      {
         for (int j = 0; j < w; j++)
         {
            widthProp = j / (float)(w - 1);
            heightProp = i / (float)(h - 1);

            origX = floor(heightProp * (m_height - 1));
            origY = floor(widthProp * (m_width - 1));

            result.set(i, j, get(origX, origY));
         }
      }

      return result;
   }

   Image Image::flipHorizontal() const
   {

      Image result(m_width, m_height);
      int mid = m_width / 2;

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < mid; j++)
         {

            Pixel pixel = get(i, j);
            Pixel flipped = get(i, m_width - j - 1);

            result.set(i, j, flipped);
            result.set(i, m_width - j - 1, pixel);
         }
      }

      if (m_width % 2 != 0)
      {
         for (int i = 0; i < m_height; i++)
         {
            result.set(i, mid, get(i, mid));
         }
      }

      return result;
   }

   Image Image::flipVertical() const
   {
      Image result(0, 0);
      return result;
   }

   Image Image::rotate90() const
   {
      Image result(0, 0);

      return result;
   }

   Image Image::subimage(int startx, int starty, int w, int h) const
   {

      Image sub(w, h);

      for (int i = 0; i < h; i++)
      {
         for (int j = 0; j < w; j++)
         {
            Pixel pixel = get(startx + i, starty + j);
            sub.set(i, j, pixel);
         }
      }

      return sub;
   }

   void Image::replace(const Image &image, int startx, int starty)
   {

      for (int i = startx; i < startx + image.height(); i++)
      {
         for (int j = starty; j < starty + image.width(); j++)
         {
            // in case new img boundaries bigger than base img
            if (i < m_height && j < m_width)
            {
               set(i, j, image.get(i - startx, j - starty));
            }
         }
      }
   }

   Image Image::swirl() const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {
            Pixel curr = get(i, j);

            curr.g = curr.b;
            curr.b = curr.r;
            curr.r = get(i, j).g;

            result.set(i, j, curr);
         }
      }

      return result;
   }

   Image Image::add(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < other.height(); i++)
      {
         for (int j = 0; j < other.width(); j++)
         {
            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel resultPx;
            // resultPx.r = std::clamp(currPx.r + otherPx.r, 0, 255);
            resultPx.r = currPx.r + otherPx.r < 0 ? 0 : 
                  (currPx.r + otherPx.r > 255 ? 255 : currPx.r + otherPx.r);
            resultPx.g = currPx.g + otherPx.g < 0 ? 0 : 
                  (currPx.g + otherPx.g > 255 ? 255 : currPx.g + otherPx.g);
            resultPx.b = currPx.b + otherPx.b < 0 ? 0 : 
                  (currPx.b + otherPx.b > 255 ? 255 : currPx.b + otherPx.b);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   Image Image::subtract(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < other.height(); i++)
      {
         for (int j = 0; j < other.width(); j++)
         {
            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel resultPx;
            resultPx.r = clamp(currPx.r - otherPx.r, 0, 255);
            resultPx.g = clamp(currPx.g - otherPx.g, 0, 255);
            resultPx.b = clamp(currPx.b - otherPx.b, 0, 255);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   Image Image::multiply(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel multiplyPx;
            multiplyPx.r = (currPx.r / (float)255) * otherPx.r;
            multiplyPx.g = (currPx.g / (float)255) * otherPx.g;
            multiplyPx.b = (currPx.b / (float)255) * otherPx.b;

            result.set(i, j, multiplyPx);
         }
      }

      return result;
   }

   Image Image::screen(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {
            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel screenPx;
            screenPx.r = (1.0 - ((1.0 - currPx.r / 255.0) 
                        * (1.0 - otherPx.r / 255.0))) * 255;
            screenPx.g = (1.0 - ((1.0 - currPx.g / 255.0) 
                        * (1.0 - otherPx.g / 255.0))) * 255;
            screenPx.b = (1.0 - ((1.0 - currPx.b / 255.0) 
                        * (1.0 - otherPx.b / 255.0))) * 255;

            result.set(i, j, screenPx);
         }
      }

      return result;
   }

   Image Image::difference(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel resultPx;

            resultPx.r = std::abs(currPx.r - otherPx.r);
            resultPx.g = std::abs(currPx.g - otherPx.g);
            resultPx.b = std::abs(currPx.b - otherPx.b);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   Image Image::lightest(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel resultPx;
            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            resultPx.r = std::max(currPx.r, otherPx.r);
            resultPx.g = std::max(currPx.g, otherPx.g);
            resultPx.b = std::max(currPx.b, otherPx.b);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   Image Image::darkest(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel resultPx;
            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            resultPx.r = std::min(currPx.r, otherPx.r);
            resultPx.g = std::min(currPx.g, otherPx.g);
            resultPx.b = std::min(currPx.b, otherPx.b);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   // used below link to figure out
   // before did not have the 255 in equ which did not work at all
   // sorry broke the link to fit 80 char lim 
   // https://stackoverflow.com/questions/16521003/gamma-correction-
   // formula-gamma-or-1-gamma

   Image Image::gammaCorrect(float gamma) const
   {

      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel pixel = get(i, j);

            pixel.r = pow((float)pixel.r / 255, 1.0 / gamma) * 255;
            pixel.g = pow((float)pixel.g / 255, 1.0 / gamma) * 255;
            pixel.b = pow((float)pixel.b / 255, 1.0 / gamma) * 255;

            result.set(i, j, pixel);
         }
      }

      return result;
   }

   Image Image::alphaBlend(const Image &other, float alpha) const
   {
      Image result(m_width, m_height);
      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {
            Pixel otherPx = other.get(i, j);
            Pixel thisPx = get(i, j);

            Pixel resultPx;
            resultPx.r = otherPx.r * alpha + thisPx.r * (1 - alpha);
            resultPx.g = otherPx.g * alpha + thisPx.g * (1 - alpha);
            resultPx.b = otherPx.b * alpha + thisPx.b * (1 - alpha);

            result.set(i, j, resultPx);
         }
      }

      return result;
   }

   Image Image::invert() const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {
            Pixel curr = get(i, j);

            curr.r = (1 - curr.r / 255.0) * 255.0;
            curr.g = (1 - curr.g / 255.0) * 255.0;
            curr.b = (1 - curr.b / 255.0) * 255.0;

            result.set(i, j, curr);
         }
      }

      return result;
   }

   Image Image::grayscale() const
   {

      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel pixel = get(i, j);

            // weighted avg
            float avg = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;

            pixel.r = avg;
            pixel.g = avg;
            pixel.b = avg;

            result.set(i, j, pixel);
         }
      }

      return result;
   }

   //https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
   Image Image::sepia() const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel pixel = get(i, j);

            // weighted avg for each color 
            float newR = 0.393 * pixel.r + 0.769 * pixel.g + 0.189 * pixel.b;
            float newG = 0.349 * pixel.r + 0.686 * pixel.g + 0.168 * pixel.b;
            float newB = 0.272 * pixel.r + 0.534 * pixel.g + 0.131 * pixel.b;

            pixel.r = newR > 255 ? 255 : newR;
            pixel.g = newG > 255 ? 255 : newG;
            pixel.b = newB > 255 ? 255 : newB;

            result.set(i, j, pixel);
         }
      }
      return result;
   }

   Image Image::colorJitter(int size) const
   {
      Image image(0, 0);

      return image;
   }

   Image Image::bitmap(int size) const
   {
      Image image(0, 0);

      return image;
   }

     std::vector<Pixel> 
     Image::getNeighbors(int i, int j, std::vector<float> gaussMatrix) const
   {
      std::vector<Pixel> neighbors;

      for (int x = i-1; x <= i + 1; x++)
      {
         if (x < 0)
         {
            continue;
         }
         if (x > m_height-1)
         {
            continue;
         }

         for (int y = j-1; y <= j+1; y++)
         {
            if (y < 0)
            {
               continue;
            }

            if (y > m_width - 1)
            {
               continue;
            }

            Pixel currPx = get(x, y);
            currPx.r = currPx.r * gaussMatrix[(x-(i-1))*3+y-((j-1))];
            currPx.g = currPx.g * gaussMatrix[(x-(i-1))*3+y-((j-1))];
            currPx.b = currPx.b * gaussMatrix[(x-(i-1))*3+y-((j-1))];

            neighbors.push_back(currPx);
            
         }
      }
      return neighbors;
   }

   Image Image::gaussianBlur(int stdev) const
   {
      Image result(m_width, m_height);


      std::vector<Pixel> neighbors;
      // std::vector<float> gaussMatrix;

      // using a fixed kernel to get more of a blur 
      std::vector<float> gaussMatrix = {1.0/16.0, 1.0/8.0, 1.0/16.0, 
                                       1.0/8.0, 1.0/4.0, 1.0/8.0,
                                       1.0/16.0, 1.0/8.0, 1.0/16.0}; 
      float gaussSum = 0.0; 

      // the stdev version 
      // for (int x = -1; x < 2; x++){
      //    for (int y = -1; y < 2; y++){
      //       gaussMatrix.push_back((1 / (2 * M_PI * pow(stdev, 2))) * 
      //       std::exp(-((pow(x, 2) + pow(y, 2)) / (2 * pow(stdev, 2)))));
      //       gaussSum += gaussMatrix[gaussMatrix.size()-1];
      //    }
      // }

      // for (int i = 0; i < gaussMatrix.size(); i++){
      //    gaussMatrix[i] = gaussMatrix[i]/gaussSum; 
      //    std::cout << gaussMatrix[i] << std::endl; 
      // }

      // cycle through pixels

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            neighbors = getNeighbors(i, j, gaussMatrix);

            // pixel = neighbors * gaussequ && all added together

            Pixel resultant = neighbors[0];
            for (int v = 1; v < neighbors.size(); v++)
            {

               resultant.r += neighbors[v].r;
               resultant.g += neighbors[v].g;
               resultant.b += neighbors[v].b;
            }

            result.set(i, j, resultant);
         }
      }

      return result;
   }

   Image Image::filmGrain() const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel currPx = get(i, j);

            // random num from 0 - 100
            int randNum = rand() % 100;

            // based on num - determine dark/light
            // creates "grain" effect
            if (randNum < 20)
            {
               currPx.r = currPx.r * 0.8;
               currPx.g = currPx.g * 0.8;
               currPx.b = currPx.b * 0.8;
            }
            else if (randNum < 40)
            {
               currPx.r = currPx.r * 0.6;
               currPx.g = currPx.g * 0.6;
               currPx.b = currPx.b * 0.6;
            }
            else if (randNum < 60)
            {
               currPx.r = currPx.r * 0.5;
               currPx.g = currPx.g * 0.5;
               currPx.b = currPx.b * 0.5;
            }
            else if (randNum < 80)
            {
               currPx.r = currPx.r * 0.4;
               currPx.g = currPx.g * 0.4;
               currPx.b = currPx.b * 0.4;
            }
            else
            {
               currPx.r = currPx.r * 0.2;
               currPx.g = currPx.g * 0.2;
               currPx.b = currPx.b * 0.2;
            }

            // apply to pixel and change val
            result.set(i, j, currPx);
         }
      }

      return result;
   }

   Image Image::overlay(const Image &other) const
   {
      Image result(m_width, m_height);

      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {

            Pixel currPx = get(i, j);
            Pixel otherPx = other.get(i, j);

            Pixel overlayPx;

            if (currPx.r < 0.5 && currPx.g < 0.5 && currPx.b < 0.5)
            {
               // multiply
               overlayPx.r = (currPx.r / (float)255) * otherPx.r;
               overlayPx.g = (currPx.g / (float)255) * otherPx.g;
               overlayPx.b = (currPx.b / (float)255) * otherPx.b;

            }
            else
            {
               // screen
               overlayPx.r = (1.0 - ((1.0 - currPx.r / 255.0) 
                        * (1.0 - otherPx.r / 255.0))) * 255;
               overlayPx.g = (1.0 - ((1.0 - currPx.g / 255.0) 
                        * (1.0 - otherPx.g / 255.0))) * 255;
               overlayPx.b = (1.0 - ((1.0 - currPx.b / 255.0) 
                        * (1.0 - otherPx.b / 255.0))) * 255;

            }

            result.set(i, j, overlayPx);
         }
      }

      return result;
   }

   // same as neighbors for gauss but not applying const 
   std::vector<Pixel> Image::pixelateNeighbors(int i, int j) const
   {
      std::vector<Pixel> neighbors;

      for (int x = i-1; x <= i + 1; x++)
      {
         if (x < 0)
         {
            continue;
         }
         if (x > m_height-1)
         {
            continue;
         }

         for (int y = j-1; y <= j+1; y++)
         {
            if (y < 0)
            {
               continue;
            }

            if (y > m_width - 1)
            {
               continue;
            }
            
            Pixel currPx = get(x, y);
            neighbors.push_back(currPx);
            
         }
      }
      return neighbors;
   }

   Image Image::pixelate() const {
      // Image result(m_width, m_height);
      Image result = *this; 

      std::vector<Pixel> neighbors;

      for (int i = 1; i < m_height; i+=3){
         for (int j = 1; j < m_width; j+=3){

            neighbors = pixelateNeighbors(i, j);

            float avgr = 0.0; 
            float avgg = 0.0; 
            float avgb = 0.0; 

            for (int n = 0; n < neighbors.size(); n++){
               avgr += neighbors[n].r; 
               avgg += neighbors[n].g; 
               avgb += neighbors[n].b; 
            }

            avgr = avgr/((float)neighbors.size()); 
            avgg = avgg/((float)neighbors.size()); 
            avgb = avgb/((float)neighbors.size()); 

            Pixel avgPix; 
            avgPix.r = avgr; 
            avgPix.g = avgg;
            avgPix.b = avgb; 

            // setting pixels as avg to create pixel chunk 
            for (int x = -1; x < 2; x++){
               for (int y = -1; y < 2; y++){
                  result.set(i+x, j+y, avgPix);
               }
            }
         }
      }
      return result; 
   }

   void Image::fill(const Pixel &c)
   {
      for (int i = 0; i < m_height; i++)
      {
         for (int j = 0; j < m_width; j++)
         {
            set(i, j, c);
         }
      }
   }

} // namespace agl