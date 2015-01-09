/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Pirate.cpp
 * \brief Implementation of a pirate ship.
 */
#include "Pirate.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

Pirate::Pirate(float r, GLuint displayList, PlayerObject *player, float maxSpeed):ForeignSpaceShip(r,displayList)
{
	    this->player = player;
		this->maxSpeed = maxSpeed;
}

Pirate::~Pirate(void)
{
}

bool Pirate::behindPlayer()
{
	Plane plane = Plane(player->getView(), player->getPos());
	return ( plane.distance( this->getPos()) < 0);
}

void Pirate::adjust( float t )
{   
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
		if (this->shipWasHitOnce())
			this->setShoot(true);
		else
			this->setShoot(false);
	}

	if (distance > 250)
	{
		if (this->shipWasHitOnce())
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
	this->setView(desiredView);

}