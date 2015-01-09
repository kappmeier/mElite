/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Thargon.h
 * \brief Declaration of the Thargon class.
 */
#pragma once
#include "ForeignSpaceShip.h"
#include "PlayerObject.h"

//! Represents an evil Thargon space ship
class Thargon :
	public ForeignSpaceShip
{
public:
	Thargon(float r, GLuint displayList, PlayerObject *player, float maxSpeed);
public:
	~Thargon(void);
public:
	void adjust (float t);
private: 
	PlayerObject *player;
	float maxSpeed;
};