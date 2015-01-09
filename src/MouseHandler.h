/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	MouseHandler.h
 * \brief Declaration of the MouseHandler class, that allows interaction with the mouse.
 */
#pragma once

#include "World.h"

//! A list of supported objects that can be hit by the mouse.
enum HitableObjects {
  none,
  planet,
  button
};

//! Allows to check, if a mouse click in a specific position has hit an object.
class MouseHandler {
public:
  MouseHandler(void);
  ~MouseHandler(void);
  bool isMouseHoveringActive();
  void setMouseHovering( bool status );
  bool isMouseOverRect( unsigned int left, unsigned int right, unsigned int top, unsigned int bottom );
  bool isMouseOverRectCenter( unsigned int centerx, unsigned int centery, unsigned int width, unsigned int height );
  void setMousePosition( Position position );
  void setMousePosition( int x, int y );
  Position getMousePosition();
  void setHittedObject( HitableObjects object );
  void setHittedObjectInfo( int value );
  HitableObjects getHittedObject( void );
  int getHittedObjectInfo( void );
  void setMouseMoveStartPosition( Position startPosition );
  void setMouseMoveStartPosition( int x, int y );
  void setMouseMoveEndPosition( Position endPosition );
  void setMouseMoveEndPosition( int x, int y );
  Position getMouseMoveStartPosition( void );
  Position getMouseMoveEndPosition( void );
private: 
  bool m_mouseHovering;
  Position m_position;
  Position m_moveStartPosition;
  Position m_moveEndPosition;
  HitableObjects m_hitted;
  int m_hittedInfo;
};
