/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoaderTGA.cpp
 * \brief	The implementation of the TextureLoaderTGA class.
 */
#include "TextureLoaderTGA.h"
#include <fstream>

using namespace std;

/**
 * \brief Default constructor, does nothing.
 */
TextureLoaderTGA::TextureLoaderTGA(void) { }

/**
 * \brief Default destructor, does nothing.
 */
TextureLoaderTGA::~TextureLoaderTGA(void) { }

//! Loads a Targa image from disk.
/**
 * \fn bool TextureLoaderTGA::load( const std::string& filename )
 * This function loads a targa file. Currently only uncompressed files are 
 * supported. 
 * \param filename String with the filename
 * \return True if the loading was successful
 */
bool TextureLoaderTGA::load( const string& filename ) {
  ifstream file( filename.c_str(), ios::binary );
  if(!file) 
    return 0;

  // read the header and check if the file is uncompressed
  BYTE tgaHeader[12];
  BYTE unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  file.read( (char *)tgaHeader, sizeof(tgaHeader) );
  if(!file)
    return 0;
  if( memcmp( unCompressHeader, tgaHeader, sizeof(unCompressHeader) ) != 0 )
    return false;

  // read and save image info, return if the bits are more than 8 bpp
  BYTE header[6];
  file.read( (char *)header, sizeof(header) );
  if( !file )
    return false;
  if( header[4] != 24 )
    return false;

  setWidth( header[1]  * 256 + header[0] );
  setHeight( header[3]  * 256 + header[2] );
  setBPP( header[4] / 8 );


  // allocate data for the image and load it
  BYTE *image = new BYTE[sizeof(BYTE)  * getWidth()  * getHeight()  * getBPP()];

  file.read( (char *)image, sizeof(BYTE)*getWidth()  * getHeight()  * getBPP());
  if( !file ) {
    delete image;
    return false;
  }

  setImage( image );
#ifdef GL_BGR_EXT
  if( useBGR() )
    setFormat( GL_BGR_EXT );
  else {
    bgr2rgb( 3 );
    setFormat( GL_RGB );
  }
#else
  bgr2rgb( 3 );
  setFormat( GL_RGB );
#endif

  return true;
}

//! Writes a Targa file.
/**
 * \fn bool TextureLoaderTGA::write( const std::string& filename )
 * Writes an uncompressed targa file. This could be used to make screenshots.
 * \param filename String containing the filename
 * \return True if the writing was successful.
 */
bool TextureLoaderTGA::write( const std::string& filename ) {
  // open file for output
  FILE *file = 0;
  fopen_s( &file, filename.c_str(), "wb" );
  if(!file) {
    fclose(file);
    return false;
  }

  // save width and height into the header
  unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // uncompressed Targa file
  unsigned char header[6];     // header, used to fill with width, height and bpp
  header[0] = getWidth() % 256;
  header[1] = getWidth() / 256;
  header[2] = getHeight() % 256;
  header[3] = getHeight() / 256;
  header[4] = 8 * getBPP();
  header[5] = 0;
  fwrite( unCompressHeader, sizeof(unCompressHeader), 1, file );
  fwrite( header, sizeof(header), 1, file );

  // transform into BGR format and write image
#ifdef GL_BGR_EXT
  if( getFormat() == GL_RGB ) {
    bgr2rgb( getBPP() );
    setFormat( GL_BGR_EXT );
  }
#else
  bgr2rgb( getBPP() );
  setFormat( GL_RGB );
#endif
  fwrite( getImage(), getWidth() * getHeight() * getBPP(), 1, file );
  fclose(file);
  return true;
}
