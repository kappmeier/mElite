/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Console.cpp
 * \brief Implementation of the simple Console.
 */
#include "Console.h"

using namespace std;

//! Retrieve a command from a command line input.
/**
 * \fn std::string Console::getCommand( std::string commandString )
 * Static function that returns the first word of an command string. If the
 * command string only contains one word, this is returned. Only space works as separator for words.
 * \param commandString A string that should be parsed
 * \return The command containing the commandString
 */
string Console::getCommand( string commandString ) {
  size_t space = commandString.find( ' ' );
  if( space == string::npos )
    return commandString;
  else
    return commandString.substr( 0, space );
}

//! Retrieve a single character option from a command line input.
/**
 * \fn char Console::getSingleCharacterOption( std::string commandString ) {
 * Static function that gets one single character option of any command. It
 * simply searchges the first space and returns the next character. If we only
 * get pointers to character arrays which last item is zero, this will work. If
 * the command string contains more characters they are ignored.
 * \param commandString The string whose options are searched
 * \return The option character.
 */
char Console::getSingleCharacterOption( string commandString ) {
  size_t space = commandString.find( ' ' );
  if( commandString.length() > space+1 && space != string::npos )
    return commandString[++space];
  return 0;
}

//! Retrieve an option from a command line input.
/**
 * \fn std::string Console::getOption( std::string commandString )
 * Returns the first option of an command. This option begins after the first
 * space. The option must not contain any space characters.
 * \param commandString The string whose options should be extracted.
 * \return The first option in the string
 */
string Console::getOption( string commandString ) {
  size_t firstSpace = commandString.find( ' ' );
  if( firstSpace == string::npos )// if no space return nothing
    return "";

  size_t secondSpace = commandString.find( ' ', ++firstSpace );
  if( secondSpace == string::npos)
    return commandString.substr( firstSpace );
  else
    return commandString.substr( firstSpace, secondSpace - firstSpace );
}