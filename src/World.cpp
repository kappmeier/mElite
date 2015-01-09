/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	World.cpp
 * \brief	Contains the World class, that contains all objects in the 3d game scenery.
 */
#include "World.h" 
#include "SpaceShip.h"
#include "GlobalParameters.h"
#include "Textures.h"
#include "Wrapper.h"
#include "Objects.h"
#include "FileAccess.h"
#include "MovingObject.h"
#include "Planet.h"
#include "Wrapper.h"
#include "SpaceShip.h"
#include "ForeignSpaceShip.h"
#include "Thargon.h"
#include "Pirate.h"
#include "Merchant.h"
#include "PoliceShip.h"
#include "GraphicsObject.h"
#include "PlayerObject.h"
#include "GameObject.h"
#include "Star.h"
#include "Planet.h"
#include "SpaceStation.h"

using namespace std;

/** @name Object handling
 * These methods provide the basic object handling for the game world. In World::initialize() the world
 * is created. This function needs to be called each time, a new system is loaded. The other functions mainly
 * serve to retrieve pointers to game objects. Some functions to update the world are also included. They
 * delete objects if necessary.
 */
//@{
//! Initializes the game world for one system.
/**
 * Creates all objects for one system that can be considered as a level. The first created object is the
 * player itself. After that the star and the planets and at last the coriolis station of the current
 * system are created. All objects are initialized with correct values and pushed into a list holding
 * all game objects.
 */
void World::initialize() {
  // init the data structure
  GameObjects.clear();

  float lightspeed = 0.00815;

  // create player
  PlayerObject *player = new PlayerObject( 0, 0, 4100, 0 , 0);
  player->setView( Vector3( 0, 0, -1 ) );
  player->setMaxSpeed( lightspeed );
  player->setMinSpeed( -lightspeed*0.5 );
  player->setStep( lightspeed/20.0 * 0.2 );  // 0.2 is a calibration value ;) greater value increases speed faster
  player->setMoveSpeed( player->getMaxSpeed()*0.5 );

  GameObjects.push_back( player );

  list<GameObject*>::iterator it = GameObjects.begin();
  playerPosition = it;

  // create the star(s)
  Star *sun = new Star( 0, 0, 0, 1000, star );
  //sun->setLightColor( 0, 1, 0);
  sun->setAxis( 0, 1, 0 );
  sun->setSpeed( -0.006);
  sun->setRotateAngle( 0 );
  sun->setRotateSpeed( 0 );

  GameObjects.push_back( sun );
  starPosition = ++it;

  // get planets info
  vector<PlanetInfoGraphical> planets = *World::getWorld()->getSystemPlanets();

  // get a planet for placing coriolis station later  

  unsigned int coriolisPlanetNr = nrForCoriolis[wrapper->getCurrentSystem()];
  Planet * coriolisPlanet;

  // create planets
  PlanetInfoGraphical planet = planets[0];
  float scaleFactor = (4155.0*0.5) / (planet.distance); // this sets the distance of the first planet to 4155/2
  for( unsigned int i=0; i < planets.size(); i++ ) {    
    PlanetInfoGraphical planetInfo = planets[i];
    Planet *planet = new Planet( 0, 0, scaleFactor*planetInfo.distance, planetInfo.radius*10, planet1+i );
    planet->setAxis( 0, 1, 0 );
    planet->setColor( 0, 0, 0 );
    planet->setSpeed( planetInfo.spinningSpeed );
	// set rotate angle randomly, but in [-30,30] intervall
	  // set corolis planet rotate angle randomly, but in [-15,15] intervall
    planet->setRotateAngle((rand()%61-30));
    //planet->setRotateAngle( 0 );
    planet->setRotateSpeed( 0 );
    GameObjects.push_back( planet );
	if (i == coriolisPlanetNr) 
		coriolisPlanet = planet;
    it++;
  }
  
  // set corolis planet rotate angle randomly, but in [-15,15] intervall
  // coriolisPlanet->setRotateAngle((rand()%31-15));
  coriolisPlanet->setRotateAngle(0);

  // create coriolis station
  SpaceStation *station = new SpaceStation( 1, Objects::getObjects()->getDisplayList( coriolis ) );
  // calculate random position in orbit
  float sx, sy, sz;
  sx = coriolisPlanet->getPos().x;
  sy = coriolisPlanet->getPos().y;
  sz =  coriolisPlanet->getPos().z;
  Vector3 a = Vector3( rand()%100, rand()%100, rand()%100);
  a.normalize();
  a = a * coriolisPlanet->getCollisionRadius()*2.0f;//4150 ); 
  station->setPos( sx+a.x, sy+a.y, sz+a.z );
  //station->setPos(sx,sy,sz);
  station->setSpeed( 0.006 );
  station->setAxis( a ); // Vector3( 0, 0, 1 ) );
  GameObjects.push_back( station );
  stationPosition = ++it;

  playerStartDistance = 1000;

  player->setPos(station->getPos().x,station->getPos().y,station->getPos().z+playerStartDistance);

  // create 5 up to 10 pirates
  unsigned int numOfPirates = rand()%6+5;
  //numOfPirates = 0;

  for (unsigned i = 0; i < numOfPirates; i++)
  {
	  Pirate* ship;
      ship = World::getWorld()->createNewPirate( anaconda, 1, player, lightspeed*0.75 );

	  // pirates are between player and station, but not at station (there is the police)
	  int rangeStart = (int)((playerStartDistance) * 0.50); //-(int)playerStartDistance* 0.5;
      int rangeEnd = (int)((playerStartDistance) * 0.90);
	  int range = rangeEnd-rangeStart;

	  Vector3 offset;
	  offset.x = (rand()%101)-50;
	  offset.y = (rand()%101)-50;
	  offset.z = (rand()%range)+rangeStart;

	  Vector3 stationPos = station->getPos();

	  ship->setPos (stationPos+offset);
  }

  // create thargoids; maximal 5, but less if there are already more than 5 pirates
  unsigned int maxNumberOfThargoids = max((int)0,(int)(11-numOfPirates));
  unsigned int numOfThargoids = rand()%(maxNumberOfThargoids+1);
  
  //numOfThargoids = 0;

  int thargoidPositionRangeStart = (int)((playerStartDistance) * 0.20);
  int thargoidPositionRangeEnd   = (int)((playerStartDistance) * 0.45);
  int thargoidPositionRange = thargoidPositionRangeEnd-thargoidPositionRangeStart;
  
  int thargoidPosition = rand()%thargoidPositionRange+thargoidPositionRangeStart;

  int thargoidRange = 100;

  for (unsigned i = 0; i < numOfThargoids; i++)
  {
	  Thargon* ship;
      ship = World::getWorld()->createNewThargon( ball, 1, player, lightspeed/2 );

	  // thargoids are all in a certain field      

	  Vector3 offset;
	  offset.x = (rand()%101)-50;
	  offset.y = (rand()%101)-50;
	  offset.z = (thargoidPosition+rand()%thargoidRange-thargoidRange/2);

	  Vector3 stationPos = station->getPos();
	  ship->setPos( stationPos + offset);

  }

  // create PoliceShips
  unsigned int numOfPoliceShips = 3 + rand()%3;

  int shipsPositionRangeStart = (int)((playerStartDistance) * 0.01);
  int shipsPositionRangeEnd   = (int)((playerStartDistance) * 0.15);
  int shipsPositionRange = shipsPositionRangeEnd-shipsPositionRangeStart;

  for (unsigned int i = 0; i < numOfPoliceShips; i++)
  {  
	  std::vector<Vector3> pointList;
      Vector3 offset;

	  Vector3 stationPos = station->getPos();

      unsigned int shipPosition= rand()%shipsPositionRange+shipsPositionRangeStart;

	  for (unsigned int j = 0; j < 3; j++)
	  {
		  offset.x = (rand()%51)-25;
	      offset.y = (rand()%51)-25;
	      offset.z = shipPosition;
		  pointList.push_back(offset+stationPos);
	  }	    

	  PoliceShip* ship;
      ship = World::getWorld()->createNewPoliceShip( adder, 1, pointList,player, lightspeed*0.75 );
  }

  // create a merchant

  int shipPositionRangeStart = (int)((playerStartDistance) * 0.01);
  int shipPositionRangeEnd   = (int)((playerStartDistance) * 0.05);
  int shipPositionRange = shipsPositionRangeEnd-shipsPositionRangeStart;
  
  Vector3 offset;
  offset.x = (rand()%41)-20;
  offset.y = (rand()%41)-20;
  offset.z = rand()%shipsPositionRange+shipsPositionRangeStart;

  Merchant* ship;
  ship = World::getWorld()->createNewMerchant( thargoid, 1, coriolisPlanet, player->getPos()+offset);

  Vector3 stationPos = station->getPos();
  ship->setPos( stationPos + offset);
  Vector3 viewVector = player->getPos()+offset - stationPos + offset;
  viewVector.normalize();
  ship->setView( viewVector );
  ship->setSpeed( lightspeed/3 );
}

