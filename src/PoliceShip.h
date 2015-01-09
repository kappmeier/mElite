/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	PoliceShip.h
 * \brief Definition of the PoliceShip class representing a police ship
 */
#pragma once
#include "foreignspaceship.h"
#include "PlayerObject.h"

//! A police ship.
/**
 * A police ship has a distinct behaviour. It flies around the Coriolis station,
 * and attacks all ships that are attacking the police ship. If the Coriolis station
 * is attacked, all police ships start attacking the player at once.
 */
class PoliceShip :
	public ForeignSpaceShip
{
public:
	PoliceShip(float r, GLuint displayList, std::vector<Vector3> point, PlayerObject * player, float maxSpeed);
public:
	~PoliceShip(void);
public:
	void adjust (float t);
	void stationWasHit();
private:
	std::vector<Vector3> point;
	unsigned int aim;
	float maxSpeed;
	PlayerObject * player;
	bool behindPlayer();
	bool m_station_was_hit;
};
