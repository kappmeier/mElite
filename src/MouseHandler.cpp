/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	MouseHandler.cpp
 * \brief Implementation of a mouse handler.
 */
#include "MouseHandler.h"

MouseHandler::MouseHandler(void) {
  m_mouseHovering = false;
}

MouseHandler::~MouseHandler(void) {
}

bool MouseHandler::isMouseHoveringActive() {
  return m_mouseHovering;
}

void MouseHandler::setMouseHovering( bool status ) {
  m_mouseHovering = status;
}

//! Detects if the mouse is over an rectangular area of the screen.
/**
 * If mouse hovering is set to false, then false is returned. This means, that the mouse is never
 * above an object. Thus the function can be used, even if mouse hovering shouldn't be active.
 *
 * Mouse coorinates begin in (0,0) at the top of the screen. Our coordinates begin at the bottom
 * of the screen. Thus y-coordinates have to be turned over.
 */
bool MouseHandler::isMouseOverRect( unsigned int left, unsigned int right, unsigned int top, unsigned int bottom ) {
  if( m_mouseHovering )
    if( m_position.x > left && m_position.x < right && m_position.y > top && m_position.y < bottom )
      return true;
  return false; // other cases (not over rect and mouse hovering disabled!
}


//! Detects if the mouse is over an rectangular area of the screen.
/**
 * If mouse hovering is set to false, then false is returned. This means, that the mouse is never
 * above an object. Thus the function can be used, even if mouse hovering shouldn't be active.
 *
 * Mouse coorinates begin in (0,0) at the top of the screen. Our coordinates begin at the bottom
 * of the screen. Thus y-coordinates have to be turned over.
 */
bool MouseHandler::isMouseOverRectCenter( unsigned int centerx, unsigned int centery, unsigned int width, unsigned int height ) {
  return isMouseOverRect( centerx - width*0.5, centerx + width*0.5, centery - height*0.5, centery + height*0.5 );
}

void MouseHandler::setMousePosition( Position position ) {
  m_position = position;
}

void MouseHandler::setMousePosition( int x, int y ) {
  m_position.x = x;
  m_position.y = y;
}

Position MouseHandler::getMousePosition() {
  return m_position;
}

void MouseHandler::setHittedObject( HitableObjects hitObject ) {
  m_hitted = hitObject;
}

void MouseHandler::setHittedObjectInfo( int hitInfo ) {
  m_hittedInfo = hitInfo;
}

HitableObjects MouseHandler::getHittedObject() {
  return m_hitted;
}

int MouseHandler::getHittedObjectInfo() {
  return m_hittedInfo;
}

// todo:
// for better mouse interaction use this positions and some more --> hitObjectMouse down and so on
// but it also works without them, but no moving is possible and the button-up-position is _only_
// used for klicking, not, as used in windows, the button-down and button-up-positon!!!
void MouseHandler::setMouseMoveStartPosition( Position startPosition ) {

}

void MouseHandler::setMouseMoveStartPosition( int x, int y ) {

}

void MouseHandler::setMouseMoveEndPosition( Position endPosition ) {

}

void MouseHandler::setMouseMoveEndPosition( int x, int y ) {

}

Position MouseHandler::getMouseMoveStartPosition( void ) {
  return this->m_moveStartPosition;
}

Position MouseHandler::getMouseMoveEndPosition( void ) {
  return this->m_moveEndPosition;
}
