/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Planet.cpp
 * \brief Implementation of a Planet.
 */
#include "Planet.h"
#include "Shapes.h"
#include "Textures.h"
#include "ColorConstants.h"

//! Draws a planet with texture and spinning. 
/**
 * The spinning is done by a
 * SpinningObject, the texture is set through Drawable. The Planet itself is 
 * basically an quadric sphere. In order to run in the xz-area, the planet is
 * rotated about 90 degrees.
 */
Planet::Planet( float planetRadius, float crashRadius, GLuint textureName )
: SpinningObject( ),
QuadricObject( textureName ),
GameObject( ),
GraphicsObject( crashRadius ) {
  m_planetRadius = planetRadius;
  m_rotateAngle = 0;
  m_rotateSpeed = 0;
  m_drawRadiusLine = false;
}

//! Constructor wich sets the position and radius of the planet. 
/**
 * A planet has 
 * two different radiusses, the outer one gives the distance in which an
 * vessel will crash onto the planet. In this version, both radiuses are the 
 * same.
 */
Planet::Planet( float x, float y, float z, float bothRadiuses, GLuint textureName )
: SpinningObject( x, y, z ),
QuadricObject( textureName ),
GameObject( x, y, z ),
GraphicsObject( bothRadiuses ) {
  m_planetRadius = bothRadiuses;
  m_rotateAngle = 0;
  m_drawRadiusLine = false;
}

//! Constructor wich sets the position and radius of the planet. 
/**
 * A planet has 
 * two different radiusses, the outer one gives the distance in which an
 * vessel will crash onto the planet.
 * rotated about 90 degrees.
 */
Planet::Planet( float x, float y, float z, float planetRadius, float crashRadius, GLuint textureName )
: SpinningObject( x, y, z ),
QuadricObject( textureName ),
GameObject( x, y, z ),
GraphicsObject( crashRadius ) {
  m_planetRadius = planetRadius;
}

/**
 * \brief Destructor 
 */
Planet::~Planet(void) {
}

//! Draws a planet with texture and spinning. 
/**
 * The spinning is done by a
 * SpinningObject, the texture is set through Drawable. The Planet itself is 
 * basically an quadric sphere. In order to run in the xz-area, the planet is
 * rotated about 90 degrees. If the frustum is set to NULL, the planet is
 * always drawn. If frustum is an actual object it is tested if the planet can
 * be culled.
 */
GLvoid Planet::draw() {
  Textures::getTextures()->bindTexture( getTexture() );
  glColor4f( getColor().x, getColor().y, getColor().z, getColor().a );

  glPushMatrix();
  glRotatef( m_rotateAngle, 0.0, 1.0, 0.0 ); // rotation about a certain angle --> startposition 
  glRotatef( (glutGet(GLUT_ELAPSED_TIME)*m_rotateSpeed  * 0.001), 0.0, 1.0, 0.0 ); // jahresdrehung
  //Vector3 pos1 = Vector3( getPos().x, getPos().y, getPos().z );
  Vector3 pos2 = Vector3( SpinningObject::getPos().x, SpinningObject::getPos().y, SpinningObject::getPos().z );
  Vector3 pos = pos2;
  glTranslatef( pos.x, pos.y, pos.z );// translate to supposed position
  this->spin();// spin the planet
  glRotatef( -90, 1.0, 0.0, 0.0 );// rotate because the planet should run xz-space

  // this is not perfect, object is created everytime it's drawn
  GLUquadricObj *quadObj = gluNewQuadric();
  gluQuadricDrawStyle( quadObj, GLU_FILL );
  gluQuadricTexture( quadObj, GL_TRUE );
  gluSphere( quadObj, m_planetRadius, 32, 32 );
  gluDeleteQuadric( quadObj );
  glPopMatrix();

  // try to draw radius line
  if( m_drawRadiusLine ) {
    glPushMatrix();
    glRotatef( -90, 1.0, 0.0, 0.0 );
    float radius = sqrt(SpinningObject::getPos().x*SpinningObject::getPos().x +SpinningObject::getPos().y*SpinningObject::getPos().y +SpinningObject::getPos().z*SpinningObject::getPos().z);
    Shapes::drawCircleShape(0, 0, 0, 300, radius, colorWhite );
    glPopMatrix();
  }
}

//! Get the current position of the center of the planet.
/**
 * \return The position after all the rotations have been done. 
 */
Vector3 Planet::getPos() {
  GLfloat m[4][4];// define memory space for a single matrix
  glPushMatrix();
  glLoadIdentity();
  glRotatef( m_rotateAngle, 0.0, 1.0, 0.0 ); // rotation about a certain angle --> startposition 
  glRotatef( (glutGet(GLUT_ELAPSED_TIME)*m_rotateSpeed  * 0.001), 0.0, 1.0, 0.0 ); // jahresdrehung
  //Vector3 pos1 = Vector3( getPos().x, getPos().y, getPos().z );
  Vector3 pos2 = Vector3( SpinningObject::getPos().x, SpinningObject::getPos().y, SpinningObject::getPos().z );
  Vector3 pos = pos2;
  glTranslatef( pos.x, pos.y, pos.z );// translate to supposed position
  this->spin();// spin the planet
  glGetFloatv( GL_MODELVIEW_MATRIX, &m[0][0] );// get the matrix
  glPopMatrix();
  // the vector is (0,0,0,1) and therefore we can directly access the needed values!
  // the components in the 4th column are used as the 4th component of the vector contains the only non-zero-value
  Vector3 actPos( m[3][0], m[3][1], m[3][2] );

  return actPos;
}


/**
 * \brief Get the current rotate speed.
 */
float Planet::getRotateSpeed( void ) {
  return m_rotateSpeed;
}

//! Sets the rotate angle.
/**
 * This is the angle by which the planets are rotated 
 * when they are first drawn. 
 * \param angle the angle
 */
void Planet::setRotateAngle( float angle ) {
  m_rotateAngle = angle;
}

/**
 * \brief Sets the speed that the planets use to rotate.
 * \param speed the speed
 */
void Planet::setRotateSpeed( float speed ) {
  m_rotateSpeed = speed;
}

/**
 * \brief Enables and disables if the line on which the planet moves should be drawn.
 * \param draw defines whether it's true or not
 */
void Planet::setRadiusLineDrawing( bool draw ) {
  m_drawRadiusLine = draw;
}