/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Pirate.h
 * \brief Declaration of a Pirate ship.
 */
#pragma once
#include "foreignspaceship.h"
#include "PlayerObject.h"

//! Pirates have a specific position. If the player comes too near, they start to attack.
class Pirate :
	public ForeignSpaceShip
{
public:
	Pirate(float r, GLuint displayList, PlayerObject *player, float maxSpeed);
public:
	~Pirate(void);
public:
	void adjust (float t);
private: 
	PlayerObject *player;
	float maxSpeed;
	bool behindPlayer();
};
