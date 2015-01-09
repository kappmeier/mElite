/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt, Ian Bell
 * \file	txtelite.h
 * \brief Contains structures and definitions of the original TextElite by Ian Bell wrapped into a class.
 */

/*jm* * Within this file all comments starting with /*jm are made by 
Jan-Philipp and Melanie. All other comments are from the 
original file *** */
/*jm* * The original code has been changed like this:
- we splitted it into declarations (txtelite.h) and
program code (txtelite.cpp)
- the code was transformed into a class "TextEliteGame",
so that we can construct a textEliteGame-object and use
it nicely with our other cpp-classes
- all functions that execute user commands (like buy fuel,
jump somewhere, ..) were rewritten and extended with 
more error messages (the original code only provided
unsharp error messages like "Can't buy any fuel" instead
of "tank is full" or "you don't have enough cash").
- all functionality for printing on the screen was deleted
and, if necessary, replaced by returning strings ******************** */

/* txtelite.c1.4  */
/* Textual version of Elite trading (C implementation)  */
/* Converted by Ian Bell from 6502 Elite sources.
Original 6502 Elite by Ian Bell & David Braben.  */


/* ----------------------------------------------------------------------
The nature of basic mechanisms used to generate the Elite socio-economic
universe are now widely known. A competant games programmer should be able to
produce equivalent functionality. A competant hacker should be able to lift 
the exact system from the object code base of official conversions.

This file may be regarded as defining the Classic Elite universe.

It contains a C implementation of the precise 6502 algorithms used in the
original BBC Micro version of Acornsoft Elite together with a parsed textual
command testbed.

Note that this is not the universe of David Braben's 'Frontier' series.


ICGB 13/10/99
iancgbell@email.com
www.ibell.co.uk
----------------------------------------------------------------------  */


/* Note that this program is "quick-hack" text parser-driven version
of Elite with no combat or missions.
 */

/*jm * added for communication with wrapper class and graphic classes ********* */
#include <sstream>
#include <string>
#include <vector>
#include "wrappDef.h"

/*jm***************** * original includes ************************************* */
//#include <stdlib.h>
//#include <cstdio>
//#include <string.h>
//#include <time.h>

#include <math.h>

/*jm***************** * original defines ************************************** */
#define true (-1)
#define false (0)
#define tons (0)

#define maxlen (20) /* Length of strings  */

#define galsize (256)
#define AlienItems (16)
#define lasttrade AlienItems

#define numforLave 7/* Lave is 7th generated planet in galaxy one  */
#define numforZaonce 129
#define numforDiso 147
#define numforRied 46
#define POLITICALLY_CORRECT	0
/* Set to 1 for NES-sanitised trade goods  */
#define nocomms (14)

/*jm***************** * original typedefs ************************************* */
typedef int myboolean;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed short int16;
typedef signed long int32;
typedef unsigned int uint;
typedef int planetnum;

/*jm***************** * original structs * ************************************* */
struct fastseedtype
{ uint8 a,b,c,d;
};/* four byte random number used for planet description  */


struct seedtype
{ uint16 w0;
  uint16 w1;
  uint16 w2;
};/* six byte random number used as seed for planets  */

struct plansys
{ uint x;
  uint y;/* One byte unsigned  */
  uint economy; /* These two are actually only 0-7*/
  uint govtype; 
  uint techlev; /* 0-16 i think  */
  uint population; /* One byte  */
  uint productivity; /* Two byte  */
  uint radius; /* Two byte (not used by game at all)  */
  fastseedtype	goatsoupseed;
  char name[12];
};

struct tradegood
{/* In 6502 version these were:  */
  uint baseprice; /* one byte  */
  int16 gradient; /* five bits plus sign  */
  uint basequant; /* one byte  */
  uint maskbyte;/* one byte  */
  uint units; /* two bits  */
  char name[20];/* longest="Radioactives"  */
};

struct markettype
{	uint quantity[lasttrade+1];
  uint price[lasttrade+1];
};


/*jm****************************************************************************
********************* * Class definition ****************************************
 */
class TextEliteGame{

private:

  /*jm********** * variables and constants ************************************** */

  /*jm******************* * new pointers **************************************** */
  void (*userErrorFnc)(std::string); /*jm pointer to error function for user errors  */
  void (*progErrorFnc)(std::string); /*jm pointer to error function for programmers errors  */

  /*jm********** * original variables ******************************************* */

  plansys galaxy[galsize]; /* Need 0 to galsize-1 inclusive  */
  seedtype seed;
  fastseedtype rnd_seed;
  myboolean nativerand;

  unsigned int lastrand; 

  /* Player workspace  */
  uint shipshold[lasttrade+1];/* Contents of cargo bay  */
  planetnum currentplanet;/* Current planet  */
  uint galaxynum;/* Galaxy number (1-8)  */
  int32 cash;
  uint fuel;
  markettype localmarket;
  uint holdspace;

  /*jm*********************variables changed into static constants ************* */

  static const int fuelcost=2; /* 0.2 CR/Light year  */ 
  static const int maxfuel =70; /* 7.0 LY tank  */
  static const uint16 base0=0x5A4A; 
  static const uint16 base1=0x0248; 
  static const uint16 base2=0xB753;/* Base seed for galaxy 1  */ 

  /*jm***arrays and structures that were originally also filled with data here. **
  values are set in the class constructor ******************************* */
  char unitnames[3][5];

  /* Data for DB's price/availability generation system  */
  /* BaseGrad Base Mask Un Name
  price ient quantit */ 
  tradegood commodities[17];

  /**-Required data for text interface * */
  char tradnames[lasttrade+1][maxlen]; /* Tradegood names used in text commands
                                       Set using commodities array  */

  struct desc_choice {	const char *option[5];};

  /*char commands[nocomms+1][maxlen]; */

  /*jm****************************** * functions ******************************** */

  /*jm**************** * new functions: constructor and destructor ************** */
public: 
  TextEliteGame( void (*vuserErrorFnc)(std::string), void (*vprogErrorFnc)(std::string) );
  ~TextEliteGame( void );

  /*jm**************** * new functions: get functions ************************** */
  int getCurrentSystem();
  std::vector<int> * getLocalSystems();
  std::vector<int> * getSystemsInRectangle(unsigned int left, 
    unsigned int right, unsigned int bottom, unsigned int top);
  System getSystemInformation(int number);

  bool systemIsReachable(int number);
  bool systemIsInLocalRange(int number);

  float getFuel();
  float getMaxFuel();

  std::vector<MarketplaceItem> * getCurrentMarketplace();
  std::vector<int> * getCurrentCargo();

  unsigned int getFreeHoldspace();
  unsigned int getCargoBaySize();

  int32 getCurrentCash();

  int getFuelCost();
  bool tradegoodIsInTons(int number);

  /*jm**************** * new functions: actions ********************************* */
  void performJump(int number);

  std::string performPurchase(uint number, uint amount);
  std::string performSale(uint number, uint amount);
  void buyFuel(float amount);

  /*jm************** * new functions: control functions ************************* */
  void setFuel(uint amount);
  void setCash(signed long amount);
  void setCargoBay(uint amount);

  /*jm**************** * new functions: help functions ************************** */
  void initValues();
  govTypes getEnumOfGovernment(uint gov);
  econTypes getEnumOfEconomy(uint econ);

  /*jm*************** * list of remaining original functions ******************** */
private:
  void mysrand(unsigned int seed);
  int myrand(void);
  char randbyte(void);
  uint mymin(uint a,uint b);
  signed int ftoi(double value);
  void tweakseed(seedtype *s);
  void stripout(char *s,const char c);
  int toupper(char c);
  int tolower(char c);
  markettype genmarket(uint fluct, plansys p);
  plansys makesystem(seedtype *s);
  uint16 rotatel(uint16 x);
  uint16 twist(uint16 x);
  void nextgalaxy(seedtype *s);
  void buildgalaxy(uint galaxynum);
  uint distance(plansys a,plansys b);
  myboolean dogalhyp(char *s);
  int gen_rnd_number (void);
  std::string goat_soup(const char *source,plansys  * psy);

};




