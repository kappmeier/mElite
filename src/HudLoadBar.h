/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	HudLoadBar.h
 * \brief Declaration of a simple bar showing information on the screen.
 */
#pragma once

//! A bar that can show a current value in a given range.
/**
 * The bar is displayed in 2d on top of the image and supports transparency.
 * The bar is colored from green to red, showing how near the value is to the maximum or
 * minimum. 0 is green, min and max are red, if they are different from 0.
 */
class HudLoadBar {
public:
  HudLoadBar( void );
  HudLoadBar( float min, float max, float current );
  ~HudLoadBar( void );

  void draw();
  void draw( int x, int y, int width, int height );
  void setCurrent( float current );
  void setMax( float max );
  void setMin( float min );
  void setSlopePixel( int pixel );
private:
  void update();
  float m_max;
  float m_min;
  float m_current;
  int m_slope;

  // help variables
  float offset;
  float isNegative;
  float currentRange;
  float currentPercentage;
  float currentDifference;
};
