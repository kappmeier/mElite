/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	BitmapFont.cpp
 * \brief	Implements the bitmap font class.
 */

#include "BitmapFont.h"

/**
 * \brief Constructor setting up the device context and the font size.
 */
BitmapFont::BitmapFont( HDC h ) {
  this->hDC = h;
  this->m_size = 24;
}

/**
 * \brief Destructor.
 * 		  
 * The Font is destroyed and deleted from the GDI-Space.
 */
BitmapFont::~BitmapFont() {
  killFont();
}

/**
 * \brief Builds a BitmapFont on Microsoft Windows
 */
GLvoid BitmapFont::buildFont( GLvoid ) {
  HFONT	font; // Windows font-id
  HFONT	oldfont;

  base = glGenLists( 96 ); // create storage space for 96 characters

  font = CreateFont( -1*m_size, // height of the font
    0, // width
    0, // escapement angle
    0, // orientation angle
    0, // weight
    FALSE, // italic
    FALSE, // underline
    FALSE, // strikeout
    ANSI_CHARSET, // charset encoding
    OUT_TT_PRECIS, // output precision
    CLIP_DEFAULT_PRECIS, // clipping precision
    ANTIALIASED_QUALITY, // anti aliasing
    FF_DONTCARE | DEFAULT_PITCH, // family, pith
    "Arial Narrow"); // the actual font

  oldfont = (HFONT)SelectObject( hDC, font );
  wglUseFontBitmaps( hDC, 32, 96, base );			// builds 96 characters starting at character 32 (space)
  SelectObject( hDC, oldfont );
  DeleteObject( font );
}

/**
 * \brief Builds a BitmapFont on Microsoft Windows
 * \param size size of the font (in pixel, not usual font size)
 */
GLvoid BitmapFont::buildFont( GLuint size ) {
  m_size = size;
  this->buildFont();
}

/**
 * \brief Deletes the displaylists for all 96 characters.
 */
GLvoid BitmapFont::killFont( GLvoid ) {
  glDeleteLists( base, 96 );				// delete all 96 characters
}

/**
 * \brief Prints a text to the screen.
 * 				
 * The text is allowed to be up to 256 chars.  Old C-Style functions are used.
 */
GLvoid BitmapFont::print(const char *fmt, ...) {
  char text[256];
  va_list ap;

  if (fmt == NULL)
    return;

  va_start(ap, fmt); // parse string for variables
  vsprintf_s(text, fmt, ap); // convert symbols to numbers
  va_end(ap); // store result

  glPushAttrib( GL_LIST_BIT ); // push the display list bits (we use them now)
  glListBase(base - 32); // reset the base as we start only with char 32

  glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text); // draw string using display list
  glPopAttrib(); // restore
}