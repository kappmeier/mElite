/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Textures.cpp
 * \brief	Implementation of the texture manager.
 */
#include "Textures.h"
#include "TextureLoaderBMP.h"
#include "TextureLoaderTGA.h"
#include "LittleHelper.h"

using namespace std;

/** @name Construction
 * The constructors and destructor for a singleton object. The function Textures::getTextures() is used to access
 * the class.
 */
//@{
//! Pointer to singleton object.
/**
 * Static pointer to the singleton instance of this class. It is set in Textures::getTextures() if an instance
 * is created.
 */
Textures *Textures::m_textures = 0;

//! Public access to singleton object.
/**
 * The public access for the singleton object. If called for the first time a
 * new instance of Textures is created, otherwise the existing instance is returned.
 * \return A pointer to the instance of the class
 */
Textures *Textures::getTextures( void ) {
  if( m_textures == 0 )
    m_textures = new Textures();
  return m_textures;
}

//! Standard constructor.
/**
 * Initializes space for ten textures and disables BGR to RGB conversion if files with BGR format
 * are loaded.
 */
Textures::Textures( void ) {
  reserve( 10 );
  switchRGB2BGR = false;
}

//! Destructor
/**
 * Trys to free all textures with freeTextures() and after that clears the vector holding
 * the generated texture numbers.
 */
Textures::~Textures( void ) {
  for( unsigned int i=0; i < textures.size(); i++ )
    freeTexture( i );
  textures.clear();
}
//@}


/** @name Textureloading
 * These functions are used to load and unload textures from the memory. To guarantee fast loading of
 * many textures, the function Textures::reserve() should be used to reserve memory. The mode in which the textures
 * are loaded can also be changed.
 */
//@{
//! Set's conversion between RGB and BGR on and off.
/**
 * If 'true' is submitted, the red and blue components of a loaded texture are
 * changed. This could be used to create textures which seem to be varied.
 * \param value determines wheather conversion is active or not
 */
void Textures::changeColors( bool value ) {
  switchRGB2BGR = value;
}

//! Frees texture memory.
/**
 * If the index of the given texture is valid and an OpenGL generated texture number has been
 * assigned to it, the texture space is freed. All variables for this texture id are resetted to defaults.
 * \param texture the number of the texture used for indexing the vector of textures
 */
void Textures::freeTexture ( unsigned int texture ) {
  if( texture >= textures.size() )// try to access texture that is not in vector
    return;
  if( textures[texture] == -1 )// try to free unassigned texture
    return;

  // delete texture and information
  GLuint id = (GLuint) textures[texture];
  glDeleteTextures( 1, &id );
  textures[texture] = -1;
}

//! Loads a texture from a file into a texture slot.
/**
 * \fn void Textures::loadTexture( unsigned int texture, std::string filename )
 * Loads a texture from a file. The texture is accessed by an texture value 
 * which is send by the application. After that a new texture name is generated
 * by OpenGL. If the texture value is already assigned to a texture, the old
 * texture is deleted and the OpenGL texture name is freed. 
 * \param filename the filename of the texture
 * \param texture the number of the texture, used for indexing the texture vector
 */