//! Destructs the game world.
/**
 * Deletes all objects that have been inserted into the object list. As the list contains generic pointers
 * to objects of the type Objects::getObjects(), all object have to be casted into their original type. At last the
 * list is cleared itself. The next level can be initialized through World::initialize().
 */
void World::deinitialize() {
  list<GameObject*>::iterator it = GameObjects.begin();
  while( it !=GameObjects.end() ) {
    PlayerObject * pl = dynamic_cast<PlayerObject*>(*it);
    Star * st = dynamic_cast<Star*>(*it);
    Planet * pln = dynamic_cast<Planet*>(*it);
    PoliceShip * pol = dynamic_cast<PoliceShip*>(*it);
    Pirate * pir = dynamic_cast<Pirate*>(*it);
    Thargon * tha = dynamic_cast<Thargon*>(*it);
    Merchant * mer = dynamic_cast<Merchant*>(*it);
    SpaceShip * sh = dynamic_cast<SpaceShip*>(*it);
    SpaceStation * sta = dynamic_cast<SpaceStation*>(*it);
    if( pl ) {
      delete pl;
      it++;
      continue;
    }
    if( st ) {
      delete st;
      it++;
      continue;
    }
    if( pln ) {
      delete pln;
      it++;
      continue;
    }
    if( pol ) {
      delete pol;
      it++;
      continue;
    }
    if( pir ) {
      delete pir;
      it++;
      continue;
    }
    if( tha ) {
      delete tha;
      it++;
      continue;
    }
    if( mer ) {
      delete mer;
      it++;
      continue;
    }
    if( sh ) {
      delete sh;
      it++;
      continue;
    }
    if( sta ) {
      delete sta;
      it++;
      continue;
    }
    MessageBox(0, "Unknown Type!", "World::deinitialize()", 0 );
    it++;
  }
  GameObjects.clear();
}

//! Returns the current player object.
/**
 * Uses an iterator that has been saved during initialization to find the player object in the list of
 * objects. The object is casted to PlayerObject and returned. The pointer can be saved but gets invalid
 * if a new system is loaded, that means World::deinitialize() and World::initialize() have been called.
 * \return a pointer to the current player object.
 */
PlayerObject *World::getPlayer() {
  PlayerObject * pl = dynamic_cast<PlayerObject*>(*playerPosition);
  return pl;
}

