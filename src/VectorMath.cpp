/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	VectorMath.cpp
 * \brief	Implementation of some more advanced vector math operations.
 */
#include "GlobalGL.h"
#include "VectorMath.h"

/**
 * Calculates a normal of three given points defining a triangle.
 * \param p1 point on a triangle
 * \param p2 point on a triangle
 * \param p3 point on a triangle
 */
Vector3 VectorMath::normal( Vector3 p1, Vector3 p2, Vector3 p3 ) {
  Vector3 a, b, v;

  a.x = p2.x - p1.x;
  a.y = p2.y - p1.y;
  a.z = p2.z - p1.z;

  b.x = p3.x - p1.x;
  b.y = p3.y - p1.y;
  b.z = p3.z - p1.z;

  v.x = a.y*b.z - b.y*a.z;
  v.y = a.z*b.x - b.z*a.x;
  v.z = a.x*b.y - b.x*a.y;

  v.normalize();
  return v; // normalize(v);
}

/**
 * Rotates a vector around an axis.
 * \param angle angle by which the vector is rotated
 * \param axis rotation axis
 * \param oVec base point of rotation
 */
Vector3 VectorMath::rotateVector( float angle, Vector3 axis, Vector3 oVec ) {
  Vector3 nVec;

  float x=axis.x;
  float y=axis.y;
  float z=axis.z;

  // sine and cosine of the angle
  float cosTheta = (float)cos( angle  * M_PI/180.0 );
  float sinTheta = (float)sin( angle  * M_PI/180.0 );

  // new x
  nVec.x= (cosTheta + (1 - cosTheta)  * x  * x)	 * oVec.x;
  nVec.x += ((1 - cosTheta)  * x  * y - z  * sinTheta)	 * oVec.y;
  nVec.x += ((1 - cosTheta)  * x  * z + y  * sinTheta)	 * oVec.z;

  // new y
  nVec.y= ((1 - cosTheta)  * x  * y + z  * sinTheta)	 * oVec.x;
  nVec.y += (cosTheta + (1 - cosTheta)  * y  * y)	 * oVec.y;
  nVec.y += ((1 - cosTheta)  * y  * z - x  * sinTheta)	 * oVec.z;

  // new z
  nVec.z= ((1 - cosTheta)  * x  * z - y  * sinTheta)	 * oVec.x;
  nVec.z += ((1 - cosTheta)  * y  * z + x  * sinTheta)	 * oVec.y;
  nVec.z += (cosTheta + (1 - cosTheta)  * z  * z)	 * oVec.z;

  return nVec;
}
