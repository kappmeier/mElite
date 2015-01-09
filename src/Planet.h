/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Planet.h
 * \brief Declaration of Planet class representing a Planet in 3d space.
 */
#pragma once
#include "GlobalGL.h"
#include "Structures.h"// CullingPlanet

#include "SpinningObject.h"// base class
#include "QuadricObject.h"// base class

//#include "Frustum.h" // frustum object

class Planet :
  public SpinningObject, public QuadricObject {
public:
  Planet( float planetRadius, float collisionRadius, GLuint textureName );
  Planet( float x, float y, float z, float planetRadius, GLuint textureName );
  Planet( float x, float y, float z, float planetRadius, float collisionRadius, GLuint textureName );
  ~Planet( void );
  void draw( void );
  float getRotateSpeed( void );
  void setRotateAngle( float angle );
  void setRotateSpeed( float speed );
  void setRadiusLineDrawing( bool draw );
  Vector3 getPos();
  //void setFrustum( Frustum *frustum );
  //void setCulling( CullingPlanet cullType );
private:
  float m_planetRadius;
  float m_rotateAngle;
  float m_rotateSpeed;
  bool m_drawRadiusLine;
  //Frustum * m_frustum;
  //CullingPlanet m_cullType;
};
