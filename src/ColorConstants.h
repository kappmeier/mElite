/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ColorConstants.h
 * \brief	Defines some constants for colors used in game.
 */

#pragma once

//#include <windows.h> // Windows header. Needs to be included before glut, otherwise we get errors due to wrong definitions
#include <GL/glut.h>

static const GLfloat colorWhite[]       = { 1.00f, 1.00f, 1.00f, 1.0f };
static const GLfloat colorYellow[]      = { 1.00f, 1.00f, 0.00f, 1.0f };
static const GLfloat colorDarkYellow[]  = { 1.00f, 210.0/255.0f, 0.0f, 1.0f };
static const GLfloat colorOrange[]      = {242.0/255.0f, 166.0/255.0f, 0.0f, 1.0f };
static const GLfloat colorRed[]         = { 1.00f, 0.00f, 0.00f, 1.0f };
static const GLfloat colorDarkRed[]     = { 0.60f, 0.00f, 0.00f, 1.0f };
static const GLfloat colorPurple[]      = { 1.00f, 0.00f, 1.00f, 1.0f };
static const GLfloat colorLightBlue[]   = { 0.00f, 97.0/255.0f,1.0f, 1.0f };
static const GLfloat colorBlue[]        = { 0.00f, 0.00f, 1.00f, 1.0f };
static const GLfloat colorDarkblue[]    = { 0.00f, 0.00f, 0.33f, 1.0f };
static const GLfloat colorGreen[]       = { 0.00f, 1.00f, 0.00f, 1.0f };
static const GLfloat colorDarkGreen[]   = { 0.00f, 0.25f, 0.00f, 1.0f };
static const GLfloat colorDarkGreen2[]  = { 0.00f, 0.50f, 0.00f, 1.0f };
static const GLfloat colorDarkGrey[]    = { 0.33f, 0.33f, 0.33f, 1.0f };
static const GLfloat colorGrey[]        = { 0.50f, 0.50f, 0.50f, 1.0f };
static const GLfloat colorLightGrey[]   = { 0.66f, 0.66f, 0.66f, 1.0f };
static const GLfloat colorBlack[]       = { 0.00f, 0.00f, 0.00f, 1.0f };