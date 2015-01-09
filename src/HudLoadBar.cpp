/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	HudLoadBar.cpp
 * \brief Implementation of a simple bar used to display the heat and speed.
 */
#include "GlobalParameters.h"
#include "HudLoadBar.h"
#include "Textures.h"
#include "ColorConstants.h"
#include "Structures.h"

HudLoadBar::HudLoadBar( void ) {
  m_current = 5.0f;
  m_max = 10.0f;
  m_min = 0.0f;
  m_slope = 0;
  update();
}

HudLoadBar::HudLoadBar( float min, float max, float current ) {
  if( min < max ) {
    m_min = min;
    m_max = max;
    setCurrent( current );
  } else {
    m_current = 5.0f;
    m_max = 10.0f;
    m_min = 0.0f;
  }
  m_slope = 0;
  update();
}

HudLoadBar::~HudLoadBar( void ) {
}

void HudLoadBar::draw() {
  draw( 0, 0, 50, 10 );
}

void HudLoadBar::draw( int x, int y, int width, int height ) {
  //glDisable( GL_BLEND );
  glColor3fv( colorWhite );
  glDisable( GL_LIGHTING );
  Textures::getTextures()->bindTexture( bar );
  // calculate percentage of current
  float percent = (m_current-currentDifference) / currentRange;

  glBegin( GL_QUADS );
  glTexCoord2f( 0, 1 );
  glVertex3f( m_slope + x + offset*width, y+height, 0);
  glTexCoord2f( 0, 0 );
  glVertex3f( x + offset*width, y, 0);
  glTexCoord2f( isNegative*percent, 0 );
  glVertex3f( x+ offset*width +currentPercentage * width*percent, y, 0 );
  glTexCoord2f( isNegative*percent, 1 );
  glVertex3f( m_slope + x+ offset*width +currentPercentage * width*percent, y + height, 0 );
  glEnd();
  glEnable( GL_BLEND );
}

void HudLoadBar::setCurrent( float current ) {
  if( current > m_max)
    m_current = m_max;
  else if( current < m_min )
    m_current = m_min;
  else
    m_current = current;
  update();
}

void HudLoadBar::setMax( float max ) {
  if( max > m_min )
    m_max = max;
  if( m_max < m_current )
    m_current = max;
  update();
}

void HudLoadBar::setMin( float min ) {
  if( min < m_max )
    m_min = min;
  if( m_min > m_current )
    m_current = m_min;
  update();
}

void HudLoadBar::setSlopePixel( int pixel ) {
  m_slope = pixel;
}

void HudLoadBar::update() {
  // thecases both values greater than 0 and both values smaller than 0 are not tested!
  float positivePercentage;
  float negativePercentage;
  if( m_min >= 0) {
    positivePercentage = 1;
    currentRange = m_max - m_min;
    currentDifference = m_min;
  } else if( m_max <= 0 ) {
    negativePercentage = 1;
    currentRange = m_max - m_min;
    currentDifference = m_max;
  } else {
    currentDifference = 0;
    positivePercentage = m_max / (m_max - m_min);
    currentRange = m_current < 0 ? -m_min : m_max;
  }
  negativePercentage = 1 - positivePercentage;

  if( m_current < 0)
    currentPercentage = negativePercentage;
  else
    currentPercentage = positivePercentage;

  offset = negativePercentage;
  isNegative = m_current < 0 ? -1.0 : 1.0;
}