/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoader.h
 * \brief	Declares the TextureLoader, a class that can load a texture from a file and set it
 * 			up as OpenGL texture.
 * 			
 * The loader is an abstract class, the support for loading the image data from a file, a subclass must be derive.
 */

#pragma once

#include "GlobalGL.h"
#include <string>

/**
 * \class	TextureLoader
 *
 * \brief	The a loader that can set up the image data of a graphics file as an OpenGL texture.
 *
 * The class itself does not provide ability to load a file, but provides virtual methods. For
 * derived loaders, these methods must be implemented. Besides loading from hard drives, the
 * loader can also load from data stored in memory. This can be used to render images that are
 * used as textures.
 */

class TextureLoader {
public:
  TextureLoader( void );
  ~TextureLoader( void );
  virtual bool load( const std::string& filename ) = 0;
  virtual bool write( const std::string& filename ) = 0;
  bool load( BYTE *data, unsigned int width, unsigned int height, unsigned int bpp);
  unsigned int getWidth( void );
  unsigned int getHeight( void );
  unsigned int getBPP( void );
  GLenum getFormat( void );
  BYTE *getImage();
  void useBGR( bool value );
  bool useBGR( void );
  bool isBGR( void );
  void toRGB( void );
  void toBGR( void );
protected:
  void setWidth( unsigned int width );
  void setHeight( unsigned int height );
  void setBPP( unsigned int bpp );
  void setFormat( GLenum format );
  void setImage( BYTE **image );
  void setImage( BYTE *image );
  void bgr2rgb( unsigned __int8 colorMode );
private:
  unsigned int m_width;
  unsigned int m_height;
  unsigned int m_bpp;
  GLenum m_format;
  BYTE **image2d;
  BYTE *image;
  bool m_useBGR;
};
