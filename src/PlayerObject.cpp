/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	PlayerObject.cpp
 * \brief Implementation of the class representing the player in the 3d environment.
 */
#include "PlayerObject.h"

PlayerObject::PlayerObject( float r, GLuint displayList ) : CameraObject( ), SpaceShip( r, displayList ), MovingObject( ) {
  setShieldRecovering( true );
}

PlayerObject::PlayerObject( Vector3 position , float r, GLuint displayList ) : CameraObject( position ), SpaceShip( r, displayList ), MovingObject( position ) {
  setShieldRecovering( true );
}

PlayerObject::PlayerObject( float x, float y, float z, float r, GLuint displayList ) : CameraObject( x, y, z ), SpaceShip( r, displayList ), MovingObject( x, y, z ), GameObject( x, y, z ) {
  setShieldRecovering( true );
}

PlayerObject::~PlayerObject(void) {

}
void PlayerObject::coolLaser( DWORD time ) {
  SpaceShip::coolLaser( 3 * time );
}
