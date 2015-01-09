/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Vector3.cpp
 * \brief	Implementation of simple vector in 3d space.
 */
#include "Vector3.h"
#include <cmath>

Vector3::Vector3() {
  x = 0;
  y = 0;
  z = 0;
}

Vector3::Vector3( const Vector3 &v ) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3::Vector3( float x, float y, float z ) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3::~Vector3() {
}

float Vector3::at( Vector3Index index ) {
  switch( index ) {
case Vector3Index::x:
  return this->x;
  break;
case Vector3Index::y:
  return this->y;
  break;
case Vector3Index::z:
  return this->z;
default:
  return 0;
  }
}

float Vector3::at( unsigned __int8 index ) {
  switch( index ) {
case 0:
  return this->x;
  break;
case 1:
  return this->y;
  break;
case 2:
  return this->z;
default:
  return 0; // no exception is thrown
  }
}

/**
 * Addition of an vector.
 * \param v The vector to add
 */
Vector3 Vector3::operator+(const Vector3 &v) {
  Vector3 result;

  result.x = x + v.x;
  result.y = y + v.y;
  result.z = z + v.z;

  return result;
}

/**
 * Subtraction of an vector.
 * \param v The vector to substract 
 */
Vector3 Vector3::operator -( const Vector3 &v ) {
  Vector3 result;
  result.x = x - v.x;
  result.y = y - v.y;
  result.z = z - v.z;
  return result;
}

/**
 * Changes the sign of the vector.
 */
Vector3 Vector3::operator-(void) {
  Vector3 result;
  result.x = -x;
  result.y = -y;
  result.z = -z;
  return result;
}

/**
 * Scalar multiplikation.
 */
Vector3 Vector3::operator*(float t) {
  Vector3 result;
  result.x = x  * t;
  result.y = y  * t;
  result.z = z  * t;
  return result;
}


/**
 * Scalar division.
 */
Vector3 Vector3::operator/(float t) {
  Vector3 result;
  result.x = x / t;
  result.y = y / t;
  result.z = z / t;
  return result;
}

/**
 * Cross product.
 */
Vector3 Vector3::operator*(const Vector3 &v) {
  Vector3 result;
  result.x = y  * v.z - z  * v.y;
  result.y = z  * v.x - x  * v.z;
  result.z = x  * v.y - y  * v.x;
  return result;
}


Vector3 Vector3::operator =( const Vector3 &v ) {
  Vector3 result = Vector3( v );
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
  return result;
}


/**
 * Calculates the euclidian length of a vector. 
 */
float Vector3::length() {
  return (float)sqrt( x*x + y*y + z*z );
}

/**
 * Normalizes the vector.
 */
void Vector3::normalize() {
  float len;
  len = length();
  if (len) {// only dvide if len is not equal to zero
    x /= len;
    y /= len;
    z /= len;
  }
}


/**
 * Calculates the inner product with an vector. 
 */
float Vector3::innerProduct( const Vector3 &v ) { // or scalar product (_not_ multiplication with scalar!)
  return x  * v.x + y  * v.y + z  * v.z;
}

/**
 * Copys the value of a Vector3 into this instance. 
 */
void Vector3::copy( const Vector3 &v ) {
  x = v.x;
  y = v.y;
  z = v.z;
}

/**
 * Sets the value of all components of the vector at the same time. 
 */
void Vector3::set( float x, float y, float z ) {
  this->x = x;
  this->y = y;
  this->z = z;
}

/**
 * Multiplikates this vector with a scalar value. 
 */
Vector3 Vector3::scalarMult( float a ) {
  Vector3 result;
  result.x = x  * a;
  result.y = y  * a;
  result.z = z  * a;
  return result;
}
