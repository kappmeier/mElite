/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	QuadricObject.cpp
 * \brief Implementation of the QuadricObject class, that can draw objects based on OpenGL quadrics.
 */
#include "QuadricObject.h"

QuadricObject::QuadricObject( void ) { }

QuadricObject::QuadricObject( GLuint textureName ) : GraphicsObject( ) {
  m_texture = textureName;
}

QuadricObject::QuadricObject( float r, GLuint textureName ) : GraphicsObject( r ) {
  m_texture = textureName;
}

QuadricObject::QuadricObject( Vector3 position, float r, GLuint textureName ) : GraphicsObject( position, r ) {
  m_texture = textureName;
}

QuadricObject::QuadricObject( float x, float y, float z, float r, GLuint textureName ) : GraphicsObject( x, y, z, r ) {
  m_texture = textureName;
}


QuadricObject::~QuadricObject( void ) {
}

Point4 QuadricObject::getColor( void ) {
  return m_color;
}

GLuint QuadricObject::getTexture( void ) {
  return m_texture;
}

/**
 * \brief Sets the color used for drawing.
 * \param color a vector with the rgb color-information 
 */
GLvoid QuadricObject::setColor( Vector3 color ) {
  m_color.x = color.x;
  m_color.y = color.y;
  m_color.z = color.z;
}

/**
 * \brief Sets the color used for drawing.
 * \param r the red component
 * \param g the green component
 * \param b the blue component
 */
GLvoid QuadricObject::setColor( float r, float g, float b ) {
  m_color.x = r;
  m_color.y = b;
  m_color.z = b;
}

/**
 * \brief Sets the color used for drawing, allows setting of alpha channel.
 * \param r the red component
 * \param g the green component
 * \param b the blue component
 * \param a the alpha channel
 */
GLvoid QuadricObject::setColor( float r, float g, float b, float a ) {
  m_color.x = r;
  m_color.y = b;
  m_color.z = b;
  m_color.a = a;
}

/**
 * \brief Sets the texture used for drawing, if texturing is on.
 */
GLvoid QuadricObject::setTexture( GLuint texture ) {
  m_texture = texture;
}
