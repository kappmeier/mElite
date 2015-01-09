/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	QuadricObject.h
 * \brief Definition of the QuadricObject class, that is a representation of an object that can be drawn using OpenGL quadrics.
 */
#pragma once
#include "GlobalGL.h"
#include "GraphicsObject.h"
#include "Vector3.h"

//! An extension of GraphicsObject that can draw using OpenGL quadrics (GLU)
class QuadricObject :
  virtual public GraphicsObject {
public:
  QuadricObject( GLuint textureName );
  QuadricObject( float r, GLuint textureName );
  QuadricObject( Vector3 position, float r, GLuint textureName );
  QuadricObject( float x, float y, float z, float r, GLuint textureName );
  ~QuadricObject( void );
private:
  QuadricObject( void );

  // drawing: texture, color
public:
  Point4 getColor( void );
  GLuint getTexture( void );
  void setColor( Vector3 color );
  void setColor( float r, float g, float b );
  void setColor( float r, float g, float b, float a );
  void setTexture( GLuint texture );

public:
  GLuint m_texture;
  Point4 m_color;

};
