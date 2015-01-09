/**
/* Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureFont.h
 * \brief	Declares the texture font class that handles fonts based on texture images.
 */

#pragma once

#include "GlobalGL.h"
#include <string>

/**
 * \class	TextureFont
 * \brief	Realizes a texture font.
 *
 * Texture fonts are fonts based on an image. The image is used as a texture that is mapped
 * on rectangles to display the font.
 * \author	Jan-Philipp Kappmeier
 * \date	27/09/2012
 */
class TextureFont {
public:
  TextureFont( GLuint texindex );
  ~TextureFont(void);
  GLvoid buildFont( GLuint cpl, GLuint cpr, GLuint size, GLuint targetsize, GLuint skipspace );
  unsigned int getSize( void );
  unsigned int getWidth( void );
  void print( int x, int y, const char *string, ... );
  void print( int x, int y, const std::string *text );
  GLvoid killFont( GLvoid );
private:
  GLuint base;
  GLuint texture;
  GLuint count;
  GLuint width;
  GLuint height;
  unsigned int m_size;
  unsigned int m_width;
};
