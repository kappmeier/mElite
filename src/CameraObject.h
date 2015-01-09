/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	CameraObject.h
 * \brief Declares the class CameraObject used for a camera or eye in the 3d environment.
 */
#pragma once
#include "MovingObject.h"
#include "Vector3.h"

//! Extends the functionality of a MovingObject to the ones of a camera, usable in games. 
/**
 * It therefore adds a new type of moving: strafing.
 * The camera allows to set up an projection using GLU functions, it also
 * has the ability to change the direction of view dynamically, such as to 
 * look backwards. But it does not change anything in the actual displaying of
 * the program, so if view frustum culling or something equivalent is used,
 * the displayed items have to changed outside this class.
 */
class CameraObject :
  virtual public MovingObject {
    // constructor and initialization
public:
  CameraObject( void );
  CameraObject( Vector3 position );
  CameraObject( float x, float y, float z );
  ~CameraObject(void);

  // acting as camera: provide projection
public:
  bool isRearView( void );
  void look( void );
  void toggleRearView( void );
private:
  bool m_rearView;

  // additional moving: strafing
public:
  void moveDown( float t );
  void moveLeft( float t );
  void moveRight( float t );
  void moveUp( float t );
  void setMoveSpeed( float t );
private:
  float m_moveSpeed;
};