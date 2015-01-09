/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Merchant.cpp
 * \brief Implementation of a Merchant space ship.
 */
#include "Merchant.h"

Merchant::Merchant(float r, GLuint displayList, Planet * planet, Vector3 aim):ForeignSpaceShip(r,displayList)
{
	this->planet=planet;
	this->aim = aim;
}

Merchant::~Merchant(void)
{
}


void Merchant::adjust( float t )
{

}