//! Returns the star of the current system.
/**
 * Uses an iterator that has been saved during initialization to find the star object in the list of objects.
 * The found object is casted to Star and returned. The pointer can be saved but gets invalid
 * if a new system is loaded, that means World::deinitialize() and World::initialize() have been called.
 * \return a pointer to the current systems star.
 */
Star *World::getStar() {
  Star * st = dynamic_cast<Star*>(*starPosition);
  return st;
}

//! Returns a vector containing the planets of the current system.
/**
 * \fn std::vector<Planet*> * World::getPlanets()
 * Uses an iterator that has been saved during initialization to find the first planet object in the list of
 * objects. It iterates through the list beginning at the position of the first planet to find all the planets.
 * The iteration stops when the first non-planet object is found. It is assumed that all planets are inserted
 * one after another. The objects are casted to Planet and put into a vector. The pointers became invalid
 * if a new system is loaded, that means World::deinitialize() and World::initialize() have been called.
 * \return a pointer to a vector containing pointers to the planets. it is created in this function and has to be deleted from the caller.
 */
vector<Planet*> * World::getPlanets() {
  vector<Planet*> * planets = new vector<Planet*>;

  if( starPosition == GameObjects.end() )
    return planets;

  list<GameObject*>::iterator it = starPosition;
  it++;

  Planet * pl;
  while( it != GameObjects.end() && (pl = dynamic_cast<Planet*>(*it++)) != 0 )
    planets->push_back( pl );

  return planets;
}

//! Returns the station of the current system.
/**
 * Uses an iterator that has been saved during initialization to find the station object in the list of objects.
 * The found object is casted to SpaceStation and returned. The pointer can be saved but gets invalid
 * if a new system is loaded, that means World::deinitialize() and World::initialize() have been called.
 * \return a pointer to the current systems station.
 */
SpaceStation *World::getStation() {
  SpaceStation * st = dynamic_cast<SpaceStation*>(*stationPosition);
  return st;
}

//! Returns a list of all objects of type GraphicsObject.
/**
 * \fn std::vector<GraphicsObject*> *World::getGraphicObjects()
 * Uses the saved iterator to the star to find all drawable objects in the list of objects. The found
 * objects are casted to GraphicsObject and pushed into a vector. The whole list started at the star 
 * position is iterated until the first object that can't be casted into GraphicsObject is found.
 * \return a pointer to a vector containing pointers to the objects. it is created in this function and has to be deleted from the caller.
 */
vector<GraphicsObject*> *World::getGraphicObjects() {
  vector<GraphicsObject*> * retObjects = new vector<GraphicsObject*>;

  list<GameObject*>::iterator it = starPosition;

  GraphicsObject * obj;
  while( it!= GameObjects.end() && (obj = dynamic_cast<GraphicsObject*>(*it++)) != 0 )
    retObjects->push_back( obj );

  return retObjects;
}


//! Returns a list of all space ships.
/**
 * \fn std::vector<SpaceShip*> * World::getSpaceShips()
 * Uses the saved iterator to the station to find all space ship objects in the list of objects. The found
 * objects are casted to SpaceShip and pushed into a vector. The whole list started at the station 
 * position and is iterated until the first object that can't be casted into SpaceShip is found. This list can
 * be used to update all ships and is used to perform the collision detection in World::deleteCrashedShips().
 * \return a pointer to a vector containing pointers to the objects. it is created in this function and has to be deleted from the caller.
 */
vector<SpaceShip*> * World::getSpaceShips() {
  vector<SpaceShip*> * retObjects = new vector<SpaceShip*>;

  list<GameObject*>::iterator it = stationPosition;

  if (it==GameObjects.end()) return retObjects;

  it++;

  SpaceShip * obj;
  while( it!= GameObjects.end() && (obj = dynamic_cast<SpaceShip*>(*it++)) != 0 )
    retObjects->push_back( obj );

  return retObjects;
}

//! Returns a list of all space ships that can move freely.
/**
 * \fn std::vector<ForeignSpaceShip*> * World::getForeignSpaceShips()
 * Uses the saved iterator to the station to find all space ship objects in the list of objects. The found
 * objects are casted to ForeignSpaceShip and pushed into a vector. The whole list started at the station 
 * position and is iterated until the first object that can't be casted into ForeignSpaceShip is found. This list can
 * be used to update the positions and status of self acting ships.                                            
 * \return a pointer to a vector containing pointers to the objects. it is created in this function and has to be deleted from the caller.
 */
vector<ForeignSpaceShip*> * World::getForeignSpaceShips() {
  vector<ForeignSpaceShip*> * retObjects = new vector<ForeignSpaceShip*>;

  list<GameObject*>::iterator it = stationPosition;

  if (it==GameObjects.end()) return retObjects;

  it++;

  ForeignSpaceShip * obj;
  while( it!= GameObjects.end() && (obj = dynamic_cast<ForeignSpaceShip*>(*it++)) != 0 )
    retObjects->push_back( obj );

  return retObjects;
}

//! Returns a list of all police ships.
/**
 * \fn std::vector<PoliceShip*> * World::getPoliceShips()
 * Uses the saved iterator to the station to find all space ship objects in the list of objects. The found
 * objects are casted to PoliceShop and pushed into a vector if the cast was successful. 
 * We check all objects up to the end of the list. 
 * \return a pointer to a vector containing pointers to the objects. it is created in this function and has to be deleted from the caller.
 */vector <PoliceShip*> * World::getPoliceShips() {
  vector<PoliceShip*> * retObjects = new vector<PoliceShip*>;

  list<GameObject*>::iterator it = stationPosition;

  if (it==GameObjects.end()) return retObjects;

  it++;

  PoliceShip * obj;
  while( it!= GameObjects.end() )
  {
	obj = dynamic_cast<PoliceShip*>(*it++);
	if ( obj != 0)
		retObjects->push_back( obj );
  }

  return retObjects;
}

