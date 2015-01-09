/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	GraphicsObject.h
 * \brief Definition of a general graphical object in 3d space.
 */
#pragma once

#include "GlobalGL.h"
#include "GameObject.h"
#include "Vectormath.h"

//! Represents a basic drawable object. 
/**
 * It's derived from GameObject
 * and so has a position. A pure virtual function draw() is provided to use
 * in drawing procedures. Also graphical objects have a radius which is
 * supposed to be used in some tests, such as visibility and collision testing.
 */
class GraphicsObject : virtual public GameObject {
  // constructor and initialization
public:
  GraphicsObject( void );
  GraphicsObject( GLfloat r );
  GraphicsObject( Vector3 position, float r );
  GraphicsObject( float x, float y, float z, float r );
  ~GraphicsObject( void );

  // stuff for graphical object, radius for collisions and drawing
public:
  virtual void draw( void ) = 0;
  float getRadius( void );
  float getCollisionRadius( void );
private:
  float m_radius;// radius of the object, used for collision detection
};
