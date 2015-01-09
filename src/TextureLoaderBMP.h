/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoaderBMP.h
 * \brief	Declares the TextureLoaderBMP, an implementation of the TextureLoader.
 */
#pragma once

#include <GL/glaux.h>// for loading bitmaps-textures using GLaux lib
#include "GlobalGL.h"
#include "TextureLoader.h"
#include <fstream>
#include <string>

//!An implementation of the TextureLoader that can load windows bitmaps.
/**
 * This class is basically an extension to the TextureLoader class which is
 * capable to load bitmap textures. Therefore it overloads the load method of
 * the base class. If this doesn't work the GLaux functions could be used as 
 * some kind of rescue.
 */
class TextureLoaderBMP :
  public TextureLoader {
public:
  TextureLoaderBMP( void );
  ~TextureLoaderBMP( void );
  static AUX_RGBImageRec *loadAux( const char *filename );
  bool load( const std::string& filename );
  bool write( const std::string& filename );
private:
  bool ReadDIBFile( std::istream &file );
};
