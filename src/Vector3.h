/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Vector3.h
 * \brief	Definition of a simple 3d vector.
 */
#pragma once

enum Vector3Index {
  x = 0,
  y,
  z
};

//! A simple 3d vector and basic operations
/**
 * This class provides vector arithmetics for a 3-dimensional vactor. All
 * members are public, so the three components can directly be accessed. The 
 * basic operations with vectors are supported through operator overloading. 
 * Some other operations are available as member functions.
 */
class Vector3 {
public:

  float x,y,z;

  Vector3( float x, float y, float z );
  Vector3( const Vector3 &v );
  Vector3( void );
  ~Vector3( void );

  Vector3 operator +( const Vector3 &v ); // vector addition
  Vector3 operator -( const Vector3 &v ); // vector subtraction
  Vector3 operator *( const Vector3 &v ); // vector multiplication
  Vector3 operator *( float t ); // scalar multiplication
  Vector3 operator /( float t ); // scalar division
  Vector3 operator -( void ); // negative

  Vector3 operator =( const Vector3 &v );// assign value

  float at( Vector3Index index );
  float at( unsigned __int8 index );

  float length( void );
  void normalize( void );
  float innerProduct( const Vector3 &v );
  void copy( const Vector3 &v );
  void set( float x, float y, float z );
  Vector3 scalarMult( float a );

  //void print();
};
