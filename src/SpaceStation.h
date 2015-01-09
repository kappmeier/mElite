/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpaceStation.h
 * \brief Declaration of the SpaceStation class.
 */
#pragma once
#include "SpinningObject.h"
#include "ModelObject.h"
#include "Vector3.h"

//! Represents a space station. 
/**
 * It is derived from the ModelObject 
 * class which means, the station is drawable using the base draw() method. As
 * a station does not move but is rotating it inherits from the clas
 * SpinningObject.
 */
class SpaceStation :
  public SpinningObject, public ModelObject {
public:
  SpaceStation( float r, GLuint displayList );
  SpaceStation( Vector3 position, float r, GLuint displayList );
  SpaceStation( float x, float y, float z, float r, GLuint displayList );
  ~SpaceStation( void );
  // flying into the station
public:
  Vector3 getOutVector( void );
  Vector3 getOutPosition( void );
  bool test( const Vector3 &position );

  //drawing
public:
  void draw();

  // shooting
public:
//  receiveHit ( );
private:
  
};
