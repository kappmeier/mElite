/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Objects.h
 * \brief	Defines the singleton objects class collecting 3d objects in the game.
 */

#pragma once

#include "GlobalGL.h"
#include "Structures.h"

#include <vector>
#include <string>

class ObjectLoader3ds;
class Vector3;

/**
 * \brief Contains a list of loaded 3d objects in the game world and loads more objects.
 *
 * Can load objects from 3d studio files. The objects are loaded and then stored
 * in an OpenGL display list. The class acts as a singleton because each object file
 * has to be loaded only once, regardless how often the object is used in the actual
 * scene.
 */
class Objects {
  // singleton stuff
public:
  static Objects  * getObjects( void );
  ~Objects( void );
private:
  static class Objects *m_objects;
  Objects( void );

  // loading, unloading
public:
  void Objects::freeObject( unsigned int object );
  void Objects::loadObject( unsigned int object, std::string filename, float factor );
  void reserve( unsigned int count );
private:
  void loadObject3DS( unsigned int object, std::string filename, float factor );
  GLuint createDisplayList( ObjectLoader3ds * model, float factor );

  // using objects
public:
  GLuint getDisplayList ( unsigned int object );
  Vector3 getObjectView ( unsigned int object );

  // internal data structure
private:
  std::vector<ObjectInfo> objectList;
};
