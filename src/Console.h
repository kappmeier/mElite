/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Console.h
 * \brief Declaration of the simple Console class for text input.
 */
#pragma once

#include "GlobalGL.h"
#include <string>

//! Handles command parsing for a console and draws the console using OpenGL
/**
 * Provides some static functions to parse lines that are sent to the program as command lines.
 * It contains a container that stores the last commands and can search through them. Also it can be tested
 * if a command starts with a given string to implement auto filling for text input.
 */
class Console {
public:
  static std::string getCommand( std::string commandString );
  static char getSingleCharacterOption( std::string commandString );
  static std::string Console::getOption( std::string commandString );
private:
};