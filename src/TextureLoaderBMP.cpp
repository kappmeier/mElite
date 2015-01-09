/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextureLoaderBMP.cpp
 * \brief	The implementation of the TextureLoaderBMP class.
 */
#include "TextureLoaderBMP.h"
#include <Windows.h>
#include <fstream>

using namespace std; // for iostream

/**
 * \brief Default constructor, does nothing.
 */
TextureLoaderBMP::TextureLoaderBMP( void ) {
}

/**
 * \brief Default destructor, does nothing.
 */
TextureLoaderBMP::~TextureLoaderBMP( void ) {
}

//! Loads a Windows bitmap using auxDIBImageLoad function from the GLaux lib.
/**
 * \return A pointer to a AUX_RGBImageRec structure if loading was successful or NULL elsewise.
 * \param filename pointer to the file to load 
 */
AUX_RGBImageRec * TextureLoaderBMP::loadAux( const char *filename ) {
  FILE *file = NULL;

  if (!filename) // file could not be opened
    return NULL;

  fopen_s( &file, filename, "r" );			// try opening file for read access
  if( file ) {// if opening was successful send file to glaux function and let it do all the stuff
    fclose( file );
    return auxDIBImageLoad( filename );
  }

  return NULL;
}

//! Loads a bitmap.
/**
 * \fn bool TextureLoaderBMP::load( const std::string& filename )
 * It is currently only able to load windows bitmaps. It tests if
 * the filetype is correct and calls the corresponding method.
 * \param filename zero terminated character array as filename 
 */
bool TextureLoaderBMP::load( const string& filename ) {
  std::ifstream file( filename.c_str(), ios::binary );
  if(!file) 
    return 0;

  BYTE begin[20];
  file.read( (char *)begin, sizeof(begin) );
  if(!file) {
    return 0;
  }

  file.seekg(0); // rewind to beginning to read file again

  if( begin[0] == 'B' && begin[1] == 'M' ) { // windows bitmap file
    bool ret = ReadDIBFile( file );
    if( ret == false)
      return false;
  } else { // unsupported file type
    return false;
  }
  return false;
}

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')
const int PADSIZE = 4;

//! Loads a bitmap from a given file stream.
/**
 * \fn bool TextureLoaderBMP::ReadDIBFile( std::istream &file )
 * As a first step, the header is checked for correctness.
 * Then the image data is being read. The colors 
 * are returned in BGR order, as typically for windows bitmaps. The flag is
 * set to GL_BGR_EXT which should exist in a lot of OpenGL implementations.
 * In fact a possibility to select output data is missing and will be 
 * implemented later. No checking if the textures have a 2^n size is performed
 * as since OpenGL 2.0 all texture sizes are possible.
 * \param file file stream object 
 */
bool TextureLoaderBMP::ReadDIBFile( istream &file ) {
  BITMAPFILEHEADER bmfHeader;
  int nBMISize;
  BITMAPINFOHEADER  * pBMI;

  // try reading file header
  file.read( (char *)&bmfHeader, sizeof(bmfHeader) );
  if( !file )
    return false;

  // check if header is valid
  if ( bmfHeader.bfType != DIB_HEADER_MARKER )
    return false;

  // A llocate memory for bitmap info header. The actual size of the header varies in the different
  // file versions. we suppose it starts at current position and stops where the image data begins.
  nBMISize = bmfHeader.bfOffBits - sizeof(bmfHeader);
  pBMI = (BITMAPINFOHEADER *)(new BYTE[nBMISize]);
  file.read( (char *)pBMI, nBMISize );
  if( !file ) { // file error. free again.
    delete (BYTE *)pBMI;
    return false;
  }

  // check for unsupported file type
  if( pBMI->biHeight < 0 || pBMI->biWidth < 0 || pBMI->biCompression != BI_RGB ) {
    delete (BYTE *)pBMI;
    return false;
  }

  // save size information in class
  setHeight( pBMI->biHeight );
  setWidth( pBMI->biWidth );

  // if a colormap is present, we create a pointer to it
  RGBQUAD *bmiColors = (RGBQUAD *)(((BYTE *)pBMI) + pBMI->biSize);

  // create some width. this is needed for some different file versions and colordephts
  int usewidth1 = (getWidth() + (PADSIZE - 1)) / PADSIZE;usewidth1 *= PADSIZE;
  int usewidth3 = (getWidth()  * 3 + (PADSIZE - 1)) / PADSIZE;usewidth3 *= PADSIZE;
  int usewidth4 = (getWidth()  * 4 + (PADSIZE - 1)) / PADSIZE;usewidth4 *= PADSIZE;

  // allocate memory for one row in the file. this row has to be aligned on DWORD (4 bytes)
  BYTE *rowbuf = new BYTE[usewidth4];
  bool err = false;

  //allocate memory for the whole image. we use the aligned size!
  int usewidth = (getWidth()  * 3 + (PADSIZE - 1)) / PADSIZE;
  usewidth *= PADSIZE;

  // we create an one dimensional array but it can be accessed as two dimensional array
  BYTE *image = new BYTE[usewidth  * getHeight()];
  BYTE **pimage;
  pimage = new BYTE *[getHeight()];
  for( unsigned int i=0;i<getHeight();i++, image += usewidth )
    pimage[i] = image;

  switch( pBMI->biBitCount ) {
default:
  err = true;
  break;

case 8:
  for( unsigned int r=0;r < getHeight();r++ ) {
    file.read((char *)rowbuf, usewidth1);
    if(!file) {
      err = true;
      break;
    }

    BYTE *img = rowbuf;
    BYTE *row = pimage[r];
    for( unsigned int c=0;c < getWidth();c++ ) {// note BGR order of colors!
      *row++ = bmiColors[*img].rgbBlue;
      *row++ = bmiColors[*img].rgbGreen;
      *row++ = bmiColors[*img++].rgbRed;
    }
  }
  break;

case 24:
  for( unsigned int r=0; r < getHeight(); r++ ) {
    file.read( (char *)rowbuf, usewidth3 );
    if(!file) {
      err = true;
      break;
    }

    BYTE *img = rowbuf;
    BYTE *row = pimage[r];
    for( unsigned int c=0;c < getWidth()*3;c++ ) {
      *row++ = *img++;
    }
  }
  break;

case 32:
  for( unsigned int r=0; r < getHeight(); r++ ) {
    file.read( (char *)rowbuf, usewidth4 );
    if(!file) {
      err = true;
      break;
    }

    BYTE *img = rowbuf;
    BYTE *row = pimage[r];
    for( unsigned int c=0;c<getWidth();c++ ) {
      *row++ = *img++;
      *row++ = *img++;
      *row++ = *img++;
      img++;
    }
  }
  break;
  }

  // free temp memory, image data is freed in base class destructor
  delete (BYTE *)pBMI;
  delete [] rowbuf;
  if(err) {
    delete pimage[0];
    delete pimage;
    pimage = NULL;
    setWidth(0);
    setHeight(0);
    return false;
    setFormat(0);
  }

  // set format to windows BGR format if possible, elsewise switch to rgb
  // this ensures compilability, however it does not check if the opengl at runtime can use bgr colors
  if( pimage == NULL )
    return false;
  setImage( pimage );
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

//! Does nothing, because writing bitmaps is not supported.
/**
 * \fn TextureLoaderBMP::write( const std::string& filename )
 */
bool TextureLoaderBMP::write( const std::string& filename ) {
  return false;
}
