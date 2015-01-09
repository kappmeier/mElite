/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	PoliceShip.cpp
 * \brief Implementation of a police ship
 */
#include "PoliceShip.h"

PoliceShip::PoliceShip(float r, GLuint displayList, std::vector<Vector3> point, PlayerObject* player, float maxSpeed):ForeignSpaceShip(r,displayList)
{
	this->point = point;
	aim = 0;
	this->setShoot(false);
	this->maxSpeed = maxSpeed;
	this->player = player;
	this->setPos(point[aim]);
	this->setSpeed(maxSpeed);
	m_station_was_hit = false;
}

PoliceShip::~PoliceShip(void)
{
}

void PoliceShip::stationWasHit(){
	m_station_was_hit = true;
}

bool PoliceShip::behindPlayer()
{
	Plane plane = Plane(player->getView(), player->getPos());
	return ( plane.distance( this->getPos()) < 0);
}
void PoliceShip::adjust (float t)
{
	if (m_station_was_hit || this->shipWasHitOnce())
	{ // player has shot the ship -> behave like a pirate and chase player
		Vector3 playerPos = player->getPos();
		Vector3 myPos = this->getPos();

		Vector3 desiredView = playerPos - myPos;
		float distance = desiredView.length();

		bool behind = this->behindPlayer();
		float playerSpeed = player->getSpeed();

		float actualSpeed = this->getSpeed();
		if (distance < 6)
		{
			if (behind) // we are chasing the player
				this->setSpeed(playerSpeed-0.005); // move slower to get out of range
			// (or, if playerSpeed negative, move faster backwards)
			if (!behind) // we are in front of the player
				this->setSpeed(-playerSpeed-0.005); // our speed must be vise versa

			this->setShoot(true);
		}
		if (distance >=6 && distance <= 8)
		{
			if (behind)
				this->setSpeed( playerSpeed );
			if (!behind)
				this->setSpeed( -playerSpeed );
			this->setShoot(true);
		}
		if (distance > 8 && distance <= 15)
		{
			if (behind) // we are chasing the player
				this->setSpeed(playerSpeed+0.005); // move faster to catch him
			// (or, if playerSpeed negative, move slower backwards)
			if (!behind) // we are in front of the player
				this->setSpeed(-playerSpeed+0.005); // our speed must be vise versa
			this->setShoot(true);
		}
		if (distance > 15 && distance <= 250)
		{
			this->setSpeed( maxSpeed );
			if (m_station_was_hit || this->shipWasHitOnce())
				this->setShoot(true);
			else
				this->setShoot(false);
		}

		if (distance > 250)
		{
			if (m_station_was_hit || this->shipWasHitOnce())
			{
				this->setShoot(true);
				this->setSpeed(maxSpeed);
			}
			else
			{
				this->setSpeed(0);
				this->setShoot(false);
			}		
		}

		desiredView.normalize();
		this->setView(desiredView);	}
	else
	{ // guarding something (probably the coriolis station)
		Vector3 directionToAim = point[aim] - this->getPos();
		float distanceToAim = directionToAim.length();
		if (distanceToAim < 0.5)
		{
			aim = (aim +1);
			if (aim >= point.size()) aim = 0;
			directionToAim = point[aim] - this->getPos();
		}
		directionToAim.normalize();
		this->setView(directionToAim);
	}

}