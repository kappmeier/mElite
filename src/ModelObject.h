/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ModelObject.h
 * \brief Contains the definition of a general model object, which is a graphical representation consisting of
 *        a triangle mesh net.
 */
#pragma once
#include "GlobalGL.h"
#include "GraphicsObject.h"
#include "Vector3.h"

//! A real drawing class that can render triangle mesh net based objects. 
/**
 * It's derived from the abstract 
 * GraphicsObject and implements the draw() method. A display list has to be 
 * submitted to this class and this display list is executed each time the 
 * draw method is called. Therefore no color, texture and other information is
 * needed.
 */
class ModelObject : public GraphicsObject {
  // constructors, a display list has to be submitted to the constructor
public:
  ModelObject( GLuint displayList );
  ModelObject( float r, GLuint displayList );
  ModelObject( Vector3 position, float r, GLuint displayList);
  ModelObject( float x, float y, float z, float r, GLuint displayList);
  ~ModelObject( void );
private:
  ModelObject( void );
  ModelObject( float r );
  ModelObject( Vector3 position, float r );
  ModelObject( float x, float y, float z, float r );

  // drawing
public:
  virtual void draw();
private:
  GLuint m_list;										// Display list
};

