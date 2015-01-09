/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Textures.h
 * \brief	Declares Texture class, a texture manager.
 */
#pragma once

#include <vector>

class TextureLoader;

//! Implements a texture manager that loads files and sets up OpenGL textures.
/**
 * \nosubgrouping
 * Allows automatically to load and unload textures into OpenGL. It hides all the stuff needed by OpenGL
 * such as the TexIDs. All textures can be accessed by a simple unsingned integer value. Internally the   
 * OpenGL TexIDs are assigned to one of these values. The assignment is stored in a vector. If a texture
 * should be loaded with an id that has already been used, the old texture is unloaded. The textures
 * are actual loaded by an abstract TextureLoader object. Depending from the format of the texture an
 * instance of inherited loader classes is created and the appropriate Textures::load() function is called via
 * the polymorphism.
 *
 * Also this class gives the ability to change the way how the textures should be created such as defining
 * the quality and the color type. It can also be defined if some or all types of EXT texture types
 * are allowed or forbidden.
 */
class Textures {
public:
  static Textures *getTextures( void );
  ~Textures();
private:
  static class Textures *m_textures;
  Textures();

public:
  void changeColors( bool value );
  void loadTexture( unsigned int texture, std::string filename );
  void loadTexture( unsigned int texture, TextureLoader* loader );
  void reserve( unsigned int count );
  void freeTexture ( unsigned int texture );
private:
  bool switchRGB2BGR;         //!< Status variable that indicates wheather red and blue components in image data are interchanged
  std::vector<int> textures;  //!< Vector that holds the OpenGL created texture names, -1 for no texture
  TextureLoader *texLoader;   //!< Abstract class that is used to load an texture file into memory

public:
  void bindTexture( unsigned int texture ); // this texture is _not_ the OpenGL texture name but an application dependent!
};
