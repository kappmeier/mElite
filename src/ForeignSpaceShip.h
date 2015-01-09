/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	ForeignSpaceShip.h
 * \brief Declaration of a general space ship.
 */
#pragma once
#include "spaceship.h"
#include "Plane.h"

class ForeignSpaceShip abstract :
  public SpaceShip
{
public:
  ForeignSpaceShip(float r, GLuint displayList);
public:
  ~ForeignSpaceShip(void);
private:
  virtual void adjust( float t )=0;  
public:
  void update(float t);
};
