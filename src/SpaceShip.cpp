/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	SpaceShip.cpp
 * \brief Implementation of a space ship.
 */
#include "SpaceShip.h"
#include "Plane.h"

//! Initializes the space ship.
/**
 * The radius and the display list have to be
 * submitted if an object is initialized. The starting position is set to the
 * origin.
 * \param r the radius
 * \param displayList the display list
 */
SpaceShip::SpaceShip( float r, GLuint displayList ) : MovingObject( ), ModelObject( r, displayList ) {
  initialize();
}

//! Initializes the space ship.
/**
 * The radius and the display list have to be
 * submitted if an object is initialized.
 * \param position the position as Vector3
 * \param r the radius
 * \param displayList the display list
 */
SpaceShip::SpaceShip( Vector3 position, float r, GLuint displayList ) : MovingObject( position ), ModelObject( r, displayList ) {
  initialize();
}

//! Initializes the space ship.
/**
 * The radius and the display list have to be
 * submitted if an object is initialized.
 * \param x the x-position
 * \param y the y-position
 * \param z the z-position
 * \param r the radius
 * \param displayList the display list
 */
SpaceShip::SpaceShip( float x, float y, float z, float r, GLuint displayList ) : MovingObject( x, y, z ), ModelObject( r, displayList ) {
  initialize();
}

/**
 * \brief Destructor
 */
SpaceShip::~SpaceShip(void) {

}

/**
 * \brief Initializes the propertys of the ship such as laser and shields.
 */
void SpaceShip::initialize( void ) {
  m_laserHeat = 0;
  m_maxLaserHeat = 100;
  m_overHeated = false;
  m_backShieldPower = 100;
  m_frontShieldPower = 100;
  m_maxShieldPower = 100;
  m_missiles = 4;
  m_maxMissiles = 4;
  heatPunishTime = 1000;
  originalView = Vector3(0,0,1);
  originalUp = Vector3(0,1,0);
  shooting = false;
  m_shieldRecovers = false;
  m_ship_was_hit = false;
}

//! Draws the object using the base class drawing method.
/**
 * Previously it is translated to the current position.
 */
GLvoid SpaceShip::draw( void ) {
  glPushMatrix();
  glTranslatef( getPos().x, getPos().y, getPos().z );

  Vector3 orgView = this->getOriginalView();
  orgView.normalize();

  Vector3 actView = this->getView();
  actView.normalize();

  Vector3 orthogonal = orgView  * actView;
  orthogonal.normalize();

  float projection = orgView.innerProduct(actView); // / actView.length();
  float angle = acos(projection) / ANGLE2DEG;

  Vector3 nullvector = Vector3(0,0,0);

  Plane * testplane = new Plane(nullvector, orthogonal, orgView);

  float testdistance = testplane->distance(actView);

  if (testdistance < 0)
    angle = 360 - angle;

  glRotatef(angle,orthogonal.x,orthogonal.y,orthogonal.z);

  ModelObject::draw();
  glPopMatrix();
}

//! Determines wheather a ship is destroyed or not.
/**
 * If the current front or back shield power is less than zero, the ship is destroyed.
 * \return True if the ship is destroyed, false elsewise.
 */
bool SpaceShip::isDestroyed() {
  if( m_frontShieldPower < 0 || m_backShieldPower < 0 )
    return true;
  else
    return false;
}

//! Cools the laser.
/**
 * If the laser is not used, this function is called. It cools the laser until
 * its heat is zero. This is called in the SpaceShip::advance() method.
 * \param time the time that has passed
 */
void SpaceShip::coolLaser( DWORD time ) {
  m_laserHeat -= time * m_maxLaserHeat/5000.0;
  if( m_laserHeat < 0 )
    m_laserHeat = 0;
  if( heatPunishTime > 0)
    heatPunishTime -= time;
  else
    m_overHeated = false;
}

//! Moves a ship forward.
/**
 * The space ship is moved according to the distance that is covered with
 * the current speed in the given amount of time.
 * \param time the delta time in milliseconds
 */
void SpaceShip::advance( float time)
{
	MovingObject::advance(time);
}

//! Updates laser status after a given time.
/**
 * Updates the state of the laser after a time difference.
 * \param time the delta time in milliseconds
 */
void SpaceShip::updateLaser( float time ) {
	if ( shooting && !m_overHeated )
		useLaser(t);
	else
		coolLaser(t);
}

//! Updates the ship's status after a time interval.
/**
 * Recovers shields, weapons and updates the position.
 * \param time the delta time in milliseconds
 */
void SpaceShip::update( float t) {
  if( m_shieldRecovers ) {
    this->recoverShieldBack( t );
    this->recoverShieldFront( t );
  }
	advance(t);
	updateLaser(t);
}

/**
 * \brief Returns the current heat of the laser.
 */
float SpaceShip::getLaserHeat() {
  return m_laserHeat;
}

/**
 * \brief Returns the maximum laser heat.
 */
float SpaceShip::getMaxLaserHeat( void ) {
  return m_maxLaserHeat;
}

