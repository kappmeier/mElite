/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	FrameCounter.cpp
 * \brief Implementation of a simple frame counter
 */
#include "GlobalGL.h"
#include "FrameCounter.h"

//! Initializes the variables.
/**
 * The first start-time is set.
 */
FrameCounter::FrameCounter( void ) {
  frames = 0;
  //start = glutGet( GLUT_ELAPSED_TIME );
  fps = 0;
  m_frameStart = start = getTime();
  m_lastFrameTime = 0;
}

/**
 * \brief Destructor.
 */
FrameCounter::~FrameCounter( void ) {

}

//! brief Returns the last calculated framerate.
/**
 * The framerate is actualized every second.
 */
int FrameCounter::getFrameRateAverage( void ) {
  return fps;
}

long FrameCounter::getFrameStart( void ) {
  return m_frameStart;
}

//! Returns the time needed for drawing one frame.
/**
 * The time is an average value calculated by the last frames per seconds value. 
 */
float FrameCounter::getFrameTimeAverage( void ) {
  return 1000.0/getFrameRateAverage();
}

/**
 * \brief Returns the time passed since the last frame started. 
 */
long FrameCounter::getFrameTimeCurrent( void ) {
  long now = getTime();
  long passed = getFrameStart();
  return now - passed;
}

/**
 * \brief Returns the time passed since the last frame started. 
 */
long FrameCounter::getFrameTimeLast( void ) {
  return m_lastFrameTime;
}

long FrameCounter::getTime( void ) {
  return glutGet( GLUT_ELAPSED_TIME );
}

//! Increases the counted number of frames. 
/**
 * Needs to be called everytime when a new frame is painted to the screen.
 */
void FrameCounter::newFrame( void ) {
  long now = getTime();
  m_lastFrameTime = now - m_frameStart;
  m_frameStart = now;
  frames++;
}

//! Updates the current framerate if one second has passed since the last update. 
/**
 * The elapsed time is calculated with the glutGet function which
 * counts the milliseconds since start of the game. 
 */
void FrameCounter::update( void ) {
  long now = getTime();
  long dTime = now - start;

  if( dTime > 1000 ) {
    start = now;
    fps = (long)(1e3  * (double)frames/(double) dTime);
    frames = 0;
  }
}