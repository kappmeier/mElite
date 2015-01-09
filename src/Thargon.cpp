/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Thargon.cpp
 * \brief Implementation of the behaviour of a Thargon ship.
 */
#include "Thargon.h"

Thargon::Thargon(float r, GLuint displayList, PlayerObject *player, float maxSpeed):ForeignSpaceShip(r,displayList)
{ 
  this->player = player;
  this->setShoot(false);
  this->maxSpeed = maxSpeed;
  this->setSpeed(0);
}

Thargon::~Thargon(void)
{
}

void Thargon::adjust( float t )
{
  Vector3 playerPos = player->getPos();
  Vector3 myPos = this->getPos();
  Vector3 desiredView = playerPos - myPos;

  float distance = desiredView.length();

  if (distance < 100)
	  setSpeed(maxSpeed);
  else
  {
	  if (this->shipWasHitOnce())
		  setSpeed(10*maxSpeed);
	  else
		  setSpeed(0);
  }

  desiredView.normalize();

  this->setView(desiredView);
}