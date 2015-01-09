/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	CameraObject.cpp
 * \brief Implementation of a camera in 3d space. The camera is a Object that has no drawing abilities, but a lot of possible movements.
 */
#include "CameraObject.h"
#include "GlobalGL.h"

//! Initializes the camera residing in the center of the scene.
/**
 * Start coordinates are (0,0,0) for the eye with view vector (0,0,1) and
 * direction (0,1,0) for up. This equals a position in the center viewing along
 * z-axis with y-axis pointing upwards.
 */
CameraObject::CameraObject( ) : MovingObject( ) {
  m_rearView = false;
  m_moveSpeed = 1;
}

//! Initializes the eye with a given position but standard view direction.
/**
 * Start coordinate for the eye is given by position. The view vector is set to (0,0,1) and
 * direction (0,1,0) is set for up. This equals a position in the center viewing along
 * z-axis with y-axis pointing upwards.
 * \param position the initial position of the eye
 */
CameraObject::CameraObject( Vector3 position ) : MovingObject( position ) {
  m_rearView = false;
  m_moveSpeed = 1;
}

//! Initializes the eye with a given position but standard view direction.
/**
 * Start coordinate for the eye is given by coordinates in 3d space. The view vector
 * is set to (0,0,1) and direction (0,1,0) is set for up. This equals a position in
 * the center viewing along z-axis with y-axis pointing upwards.
 * \param x x-coordinate for the eye
 * \param y y-coordinate for the eye 
 * \param z z-coordinate for the eye 
 */
CameraObject::CameraObject( float x, float y, float z ) : MovingObject( x, y, z ) {
  m_rearView = false;
  m_moveSpeed = 1;
}

/**
 * \brief Destructor 
 */
CameraObject::~CameraObject(void) {

}

/**
 * \brief Checks, if rear view is enabled.
 */
bool CameraObject::isRearView() {
  return m_rearView;
}


/**
 * \brief Moves the camera to the position defined by eye and adjusts it to view to 
 * the correct position. 
 */
void CameraObject::look() {
  if( m_rearView )
    gluLookAt( getPos().x, getPos().y, getPos().z, getPos().x - getView().x, getPos().y - getView().y, getPos().z - getView().z, getUp().x, getUp().y, getUp().z );
  else
    gluLookAt( getPos().x, getPos().y, getPos().z, getPos().x + getView().x, getPos().y + getView().y, getPos().z + getView().z, getUp().x, getUp().y, getUp().z );
}

/**
 * \brief Switches to rear view and back to normal view. 
 */
void CameraObject::toggleRearView() {
  m_rearView = !m_rearView;
}

//! Moves the camera position downwards.
/**
 * The factor specifies how much the camera is moved and is multiplicted with the current movement speed.
 * In a dynamic situation it can be used to reflect passed time since the last update occured.
 * \param t a factor for the distance 
 */
void CameraObject::moveDown( float t ) {
  setPos( getPos().x - t  * m_moveSpeed  * getUp().x, getPos().y - t  * m_moveSpeed  * getUp().y, getPos().z - t  * m_moveSpeed  * getUp().z);
}

//! Moves the camera position to the left.
/**
 * The factor specifies how much the camera is moved and is multiplicted with the current movement speed.
 * In a dynamic situation it can be used to reflect passed time since the last update occured.
 * \param t a factor for the distance 
 */
void CameraObject::moveLeft( float t ) {
  setPos(getPos().x - t  * m_moveSpeed  * getRight().x, getPos().y - t  * m_moveSpeed  * getRight().y, getPos().z - t  * m_moveSpeed  * getRight().z);
}

//! Moves the camera position to the right.
/**
 * The factor specifies how much the camera is moved and is multiplicted with the current movement speed.
 * In a dynamic situation it can be used to reflect passed time since the last update occured.
 * \param t a factor for the distance 
 */
void CameraObject::moveRight( float t ) {
  setPos(getPos().x + t  * m_moveSpeed  * getRight().x, getPos().y + t  * m_moveSpeed  * getRight().y, getPos().z + t  * m_moveSpeed  * getRight().z);
}

//! Moves the camera position upwards.
/**
 * The factor specifies how much the camera is moved and is multiplicted with the current movement speed.
 * In a dynamic situation it can be used to reflect passed time since the last update occured.
 * \param t a factor for the distance 
 */
void CameraObject::moveUp( float t ) {
  setPos(getPos().x + t  * m_moveSpeed  * getUp().x, getPos().y + t  * m_moveSpeed  * getUp().y, getPos().z + t  * m_moveSpeed  * getUp().z);
}

/**
 * \brief Sets the speed for camera movement. 
 * speed the speed
 */
void CameraObject::setMoveSpeed( float speed ) {
  m_moveSpeed = speed;
}
