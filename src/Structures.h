/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Structures.h
 * \brief	Declares some structures needed in the game.
 */
#pragma once

#include "GlobalGL.h"
#include <string>
#include "wrappDef.h"
#include "Vector3.h"

struct PlayerStatus {
  PlayerStatus() : fuel(0), cash(0) {}
  double fuel;
  double cash;
};

struct Position { // position on screen, no negaive coordinates
  Position() : x(0), y(0) {}
  unsigned int x;
  unsigned int y;
};

enum CullingLocation {
  outside,
  intersect,
  inside
};

enum CullingPlanet {
  never,// the planet is drawn always
  always, // the real position of the planet is calculated every time it is drawn
  manually, // the real position of the planet has to be calculated manually
  valid // with this value, the culling position is always correct
};

struct SystemInfo {
  SystemInfo() : name(""), government(Anarchy) {}
  std::string name;
  Position pos;
  float size; // defines maximal radius
  govTypes government;
  std::string description; // currentSystem.description
  unsigned int population; // currentSystem.population
  unsigned int productivity;// currentSystem.productivity
  unsigned int radius; // currentSystem.radius
  unsigned int techLevel;// currentSystem.techLevel
  econTypes economy;// currentSystem.economy
  unsigned int systemNumber;// internal number of system
};

enum SystemLists {
  reachable,
  maximalReachable,
  allSystems,
  nearSystems
};

struct FrustumInformation {
  FrustumInformation() : farDist( 100 ),
    nearDist( 0.1),
    farWidth( 0.0 ),
    farHeight( 0.0 ),
    nearWidth( 0.0 ),
    nearHeight( 0.0 ),
    fov( 45.0f ),
    aspect( 800.0 / 600.0 ) {}
  float farDist;
  float nearDist;
  float farWidth;
  float farHeight;
  float nearWidth;
  float nearHeight;
  float fov;
  float aspect;
};

enum Screens {
  systemInfo,
  localSystems,
  currentSystem,
  credits,
  help,
  loadProgram,
  loadLevel,
  marketplace,
  helpEnvironment,
  helpLocal,
  helpInfo,
  helpMarketplace
};

struct consoleCommand {
  char	*command;
  int arrayAdress;
};

/**
 * \brief Quality of textures
 */
enum TextureQuality {
  extraLow,
  low,
  medium,
  high,
  extraHigh
};

/**
 * \brief Names for all used textures in the game.
 */
enum TextureName {
  sun_corona = 0,
  bar_green_red,
  radar_tex,
  decal_sight,
  font_1,
  font_2,
  font_3,
  laser1,
  missile_hud,
  material_dirt,
  material_metal,
  planet_01,
  planet_02,
  planet_03,
  planet_04,
  planet_05,
  planet_06,
  planet_07,
  planet_08,
  planet_09,
  planet_red,
  planet_ss_earth,
  planet_ss_moon,
  planet_ss_jupiter,
  planet_ss_mars,
  planet_ss_mercury,
  planet_ss_neptune,
  planet_ss_pluto,
  planet_ss_saturn,
  planet_ss_uranus,
  planet_ss_venus,
  star_001,
  star_002,
  star_003,
  star_004,
  star_005,
  star_006,
  star_010,
  star_011,
  star_012,
  star_013,
  star_014,
  star_sun,
  universe_1,
  universe_2,
  universe_3,
  universe_4
};

/**
 * \brief Texture slots for the textures in the game. This is the maximum amount of textures used in the game at the same time.
 */
enum TextureSlot {
  planet1 = 0,
  planet2,
  planet3,
  planet4,
  planet5,
  planet6,
  planet7,
  planet8,
  planet9,
  planet10,
  star,
  background,
  corona,
  sight,
  laser,
  font1,
  font2,
  dirt,
  metal,
  bar,
  radar,
  missile_hud_tex,
  rearview
};

enum ObjectName {
  adder = 0,
  anaconda,
  ball,
  coriolis,
  missile,
  thargoid,
  tieDroid
};

struct ObjectInfo {
  ObjectInfo() : filename(""), object(-1), displayList(0), view(0,0,1) {}
  int object;
  std::string filename;
  GLuint displayList;
  Vector3 view;
};

struct PlanetInfoGraphical {
  float radius;
  float distance;
  TextureName texture;
  // bool has ring; // not supported
  float spinningSpeed;
  float rotateAngle;
  float rotateSpeed;
};
