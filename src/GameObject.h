/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	GameObject.h
 * \brief	Defines the base class for the 3d game objects.
 */
 #pragma once

#include "Vector3.h"

//! Base class for 3d game objects.
/**
 * This class represents the basic game object used in the game. A game object
 * basically has a position and does nothing.
 */
class GameObject{
  // constructor and initialization
public:
  GameObject( void );
  GameObject( Vector3 position );
  GameObject( float x, float y, float z );
  ~GameObject( void );

  // positioning
public:
  virtual Vector3 getPos( void );
  void setPos( float x, float y, float z );
  void setPos( Vector3 position );
private:
  Vector3 m_position;// position of the object
};
