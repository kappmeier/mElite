/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	GameObject.cpp
 * \brief	Implements the base class for objects in 3d space.
 */
#include "GameObject.h"

/**
 * \brief The constructor creates the object. The position is set to zero by default.
 */
GameObject::GameObject( void ) {
  setPos( 0, 0, 0 );
}

/**
 * \brief The constructor creates the object and gives it a position in the scene.
 * \param position the position as Vector3 
 */
GameObject::GameObject( Vector3 position ) {
  setPos( position );
}

/**
 * \brief The constructor creates the object and gives it a position in the scene.
 * \param x the x-position of the object 
 * \param y the x-position of the object
 * \param z the x-position of the object
 */
GameObject::GameObject( float x, float y, float z ) {
  setPos( x, y, z );
}

/**
 * \brief Destructor. Does nothing.
 */
GameObject::~GameObject(void) {
}

/**
 * \brief Returns the current position as Vector3 triple. 
 */
Vector3 GameObject::getPos( void ) {
  return m_position;
}

/**
 * \brief Sets the current position. 
 * \param position the new position as vector3
 */
void GameObject::setPos( Vector3 position ) {
  m_position = position;
}

/**
 * \brief Sets the current position. 
 * \param x the new x-position
 * \param y the new y-position
 * \param z the new z-position
 */
void GameObject::setPos( float x, float y, float z ) {
  m_position.x = x;
  m_position.y = y;
  m_position.z = z;
}
