/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Frustum.h
 * \brief Declaration of the Frustum class.
 */
#pragma once

// global definitions
#include "Structures.h"           // CullingLocation, FrustumInformation

// includes for mathematical objects
#include "Plane.h"                // plane object
#include "Vector3.h"              // vector object
#include "CameraObject.h"         // camera object

//! Allows frustum culling and other frustum calculations.
/**
 * This class provides some operations with the view frustum. The needed
 * information is stored in the FrustumInformation structure. These are size 
 * and position of near and far planes and the field of view and aspect ratio.
 * With these values the class can calculate the current positions of the
 * frustum and the equations for the bording planes from the actual eye point.
 * If this is updated everytime the camera moves, the class provides prooving
 * if an actual object (with spheric shape) is inside the frustum, or not. 
 */
class Frustum {
public:
  Frustum( FrustumInformation frustum );
  ~Frustum( void );
  void update( CameraObject *camera );

  float Frustum::getFrustumWidth( float dist );
  float Frustum::getFrustumHeight( float dist );
  CullingLocation isPointInFrustum( const Vector3 &p );
  CullingLocation isSphereInFrustum( const Vector3 &p, float radius );
  bool isInUpperHalf( const Vector3 &p );
  bool isInRightHalf( const Vector3 &p );
  void setFrustumInformation( FrustumInformation frustum );
private:
  enum {// enumeration that give's us easy access to the planes array
    topPlane = 0,
    bottomPlane,
    leftPlane,
    rightPlane,
    nearPlane,
    farPlane
  };
  Plane planes[6]; // array for the six bording planes of the frustum

  // declaration of the 8 edges of the frustum
  Vector3 farTopLeft;
  Vector3 farTopRight;
  Vector3 farBottomLeft;
  Vector3 farBottomRight;
  Vector3 nearTopLeft;
  Vector3 nearTopRight;
  Vector3 nearBottomLeft;
  Vector3 nearBottomRight;

  FrustumInformation frustum;
};
