/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Star.cpp
 * \brief Implementation of a star.
 */
#include "Star.h"

/**
 * \brief Draws a star, which is basically only a planet with light. The default
 * lightcolor is set to yellow, as the sun's color. 
 * \param planetRadius radius of the star itself 
 * \param collisionRadius radius used for collision detection
 * \param textureName the texture that is used for the star (as OpenGL texture)
 */
Star::Star( float planetRadius, float collisionRadius, GLuint textureName )
: Planet( planetRadius, collisionRadius, textureName ),
GameObject( ),
GraphicsObject( collisionRadius ) {
  m_lightPosition[0] = 0.0;
  m_lightPosition[1] = 0.0;
  m_lightPosition[2] = 0.0;
  m_lightPosition[3] = 1.0;
  m_lightColor.x = 1.0;
  m_lightColor.y = 1.0;
  m_lightColor.z = 0.0;
  glLightfv( GL_LIGHT0, GL_POSITION, m_lightPosition )
    ;}

//! Draws a star, which is basically only a planet with light. 
/**
 * The default lightcolor is set to yellow, as the sun's color. 
 * \param x the x-position of the star
 * \param y the y-position of the star
 * \param z the z-position of the star
 * \param planetRadius radius of the star itself and crashing radius
 * \param textureName the index of the texture that is used
 */
Star::Star( float x, float y, float z, float planetRadius, GLuint textureName )
: Planet( x, y, z, planetRadius, textureName ),
GraphicsObject( planetRadius ) {
  m_lightPosition[0] = 0.0;
  m_lightPosition[1] = 0.0;
  m_lightPosition[2] = 0.0;
  m_lightPosition[3] = 1.0;
  m_lightColor.x = 1.0;
  m_lightColor.y = 1.0;
  m_lightColor.z = 0.0;
  glLightfv( GL_LIGHT0, GL_POSITION, m_lightPosition );
}

/**
 * Draws a star, which is basically only a planet with light. The default
 * lightcolor is set to yellow, as the sun's color. 
 * \param x the x-position of the star
 * \param y the y-position of the star
 * \param z the z-position of the star
 * \param planetRadius radius of the star itself 
 * \param collisionRadius radius used for collision detection
 * \param textureName the OpenGL id for the texture that is used for the star
 */
Star::Star( float x, float y, float z, float planetRadius, float collisionRadius, GLuint textureName )
: Planet( x, y, z, planetRadius, collisionRadius, textureName ),
GameObject( x, z, z ),
GraphicsObject( collisionRadius ) {
  m_lightPosition[0] = 0.0;
  m_lightPosition[1] = 0.0;
  m_lightPosition[2] = 0.0;
  m_lightPosition[3] = 1.0;
  m_lightColor.x = 1.0;
  m_lightColor.y = 1.0;
  m_lightColor.z = 0.0;
  glLightfv( GL_LIGHT0, GL_POSITION, m_lightPosition );
}

/**
 * \brief Destructor 
 */
Star::~Star(void) {
}

//! Draws a star
/**
 * This drawing function uses the base planet function to draw the sun. Before
 * this the light is color is set.
 */
GLvoid Star::draw() {
  glPushAttrib( GL_ALL_ATTRIB_BITS );
  float color[3];
  color[0] = m_lightColor.x;
  color[1] = m_lightColor.y;
  color[2] = m_lightColor.z;
  glMaterialfv( GL_FRONT, GL_EMISSION, color );
  Planet::draw();
  glPopAttrib();
}

/**
 * Sets the color of the emitting light. The used light is LIGHT0 
 * \param lightColor a vector with the light color
 */
GLvoid Star::setLightColor( Vector3 lightColor ) {
  m_lightColor = lightColor;
}

/**
 * Sets the color of the emitting light. The used light is LIGHT0 
 * \param r the red component of the light 
 * \param g the green component of the light 
 * \param b the blue component of the light
 */
GLvoid Star::setLightColor( float r, float g, float b ) {
  m_lightColor.x = r;
  m_lightColor.y = g;
  m_lightColor.z = b;
}

GLvoid Star::setPos( GLfloat x, GLfloat y, GLfloat z ) {
  Planet::setPos( x, y, z );
  m_lightPosition[0] = x;
  m_lightPosition[1] = y;
  m_lightPosition[2] = z;
  glLightfv( GL_LIGHT0, GL_POSITION, m_lightPosition );
}

GLvoid Star::setPos( Vector3 position ) {
  this->setPos( position.x, position.y, position.z );
}
