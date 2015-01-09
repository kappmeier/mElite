/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureFont.cpp
 * \brief	Implements the texture font class.
 */
#include "GlobalParameters.h"
#include "TextureFont.h"
#include "Textures.h"

/**
 * \brief Builds a TextureFont, that is independent of the operating system.
 */
TextureFont::TextureFont( GLuint texindex ) {
  texture = texindex; // texture index for the font texture
  count = 0;
  m_size = 0;
}

/**
 * Destructor sets free the memory using the killFont() function. 
 */
TextureFont::~TextureFont(void) {
  this->killFont();
}

/**
 * Builds a TextureFont on Microsoft Windows
 * \param cpl the number of characters in one line
 * \param cpr the number of characters in one row
 * \param size the quadratic size of the characters within the file 
 * \param targetsize the size that each character should have
 * \param skipspace the space which the next letter should start more right
 */
GLvoid TextureFont::buildFont( GLuint cpl, GLuint cpr, GLuint size, GLuint targetsize, GLuint skipspace ) {
  m_width = skipspace;
  m_size = targetsize;
  float	cx;								// character x coordinates
  float	cy;								// character y coordinates

  count = cpr  * cpl;
  base = glGenLists( count );							 // create a display list big enough for all characters
  //glBindTexture( GL_TEXTURE_2D, texture );				// select the font texture
   Textures::getTextures()->bindTexture( texture );

  for( unsigned int i=0; i < count; i++ ) {
    cx = float(i%size)/(float)cpl;			 // x position of current character (left)
    cy = float(i/size)/(float)cpr;					// y position of current character (top)	

    glNewList( base + i, GL_COMPILE );				// create a listelement

    glBegin( GL_QUADS );					// each character is a quad
    glTexCoord2f( cx, 1-cy-(1/(float)cpr) );			// texture coordinates bottom left
    glVertex2i( 0, 0 );				// vertex coordinates bottom left
    glTexCoord2f( cx + (1/(float)cpl), 1-cy-(1/(float)cpr));	// texture coordinates bottom right
    glVertex2i( targetsize, 0 );				// vertex coordinates bottom right
    glTexCoord2f( cx + (1/(float)cpl), 1-cy);			// texture coordinates top right
    glVertex2i( targetsize, targetsize );				// vertex coordinates top right
    glTexCoord2f( cx, 1-cy );				 // texture coordinates top left
    glVertex2i( 0, targetsize );				// vertex coordinates top left
    glEnd();

    glTranslated( skipspace, 0, 0 );	// each character is only 10 pixels wide, move to the right
    glEndList();							 // finish list
  }
}

//! Gets the size of the texture
unsigned int TextureFont::getSize( void ) { 
  return m_size;
}

//! Gets the width of the texture
unsigned int TextureFont::getWidth( void ) {
  return m_width;
}

/**
 * \brief Deletes the displaylists for all 96 characters.
 */
GLvoid TextureFont::killFont( GLvoid ) {
  glDeleteLists( base, count );						// delete all caracters
}

//! Prints a text to the screen.
/**
 * The text is allowed to be up to 256 chars. It
 * is assumed that the current projection is an orthogonal projection.
 * \param x x-position where the text starts
 * \param y y-position where the text starts
 * \param string pointer to a char array with the text to be printed
 */
GLvoid TextureFont::print( int x, int y, const char *string, ... ) {
  char text[256];				// created string
  va_list ap;					// pointer to list of arguments

  if( string == NULL )// quit, if there is no texst
    return;

  va_start( ap, string );					// parse the string for variables
  vsprintf_s( text, string, ap );		// convert symbols to actual numbers
  va_end( ap );						 // store results in text

  //glBindTexture( GL_TEXTURE_2D, texture );				// select font texture
   Textures::getTextures()->bindTexture( texture );

  glPushMatrix();
  glPushAttrib( GL_LIST_BIT );				// push display list bits
  glTranslated( x, y, 0 );							// textposition (0,0 - Bottom Left)
  glListBase( base - 32 );

  glCallLists( (int)strlen(text), GL_UNSIGNED_BYTE, text );			// print text to screen
  glPopAttrib();
  glPopMatrix();
}

//! Prints a text to the screen.
/**s
 * \param x the x-coordinate where the text starts
 * \param y the y-coordinate where the text starts
 * \param text the text
 */
void TextureFont::print( int x, int y, const std::string *text ) {
  print(x, y, text->c_str());
}
