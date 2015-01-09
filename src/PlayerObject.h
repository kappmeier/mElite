/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	PlayerObject.h
 * \brief Declaration of the PlayerObject class, representing the player in the game.
 */
#pragma once
#include "CameraObject.h"
#include "GlobalGL.h"
#include "SpaceShip.h"
#include "Vector3.h"

//! This class represents the player.
/**
 * A player combines a camera and a space ship.
 */
class PlayerObject :
  public CameraObject, public SpaceShip {
    // constructors
public:
  PlayerObject::PlayerObject( float r, GLuint displayList );
  PlayerObject::PlayerObject( Vector3 position , float r, GLuint displayList );
  PlayerObject::PlayerObject( float x, float y, float z, float r, GLuint displayList );
  ~PlayerObject(void);
  void coolLaser( DWORD time );
};
