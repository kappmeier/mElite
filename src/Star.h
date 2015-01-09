/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Star.h
 * \brief Definition of a star as a special kind of planet.
 */
#pragma once
#include "Planet.h"
#include "Vectormath.h"

//! A Star is a special type of Planet that has supports lighting.
class Star :
  public Planet {
public:
  Star( float planetRadius, float collisionRadius, GLuint textureName );
  Star( float x, float y, float z, float planetRadius, GLuint textureName );
  Star( float x, float y, float z, float planetRadius, float collisionRadius, GLuint textureName );
  ~Star(void);
  GLvoid draw( GLvoid );
  GLvoid setLightColor( Vector3 lightColor );
  GLvoid setLightColor( float r, float g, float b );
  GLvoid setPos( GLfloat x, GLfloat y, GLfloat z );
  GLvoid setPos( Vector3 position );
private:
  Vector3 m_lightColor;
  GLfloat m_lightPosition[4];
};
