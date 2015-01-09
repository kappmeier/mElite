/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	BitmapFont.h
 * \brief	Declares the bitmap font class.
 */

#pragma once

#include "GlobalGL.h"

/**
 * \class	BitmapFont
 *
 * \brief	Bitmap represents windows bitmap font.
 * 			
 * The font uses the supporting methods of the operating system to represent
 * text on an OpenGL context. It has a proper windows font handle and the
 * font is built by the system, in contrast to a TextureFont, where the font
 * is loaded from symbols existing as an image that is used as a texture.
 *
 * \author	Jan-Philipp Kappmeier
 * \date	27/09/2012
 */

class BitmapFont {
public:
  BitmapFont( HDC h );
  ~BitmapFont();
  GLvoid buildFont( GLvoid );
  GLvoid buildFont( GLuint size );
  GLvoid print( const char *fmt, ... );
  GLvoid killFont( GLvoid );
private:
  GLuint base; // display list index for the font list
  HDC hDC; // device context in which the font should be
  GLuint m_size; // font size (not normal font size as used in windows!)
};