//! Creates a new space ship.          
/**
 * Creates a new space ship with a given type. The type has to be passed as variable of the type ObjectName.
 * With this name the correct display list can be loaded to display the ship. The loading of the model is
 * automatically done by the ModelObject class. A radius to perform 
 * collision detection and view frustum culling has also to be permitted.
 * \param objectType the type of the ship
 * \param radius the radius of the ship
 * \return a pointer to the new created ship
 */
SpaceShip * World::createNewSpaceShip( ObjectName objectType, float radius ) {
  SpaceShip *ship = new SpaceShip( radius, Objects::getObjects()->getDisplayList( objectType ) );
  ship->setView( FileAccess::getObjectView( objectType ) );
  GameObjects.push_back( ship );

  return ship;
}

//! Creates a new thargon ship.
/**
 * \param objectType The type of the thargon ship (as 3d model).
 * \param radius The size of the Thargon.
 * \param player The player.
 * \param maxSpeed The maximum speed of the new Thargon.
 */
Thargon * World::createNewThargon( ObjectName objectType, float radius, PlayerObject *player, float maxSpeed ) {
  Thargon *ship = new Thargon( radius, Objects::getObjects()->getDisplayList( objectType ), player, maxSpeed );
  ship->setView( FileAccess::getObjectView( objectType ) );

  GameObjects.push_back( ship );

  return ship;
}

//! Creates a new police ship.
/**
 * \param objectType The type of the pirate (as 3d model).
 * \param radius The size of the pirate.
 * \param player The player.
 * \param maxSpeed The maximum speed of the new pirate.
 */
Pirate* World::createNewPirate( ObjectName objectType, float radius, PlayerObject *player, float maxSpeed ) {
  Pirate *ship = new Pirate( radius, Objects::getObjects()->getDisplayList( objectType ), player, maxSpeed );
  ship->setView( FileAccess::getObjectView( objectType ) );

  GameObjects.push_back( ship );

  return ship;
}

//! Creates a new merchant.
/**
 * \param objectType The type of the merchant (as 3d model).
 * \param radius The size of the merchant.
 * \param planet The planet the merchant is flying to.
 * \param aim The direction of the merchant.
 */
Merchant * World::createNewMerchant( ObjectName objectType, float radius, Planet * planet, Vector3 aim ) {
  Merchant *ship = new Merchant( radius, Objects::getObjects()->getDisplayList( objectType ), planet, aim );
  ship->setView( FileAccess::getObjectView( objectType ) );

  GameObjects.push_back( ship );

  return ship;
}

//! Creates a police ship.
/**
 * \param objectType The type of the police ship (as 3d model).
 * \param radius The size of the police ship.
 * \param point The start position of the police ship.
 * \param player The player.
 * \param maxSpeed The maximum speed of the new police ship.
 */
PoliceShip * World::createNewPoliceShip( ObjectName objectType, float radius, std::vector<Vector3> point, PlayerObject * player, float maxSpeed ) {
  PoliceShip *ship = new PoliceShip( radius, Objects::getObjects()->getDisplayList( objectType ), point, player, maxSpeed );
  ship->setView( FileAccess::getObjectView( objectType ) );

 GameObjects.push_back( ship );

  return ship;
}

//! Checks if ships have hit each other and deletes them if necessary.
/**
 * The list of objects is iterated by two iterators starting at the position of the station. The second
 * iterator always starts one position on as the first. This procedure avoids testing a pair of ships two 
 * times. For each pair the distance between the centers is calculated and it is checked if they hit ech
 * other using the collision radius. If that happens, both ships are deleted from the list. As the deleting
 * invalidates the iterators a iterator which leads to last previous ship in the list is saved. If one of
 * the two ships (in fact, it could ouly be the first one) is the coriolis station, it is not deleted because
 * it is indestructible.
 */
void World::deleteCrashedShips( void ) {
  list<GameObject*>::iterator iti = stationPosition;
  list<GameObject*>::iterator itj = stationPosition;

  if( itj == GameObjects.end() )
    return;
  //itj++;

  GraphicsObject* goi;
  GraphicsObject* goj;
  while( iti != GameObjects.end() ) {
    itj = iti;
    itj++;
    while( itj != GameObjects.end() ) {
      // check if iti and itj have a collision
      goi = dynamic_cast<GraphicsObject*>(*iti);
      goj = dynamic_cast<GraphicsObject*>(*itj);

      Vector3 distVector = goi->getPos() - goj->getPos();
      float dist = distVector.length();

      if( dist - goi->getCollisionRadius() - goj->getCollisionRadius() <= 0 ) {
        // collision occured
        list<GameObject*>::iterator pred = iti;
        pred--;
        // check for coriolis
        SpaceStation* st = getStation();
        if( *iti != st )
          GameObjects.erase( iti );
        else
          pred = iti;
        //if( *itj != st )  // this can't happen if the station is always in front of the other ships
        //  Objects::getObjects().erase( itj );
        //else
        //  pred = iti;
        // delete both objects if
          GameObjects.erase( itj );
        
        iti = pred;
        itj = pred;
        //if( itj != Objects::getObjects().end() )
          itj++;
      } else {
        itj++;
      }
    }
    iti++;
  }
}

//! Checks if ships were distroyed and deletes the objects if necessary.
/**
 * The list of objects is iterated by an iterator. The objects are casted to ships and it is checked
 * if they are destroyed.
 */
void World::deleteDestroyedShips( void ) {
  list<GameObject*>::iterator it = stationPosition;

  if( it ==GameObjects.end() )
    return;
  it++;

  SpaceShip* go;
  while( it != GameObjects.end() ) {
    go = dynamic_cast<SpaceShip*>(*it);
    if( go->isDestroyed() ) {
      list<GameObject*>::iterator pred = it;
      pred--;
      GameObjects.erase( it );
      it = pred;
    }
    it++;
  }
}
//@}

