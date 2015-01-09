/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	FileAccess.h
 * \brief	Defines a static class and needed data structures
 *          to load needed files for the game. These include
 *          textures and game objects.
 */

#pragma once

#include "Structures.h"
#include <string>
#include <vector>

class Vector3;

/**
 * \brief Contains the necessary data for a texture in mElite.
 *
 * A texture is defined by its name (the file name on the hard drive), the
 * type of the image (bmp as default). Additionally, textures for mElite are
 * produced in different qualities (resolutions). These are reflected by 
 * different prefixes to the file name, such as xs and l.
 */
struct TextureAccess {
  TextureAccess() : name( "" ), hasQualities( false ), type( "bmp" ) {}
  TextureAccess( std::string n, bool h, std::string t ) : name(n), hasQualities(h), type(t) {}

  std::string name;// should contain the filename without ending _and_ without size information e.g. _xs
  bool hasQualities;// is set to true, if different sizes are available
  std::string type;// should contain the ending
};

/**
 * \brief Contains the necessary data for a 3d object in mElite.
 *
 * An object is loaded from a file (which is given by its file name). During the
 * load process, the size of the object can be scaled. Each object has a view
 * direction (e. g. for space ships, this is the direction in which the ships
 * are flying, if they are flying forward).
 */
struct ObjectAccess {
  ObjectAccess() : name(""), view(0,0,1), factor(1) {}
  ObjectAccess( std::string n, Vector3 v, float f) : name(n), view(v), factor(f) {}
  std::string name;
  Vector3 view;
  float factor;
};

/**
 * \brief Static class which allows all the file access.
 *
 * Handling the file access means all accesses to textures and objects
 * are handled. Included are methods to load files containt images and
 * 3d game objects, retrieving information about loaded objects and
 * modify them in a way that they can be used in the program.
 */
class FileAccess {
  // textures
public:
  static TextureSlot getPlanetSlot( unsigned int index );
  static std::string getTextureFile( TextureName texture );
  static TextureAccess getTextureInfo( TextureName texture );
  static void loadTexture( TextureSlot slot, TextureName texture );
private:
  static const int planetSlotCount = 10;
  static const TextureSlot planetSlots[];
  static const TextureAccess textureInfos[];

  // objects
public:
  static std::string getObjectFile( ObjectName object );
  static ObjectAccess getObjectInfo( ObjectName object );
  static Vector3 getObjectView( ObjectName object );
  static float getObjectFactor( ObjectName object );
  static void loadObject( ObjectName object );
private:
  static const ObjectAccess objectInfos[];
};
