/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextInput.h
 * \brief Declares the TextInput class allowing keyboard interaction.
 */
#pragma once

#include "GlobalGL.h"
#include "TextureFont.h"
#include <string>

//! Allows keyboard interacton, receives key presses and can print the current text to the screen using OpenGL TextureFont.
class TextInput {
public:
  TextInput( TextureFont *tf );
  ~TextInput(void);
  GLvoid back( GLvoid );
  char * getText( GLvoid );
  GLvoid newLetter( char letter );
  GLvoid print( GLvoid );
  GLvoid reset( GLvoid );
private:
  TextureFont *m_tf;
  char text[256];
  int letterCount;
};