/**
 * \brief Returns the percentage of the heat of the maximal heat.
 */
float SpaceShip::getLaserPercentage() {
  return m_laserHeat/m_maxLaserHeat;
}

/**
 * \brief Returns the current amount of missiles.
 */
unsigned int SpaceShip::getMissileCount( void ) {
  return this->m_missiles;
}

/**
 * \brief Returns true if the laser is overheated, otherwise false.
 */
bool SpaceShip::isOverHeated() {
  return m_overHeated;
}

//! Updates the laser heat, if it is used.
/**
 * If the laser is used, this function is called. It updates the heat until it
 * is overheated.This is called in the advance() method.
 * \param time the time that has passed
 */
void SpaceShip::useLaser( DWORD time ) {
  m_laserHeat += time*6 * m_maxLaserHeat/5000.0; // this creates over-heating in 5 seconds or 5000 ms
  if( m_laserHeat > m_maxLaserHeat ) {
    m_laserHeat = m_maxLaserHeat;
    m_overHeated = true;
    heatPunishTime = 1000;
  }
}

//! Shoots a missile, if possible.
/**
 * Reduces the missile count by one.
 */
void SpaceShip::useMissile( void ) {
  if( this->m_missiles > 0 )
    m_missiles--;

}

/**
 * \brief Decides whether the ship is shooting currently.
 */
bool SpaceShip::isShooting(){
	return (shooting&&!m_overHeated);
}

/**
 * \brief Sets the laser heat.
 */
void SpaceShip::setLaserHeat( float heat ) {
  m_laserHeat = heat;
}

/**
 * \brief Specifies if the ship is shooting.
 */
void SpaceShip::setShoot(bool status){
	shooting = status;
}

/**
 * \brief Returns the current back shield power.
 */
float SpaceShip::getBackShieldPower() {
  return m_backShieldPower;
}

/**
 * \brief Returns the current percentage of back shield power.
 */
float SpaceShip::getBackShieldPowerPercentage() {
  return m_backShieldPower / m_maxShieldPower;
}

/**
 * \brief Returns the current front shield power.
 */
float SpaceShip::getFrontShieldPower() {
  return m_frontShieldPower;
}

/**
 * \brief Returns the current percentage of front shield power.
 */
float SpaceShip::getFrontShieldPowerPercentage() {
  return m_frontShieldPower / m_maxShieldPower;
}

/**
 * \brief Returns the original up vector.
 */
Vector3 SpaceShip::getOriginalUp(){
  return originalUp;
}

/**
 * \brief Returns the original view vector.
 */
Vector3 SpaceShip::getOriginalView(){
  return originalView;
}

/**
 * \brief Receives a hit into the back shield over a given time interval.
 * \param time the time that has passed
 */
void SpaceShip::receiveHitBack( DWORD time ) {
  m_ship_was_hit=true;
  if( m_backShieldPower >= 0 )
    m_backShieldPower -= time*3* m_maxShieldPower/5000.0; // this creates over-heating in 5 seconds or 5000 ms
}

/**
 * \brief Receives a hit into the front shield over a given time interval.
 * \param time the time that has passed
 */
void SpaceShip::receiveHitFront( DWORD time ) {
  m_ship_was_hit=true;
  if( m_frontShieldPower >= 0 )
    m_frontShieldPower -= time*3* m_maxShieldPower/5000.0; // this creates over-heating in 5 seconds or 5000 ms
}

/**
 * \brief Recovers the back shield capacity for a given time interval.
 * \param time the time that has passed
 */
void SpaceShip::recoverShieldBack( DWORD time ) {
  m_backShieldPower += time*0.75 * m_maxShieldPower / 10000.0;
  if( m_backShieldPower > m_maxShieldPower )
    m_backShieldPower = m_maxShieldPower;
}

/**
 * \brief Recovers the front shield capacity for a given time interval.
 * \param time the time that has passed
 */
void SpaceShip::recoverShieldFront( DWORD time ) {
  m_frontShieldPower += time*0.75 * m_maxShieldPower / 10000.0;
  if( m_frontShieldPower > m_maxShieldPower )
    m_frontShieldPower = m_maxShieldPower;

}

/**
 * \brief Sets a new total capacity for the shields.
 * \param capacity the capacity
 */
void SpaceShip::setShieldCapacity( float capacity ) {
  if( capacity < 0 )
    return;
  m_maxShieldPower = capacity;
  if( m_frontShieldPower > m_maxShieldPower )
    m_frontShieldPower = m_maxShieldPower;
  if( m_backShieldPower > m_maxShieldPower )
    m_backShieldPower = m_maxShieldPower;
}

/**
 * \brief Enables recovering of shield energy
 * \param recovers decides whether the shield is recovering, or not.
 */
void SpaceShip::setShieldRecovering( bool recovers ) {
  m_shieldRecovers = recovers;
}

bool SpaceShip::shipWasHitOnce()
{
	return m_ship_was_hit;
}
