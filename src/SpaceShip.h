/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpaceShip.h
 * \brief Declaration of a space ship (including weapons and defense)
 */
#pragma once
#include "GlobalGL.h"
#include "MovingObject.h"
#include "ModelObject.h"
#include "ObjectLoader3ds.h"
#include "Vector3.h"

//! Represents a space ship.
/**
 * Derived from the ModelObject class which means, the ship is drawable using
 * the base draw() method. As a ship is movable, it inherits also from the
 * MovingObject class. Besides that the class provides the properties and the
 * behavior of a space ship. It contains shooting, the shield.
 */
class SpaceShip :
  virtual public MovingObject, public ModelObject {
    // constructor, needs a display list
public:
  SpaceShip( float r, GLuint displayList );
  SpaceShip( Vector3 position, float r, GLuint displayList );
  SpaceShip( float x, float y, float z, float r, GLuint displayList );
  ~SpaceShip(void);
private:
  void initialize();

  // draw the ship
public:
  void draw();
  Vector3 getOriginalView( void );
  Vector3 getOriginalUp( void );
  bool isDestroyed();
private:
  Vector3 originalView;
  Vector3 originalUp;

  // updating
protected:
  void advance( float t );
  void updateLaser( float t );
public:
  virtual void update( float t);

  // shooting with laser and missiles
public:
  float getLaserHeat( void );
  float getMaxLaserHeat( void );
  float getLaserPercentage( void );
  unsigned int getMissileCount( void );
  bool isOverHeated( void );
  void useMissile( void );
  bool isShooting();
  void setShoot(bool status);
protected:
  void setLaserHeat( float heat );
  virtual void coolLaser( DWORD time );
private:
  void useLaser( DWORD time );
  int heatPunishTime;
  float m_laserHeat;
  unsigned int m_maxLaserHeat;
  unsigned int m_maxMissiles;
  unsigned int m_missiles;
  bool m_overHeated;
  bool m_ship_was_hit;

  // the shield
public:
  float getBackShieldPower( void );
  float getBackShieldPowerPercentage( void );
  float getFrontShieldPower( void );
  float getFrontShieldPowerPercentage( void );
  void setShieldRecovering( bool recovers );
  void receiveHitBack( DWORD time );
  void receiveHitFront( DWORD time );
  void setShieldCapacity( float capacity );
  bool shipWasHitOnce();
private:
  void recoverShieldBack( DWORD time );
  void recoverShieldFront( DWORD time );
  float m_backShieldPower;
  float m_frontShieldPower;
  float m_maxShieldPower;
  bool shooting;
  bool m_shieldRecovers;
};
