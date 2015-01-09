/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpinningObject.h
 * \brief	Defines the class for spinning objects in 3d game world.
 */
#pragma once
#include "GlobalGL.h"
#include "GraphicsObject.h"
#include "Vectormath.h"
#include "Vector3.h"

//! Implements a simple spinning object.
/**
 * It has a position since it
 * is derived from GameObject. To allow rotating it has an axis around which 
 * the object can rotate and a speed.
 */
class SpinningObject :
  virtual public GameObject {
    // constructors
public:
  SpinningObject( );
  SpinningObject( Vector3 position );
  SpinningObject( GLfloat x, GLfloat y, GLfloat z );
  ~SpinningObject( void );

  // rotating
public:
  void setAxis( Vector3 axis );
  void setAxis( float x, float y, float z );
  void setSpeed( float speed );
  void spin( void );
private:
  Vector3 m_axis;
  float m_speed;
};