/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Plane.h
 * \brief	Definition of Plane class representing a plane in 3d space.
 */
#pragma once

#include "Vector3.h"

/**
 * \class	Plane
 * \brief	Represents a plane in 3d space.
 * 			
 * 	Several methods of defining the plane are supported. The plane knows its normal and supports
 * 	the operation of calculating the distance of a point to the plane.
 */

class Plane {
  // constructors
public:
  Plane( void );
  Plane( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3 );
  Plane( Vector3 normal, Vector3 point);
  ~Plane( void );

  // setting up the plane
public:
  void setPlane( const Vector3 &normal, const Vector3 &point );
  void setPlane( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3 );
  void setPlane( float a, float b, float c, float d );
private:
  Vector3 normal;
  Vector3 point;
  float d; // used for calculation of signed distance

  // retrieving informations
public:
  float distance( const Vector3 &p ); // calculates signed distance to p
  Vector3 getNormal( void );
};
