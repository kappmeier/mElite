/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Plane.cpp
 * \brief	Implementation of a plane in 3d.
 */
#include "Plane.h"

/**
 * \brief Constructs a plane defined by three points in 3d space.
 */
Plane::Plane( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3 ) {
  setPlane(v1,v2,v3);
}

/**
 * \brief Constructs a plane defined by a point on the plane and the normal.
 */
Plane::Plane( Vector3 normal, Vector3 point){
	setPlane(normal,point);
}

Plane::Plane() { }

Plane::~Plane( void ) { }

/**
 * Sets up the plane defined through three points on it. 
 * \param v1 the first point 
 * \param v2 the second point
 * \param v3 the third point
 */
void Plane::setPlane( const Vector3 &v1, const Vector3 &v2, const Vector3 &v3 ) {
  Vector3 aux1, aux2;
  Vector3 h1 = v1;
  Vector3 h3= v3;
  aux1 = h1 - v2;
  aux2 = h3 - v2;
  normal = aux2  * aux1;
  normal.normalize();
  point.copy(v2);
  d = -(normal.innerProduct(point));
}

/**
 * Sets up the plane defined through its normal and one point on the plane.
 * \param normal the planes normal
 * \param point one point on the plane
 */
void Plane::setPlane( const Vector3 &normal, const Vector3 &point ) {
  this->normal.copy( normal );
  this->point.copy( point );
  this->normal.normalize();
  d = -(this->normal.innerProduct(point));
}

/**
 * Sets up the plane defined through the four coefficients of an equation of 
 * the type ax_1 + b_x2 + cx_3 + d = 0.
 * \param a the first parameter
 * \param b the second parameter
 * \param c the third parameter
 * \param d the fourth parameter
 */
void Plane::setPlane( float a, float b, float c, float d ) {
  // set the normal vector
  normal.set(a,b,c);
  //compute the lenght of the vector
  float l = normal.length();
  // normalize the vector
  normal.set(a/l,b/l,c/l);
  // and divide d by th length as well
  this->d = d/l;
}

/**
 * Calculates the signed distance between a point and the plane using the
 * inner product.
 * \param p the point that should be tested.
 */
float Plane::distance( const Vector3 &p ) {
  return d + normal.innerProduct(p);
}

/**
 * Returns the normal of the plane.
 */
Vector3 Plane::getNormal( void ) {
  return this->normal;
}
