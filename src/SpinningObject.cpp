/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpinningObject.cpp
 * \brief	Defines the class for spinning objects in 3d game world.
 */
#include "SpinningObject.h"

/**
 * Initializes the object with position in the origin and y-axis as axis of
 * rotation.
 */
SpinningObject::SpinningObject( ) : GameObject( ) {
  m_axis.x = 0;
  m_axis.y = 1;
  m_axis.z = 0;
  m_speed = 0;
}

/**
 * Initializes the object at given position and with y-axis as axis of rotation
 * \param position position as Vector3
 */
SpinningObject::SpinningObject( Vector3 position ) : GameObject( position ) {
  m_axis.x = 0;
  m_axis.y = 1;
  m_axis.z = 0;
  m_speed = 0;
}

/**
 * Initializes the object at given position and with y-axis as axis of rotation
 * \param x the x-position 
 * \param y the y-position 
 * \param z the z-position 
 */
SpinningObject::SpinningObject( GLfloat x, GLfloat y, GLfloat z ) : GameObject( x, y, z ) {
  m_axis.x = 0;
  m_axis.y = 1;
  m_axis.z = 0;
  m_speed = 0;
}

/**
 * Destructor 
 */
SpinningObject::~SpinningObject(void) {
}

/**
 * Sets the spin axis. 
 * \param axis the axis of rotation 
 */
void SpinningObject::setAxis( Vector3 axis ) {
  m_axis = axis;
}

/**
 * Sets the spin axis. 
 * \param x the x-coordinate of the axis of rotation
 * \param y the y-coordinate of the axis of rotation
 * \param z the z-coordinate of the axis of rotation
 */
void SpinningObject::setAxis( float x, float y, float z ) {
  m_axis.x = x;
  m_axis.y = y;
  m_axis.z = z;
}

/**
 * Sets the rotate speed.
 * \param speed the speed
 */
void SpinningObject::setSpeed( float speed ) {
  m_speed = speed;
}

/**
 * Rotates the object using the glRotate*() function.
 */
void SpinningObject::spin( void ) {
  glRotatef( glutGet(GLUT_ELAPSED_TIME)  * m_speed, m_axis.x, m_axis.y, m_axis.z ); // rotation for a day
}