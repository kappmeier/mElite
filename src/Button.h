/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Button.h
 * \brief Declaration of the Button class, which represents a clickable button in 2d menus.
 */
#pragma once

#include <string>
#include "MouseHandler.h"
#include "TextureFont.h"
#include "ClickableObject.h"

//! An OpenGL button in a 2d environment.
class Button : public ClickableObject
{
public:
  Button(int innerBorder, int outerBorder, MouseHandler * mouse, std::string text, TextureFont * tf, unsigned int minLength,
    unsigned int centerx=0, unsigned int centery=0, int windowWidth=1, int windowHeight=1);
  void draw();
  bool isMouseOverObject();
  void mouseClick();
  void setCenter(unsigned int centerx, unsigned int centery);
  void setWindowDimensions(int wwidth, int wheight);
  size_t getButtonWidth();
  size_t getButtonHeight();
  unsigned int getButtonInnerWidth();
  unsigned int getButtonInnerHeight();
  bool isClicked();
  void noClick();
  void activateButton();
  void deactivateButton();
  void switchOn();
  void switchOff();
  bool isActive();
  bool isOn();
public:
  ~Button(void);
private:
  int centerx, centery, windowWidth, windowHeight, messageHeight;
  size_t messageWidth;
  MouseHandler * mouse;
  std::string text;
  TextureFont * tf;
  GLint outerBorder, innerBorder;

  GLfloat mouseOverColor[4];
  GLfloat normalColor[4];
  GLfloat inactiveColor[4];

  GLfloat normalFrameColor[4];
  GLfloat clickedFrameColor[4];
  GLfloat inactiveFrameColor[4];


  /*bool mouseOver; */
  bool clicked;
  bool on; /* if button is not on, it cannot be clicked and is not drawn  */
  bool active; /* if button is not active, it cannot be blicked  */
};
