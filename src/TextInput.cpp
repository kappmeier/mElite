/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	TextInput.cpp
 * \brief Implementation of the TextInput class that handles text input to the Console.
 */
#include "TextInput.h"

/**
 * \brief The constructor initializes the character array and sets the font object
 * for printing the text to the screen.
 */
TextInput::TextInput( TextureFont *tf ) {
  m_tf = tf;
  text[0] = 0;
  letterCount = 0;
}

/**
 * \brief Destructor, does nothing.
 */
TextInput::~TextInput(void) {
}

/**
 * \brief Deletes one character of the character array until it's empty. 
 */
GLvoid TextInput::back( GLvoid ) {
  if( letterCount > 0 )
    text[letterCount--] = 0;
}


/**
 * \brief Returns a pointer to the textarray. 
 */
char * TextInput::getText( GLvoid ) {
  return text;
}

//! Add a new letter. 
/**
 * The maximum is set to 255 due to the last character has to be \0.
 * \param letter the new letter that should be inserted
 */
GLvoid TextInput::newLetter( char letter ) {
  if( letterCount == 254 )
    return;
  text[letterCount++] = letter;
  text[letterCount] = 0;
}

//! Prints the text in the character array.
/**
 * Therefore a TextInput object is used which is submitted to the constructor at creation time. 
 */
GLvoid TextInput::print( GLvoid ) {
  // show blinking cursor
  if( glutGet( GLUT_ELAPSED_TIME )/500 % 2 == 0)
    m_tf->print( 50, 50, text, 800, 600 );
  else {
    text[letterCount] = '_';
    text[letterCount + 1] = 0;
    m_tf->print( 50, 50, text, 800, 600 );
    text[letterCount] = 0;
  }
}

/**
 * \brief resets the text to an empty string.
 */
GLvoid TextInput::reset( GLvoid ) {
  text[0] = 0;
  letterCount = 0;
}