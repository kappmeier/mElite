/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	GlobalGL.h
 * \brief	Some inclusions used for a lot of OpenGL commands.
 */
#ifndef GLOBAL_H_
#define GLOBAL_H_

// basic includes for the work with OpenGL
#include <windows.h> // Windows header. Needs to be included before glut, otherwise we get errors due to wrong definitions
#include <GL/glut.h>
#include <stdio.h> // input/output
#define _USE_MATH_DEFINES // activate mathmatic constants (pi, ...)
#include <math.h>// mathmatic functions
#include <cmath> // c++ header for c-style math functions
static const float ANGLE2DEG = M_PI/180.0;
#include <stdarg.h>			// header for variable argument routines

#endif // GLOBAL_H