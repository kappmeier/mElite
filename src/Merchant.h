/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Merchant.h
 * \brief Declaration of a Merchant space ship.
 */
#pragma once
#include "foreignspaceship.h"
#include "Planet.h"

//! A Merchant is a specific space ship that travels between a planet and the station.
class Merchant :
	public ForeignSpaceShip
{
public:
	Merchant(float r, GLuint displayList, Planet * planet, Vector3 aim);
public:
	~Merchant(void);
public:
	void adjust (float t);
private: 
	Planet *planet;
	Vector3 aim;
};
