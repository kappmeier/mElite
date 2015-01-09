/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	World.h
 * \brief	Declares the application class for the mElite game.
 */
#pragma once
 
#include <vector>
#include <string>
#include <list>
#include "Structures.h"

class MovingObject;
class Planet;
class Wrapper;
class SpaceShip;
class ForeignSpaceShip;
class Thargon;
class Pirate;
class Merchant;
class PoliceShip;
class GraphicsObject;
class PlayerObject;
class GameObject;
class Star;
class Planet;
class SpaceStation;

//! This class represents the game world. 
/**
 * \nosubgrouping
 * All information that are needed are 
 * hold by this class. Therefore it is designed as a singleton. It creates all
 * graphical information, e.g. the planets textures, randomized. The access to
 * the Elite data provided by Textelite is given through a wrapper object. 
 */
class World {
  // singleton stuff
public:
  static World  * getWorld( void );
  ~World( void );
private:
  World( void );
  static class World *m_world;

  // object handling
public:
  void initialize();
  void deinitialize();
  PlayerObject *getPlayer();
  Star * getStar();
  std::vector<Planet*> * getPlanets();
  SpaceStation * getStation();

  SpaceShip * createNewSpaceShip( ObjectName objectType, float radius );
  Thargon * createNewThargon( ObjectName objectType, float radius, PlayerObject *player, float maxSpeed );
  Pirate * createNewPirate( ObjectName objectType, float radius, PlayerObject *player, float maxSpeed );
  Merchant * createNewMerchant( ObjectName objectType, float radius, Planet *planet, Vector3 aim );
  PoliceShip * createNewPoliceShip( ObjectName objectType, float radius, std::vector<Vector3> point, PlayerObject * player, float maxSpeed );

  std::vector<GraphicsObject*> * getGraphicObjects();
  std::vector<SpaceShip*> * getSpaceShips();
  std::vector<ForeignSpaceShip*> * getForeignSpaceShips();
  std::vector <PoliceShip*> * getPoliceShips();

  void deleteCrashedShips( void );
  void deleteDestroyedShips( void );
private:

  std::list<GameObject*> GameObjects;
  std::list<GameObject*>::iterator playerPosition;
  std::list<GameObject*>::iterator starPosition;
  std::list<GameObject*>::iterator stationPosition;

  // these are stored here to be consistent until the new code is completed
  // contains all objects that are drawable
  std::vector<GraphicsObject*> m_drawable;          // a list of all drawable objects in the scene
  // contains all objects that move
  std::vector<MovingObject*> m_moving;

  unsigned long playerStartDistance;
  unsigned int nrForCoriolis[256];

  // information about the systems - submitted by wrapper or selfcreated
public:
  SystemInfo getSystemInfo( void );
  SystemInfo getSystemInfo( unsigned __int8 number );
  std::string getSystemName( int number );
  std::vector<PlanetInfoGraphical>  * getSystemPlanets( void );
  std::vector<PlanetInfoGraphical>  * getSystemPlanets( unsigned __int8 number );
  std::vector<SystemInfo> *getSystems( SystemLists systemList );
  std::vector<SystemInfo> *getSystems( int left, int right, int bottom, int top );
  TextureName getSystemStarTexture( void );
  TextureName getSystemStarTexture( unsigned __int8 number );
  bool isSystemReachable( int number );
  bool isSystemLocal( int number );
  void World::performJump( int number );
private:
  TextureName getPlanetTexture( unsigned __int8 number );
  TextureName getPlanetTextureR( bool reset );
  TextureName getStarTexture( unsigned __int8 number );
  TextureName getStarTextureR( void );
  std::vector<PlanetInfoGraphical>  * getSystemPlanetsR( void ); // create randomly an system info
  std::vector<PlanetInfoGraphical> *systemPlanets[256];// array of vectors wich holds the informations about all 256 systems
  std::vector<TextureName> systemStars;

  // marketplace and fuel
public:
  void buyFuel( float amount );
  std::vector<Tradegood> * getAllTradegoodDescriptions( void );
  unsigned int getCargoBaySize( void );
  float getCurrentCash( void );
  std::vector<int> * getCurrentCargo( void );
  std::vector<MarketplaceItem> * getCurrentMarketplace( void );
  unsigned int getFreeHoldspace( void );
  int getNumberOfTradegoods( void );
  Tradegood getTradegoodDescription( int );
  std::string performPurchase( int number, int amount );
  std::string performSale( int number, int amount );

  float getFuelCost();
  float getFuelAmount();
  float getMaxFuel();
  bool tradegoodIsInTons(int number);

  // player status
public:
  PlayerStatus  * getPlayerStatus( void );
private:
  PlayerStatus  * m_playerStatus;

  // some helping stuff
public:
  std::string getEconomyString( econTypes economy );
  std::string getGovernmentString( govTypes government );
private:
  unsigned int getRandNumber( unsigned int min, unsigned int max );
  Wrapper *wrapper;
  friend void error1( std::string error );
  friend void error2( std::string error );
};
