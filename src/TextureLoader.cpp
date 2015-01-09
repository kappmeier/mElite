/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoader.cpp
 * \brief	Implements the texture loader.
 */

#include "TextureLoader.h"

/**
 * \brief Default constructor. Initializes variables and does nothing.
 */
TextureLoader::TextureLoader( void ) {
  m_width = 0;
  m_height = 0;
  image2d = NULL;
  image = NULL;
  m_format = 0;
  m_useBGR = true;
}

/**
 * \brief Destructor. Frees image data, if an image has been loaded.
 */
TextureLoader::~TextureLoader( void ) {
  if( image2d ) {
    delete [] image2d[0];
    delete [] image2d;
  } else {
    delete [] image;
  }
}

/**
/* \brief Load an image from memory.
 *
 * Loads an image from memory into the texture loader. The size of the image and the color dephth has
 * to be submitted. If the dephth has the wrong value, false is returned. The picture information is
 * stored in the class without any checking. It is assumed that the data is in RGB format and the
 * corrisponding values GL_RGB or GL_RGBA are set. For more complex loading from memory this function has
 * to be overridden by inheritors.
 * \param data A pointer to the actual image data.
 * \param width The width of the image.
 * \param height The height of the image.
 * \param bpp The color depth of the image in bits per pixel . Has to be 3 or 4.
 * \return Usually true is returned as no presumption about the image data is made.
 */
bool TextureLoader::load( BYTE *data, unsigned int width, unsigned int height, unsigned int bpp) {
  if( bpp < 3 || bpp > 4 )
    return false;
  setWidth( width );
  setHeight( height );
  setBPP( bpp );
  setImage( data );
  this->setFormat( bpp == 3 ? GL_RGB : GL_RGBA );
  return true;
}

/**
 * \fn	unsigned int TextureLoader::getWidth()
 * \brief	Returns the width of the image.
 * \return	The width.
 */
unsigned int TextureLoader::getWidth() {
  return m_width;
}

/**
 * \fn	unsigned int TextureLoader::getHeight()
 * \brief	Gets the height of the image.
 * \return	The height.
 */
unsigned int TextureLoader::getHeight() {
  return m_height;
}

/**
 * \fn	unsigned int TextureLoader::getBPP()
 * \brief	Gets the bits per pixel of the image. This is usually 24 or 32.
 * \return	The bits per pixel.
 */
unsigned int TextureLoader::getBPP() {
  return m_bpp;
}

/**
 * \fn	GLenum TextureLoader::getFormat()
 * \brief	Gets the OpenGL format of the image data.
 * 			
 * This may be RGBA for a RGB encoded image containing an alpha channel.
 * \return	The format.
 */
GLenum TextureLoader::getFormat() {
  return m_format;
}

/**
 * \fn	BYTE *TextureLoader::getImage()
 *
 * \brief	Gets the actual image data as byte array.
 *
 * The meaning of the data depends on the format of the image. In a 32 bit image with alpha channel,
 * four bytes contain the information of 1 pixel.
 * \return	a pointer to the image data
 */

BYTE *TextureLoader::getImage() {
  return image;
}

/**
 * \fn	void TextureLoader::setWidth( unsigned int width )
 * \brief	Sets the width of the image.
 * 			
 * The width, height and format information are used to reserve the necessary space in memory for an image.
 * \param	width	The width.
 */
void TextureLoader::setWidth( unsigned int width ) {
  m_width = width;
}

//! Sets the height of the image.
/**
 * \fn	void TextureLoader::setHeight( unsigned int height )
 * The width, height and format information are used to reserve the necessary space in memory for an image.
 * \param	height The width.
 */
void TextureLoader::setHeight( unsigned int height ) {
  m_height = height;
}

//! Sets the number of bits per pixel for the image.
/**
 * \fn	void TextureLoader::setBPP( unsigned int bpp )
 * The width, height and format information are used to reserve the necessary space in memory for an image.
 * \param	bpp The width.
 */
void TextureLoader::setBPP( unsigned int bpp ) {
  m_bpp = bpp;
}