void Textures::loadTexture( unsigned int texture, std::string filename ) {
  if( texture >= textures.size() )
    reserve( texture + 1 );

  if( textures[texture] != -1 ) {
    freeTexture( texture );
  }

  GLuint id;
  glGenTextures ( 1, &id );
  textures[texture] = (int) id;

  // load new texture. currently, only bmp is usable. all files are identified by their ending
  if( LittleHelper::stringCmpi( filename.substr(filename.length() - 3, 3), "bmp" ) == 0) {
    //loadTextureBMP( texture, filename );
    texLoader = new TextureLoaderBMP();
    texLoader->load( filename );
  }
  if( LittleHelper::stringCmpi( filename.substr(filename.length() - 3, 3), "tga" ) == 0) {
    //loadTextureTGA( texture, filename );
    texLoader = new TextureLoaderTGA();
    texLoader->load( filename );
    // try to write on harddisk
    texLoader->write( "testoutput.tga");
  }

  // now the texture should be loaded
  bindTexture( texture );

  // possible qualities
  // GL_NEAREST
  // GL_LINEAR
  // GL_LINEAR_MIPMAP_NEAREST
  // GL_LINEAR_MIPMAP_LINEAR
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

  // enable texture repeating
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  GLenum usedFormat;
#ifdef GL_BGR_EXT
  if( switchRGB2BGR )
    if( texLoader->isBGR() )
      usedFormat = GL_RGB;
    else
      usedFormat = GL_BGR_EXT;
  else
    usedFormat = texLoader->getFormat();
#else
  usedFormat = GL_RGB;
#endif

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texLoader->getWidth(), texLoader->getHeight(), usedFormat, GL_UNSIGNED_BYTE, texLoader->getImage() );
  // glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
  delete texLoader;
}

//! Loads a texture from memory into a texture slot.
/**
 * \fn Textures::loadTexture( unsigned int texture, TextureLoader* loader )
 * Loads a texture from memory. The texture has to be provided by a TextureLoader.
 * \param texture The index of the texture. Used for indexing the texture vector
 * \param loader Pointer to an TextureLoader instance that holds the picture data.
 */
void Textures::loadTexture( unsigned int texture, TextureLoader* loader ) {
  if( texture >= textures.size() )
    reserve( texture + 1 );

  if( textures[texture] != -1 ) {
    freeTexture( texture );
  }

  GLuint id;
  glGenTextures ( 1, &id );
  textures[texture] = (int) id;

  // now the texture should be loaded
  bindTexture( texture );

  // possible qualities
  // GL_NEAREST
  // GL_LINEAR
  // GL_LINEAR_MIPMAP_NEAREST
  // GL_LINEAR_MIPMAP_LINEAR
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Linear Filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Linear Filtering
  // enable texture repeating
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  GLenum usedFormat;
#ifdef GL_BGR_EXT
  if( switchRGB2BGR )
    if( loader->isBGR() )
      usedFormat = GL_RGB;
    else
      usedFormat = GL_BGR_EXT;
  else
    usedFormat = loader->getFormat();
#else
  usedFormat = GL_RGB;
#endif

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, loader->getWidth(), loader->getHeight(), usedFormat, GL_UNSIGNED_BYTE, loader->getImage() );
  //glTexImage2D(GL_TEXTURE_2D, 0, 3, loader->getWidth(), loader->getHeight(), 0, usedFormat, GL_UNSIGNED_BYTE, loader->getImage() );
}

//! Reserves space for textures in the vector.
/**
 * Reserves space in order to save textures and spare some time because the vector does not need to resize itself.
 * The values are initialized with -1 which is the value used for testing if a slot is uninitialized. If the
 * submitted value is smaller than the current size of the vector, it is ignored. Note that it is possible to
 * load more than the reserved number of textures.
 * \param textureCount the space that should be reserved
 */
void Textures::reserve( unsigned int textureCount ) {
  if( textureCount <= textures.size() )// new size is not bigger than the old one
    return;

  size_t oldSize = textures.size();

  textures.reserve( textureCount );
  textures.resize ( textureCount );

  // initialize new texture space
  for( size_t i=oldSize; i < textures.size(); i++ )
    textures[i] = -1;
}
//@}

/** @name Additional functionalities
 * These functions provide some additional functionalities. The Textures::bindTexture() function allows it
 * easyly to bind a loaded texture as GL_TEXTURE_2D.
 */
//@{
//! Binds a texture.
/**
 * Binds the texture with the given index number as a GL_TEXTURE_2D for use with OpenGL.
 * \param texture the number of the texture, used for indexing the texture vector
 */
void Textures::bindTexture( unsigned int texture ) {
  if( texture >= textures.size() )// texture to high
    return;
  if( textures[texture] == -1 )// texture not loaded
    return;

  glBindTexture( GL_TEXTURE_2D, textures[texture] );
} 
//@}
