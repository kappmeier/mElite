/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	MovingObject.cpp
 * \brief	Implements the class for moving objects in 3d game world.
 */
#include "MovingObject.h"
#include "Vector3.h"
#include "Vectormath.h"

/**
 * \brief Constructor, initializes to the origin. The line of vision is aligned to
 * the z axis, the up vector is aligned to the y-axis.
 */
MovingObject::MovingObject( ) : GameObject( ) {
  view = Vector3( 0, 0, 1 ); 
  up = Vector3( 0, 1, 0 );
  m_speed = 0.0;
  m_maxspeed = 1.0;
  m_minspeed = -0.09f;
  m_step = 1.0f/20.0f;
}

/**
 * \brief Constructor initializing to a given position as Vector3.
 * \param position the eye position 
 */
MovingObject::MovingObject( Vector3 position ) : GameObject( position ) {
  view = Vector3( 0, 0, 1 ); 
  up = Vector3( 0, 1, 0 );
  m_speed = 0.0f;
  m_maxspeed = 1.0f;
  m_minspeed = -0.09f;
  m_step = 1.0f/20.0f;
}

/**
 * \brief Constructor, the startposition is (x,y,z).
 * \param x x-coordinate vor eye point
 * \param y y-coordinate for eye point
 * \param z z-coordinate for eye point
 */
MovingObject::MovingObject( float x, float y, float z ) : GameObject( x, y, z ) {
  view = Vector3( 0, 0, 1 ); 
  up = Vector3( 0, 1, 0 );
  m_speed = 0.0f;
  m_maxspeed = 1.0f;
  m_minspeed = -0.09f;
  m_step = 1.0f/20.0f;
}

/**
 * \brief Destructor 
 */
MovingObject::~MovingObject( void ) {

}

//! Pitches the line of vision.
/**
 * Rotates it along the x-axis to look up or down.
 * \param angle the angle of the rotation
 */
void MovingObject::pitch( float angle ) {
  Vector3 xa;

  xa = view * up;
  view = VectorMath::rotateVector( angle, xa, view );
  up = VectorMath::rotateVector( angle, xa, up );
}

//! Rolls the eye.
/**
 * The up vector is rotated around the viewing vector.
 * \param angle the angle of the rotation
 */
void MovingObject::roll( float angle ) {
  up = VectorMath::rotateVector( angle, view, up );
}

/**
 * \brief Returns the vector indiciating what is 'right' of the eye.
 */
Vector3 MovingObject::getRight() {
  return view * up;
}

/**
 * \brief Returns the vector indication what is 'up'.
 */
Vector3 MovingObject::getUp() {
  return up;
}

/**
 * \brief Returns the current viewing vector. 
 */
Vector3 MovingObject::getView() {
  return view;
}

/**
 * \brief Sets the current viewing vector.
 */
void MovingObject::setView( Vector3 v ) { 
  view = v; 
}

//! Sets the speed.
/**
 * Positive values for acceleration, negative to decrease
 * speed. Minimal and maximal speed are defined through the variables maxspeed
 * and minspeed.
 * \param step determines the value to accelerate, a length of stride factor
 */
void MovingObject::accelerate( float step ) {
  m_speed += step  * m_step;
  if( m_speed < m_minspeed )
    m_speed = m_minspeed;
  if( m_speed > this->m_maxspeed )
    m_speed = this->m_maxspeed;
}

/**
 * \brief Move the camera depending on the current speed and line of vision. 
 * \param t sets the time in milliseconds
 */
void MovingObject::advance( float t ) {
  setPos( getPos().x + t  * m_speed  * view.x, getPos().y + t  * m_speed  * view.y, getPos().z + t  * m_speed  * view.z );
}

/**
 * \brief Returns the maximal speed. 
 */
float MovingObject::getMaxSpeed() {
  return m_maxspeed;
}

/**
 * \brief Returns the minimal speed. 
 */
float MovingObject::getMinSpeed() {
  return m_minspeed;
}

/**
 * \brief Returns the vector indicating the direction 'right'.
 */
float MovingObject::getSpeed() {
  return m_speed;
}

/**
 * \brief Sets the maximal speed.
 * \param speed determines the speed
 */
void MovingObject::setMaxSpeed( float speed) {
  m_maxspeed = speed;
}

/**
 * \brief Sets the minimal speed.
 * \param speed determines the speed
 */
void MovingObject::setMinSpeed( float speed ) {
  m_minspeed = speed;
}

/**
 * \brief Sets the current speed.
 * \param speed the speed
 */
void MovingObject::setSpeed( float speed ) {
  if( speed < m_minspeed )
    m_speed = m_minspeed;
  else if( speed > m_maxspeed )
    m_speed = m_maxspeed;
  else
    m_speed = speed;
}

/**
 * \brief Sets the value that is advanced in one step. 
 * \param step the stride
 */
void MovingObject::setStep( float step ) { 
  m_step = step; 
}
