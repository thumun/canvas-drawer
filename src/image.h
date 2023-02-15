// Copyright 2021, Aline Normoyle, alinen

#ifndef AGL_IMAGE_H_
#define AGL_IMAGE_H_

#include <iostream>
#include <fstream>  
#include <string>
#include <vector>

namespace agl {

/**
 * @brief Holder for a RGB color
 * 
 */
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

/**
 * @brief Implements loading, modifying, and saving RGB images
 */
class Image {
 public:
  Image();
  Image(int width, int height);
  Image(const Image& orig);
  Image& operator=(const Image& orig);

  virtual ~Image();

  /** 
   * @brief Load the given filename 
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally when loaded
   * 
   * @verbinclude sprites.cpp
   */
  bool load(const std::string& filename, bool flip = false);

  /** 
   * @brief Save the image to the given filename (.png)
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally before being saved
   */
  bool save(const std::string& filename, bool flip = false) const;

  /** @brief Return the image width in pixels
   */
  int width() const;

  /** @brief Return the image height in pixels
   */
  int height() const;

  /** 
   * @brief Return the RGB data
   *
   * Data will have size width * height * 4 (RGB)
   */
  char* data() const;

  /**
   * @brief Replace image RGB data
   * @param width The new image width
   * @param height The new image height
   *
   * This call will replace the old data with the new data. Data should 
   * match the size width * height * 3
   */
  void set(int width, int height, unsigned char* data);

  /**
   * @brief Get the pixel at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  Pixel get(int row, int col) const;

  /**
   * @brief Set the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  void set(int row, int col, const Pixel& color);

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  Pixel get(int i) const;

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  void set(int i, const Pixel& c);

  // resize the image
  Image resize(int width, int height) const;

  // flip around the horizontal midline
  Image flipHorizontal() const;

  // flip around the vertical midline
  Image flipVertical() const;

  // rotate the Image 90 degrees
  Image rotate90() const;

  // Return a sub-Image having the given top,left coordinate and (width, height)
  Image subimage(int x, int y, int w, int h) const;

  // Replace the portion starting at (row, col) with the given image
  // Clamps the image if it doesn't fit on this image
  void replace(const Image& image, int x, int y);

  // swirl the colors 
  Image swirl() const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel + other.pixel
  // Assumes that the two images are the same size
  Image add(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel - other.pixel
  // Assumes that the two images are the same size
  Image subtract(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel * other.pixel
  // Assumes that the two images are the same size
  Image multiply(const Image& other) const;

  // Apply the follow calculation to the pixels in 
  // our image and the given image: 
  //    result.pixel = 1 - (1-this.pixel)*(1-other.pixel)
  // Assumes that the two images are the same size 
  Image screen(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = abs(this.pixel - other.pixel)
  // Assumes that the two images are the same size
  Image difference(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = max(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image lightest(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = min(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image darkest(const Image& other) const;

  // Apply gamma correction
  Image gammaCorrect(float gamma) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    this.pixels = this.pixels * (1-alpha) + other.pixel * alpha
  // Assumes that the two images are the same size
  Image alphaBlend(const Image& other, float amount) const;

  // invert the image 
  // result = 1 - image 
  Image invert() const;

  // Convert the image to grayscale
  Image grayscale() const;

  // convert the image to sepia : ) 
  Image sepia() const;

  // return a bitmap version of this image
  Image colorJitter(int size) const;

  // return a bitmap version of this image
  Image bitmap(int size) const;

  // return an image with gaussian blur applied to it 
  Image gaussianBlur(int stdev) const; 

  // extension func for gaussian blur 
  // returns the neighbors of a pixel 
  std::vector<Pixel> 
  getNeighbors(int i, int j, std::vector<float> gaussMatrix) const; 

  // returns image w/ film grain applied to it
  // lightens/darkens px based on random number to get effect 
  Image filmGrain()const; 

  // applies overlay to image and returns said image
  // applies screen or multiply depending on the pixel values 
  // Assumes that the two images are the same size
  Image overlay(const Image &other) const; 

  // extension func to get neighbors for pixelate 
  std::vector<Pixel> pixelateNeighbors(int i, int j) const; 

  // "pixelate" the image 
  // break into groups and get avg color & set 
  Image pixelate()const; 

  // Fill this image with a color
  void fill(const Pixel& c);

 private:
   int m_width; 
   int m_height; 
   int m_channels; 
   char * m_data; 
};
}  // namespace agl
#endif  // AGL_IMAGE_H_