/** @name Construction
 * The constructor and destructor for a singleton object. The function World::getWorld() is used to access
 * the class.
 */
//@{
//! Pointer to singleton object.
/**
 * Static pointer to the singleton instance of this class. Only changes in 
 * the function getWorld().
 */
World *World::m_world = 0;

//! Public access to singleton object.
/**
 * The public access for the singleton object. If accessed the first time a
 * new instance is created, otherwise the old instance is returned. 
 */
World *World::getWorld ( void ) {
  if( m_world == 0 )
    m_world = new World;
  return m_world;
}

//! Standard constructor.
/**
 * The constructor creates all needed information that will ever be asked for.
 * Basically it creates an instance of the wrapper, initializes the seed so
 * that the randomly created numbers are the same in every run and created the
 * look for all 256 systems. For each system the textures, the size and number
 * of planets is created. At least the player status is initialized.
 */
World::World() {

  wrapper = new Wrapper( &error1, &error2 );

  srand(351);
  for( unsigned int i=0; i < 256; i++ )
    systemPlanets[i] = getSystemPlanetsR();

  systemStars.reserve( 256 );
  for( unsigned int i=0; i < 256; i++ )
    systemStars.push_back( this->getStarTextureR() );

  m_playerStatus = new PlayerStatus();
  m_playerStatus->fuel = wrapper->getFuelAmount();

  for (int i = 0; i < 256; i++ )
  {
	  vector<PlanetInfoGraphical> planets = *getSystemPlanets(i);	  
	  unsigned possiblePlanets = min((float)planets.size(),5.0f);
	  unsigned coriolisPlanetNr = (rand()%possiblePlanets);
	  nrForCoriolis[i] = coriolisPlanetNr;
  }

}

//! Destructor
/**
 * Clean up all created objects, that means all system data and the wrapper
 * and player data created in the constructor are destroyed. Also all ship 
 * information is destroyed.
 */
World::~World( void ) {
  // delete ship info
  // missing cuz ship-handling is not very well now

  delete m_playerStatus;

  systemStars.clear();

  for( unsigned int i=0; i < 256; i++ ) {
    systemPlanets[i]->clear();
    delete systemPlanets[i];
  }

  delete wrapper;
}
//@}

/** @name System information
 * In this group all needed information for a system is created or fetched from the Wrapper class. The
 * graphical status is created here, such as the used textures. The functions are called from World::initialize().
 * The other methods mainly call their pendant in the wrapper object.
 */
//@{
//! Gets a texture name to a planet texture number
/**
 * Returns the TextureName of a number. To work correctly, the numbers have to
 * be in the same order as the textures are read from the hard disk to the 
 * texture names array in FileAccess. If the number is to high so that no corresponding texture name exists
 * a default texture is returned.
 * \param number a number
 * \return TextureName for the number
 */
TextureName World::getPlanetTexture( unsigned __int8 number ) {
  switch( number ) {
case 0:
  return planet_ss_earth;
  break;
case 1:
  return planet_ss_jupiter;
  break;
case 2:
  return planet_ss_mars;
  break;
case 3:
  return planet_ss_mercury;
  break;
case 4:
  return planet_ss_moon;
  break;
case 5:
  return planet_ss_neptune;
  break;
case 6:
  return planet_ss_pluto;
  break;
case 7:
  return planet_ss_saturn;
  break;
case 8:
  return planet_ss_uranus;
  break;
case 9:
  return planet_ss_venus;
  break;
case 10:
  return planet_red;
  break;
case 11:
  return planet_01;
  break;
case 12:
  return planet_02;
  break;
case 13:
  return planet_03;
  break;
case 14:
  return planet_04;
  break;
case 15:
  return planet_05;
  break;
case 16:
  return planet_06;
  break;
case 17:
  return planet_07;
  break;
case 18:
  return planet_08;
  break;
case 19:
  return planet_09;
  break;
  }
  return planet_ss_earth; // should not happen
}

//! Get a random texture for a planet.
/**
 * Creates a random texture from all possible planet textures. To get the
 * actual TextureName the getPlanetTexture function is used. Betweed two calls
 * with reset set to true no double textures are created. The already created
 * textures are saved in a vector. This function will never stop if all
 * possible textures are already created.
 * \param reset determines if the double counter has to be resetted
 * \return the texturename for the chosen texture
 */
TextureName World::getPlanetTextureR( bool reset ) {
  static vector<TextureName> usedTextures;
  if( reset )
    usedTextures.clear();

  unsigned __int8 num;
  bool newTexture;
  do {
    newTexture = false;
    num = rand()%20;
    for( unsigned int i=0; i < usedTextures.size(); i++ )
      if( usedTextures[i] == getPlanetTexture( num ) )
        newTexture = true;
  } while ( newTexture );

  usedTextures.push_back( getPlanetTexture( num ) );
  return getPlanetTexture( num );
}

//! Gets a texture name for a sun texture.
/**
 * Returns the TextureName of a number. To work correctly, the numbers have to
 * be in the same order as the textures are read from the hard disk to the 
 * texture names array in FileAccess.
 * \param number the number of the texture
 * \return the TextureName for the texture
 */
TextureName World::getStarTexture( unsigned __int8 number ) {
  switch( number ) {
case 0:
  return star_sun;
  break;
case 1:
  return star_001;
  break;
case 2:
  return star_002;
  break;
case 3:
  return star_003;
  break;
case 4:
  return star_004;
  break;
case 5:
  return star_005;
  break;
case 6:
  return star_006;
  break;
case 7:
  return star_010;
  break;
case 8:
  return star_011;
  break;
case 9:
  return star_012;
  break;
case 10:
  return star_013;
  break;
case 11:
  return star_014;
  break;
  }
  return star_sun; // should not happen
}