//! Sets the format of the image.
/**
 * \fn	void TextureLoader::setFormat( GLenum format )
 * The width, height and format information are used to reserve the necessary space in memory for an image.
 * \param	format The width.
 */
void TextureLoader::setFormat( GLenum format ) {
  m_format = format;
}

//! Sets the image data.
/**
 * \fn	void TextureLoader::setImage( BYTE **image )
 * The image data is an byte array residing in memory. It may come from a capture taken from the OpenGL rendering out.
 * \param image a pointer to an array containing image arrays. The first of them is set as actual image.
 */
void TextureLoader::setImage( BYTE **image ) {
  image2d = image;
  this->image = image[0];
}

//! Sets the image data.
/**
 * \fn	void TextureLoader::setImage( BYTE *image )
 * The imaga data is an byte array residing in memory. It may come from a capture taken from the OpenGL rendering out.
 * \param [in,out]	image	a poionter to an array containing the actual image data
 */
void TextureLoader::setImage( BYTE *image ) {
  image2d = NULL;
  this->image = image;
}

//! Convert the image data from BGR to RGB format.
/**
 * \fn	void TextureLoader::bgr2rgb( unsigned __int8 colorMode )
 * The format description describes the byte order of red, green and blue bytes in memory.
 * \param	colorMode	The color mode.
 */
void TextureLoader::bgr2rgb( unsigned __int8 colorMode ) {
  BYTE tempColor;
  for( unsigned long index = 0; index < m_width  * m_height  * colorMode; index += colorMode ) {
    tempColor = getImage()[index];
    getImage()[index] = getImage()[index + 2];
    getImage()[index + 2] = tempColor;
  }
}

//! Sets the color order to blue, green, red by force.
/**
 * \fn	void TextureLoader::useBGR( bool value )
 * With this ordering, the colors are changed and not displayed correctly, if the original ordering
 * war RGB.
 * \param	value	decides, if BGR ordering should be used, or not
 */
void TextureLoader::useBGR( bool value ) {
  m_useBGR = value;
}

//! Determines if BGR coloring is used, or not.
/**
 * \fn	bool TextureLoader::useBGR( void )
 * \return	true if BGR ordering is used
 */
bool TextureLoader::useBGR( void ) {
  return m_useBGR;
}

//! Trys to decide, if BGR is the appropriate ording for the format of the current image.
/**
 * \fn	bool TextureLoader::isBGR()
 * For the formats GL_RGB and GL_RGBA, false is returned. For GL_BGR_EXT true is returned. As for all
 * other formats, nothing special is known, as a default value, false is returned (as rgb is the
 * 'natural' ordering).
 *
 * The conversion only takes place, if GL_BGR_EXT is provided by the OpenGL implementation.
 * \return true, if the current format should be assumed to be BGR
 */
bool TextureLoader::isBGR() {
#ifdef GL_BGR_EXT
  if( getFormat() == GL_BGR_EXT )
    return true;
  if( getFormat() == GL_RGB || getFormat() == GL_RGBA )
    return false;
  // return false, if nothing about format is known ;)
  return false;
#else
  return false;
#endif
}

//! Converts the image from BGR to RBG, if the image data is in BGR ordering.
/**
 * \fn	void TextureLoader::toRGB()
 * The conversion only takes place, if GL_BGR_EXT is provided by the
 * OpenGL implementation.
 */
void TextureLoader::toRGB() {
#ifdef GL_BGR_EXT
  if( getFormat() == GL_BGR_EXT ) {
    bgr2rgb( 3 );
    setFormat( GL_RGB );
  }
#endif
}

//! Converts the image from RGB to BGR, if the image data is in RGB ordering.
/**
 * \fn	void TextureLoader::toBGR()
 * The conversion only takes place, if GL_BGR_EXT is provided by the
 * OpenGL implementation.
 */
void TextureLoader::toBGR() {
#ifdef GL_BGR_EXT
  if( getFormat() == GL_RGB ) {
    bgr2rgb( 3 );
    setFormat( GL_BGR_EXT );
  }
#endif
}
