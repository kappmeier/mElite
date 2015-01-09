/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Shapes.h
 * \brief	Defines some methods to draw 2 dimensional shapes using OpenGL
 */
#pragma once

#include "Vector3.h"

class Shapes {
public:
  static void drawCircle( Vector3 pos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface );
  static void drawCircle( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface );
  static void drawCircleTex( Vector3 pos, GLint n, GLfloat r );
  static void drawCircleTex( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r );
  static void drawCircleTex( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface );
  static void drawCircle( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat *colorCenter, const GLfloat *colorSurface );
  static void drawCircleShape( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat * color );
  static void drawCircleShape( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat * color );
  static void drawCircleShape( Vector3 pos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat * color );
  static void drawCircleShapeDashed( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat * color );
  static void drawRect( GLfloat xCenterPos, GLfloat yCenterPos, GLfloat zPos, GLfloat width, GLfloat height );
private:
  Shapes( void );
  ~Shapes( void );
};
