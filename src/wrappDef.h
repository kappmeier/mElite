/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	wrappDef.h
 * \brief contains definition for the communication of
 *  wrapperclass, texteliteclass and the graphic classes
 */
#pragma once

#include <string>

/** Different types of governments  */
enum govTypes {
  Anarchy,
  Feudal,
  MultiGov,
  Dictatorship,
  Communist,
  Confederacy,
  Democracy,
  CorporateState
};

/** Different types of economies  */
enum econTypes {
  RichInd,
  AverageInd,
  PoorInd,
  MainlyInd,
  MainlyAgri,
  RichAgri,
  AverageAgri,
  PoorAgri
};

/** Description of planet systems  */
struct System {
  System() : posx(0), posy(0), government(Anarchy), economy(PoorAgri), 
    techLevel(0), population(0), productivity(0), radius(0),
    systemnumber(257) {} 

  unsigned int systemnumber;
  std::string name;
  unsigned int posx, posy;
  govTypes government;
  econTypes economy;
  unsigned int techLevel;
  unsigned int population; 
  unsigned int productivity; 
  unsigned int radius;
  std::string description;
};

/** Description of a marketplace item  */
struct MarketplaceItem {
  double price;
  unsigned int amount;
};

/** Description of a trade good  */
struct Tradegood {
  std::string name;
  std::string unit;
};