//! Choses randomly a texture for a star.
/**
 * Creates a random texture from all possible star textures. To get the actual
 * TextureName the getStarTexture function is used. 
 * \return the TextureName of the chosen texture
 */
TextureName World::getStarTextureR( ) {
  unsigned __int8 num = rand()%11;
  return getStarTexture( num );
}

//! Returns the system information of the current system. 
/**
 * The information is taken from the Wrapper::getCurrentSystem() function.
 * \return a SystemInfo object containing the information
 */
SystemInfo World::getSystemInfo() {
  return getSystemInfo( wrapper->getCurrentSystem() );
}

//! Returns system information to a given system.
/**
 * The information are taken from Textelite via the Wrapper.
 * \param number internal number of the system which is asked for
 * \return a SystemInfo object containing the information
 */
SystemInfo World::getSystemInfo( unsigned __int8 number ) {
  // TODO: size is not correct, number of planets shall be calculated and
  //graphical information can be submitted directly?
  SystemInfo sysinfo;

  System system = wrapper->getSystemInfo( number );

  sysinfo.name = system.name;
  sysinfo.pos.x = system.posx;
  sysinfo.pos.y = system.posy;
  sysinfo.size = 591352  * (4155.0 / 149600);
  sysinfo.government = system.government;
  sysinfo.description = system.description;
  sysinfo.population = system.population;
  sysinfo.productivity = system.productivity;
  sysinfo.radius = system.radius;
  sysinfo.techLevel = system.techLevel;
  sysinfo.economy = system.economy;
  sysinfo.systemNumber = system.systemnumber;
  return sysinfo;
}

//! Returns the name of a given system.
/**
 * The name is taken from the function Wrapper::getSysteminfo( int )
 * \param number internal number of the system whose name is asked for
 */
string World::getSystemName( int number ) {
  return wrapper->getSystemInfo( number ).name;
}

//! Returns a vector of planets for the current system.
/**
 * The delivered information may be used to draw the planets on the screen. 
 * \return a pointer to a vector containing PlanetInfoGraphical objects.
 */
vector<PlanetInfoGraphical>  * World::getSystemPlanets() {
  return systemPlanets[wrapper->getCurrentSystem()];
}

//! Returns a vector of planets for any system.
/**
 * The delivered information may be used to draw the planets on the screen.
 * \param number the system number of the system whose planets are asked for
 * \return a pointer to a vector containing PlanetInfoGraphical objects
 */
vector<PlanetInfoGraphical>  * World::getSystemPlanets( unsigned __int8 number ) {
  return systemPlanets[number];
}

//! Generates randomly the planets for a system.
/**
 * Creates the planets for a system. The property of the planets are chosen
 * randomly. This means the distance to the sun, the radius, the texture and 
 * the rotate speed are set in this way. The distances are very big so a
 * special method for creating random numbers is called. It is assume that the *
 * distances between planets increase if the planets are further away from the *
 * sun but it can happen that some planets do not follow this regular. The 
 * factors are calculated in such way, that they lead to systems looking like
 * this. The rotate speed decreases with the distance of the planets. It is
 * assured that never two planets in the same system would have the same
 * texture.
 */
vector<PlanetInfoGraphical>  * World::getSystemPlanetsR() {
  vector<PlanetInfoGraphical> *solarSystem = new vector<PlanetInfoGraphical>;

  int planetCount = (rand()%5 + 1) + (rand()%5 + 1); // minimal 2 and maximal 10 planets

  int maxDistance;
  int minPlanetPosition = 0;

  // create some factors. the planet which is furthest away from the sun should? have the distance 800000
  float c1 = (float)800000 / pow((float)2, (float) planetCount-1);
  float c2 = (pow((float)2, (float)(planetCount*0.75))  * c1) / pow((float)(planetCount*0.75), (float)2);
  float rotateSpeed = -1;
  bool reset = true; // used to reset the no-double-textures-filter in getPlanetTexturesR function

  for(int i = 0; i < planetCount; i++) {
    PlanetInfoGraphical planet;

    // dist1 and dist2 are two functions that should set the maximal possible distance for planet i.
    // dist1 increases polynomial and dist2 super polynomial. the constants c1 and c2 calculated above
    // should assure that the values of the functions are the same when i is equal to the half of the
    // planets. that means that the distances for the first half of the planet increase approximately
    // polynomial and the distances for the second half of planets increases approximately exponential.
    // because this values will set the maximal distance they are only hints as the distance is calculated
    // randomly and the minimal distance must only increase by 20000!
    int dist1 = c2*(i+1)*(i+1) + 20000;
    int dist2 = pow((float)2, (float)i)  * c1;
    maxDistance = max( dist1, dist2 );

    // only create random number if maximal position is greater than minimal position
    int planetDistance = maxDistance;
    if( minPlanetPosition != 0 && minPlanetPosition < maxDistance )
      planetDistance = getRandNumber( minPlanetPosition, maxDistance );
    planet.distance = planetDistance;

    // calculate rotate speed. the speed of the first planet should be fixed to 1.
    if(rotateSpeed < 0)
      rotateSpeed = 1  * planetDistance;
    planet.rotateSpeed =(rotateSpeed / planetDistance);

    // set up the other information
    planet.radius = 10;
    planet.rotateAngle = rand()%360;
    planet.spinningSpeed = -0.012;
    planet.texture = getPlanetTextureR( reset );
    reset = false;
    solarSystem->push_back( planet );

    // set the minimal position for the next planet. it is increased by 20000 in every step. if the planets
    // shouldn't be very near together, the length of stride should be increased. if the stride is big enough
    // the distance will increase continously.
    minPlanetPosition = planetDistance + 20000;
  }
  return solarSystem;
}

