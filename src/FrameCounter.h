/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	FrameCounter.h
 * \brief Declaration of a frame counter.
 */
#pragma once

//! A class computing the current frame rate.
class FrameCounter {
public:
  FrameCounter( void );
  ~FrameCounter( void );
  void update( void );
  long getFrameStart( void );
  float getFrameTimeAverage( void );
  long getFrameTimeCurrent( void );
  long getFrameTimeLast( void );
  long getTime( void );
  void newFrame( void );
  int getFrameRateAverage( void );
private:
  long m_frameStart;
  int frames;
  long start;
  int fps;
  long m_lastFrameTime;
};
