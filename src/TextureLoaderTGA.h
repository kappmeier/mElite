/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoaderTGA.h
 * \brief	Declares the TextureLoaderTGA, an implementation of the TextureLoader class
 */
#pragma once

#include "TextureLoader.h"
#include <string>

//! An implementation of the TextureLoader that can load Targa files.
/**
 * This class is basically an extension to the TextureLoader class which is
 * capable to load targa files. Therefore it overloads the load method of
 * the base class.
 */
class TextureLoaderTGA :
  public TextureLoader {
public:
  TextureLoaderTGA( void );
  ~TextureLoaderTGA( void );
  bool load( const std::string& filename );
  bool write( const std::string& filename );
private:
};