/**
 * Returns a vector of systems. The systems that should be in the list can be
 * chosen in different ways. All systems can be chosen, the systems in the 
 * near range of the current system, the systems that could be reached with
 * full fuel amount and the systems which are reachable with the current fuel
 * amount.
 * \param systemList specifies the systems that should be returned
 */
vector<SystemInfo> *World::getSystems( SystemLists systemList ) {
  // TODO: return all systems and reachable
  vector<SystemInfo> *systems;
  switch( systemList ) {
case allSystems:
  break;
case nearSystems:
  {
    // this creates a list in an square around the reachable systems!
    // find out maximal/minimal x and y positions
    vector<int> *localSystems = wrapper->getLocalSystems();
    Position maxPos, minPos; // positions are unsigned integers in textelite --> 0 is minimal
    maxPos.x = wrapper->getSystemInfo(localSystems->at(0)).posx;
    maxPos.y = wrapper->getSystemInfo(localSystems->at(0)).posy;
    minPos = maxPos;
    for(unsigned int i = 0; i < localSystems->size(); i++) {
      System reachableSystem = wrapper->getSystemInfo(localSystems->at(i));
      if( reachableSystem.posx < minPos.x )
        minPos.x = reachableSystem.posx;
      if( reachableSystem.posy < minPos.y )
        minPos.y = reachableSystem.posy;
      if( reachableSystem.posx > maxPos.x )
        maxPos.x = reachableSystem.posx;
      if( reachableSystem.posy > maxPos.y )
        maxPos.y = reachableSystem.posy;
    }
    localSystems->clear();
    delete localSystems;
    systems = getSystems( minPos.x, maxPos.x, minPos.y, maxPos.y );
  }
  break;
case maximalReachable:
  {
    systems = new vector<SystemInfo>;
    vector<int> *localSystems = wrapper->getLocalSystems();
    for(unsigned int i = 0; i < localSystems->size(); i++) {
      SystemInfo sysinfo;
      System wsystem = wrapper->getSystemInfo(localSystems->at(i));
      sysinfo.name = wsystem.name;
      sysinfo.pos.x = wsystem.posx;
      sysinfo.pos.y = wsystem.posy;
      sysinfo.size = i; // wtf is this?
      sysinfo.government = wsystem.government;
      sysinfo.description = wsystem.description;
      sysinfo.population = wsystem.population;
      sysinfo.productivity = wsystem.productivity;
      sysinfo.radius = wsystem.radius;
      sysinfo.techLevel = wsystem.techLevel;
      sysinfo.economy = wsystem.economy;
      sysinfo.systemNumber = wsystem.systemnumber;
      systems->push_back( sysinfo );
    }
    localSystems->clear();
    delete localSystems;
  }
  break;
case reachable:
  break;
  }

  return systems;
}

/**
 * Returns a vector of systems. All systems whose positions are in a given 
 * range are returned. 
 * \param left specifies the leftmost coordinate
 * \param right specifies the rightmost coordinate 
 * \param bottom specifies the lowest coordinate
 * \param top specifies the highest coordinate
 */
std::vector<SystemInfo> *World::getSystems( int left, int right, int bottom, int top ) {
  vector<SystemInfo> *systems = new vector<SystemInfo>;

  vector<int> *localSystems = wrapper->getSystemsInRectangle( left, right, bottom, top );

  for(unsigned int i = 0; i < localSystems->size(); i++) {
    SystemInfo sysinfo;
    System wsystem = wrapper->getSystemInfo(localSystems->at(i));
    sysinfo.name = wsystem.name;
    sysinfo.pos.x = wsystem.posx;
    sysinfo.pos.y = wsystem.posy;
    sysinfo.size = i; // wtf is this?
    sysinfo.government = wsystem.government;
    sysinfo.description = wsystem.description;
    sysinfo.population = wsystem.population;
    sysinfo.productivity = wsystem.productivity;
    sysinfo.radius = wsystem.radius;
    sysinfo.techLevel = wsystem.techLevel;
    sysinfo.economy = wsystem.economy;
    sysinfo.systemNumber = wsystem.systemnumber;
    systems->push_back( sysinfo );
  }
  localSystems->clear();
  delete localSystems;

  return systems;
}

//! Gets the TextureName for the star of the current system.
/**
 * \return the TextureName for the star of the current system
 */
TextureName World::getSystemStarTexture() {
  return this->systemStars.at( wrapper->getCurrentSystem() );
}

//! Gets the TextureName for the star of any system.
/**
 * \return the TextureName for the star of any system.
 * \param number specifies the system 
 */
TextureName World::getSystemStarTexture( unsigned __int8 number ) {
  return systemStars.at( number );
}

//! Checks if a system is reachable with current fuel.
/**
 * \param systemNumber specifies the system
 */
bool World::isSystemReachable( int systemNumber ) {
  return wrapper->systemIsReachable( systemNumber );
}

//! Checks if a system is in the near range of the current system. 
/**
 * A system near in range is reachable with the _maximum_ amount of fuel.
 * \param systemNumber specifies the system
 */
bool World::isSystemLocal( int systemNumber ) {
  return wrapper->systemIsInLocalRange( systemNumber );
}

//! Perform a jump to another system.
/**
 * Tries to perform a jump to a system without any error checking. If an error
 * occurs, it is sent to the error handling functions.
 * \param number specifies the system 
 */
void World::performJump( int number ) {
  wrapper->performJump( number );
  m_playerStatus->fuel = wrapper->getFuelAmount();
  // new initialization for new level, textures are not loaded here, however
  deinitialize();
  initialize();
}
//@}


/** @name Market system
 * This group contains all method related to the marketplace, buying or selling things. They are to the
 * greater part only translators that call functions of the wrapper.
 */
