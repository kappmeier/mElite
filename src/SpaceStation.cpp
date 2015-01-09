/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpaceStation.cpp
 * \brief Implementation of the behaviour of the Coriolis space station.
 */
#include "SpaceStation.h"
#include "Plane.h"

//! Initializes the space station. 
/**
 * The radius and the display list have to be 
 * submitted if an object is initialized. The starting position is set to the
 * origin.
 * \param r the radius
 * \param displayList the display list
 */
SpaceStation::SpaceStation( float r, GLuint displayList ) : SpinningObject( ), ModelObject( r, displayList ) {
}

//! Initializes the space station. 
/**
 * The radius and the display list have to be 
 * submitted if an object is initialized.
 * \param position the position as Vector3
 * \param r the radius
 * \param displayList the display list
 */
SpaceStation::SpaceStation( Vector3 position, float r, GLuint displayList ) : SpinningObject( position ), ModelObject( r, displayList ) {
}

//! Initializes the space station. 
/**
 * The radius and the display list have to be 
 * submitted if an object is initialized.
 * \param x the x-position
 * \param y the y-position
 * \param z the z-position
 * \param r the radius
 * \param displayList the display list
 */
SpaceStation::SpaceStation( float x, float y, float z, float r, GLuint displayList ) : SpinningObject( x, y, z ), ModelObject( r, displayList ) {
}

/**
 * \brief Destructor 
 */
SpaceStation::~SpaceStation(void) {

}

void SpaceStation::draw() {
  glPushMatrix();
  glTranslatef( getPos().x, getPos().y, getPos().z );
  this->spin();
  ModelObject::draw();

  // zeichne nur viereck
  //float factor = 0.01;
  //glColor3fv( colorRed );
  //glDisable( GL_TEXTURE_2D );
  //glBegin( GL_QUADS ); 
  //glVertex3f( -10  * factor, 30  * factor, 160  * factor);
  //glVertex3f( -10  * factor,-30  * factor, 160  * factor);
  //glVertex3f(10  * factor,-30  * factor, 160  * factor);
  //glVertex3f(10  * factor, 30  * factor, 160  * factor);
  //glEnd();

  glEnable( GL_TEXTURE_2D );
  glPopMatrix();
}

Vector3 SpaceStation::getOutVector( void ) {
  float factor = 0.01;
  Vector3 ant = Vector3( -10*factor,30*factor, 160*factor );
  Vector3 bnt = Vector3( +10*factor,30*factor, 160*factor );
  Vector3 cnt = Vector3( -10*factor, -30*factor, 160*factor );

  GLfloat m[4][4];// define memory space for a single matrix
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( getPos().x, getPos().y, getPos().z );
  this->spin();
  glGetFloatv( GL_MODELVIEW_MATRIX, &m[0][0] );// get the matrix
  glPopMatrix();

  float vectStart[4];
  float vectResult[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  vectStart[0] = ant.x;
  vectStart[1] = ant.y;
  vectStart[2] = ant.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 a = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = bnt.x;
  vectStart[1] = bnt.y;
  vectStart[2] = bnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 b = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = cnt.x;
  vectStart[1] = cnt.y;
  vectStart[2] = cnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 c = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  Plane p = Plane( a, c, b );
  Vector3 ret = p.getNormal();
  ret.normalize();
  return ret;
}

Vector3 SpaceStation::getOutPosition( void ) {
  float factor = 0.01;

  Vector3 cnt = Vector3( 0,0, 160*factor );

  GLfloat m[4][4];// define memory space for a single matrix
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( getPos().x, getPos().y, getPos().z );
  this->spin();
  glGetFloatv( GL_MODELVIEW_MATRIX, &m[0][0] );// get the matrix
  glPopMatrix();

  float vectStart[4];
  float vectResult[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  vectStart[0] = cnt.x;
  vectStart[1] = cnt.y;
  vectStart[2] = cnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 c = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  // c is the point in the center of the coriolis station

  return c + this->getOutVector()  * this->getRadius();
}

bool SpaceStation::test( const Vector3 &position ) {
  float factor = 0.01;
  Vector3 ant = Vector3( -10*factor,30*factor, 160*factor );
  Vector3 bnt = Vector3( +10*factor,30*factor, 160*factor );
  Vector3 cnt = Vector3( -10*factor, -30*factor, 160*factor );
  Vector3 dnt = Vector3( +10*factor, -30*factor, 160*factor );
  Vector3 ant2 = Vector3( -10*factor,30*factor, -160*factor );
  Vector3 bnt2 = Vector3( +10*factor,30*factor, -160*factor );
  Vector3 cnt2 = Vector3( -10*factor, -30*factor, -160*factor );
  //Vector3 dnt2 = Vector3( +10*factor, -30*factor, -160*factor );

  GLfloat m[4][4];// define memory space for a single matrix
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( getPos().x, getPos().y, getPos().z );
  this->spin();
  glGetFloatv( GL_MODELVIEW_MATRIX, &m[0][0] );// get the matrix
  glPopMatrix();

  float vectStart[4];
  float vectResult[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  vectStart[0] = ant.x;
  vectStart[1] = ant.y;
  vectStart[2] = ant.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 a = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = bnt.x;
  vectStart[1] = bnt.y;
  vectStart[2] = bnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 b = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = cnt.x;
  vectStart[1] = cnt.y;
  vectStart[2] = cnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 c = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = dnt.x;
  vectStart[1] = dnt.y;
  vectStart[2] = dnt.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 d = Vector3( vectResult[0], vectResult[1], vectResult[2] );




  vectStart[0] = ant2.x;
  vectStart[1] = ant2.y;
  vectStart[2] = ant2.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 a2 = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = bnt2.x;
  vectStart[1] = bnt2.y;
  vectStart[2] = bnt2.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 b2 = Vector3( vectResult[0], vectResult[1], vectResult[2] );

  vectStart[0] = cnt2.x;
  vectStart[1] = cnt2.y;
  vectStart[2] = cnt2.z;
  vectStart[3] = 1.0f;
  for( int j =0; j<4; j++ ) {
    float val = 0;
    for( int i = 0; i<4; i++) {
      val+= m[i][j]  * vectStart[i];
    }
    vectResult[j] = val;
  }
  Vector3 c2 = Vector3( vectResult[0], vectResult[1], vectResult[2] );


  // works without rotating ;)
  //Vector3 actPos( m[3][0], m[3][1], m[3][2] );// position of the zero vector
  //Vector3 a = ant + actPos;
  //Vector3 b = bnt + actPos;
  //Vector3 c = cnt + actPos;
  //Vector3 d = dnt + actPos;

  bool in = true;
  float distance;

  Plane plinks;
  plinks.setPlane(d-c, c);
  distance = plinks.distance( position );
  if( distance < 0 )
    in = false;

  Plane prechts;
  plinks.setPlane(a-b, b);
  distance = plinks.distance( position );
  if( distance < 0 )
    in = false;

  Plane poben;
  plinks.setPlane(c-a, a);
  distance = plinks.distance( position );
  if( distance < 0 )
    in = false;

  Plane punten;
  plinks.setPlane(b-d, d);
  distance = plinks.distance( position );
  if( distance < 0 )
    in = false;

  // check now if the player is inside the object
  Plane pfront;
  pfront.setPlane( a, b, c );
  distance = pfront.distance( position);
  if( distance < 0 )
    in = false;

  Plane pback;
  pfront.setPlane( a2, c2, b2 );
  distance = pfront.distance( position);
  if( distance < 0 )
    in = false;

  return in;

}
