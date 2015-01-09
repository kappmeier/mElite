/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Frustum.cpp
 * \brief Implementation of a Frustum
 */
#include "Frustum.h"

Frustum::Frustum( FrustumInformation frustum ) {
  this->frustum = frustum;
}

Frustum::~Frustum( void ) {
}

//! Sets the current frustum.
/**
 * Defines the size of the frustum that is used for the calculations. If the frustum is changed
 * i.e. because the window is reshaped the frustum needs to be updated.
 * \param frustum The current frustum
 */
void Frustum::setFrustumInformation( FrustumInformation frustum ) {
  this->frustum = frustum;
}

//! Updates the frustum.
/**
 * Calculates the eight edges of a frustum and the six bounding planes. These can be used tho
 * verify whether an object is inside the frustum or not (or maybe intersects it).
 * \param camera The camera object to which the frustum belongs.
 */
void Frustum::update( CameraObject *camera ) {

  // calculate positions of far plane edges
  Vector3 farCenter = camera->getPos() + camera->getView()  * frustum.farDist;
  farTopLeft= farCenter + (camera->getUp()  * frustum.farHeight*0.5) - (camera->getRight()  * frustum.farWidth*0.5);
  farTopRight= farCenter + (camera->getUp()  * frustum.farHeight*0.5) + (camera->getRight()  * frustum.farWidth*0.5);
  farBottomLeft= farCenter - (camera->getUp()  * frustum.farHeight*0.5) - (camera->getRight()  * frustum.farWidth*0.5);
  farBottomRight = farCenter - (camera->getUp()  * frustum.farHeight*0.5) + (camera->getRight()  * frustum.farWidth*0.5);

  // calculate positions of near plane edges
  Vector3 nearCenter = camera->getPos() + camera->getView()  * frustum.nearDist;
  nearTopLeft= nearCenter + (camera->getUp()  * frustum.nearHeight*0.5) - (camera->getRight()  * frustum.nearWidth*0.5);
  nearTopRight= nearCenter + (camera->getUp()  * frustum.nearHeight*0.5) + (camera->getRight()  * frustum.nearWidth*0.5);
  nearBottomLeft= nearCenter - (camera->getUp()  * frustum.nearHeight*0.5) - (camera->getRight()  * frustum.nearWidth*0.5);
  nearBottomRight = nearCenter - (camera->getUp()  * frustum.nearHeight*0.5) + (camera->getRight()  * frustum.nearWidth*0.5);

  // now compute the six planes bounding the frustum. the points are given in counter clockwise
  // order so that all normals point inside the frustum. that will us easyly allow to check if a point is
  // inside the frustum
  planes[topPlane].setPlane( nearTopRight, nearTopLeft, farTopLeft );
  planes[bottomPlane].setPlane( nearBottomLeft, nearBottomRight, farBottomRight );
  planes[leftPlane].setPlane( nearTopLeft, nearBottomRight, farBottomLeft );
  planes[rightPlane].setPlane( nearBottomRight, nearTopRight, farBottomRight );
  planes[nearPlane].setPlane( nearTopLeft, nearTopRight, nearBottomRight );
  planes[farPlane].setPlane( farTopRight, farTopLeft, farBottomLeft );
}

//! Checks if a point is inside the frustum.
/**
 * Tests if a single point is inside the frustum or outside. Therefor the
 * signed distance is used. The point is tested with each plane and if the 
 * distance is greater than zero it lies on the right side. If the point lies
 * on the right side of every plane it's inside the frustum. If it is on the 
 * wrong side of only one plane, that means, the distance is zero or negative,
 * the point is not in the frustum and the calculation can be stopped.
 */
CullingLocation Frustum::isPointInFrustum( const Vector3 &p ) {
  for(int i=0; i < 6; i++) {
    if (planes[i].distance(p) < 0)
      return outside;
  }
  return inside;
}

//! Tests if a sphere intersects the frustum.
/**
 * Tests if a sphere is inside the frustrum. It also has to be tested if the 
 * sphere intersects with the frustum. For each plane is tested, if the
 * distance between it and the center of the sphere is greater than the radius
 * of the sphere. If it's definitly outside, the calculation can be stopped. 
 */
CullingLocation Frustum::isSphereInFrustum( const Vector3 &p, float radius ) {
  float distance;
  CullingLocation location = inside;

  for( int i=0; i < 6; i++ ) {
    distance = planes[i].distance( p );
    if( distance < -radius )
      return outside;
    else if( distance < radius )
      location = intersect;               // could be outside of another plane!
  }
  return location;
}

//! Checks if a point is in the upper half of the view frustum.
/**
 * The functions returns false if the point is in the lower half of the frustum or outside the
 * frustum. The result can't be used to create an opposite function 'isInUpperHalf'.
 * \param p The point that is to be tested
 * \return True is returned if and only if the given point is in the upper half
 */
bool Frustum::isInUpperHalf( const Vector3 &p ) {
  float dist1 = planes[topPlane].distance( p );
  float dist2 = planes[bottomPlane].distance( p );
  if( dist1 < 0 )
    return false;
  if( dist1 < dist2 )
    return true;
  else
    return false;
}

//! Checks if a point is in the right half of the view frustum.
/**
 * The functions returns false if the point is in the left half of the frustum or outside the
 * frustum. The result can't be used to create an opposite function 'isInLeftHalf'.
 * \param p The point that is to be tested
 * \return True is returned if and only if the given point is in the right half
 */
bool Frustum::isInRightHalf( const Vector3 &p ) {
  float dist1 = planes[rightPlane].distance( p );
  float dist2 = planes[leftPlane].distance( p );
  if( dist1 < 0 )
    return false;
  if( dist1 < dist2 )
    return true;
  else
    return false;
}

//! Calculates the frustum width at a given distance.
/**
 * If the distance is longer than the furstums far distance or shorter than the near distance
 * zero is returned. Otherwise the width is calculated using the formula
 * \f$ width = height * aspect \f$
 * \param dist The distance for what the width is calculated
 * \return Width of the furstum at the specified distance                      
 */
float Frustum::getFrustumWidth( float dist ) {
  if( dist < frustum.nearDist || dist > frustum.farDist )
    return 0;
  
  float val = getFrustumHeight( dist ) * frustum.aspect;
  return val;
}


//! Calculates the frustum height at a given distance.
/**
 * If the distance is longer than the furstums far distance or shorter than the near distance
 * zero is returned. Otherwise the width is calculated using the formula
 * \f$ height = 2 * \tan( fov * \frac{1}{2} * ANGLE2DEG ) * distance \f$
 * \param dist The distance for what the height is calculated
 * \return Height of the frustum at the specified distance
 */
float Frustum::getFrustumHeight( float dist ) {
  if( dist < frustum.nearDist || dist > frustum.farDist )
    return 0;

  float val = 2 * tan( frustum.fov * ANGLE2DEG * 0.5 ) * dist;
  return val;
}
