/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ForeignSpaceShip.cpp
 * \brief Implementation of a general space ship.
 */
#include "ForeignSpaceShip.h"

ForeignSpaceShip::ForeignSpaceShip(float r, GLuint displayList):SpaceShip(r,displayList)
{
}

ForeignSpaceShip::~ForeignSpaceShip(void)
{
}

void ForeignSpaceShip::update( float t)
{
	adjust(t);
	SpaceShip::update(t);
}