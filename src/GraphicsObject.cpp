/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	GraphicsObject.cpp
 * \brief Implementation of a general graphical object in 3d environment that is drawable.
 */
#pragma once

#include "GraphicsObject.h"

//! The constructor creates the object.
/**
 * The position and radius are set to 0 by default.
 */
GraphicsObject::GraphicsObject( void ) : GameObject() {
  m_radius = 0;
}

//! The constructor creates the object.
/**
 *  The position is set to zero by default.
 * \param r the objects radius
 */
GraphicsObject::GraphicsObject( GLfloat r ) : GameObject() {
  m_radius = r;

}

//! Creates an graphical object. 
/**
 * Needed is the position and the radius. The 
 * radius is used for collision detection and the view frustum culling.
 * \param position the position of the object in 3-dimensional space
 * \param r the radius of the object
 */
GraphicsObject::GraphicsObject( Vector3 position, float r ) : GameObject( position ) {
  m_radius = r;
}

//! Creates a graphical object.
/**
 * Needed is the position and the radius. The 
 * radius is used for collision detection and the view frustum culling.
 * \param x the x-position of the object 
 * \param y the x-position of the object 
 * \param z the x-position of the object 
 * \param r the radius of the object
 */
GraphicsObject::GraphicsObject( float x, float y, float z, float r ) : GameObject( x, y, z ) {
  m_radius = r;
}

/**
 * \brief Destructor 
 */
GraphicsObject::~GraphicsObject(void) {

}

/**
 * \brief Returns the radius of the object, usable for collision detection.
 */
float GraphicsObject::getCollisionRadius( void ) {
  return m_radius;
}

/**
 * \brief Returns the radius of the object, usable for view frustum culling.
 *
 * This function is basically the same as getCollisionRadius( void ). It is used, because
 * some objects (as Planet) have two kinds of radius which allows to have collisions in
 * another area than the actual object's size.
 */
float GraphicsObject::getRadius( void ) {
  return m_radius;
}
