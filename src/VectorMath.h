/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	VectorMath.h
 * \brief	Declares some structures for OpenGL math operations and vector math operations
 */
#pragma once

#include "Vector3.h"

typedef struct MaterialStruct {
  float emission[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float shininess[1];
} Material;



typedef struct Point2Struct {
  float x, y;
} Point2;

typedef struct Point4Struct {
  float x, y, z, a;
} Point4, Vector4;

class VectorMath{
public:
  static Vector3 normal( Vector3 p1, Vector3 p2, Vector3 p3 );
  static Vector3 rotateVector( float angle, Vector3 axis, Vector3 vView );
  static void drawEliteObject2(int numberOfFaces, Vector3 vertex[], long face[], Material material[]);
};