/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ModelObject.cpp
 * \brief Contains the implementation of a general model object. Such an object is displayed using a
 *        3d model based on triangles.
 */
#include "ModelObject.h"

/**
 * \brief Creates a model object.
 */
ModelObject::ModelObject( void ) : GraphicsObject() {
  m_list = 0;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param displayList The display list number returned by OpenGL
 */
ModelObject::ModelObject( GLuint displayList ) : GraphicsObject() {
  m_list = displayList;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param r The object radius
 */
ModelObject::ModelObject( float r ) : GraphicsObject( r ) {
  m_list = 0;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param r The object radius
 * \param displayList The display list number returned by OpenGL 
 */
ModelObject::ModelObject( float r, GLuint displayList ) : GraphicsObject( r ) {
  m_list = displayList;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param position The position as Vector3
 * \param r The object radius
 */
ModelObject::ModelObject( Vector3 position, float r ) : GraphicsObject( position, r ) {
  m_list = 0;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param position The position as Vector3
 * \param r The object radius
 * \param displayList The display list number returned by OpenGL 
 */
ModelObject::ModelObject( Vector3 position, float r, GLuint displayList) : GraphicsObject( position, r ) {
  m_list = displayList;
}

//! \brief Creates a model object.
/**
 * The display list is not provided so the constructor is private.
 * \param x The x-position
 * \param y The y-position
 * \param z The z-position
 * \param r The object radius
 */
ModelObject::ModelObject( float x, float y, float z, float r ) : GraphicsObject( x, y, z, r ) {
  m_list = 0;
}

/**
 * \brief Creates a model object and sets up the display List.
 * \param x The x-position
 * \param y The y-position
 * \param z The z-position
 * \param r The object radius
 * \param displayList The display list number returned by OpenGL 
 */
ModelObject::ModelObject( float x, float y, float z, float r, GLuint displayList) : GraphicsObject( x, y, z, r ) {
  m_list = displayList;
}

/**
 * \brief The destructor has nothing to clean up. 
 */
ModelObject::~ModelObject( void ) {
}

//! Draws the object.
/**
 * Draws the object using the display list. Therefore no textur and color information is needed. All
 * actions that are necessary to draw the object should take place in the list. It is possible to
 * create a derived class that does something else and calls this draw() method to draw the model.
 * \sa SpaceStation::draw() where this function is combined with a spinning object.
 */
void ModelObject::draw() {
  glCallList( m_list );
}