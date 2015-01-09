/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	MovingObject.h
 * \brief	Defines the class for moving objects in 3d game world.
 */
#pragma once
#include "GameObject.h"
#include "Vector3.h"

//! Provides the basic functions for an moving object.
/**
 * The provided
 * functionalitys are speed controlling, advancing and rotating in some
 * directions (yaw, pith, roll).
 */
class MovingObject : virtual public GameObject {
  // construction and destruction stuff
public:
  MovingObject( );
  MovingObject( Vector3 position );
  MovingObject( float x, float y, float z );
  ~MovingObject( void );

  // support changing the direction of view: (yaw), roll and pitch
public:
  void pitch( float angle );
  void roll( float angle ); 
  Vector3 getRight( void );
  Vector3 getUp( void );
  Vector3 getView( void );
  void setView( Vector3 v );
private:
  Vector3 view;// direction of view
  Vector3 up;// direction of up

  // speed and actual moving
public:
  void accelerate( float step );
  void advance( float t );
  float getMaxSpeed( void );
  float getMinSpeed( void );
  float getSpeed( void );
  void setMaxSpeed( float speed );
  void setMinSpeed( float speed );
  void setSpeed( float speed );
  void setStep( float step );
private:
  float m_minspeed;// maximal speed
  float m_maxspeed;// maximal speed
  float m_speed; // speed
  float m_step;// step of accelerations
};
