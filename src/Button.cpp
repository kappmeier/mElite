/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Button.cpp
 * \brief Contains implementation for a simple button in a 2d menu.
 */
#include "Button.h"
#include "Shapes.h"
#include "ColorConstants.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

//! Creates a new button
/**
 * \param innerBorder the inner border around the text
 * \param outerBorder the outer border around the text
 * \param mouse the MouseHandler that receives mouse input
 * \param text the text displayed on the button
 * \param tf the TextureFont used for the text
 * \param minLength the minimal width of the button (even if the text is shorter than the minimum length
 * \param centerx the x-coordinate of the center of the button
 * \param centery the y-coordinate of the center of the button
 * \param windowWidth the width of the containing window
 * \param windowHeight the height of the containing window
 */
Button::Button(int innerBorder, int outerBorder, MouseHandler * mouse, std::string text, TextureFont * tf, unsigned int minLength,
               unsigned int centerx, unsigned int centery, int windowWidth, int windowHeight)
               : ClickableObject()
{	
  this->centerx = centerx;
  this->centery = centery;
  this->mouse = mouse;
  this->text = text;
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->outerBorder = outerBorder;
  this->innerBorder = innerBorder;

  memcpy(mouseOverColor,colorYellow,4*sizeof(GLfloat));
  memcpy(normalColor,colorGreen,4*sizeof(GLfloat));
  memcpy(inactiveColor,colorWhite,4*sizeof(GLfloat));

  memcpy(normalFrameColor,colorLightGrey,4*sizeof(GLfloat));
  memcpy(clickedFrameColor,colorBlue,4*sizeof(GLfloat));
  memcpy(inactiveFrameColor,colorDarkblue,4*sizeof(GLfloat));

  this->tf = tf;
  /*mouseOver = false; */
  clicked = false;
  messageHeight = tf->getSize();
  messageWidth = ( max(text.length(),minLength)+1)*(tf->getWidth());

  switchOn();
  activateButton();

}


Button::~Button(void)
{
}

void Button::setCenter(unsigned int centerx, unsigned int  centery)
{
  this->centerx = centerx;
  this->centery = centery;
}

void Button::setWindowDimensions(int wwidth, int wheight)
{
  this->windowWidth = wwidth;
  this->windowHeight = wheight;
}

bool Button::isMouseOverObject()
{
  if (!on) return false;
  bool oldStatus = mouse->isMouseHoveringActive();
  mouse->setMouseHovering( true );
  return mouse->isMouseOverRectCenter( centerx, windowHeight-centery, messageWidth, messageHeight);
  mouse->setMouseHovering( oldStatus );
  /*mouseOver= */ //return mouseOver;
}

void Button::mouseClick()
{
  if (!on || !active) return;
  if (isMouseOverObject())
    clicked = !clicked;
}

size_t Button::getButtonWidth()
{
  return 2*innerBorder+2*outerBorder+messageWidth;
}

size_t Button::getButtonHeight()
{
  return 2*innerBorder+2*outerBorder+messageHeight;
}

unsigned int Button::getButtonInnerWidth()
{
  return messageWidth;
}

unsigned int Button::getButtonInnerHeight()
{
  return messageHeight;
}

void Button::draw()
{ 	
  if (!on) return;

  bool mouseOver = isMouseOverObject();

  glPushAttrib( GL_ALL_ATTRIB_BITS );

  glDisable( GL_TEXTURE_2D);
  glDisable( GL_LIGHTING );
  glDisable( GL_BLEND );

  glColor3fv( colorDarkGrey );
  Shapes::drawRect( centerx, centery, 0, messageWidth+2*innerBorder+2*outerBorder, messageHeight+2*innerBorder+2*outerBorder );

  glColor3fv( normalFrameColor);

  if (clicked)
    glColor3fv( clickedFrameColor);

  if (!active)
    glColor3fv( inactiveFrameColor);

  Shapes::drawRect( centerx, centery, 0, messageWidth+2*innerBorder, messageHeight+2*innerBorder);

  glColor3fv( colorBlack );
  Shapes::drawRect( centerx, centery, 0, messageWidth, messageHeight);

  glColor3fv(normalColor);

  if (mouseOver) 
    glColor3fv(mouseOverColor);
  if (!active)
    glColor3fv(inactiveColor);


  const char * chartext = text.c_str(); 
  glEnable( GL_BLEND );
  glEnable( GL_TEXTURE_2D );

  tf->print(centerx-messageWidth/2, centery-messageHeight/2, chartext);

  glPopAttrib();

}

bool Button::isClicked()
{
  return (on && active && clicked);
}

void Button::noClick()
{
  clicked = false;
}

void Button::activateButton()
{
  active = true;
}

void Button::deactivateButton()
{
  active = false;
  clicked = false;
}

void Button::switchOn()
{
  on = true;
}

void Button::switchOff()
{
  on = false;
}

bool Button::isOn()
{
  return on;
}

bool Button::isActive()
{
  return active;
}