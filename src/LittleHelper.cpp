/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	LittleHelper.cpp
 * \brief Some tool functions.
 */
#include "LittleHelper.h"

using namespace std;

//! Compares two strings case insensitive.
/**
 * If string one is smaller
 * than the second, -1 is returned, if it's greater 1. If they are perfectly
 * equal, that means, they have the same size, 0 is returned. If they are the 
 * same until the first string ends, the difference in size is returned.
 * \param s1 one string
 * \param s2 another string 
 */
size_t LittleHelper::stringCmpi( const string& s1, const string& s2 ) {
  string::const_iterator p1 = s1.begin(), p2 = s2.begin();
  while(p1 != s1.end() && p2 != s2.end()) {
    if(toupper(*p1) != toupper(*p2))
      return (toupper(*p1)<toupper(*p2))? -1 : 1;
    p1++;
    p2++;
  }
  return(s2.size() - s1.size());
}