//@{
//! Buys fuel.
/**
 * This buys a given amount of fuel without any error checking. If an error
 * occurs, it is sent to the error handling functions.
 * \param amount the amount of fuel that is to be bought
 */
void World::buyFuel(float amount) {
  wrapper->buyFuel(amount);
  m_playerStatus->fuel = wrapper->getFuelAmount();
}

//! Gets a list of descriptions for all trade goods.
vector<Tradegood> * World::getAllTradegoodDescriptions() {
  return wrapper->getAllTradegoodDescriptions();
}

//! Gets the size of the cargo bay.
unsigned int World::getCargoBaySize() {
  return wrapper->getCargoBaySize();
}

//! Gets the amount of cash of the player.
float World::getCurrentCash() {
  return wrapper->getCurrentCash();
}

//! Gets a list of trade goods that is currently in the cargo bay of the player's ship.
vector<int> * World::getCurrentCargo() {
  return wrapper->getCurrentCargo();
}

//! Gets the items in the current market place.
vector<MarketplaceItem> * World::getCurrentMarketplace() {
  return wrapper->getCurrentMarketplace();
}

//! Gets the free space in the players ship
unsigned int World::getFreeHoldspace() {
  return wrapper->getFreeHoldspace();
}

//! Gets the number of different trade goods
int World::getNumberOfTradegoods() {
  return wrapper->getNumberOfTradegoods();
}

//! Gets the description for a good.
/**
 * /return the description for a good
 * /param number the number of the trade good 
 */
Tradegood World::getTradegoodDescription(int number) {
  return wrapper->getTradegoodDescription(number);
}

//! Buys an amount of some trade good.
/**
 * \param number the id of the trade good
 * \param amount the amount
 */
string World::performPurchase(int number, int amount) {
  return wrapper->performPurchase(number,amount);
}

//! Sells an amount of some trade good.
/**
 * \param number the id of the trade good
 * \param amount the amount
 */
string World::performSale(int number, int amount) {
  return wrapper->performSale(number,amount);
}

//! Gets the cost of fuel
/**
 * \return the cost of fuel
 */
float World::getFuelCost()
{
  return wrapper->getFuelCost();
}

//! Gets the amount of fuel in the player's ship
/**
 * \return the amount of fuel in the tank
 */
float World::getFuelAmount()
{
  return wrapper->getFuelAmount();
}

//! Gets the maximal amount of fuel the player's ship can hold
/**
 * \return the maximal amount of fuel in the tnak
 */
float World::getMaxFuel()
{
  return wrapper->getMaxFuel();
}

//! Decides whether a trade good is measured in tons.
/**
 * \returns true, if a good is measured in tons
 */
bool World::tradegoodIsInTons(int number)
{
  return wrapper->tradegoodIsInTons(number);
}
//@}


/** @name Player
 * Controls the current player status.
 */
//@{
/**
 * \brief Returns the current player status as an pointer to a PlayerStatus structure.
 * \return a pointer to a PlayerStatus structure
 */
PlayerStatus * World::getPlayerStatus( void ) {
  return m_playerStatus;
}
//@}

/** @name Helper
 * Some additional functions.
 */
//@{
/**
 * \brief Returns the name of an economy given as econTypes.
 * \param economy the economy
 * \return a string describing the economy
 */
string World::getEconomyString( econTypes economy ) {
  switch( economy ) {
case AverageAgri:
  return "Average agricultural";
case AverageInd:
  return "Average industrial";
case MainlyAgri:
  return "Mainly agricultural";
case MainlyInd:
  return "Mainly industrial";
case PoorAgri:
  return "Poor agricultural";
case PoorInd:
  return "Poor industrial";
case RichAgri:
  return "Rich agricultural";
case RichInd:
  return "Rich industrial";
  }
  return "";// does not happen under normal circumstances
}

/**
 * \brief Returns the name of an government given as govTypes.
 * \param government the government
 * \return a string describing the government
 */
string World::getGovernmentString( govTypes government ) {
  switch(government) {
case Anarchy:
  return "Anarchy";
case Communist:
  return "Communism";
case Confederacy:
  return "Confederacy";
case CorporateState:
  return "Corporate State";
case Democracy:
  return "Democracy";
case Dictatorship:
  return "Dictatorship";
case Feudal:
  return "Feudalism";
case MultiGov:
  return "Multiple Governments";
  }
  return ""; // does not happen under normal circumstances
}

//! Generates a random number in a given range.
/**
 * Creates a random number between a min and a max value. Both values can be 
 * unsigned integers thus the created value is much greater than the values
 * created by the rand() function. The range of numbers is divided in smaller
 * parts and one of these parts is randomly chosen. After that the exact
 * value in this part is randomly chosen. To create distributed evenly values
 * for the last part the number can get to big and the calculation starts
 * again. This is not a problem as the random numbers are unindependent. This
 * algorithm may run infinite but has a constant expected running time. If the
 * minimal Value is greater than the maximum, 0 is returned. 
 * \param min the minal value that should be created
 * \param max the maximal value that schould be created 
 * \return a random number in the demanded range
 */
unsigned int World::getRandNumber( unsigned int min, unsigned int max ) {
  if( min > max )
    return 0;

  unsigned int returnVal;

  do {
    unsigned int upperBound = max - min;
    unsigned int factor = upperBound / 32767;
    if( factor == 0)
      factor = 1;
    unsigned int randomFactor = rand()%factor; // creates random number from 0 to factor
    unsigned int randomOffset = rand(); // create offset
    returnVal = (randomFactor  * 32767 + randomOffset) + min; // create value to upperBound and add min to get a value in range
  } while( returnVal > max );

  return returnVal;
}


void error1( string error ) {

}

void error2( string error ) {

}
//@}
