/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt, Ian Bell
 * \file	txtelite.cpp
 * \brief Contains the original TextElite by Ian Bell converted to a class.
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
jump somewhere, ..) were rewritten andextended with 
more error messages (the original code only provided
unsharp error messages like "Can't buy any fuel" instead
of "tank is full" or "you don't have enough cash").
- all functionality for printing on the screen was deleted
and, if necessary, replaced by returning strings
- all functionality for user input from keyboard was deleted (like the 
parser for strings) and replaced by function calls ****************** */


#include "txtelite.h"

/*jm****************************** * functions ******************************** */ 

/*jm******** * constructor and destructor for new textEliteGame *************** */

TextEliteGame::TextEliteGame( void (*vuserErrorFnc)(std::string), void (*vprogErrorFnc)(std::string) ){ 

  /*jm * some values couldn't be initialized in txtelite.h; this is done here  */
  initValues(); 

  /*jm these functions are for sending error messages to the class that uses 
  the game as an object ************************************************ */
  userErrorFnc = vuserErrorFnc;
  progErrorFnc = vprogErrorFnc;

  /*jm******* * part of the original main-function: starts here ************** */

  /* 6502 Elite fires up at Lave with fluctuation=00
  and these prices tally with the NES ones.
  However, the availabilities reside in the saved game data.
  Availabilities are calculated (and fluctuation randomised)
  on hyperspacing
  I have checked with this code for Zaonce with fluctaution &AB 
  against the SuperVision 6502 code and both prices and availabilities tally.
  */

  uint i;

  nativerand=1;

  for(i=0;i<=lasttrade;i++) strcpy_s(tradnames[i],commodities[i].name);

  mysrand(12345);/* Ensure repeatability  */

  galaxynum=1;	buildgalaxy(galaxynum);

  currentplanet=numforLave;/* Don't use jump  */
  localmarket = genmarket(0x00,galaxy[numforLave]);/* Since want seed=0  */

  fuel=maxfuel;

  setCargoBay(20);/* Small cargo bay  */
  setCash(1000); /* 100 CR  */

  /*jm******* * part of the original main-function: ends here **************** */

}

TextEliteGame::~TextEliteGame( void ){ }

/**
/*jm***************** * new functions: get functions ****************************
 */

/*jm**** * function returns the number of the system we are currently in ****** */
int TextEliteGame::getCurrentSystem()
{
  return currentplanet;
}

/*jm * function returns an int-vector with the numbers of all systems in range  */
std::vector<int> * TextEliteGame::getLocalSystems()
{planetnum syscount;
uint d;
std::vector<int> * myLocalSystems = new std::vector<int>;

for(syscount=0;syscount<galsize;++syscount)
{ 	d=distance(galaxy[syscount],galaxy[currentplanet]);
if(d<=maxfuel)
{ 	
  myLocalSystems->push_back(syscount); 
}
}
return myLocalSystems;
}

/*jm * function returns an int-vector with the numbers 
of all systems the given rectangle  */
std::vector<int> * TextEliteGame::getSystemsInRectangle(unsigned int left, 
                                                       unsigned int right, unsigned int bottom, unsigned int top)
{planetnum syscount;

std::vector<int> * mySystems = new std::vector<int>;

if (left < 0) left = 0;
if (bottom < 0) bottom = 0;
if (right < 0) right = 0;
if (top < 0) top = 0;

for(syscount=0;syscount<galsize;++syscount)
{ 	
  if( (galaxy[syscount].x >= left)
    && (galaxy[syscount].x <= right)
    && (galaxy[syscount].y >= bottom)
    && (galaxy[syscount].y <= top))
  {
    mySystems->push_back(syscount);
  }
}
return mySystems;
}

/*jm**** * function returns information about the system with the given number **
the structure 'system' from wrappDef.h is used ****************** */
System TextEliteGame::getSystemInformation(int number)
{
  System mySystem;
  if (number<0 || number >= galsize) {
    progErrorFnc("The integer in 'number' has to be between 0 and 256! Error occured in getSystemInformation(int number) in txtelite.cpp");
    mySystem.name = "Error! No System";
    mySystem.systemnumber = 0;
    mySystem.posx = 0;
    mySystem.posy = 0;
    mySystem.government = Anarchy;
    mySystem.economy = PoorAgri;
    mySystem.techLevel = 0;
    mySystem.population = 0;
    mySystem.productivity = 0;
    mySystem.radius = 0;
    mySystem.description = "An error occured!";
    return mySystem;
  }
  plansys mySys = galaxy[number];
  mySystem.systemnumber = number;
  mySystem.posx = mySys.x;
  mySystem.posy = mySys.y;
  mySystem.name = mySys.name;
  mySystem.government = getEnumOfGovernment(mySys.govtype);
  mySystem.economy = getEnumOfEconomy(mySys.economy);
  mySystem.techLevel = mySys.techlev;
  mySystem.population = mySys.population;
  mySystem.productivity = mySys.productivity;
  mySystem.radius = mySys.radius;

  rnd_seed = mySys.goatsoupseed;
  mySystem.description = goat_soup("\x8F is \x97.",&mySys);

  return mySystem;
}

/*jm*** * Returns whether the system 'number' is reachable from the current
system with the current fuel ********************************** */
bool TextEliteGame::systemIsReachable(int number)
{
  uint d=distance(galaxy[number],galaxy[currentplanet]);
  return (d <= fuel);
}

/*jm*** * Returns whether the system 'number' is reachable from the current
system if we have maximal amount of fuel **************************** */
bool TextEliteGame::systemIsInLocalRange(int number)
{
  uint d=distance(galaxy[number],galaxy[currentplanet]);
  return (d <= maxfuel);
}

/*jm**** * function returns the actual amount of fuel ************************* */
float TextEliteGame::getFuel()
{
  return (float)(fuel/10);
}

/*jm*** * returns the amount of fuel that fits into the tank ****************** */
float TextEliteGame::getMaxFuel()
{
  return (float)(maxfuel/10);
}


/*jm*** * Gives a list of MarketplaceItems saying how much is available of each
of the 17 items and how much it costs (both regarding the market
in the current system *********************************************** */
std::vector<MarketplaceItem> * TextEliteGame::getCurrentMarketplace()
{
  std::vector<MarketplaceItem> * result = new std::vector<MarketplaceItem>;
  for (int i=0; i < lasttrade + 1; i++)
  {
    MarketplaceItem mi;
    mi.price = (float)(localmarket.price[i])/10;
    mi.amount = localmarket.quantity[i];
    result->push_back(mi);
  }
  return result;
}

/*jm*** * Tells how much we have in our cargo for each of the 17 buyable items. */
std::vector<int> * TextEliteGame::getCurrentCargo()
{
  std::vector<int> * result = new std::vector<int>;
  for (int i=0; i < lasttrade + 1; i++)
  {
    result->push_back(shipshold[i]);
  }
  return result;
}

/*jm*** * Tells how much space is free for storing additional items. Only items 
with unit 't' are taken into account, 'kg' and 'g' make no difference */
unsigned int TextEliteGame::getFreeHoldspace()
{
  return holdspace;
}

/*jm******* * Returns the size of the cargo bay. Value must be calculated ***** */
unsigned int TextEliteGame::getCargoBaySize()
{
  uint t = 0; 
  for(int i=0;i<=lasttrade;++i)
  {	
	if ((commodities[i].units)==tons) t+=shipshold[i];
  }
  return (t+holdspace);
}

/*jm******* * Returns the amount of available cash *************************** */
int32 TextEliteGame::getCurrentCash()
{
  return cash;
}

/*jm******* * Returns the price for one unit of fuel ************************* */
int TextEliteGame::getFuelCost()
{
  return fuelcost;
}

/*jm this function returns whether a certain tradegood has tonnes as unit. *****
* * this is important because only items in tonnes count for the *
* * calculation of the free holdspace. *************************************** */
bool TextEliteGame::tradegoodIsInTons(int number)
{
  return ((commodities[number].units)==tons);
}


/*jm****************************************************************************
******************** * new functions: actions ***********************************
 */

/*jm****************** * Sets the amount of fuel that we have ***************** */
void TextEliteGame::setFuel(uint amount)
{
  if (amount > maxfuel){
    progErrorFnc("'amount' must be between 0.0 and 7.0. Error occured in setFuel in txtelite.cpp");
    return;
  }
  fuel = amount;
}

/*jm****************** * Sets the amount of cash that we have ***************** */
void TextEliteGame::setCash(signed long amount)
{ 
  cash = amount;
}

/*jm************** * Sets the size of the cargo bay that we have ****************
Sends an error if we have to much cargo for the new value  */
void TextEliteGame::setCargoBay(uint amount)
{
  uint t = 0; 
  for(int i=0;i<=lasttrade;++i)
  {	
	if ((commodities[i].units)==tons) t+=shipshold[i];
  }
  if(t>amount) {
    progErrorFnc("The cargo bay cannot be set to this value because there is too much cargo. Error occured in setCargoBay in txtelite.cpp."); 
    return;
  }
  holdspace=amount-t;
  return;
}

/*jm**************** * new functions: actions ********************************* */

/*jm*** * Attempts to jump to the system with number 'number'. ******************
In case of errors the error functions are called ******************** */
void TextEliteGame::performJump(int number)
{
  if (number < 0 || number > galsize)
  { 
	progErrorFnc("The integer in 'number' has to be between 0 and 256! Error occured in performJump(int number) in txtelite.cpp"); return; };

  uint d=distance(galaxy[number],galaxy[currentplanet]);

  if (d>maxfuel) 
  { 
	userErrorFnc("Jump too far (system not in range, even with full fuel)"); return; };

  if (d > fuel)
  { 
	userErrorFnc("Jump too far (system in range, but not enough fuel)"); return; };

  if (number == currentplanet)
  { userErrorFnc("Already there!"); return; };

  fuel-=d;

  currentplanet=number;
  localmarket = genmarket(randbyte(),galaxy[number]); 

  /*gamejump(number); */
  std::string report = "Jumped to ";
  report += galaxy[number].name;
  report += ".";
  userErrorFnc(report);
}

/*jm*** * Attempts to buy 'amount' items of type 'number'. There are a lot of ***
error texts that are returned if the player cannot buy what he wants  */
std::string TextEliteGame::performPurchase(uint number, uint amount)
{
  uint t = amount;
  std::string errorreport;

  if (number >= lasttrade+1) {
    progErrorFnc("'number' is too high. This error occured in the function performPurchase in the file txtelite.cpp");
    errorreport = "";
    return errorreport;
  }

  errorreport = "Nothing was bought";

  if (amount == 0) return errorreport;
  if (cash <= 0){ 
    userErrorFnc("You don't have any cash.");
    return errorreport;
  }
  t = mymin((uint)floor((double)cash/(localmarket.price[number])),t);
  if (t == 0) {
    std::string errortext = "You don't have enough cash to buy anything.";
    userErrorFnc(errortext);
    return errorreport;
  }
  if (localmarket.quantity[number] <= 0){
    userErrorFnc("The market does not sell this at the moment.");
    return errorreport;
  }
  t = mymin(localmarket.quantity[number],t);
  if ((commodities[number].units)==tons){
    if (holdspace == 0){
      userErrorFnc("Your cargo bay is full! You can only buy items that weigh less than a ton.");
      return errorreport;
    }
    t = mymin(holdspace,t); 
  }
  shipshold[number]+=t; 
  localmarket.quantity[number]-=t;
  cash-=t*(localmarket.price[number]);
  if ((commodities[number].units)==tons) {holdspace-=t;}

  std::stringstream freport;
  std::string sreport;
  freport << (float)t;
  freport >> sreport;
  std::string report="Bought ";
  report += sreport;
  report += unitnames[commodities[number].units];
  report += " of ";
  report += tradnames[number];
  return report;
}

/*jm* * Attempts to sell 'amount' items of type 'number'. This does work if
the user has enough items to sell, otherwise less or nothing is bought  */
std::string TextEliteGame::performSale(uint number, uint amount)
{
  uint t = amount;

  if (number >= lasttrade+1) {
    progErrorFnc("'number' is too high. This error occured in the function performPurchase in the file txtelite.cpp");
    return "";
  }

  if (shipshold[number] == 0) {
    userErrorFnc("You don't have any of these.");
    return "Nothing was selled";
  }
  shipshold[number]-=t;
  localmarket.quantity[number]+=t;
  if ((commodities[number].units)==tons) {holdspace+=t;}
  cash+=t*(localmarket.price[number]);

  std::stringstream freport;
  std::string sreport;
  freport << (float)t;
  freport >> sreport;
  std::string report="Sold ";
  report += sreport;
  report += unitnames[commodities[number].units];
  report += " of ";
  report += tradnames[number];
  return report;
}

/*jm*** * Attempts to buy 'amount' fuel. When this is not possible, the 
corrisponding error message is returned. **************************** */
void TextEliteGame::buyFuel(float amount)
{
  if (fuel == maxfuel)
    userErrorFnc("Can't buy any fuel (Tank is already full).");
  else
  {
    uint f = (uint)floor(10*amount);
    if (f+fuel>maxfuel)
    {
      userErrorFnc("Buying less fuel (otherwise it doesn't fit into the tank). ");
      f = maxfuel - fuel;
    }
    if (fuelcost>0)
    {
      if ((int)f*fuelcost>cash) 
      {
        f = (uint)(cash/fuelcost);
        if (f >0)
          userErrorFnc("Buying less fuel (not enough cash available to buy whole amount). ");
        else 
          userErrorFnc("Can't buy any fuel (Not enough cash).");
      } 
    }
    if (f > 0)
    {
      fuel+=f;
      cash-=fuelcost*f;
      std::stringstream freport;
      std::string sreport;
      freport << (float)f/10;
      freport >> sreport;
      std::string report="Bought ";
      report += sreport;
      report += " LY fuel.";
      userErrorFnc(report);
    }
  }
}

/*jm****************************************************************************
**************** * new functions: help functions ********************************
 */

/*jm**** * function to set values that were previously preinitialized ***********
******* * (initialization commands don't work within class definition) ******** */
void TextEliteGame::initValues(){
  lastrand = 0;

  strcpy_s(unitnames[0],"t");
  strcpy_s(unitnames[1],"kg");
  strcpy_s(unitnames[2],"g");

  commodities[0].baseprice=0x13;
  commodities[0].gradient=-0x02;
  commodities[0].basequant=0x06;
  commodities[0].maskbyte=0x01;
  commodities[0].units=0;
  strcpy_s(commodities[0].name,"Food "); 

  commodities[1].baseprice=0x14;
  commodities[1].gradient=-0x01;
  commodities[1].basequant=0x0A;
  commodities[1].maskbyte=0x03;
  commodities[1].units=0;
  strcpy_s(commodities[1].name,"Textiles"); 

  commodities[2].baseprice=0x41;
  commodities[2].gradient=-0x03;
  commodities[2].basequant=0x02;
  commodities[2].maskbyte=0x07;
  commodities[2].units=0;
  strcpy_s(commodities[2].name,"Radioactives"); 

  commodities[3].baseprice=0x28;
  commodities[3].gradient=-0x05;
  commodities[3].basequant=0xE2;
  commodities[3].maskbyte=0x1F;
  commodities[3].units=0;
#if POLITICALLY_CORRECT 
  strcpy_s(commodities[3].name,"Robot Slaves"); 
#else
  strcpy_s(commodities[3].name,"Slaves"); 
#endif 

  commodities[4].baseprice=0x53;
  commodities[4].gradient=-0x05;
  commodities[4].basequant=0xFB;
  commodities[4].maskbyte=0x0F;
  commodities[4].units=0;
#if POLITICALLY_CORRECT 
  strcpy_s(commodities[4].name,"Beverages "); 
#else
  strcpy_s(commodities[4].name,"Liquor/Wines"); 
#endif 

  commodities[5].baseprice=0xC4;
  commodities[5].gradient=+0x08;
  commodities[5].basequant=0x36;
  commodities[5].maskbyte=0x03;
  commodities[5].units=0;
  strcpy_s(commodities[5].name,"Luxuries"); 

  commodities[6].baseprice=0xEB;
  commodities[6].gradient=+0x1D;
  commodities[6].basequant=0x08;
  commodities[6].maskbyte=0x78;
  commodities[6].units=0;
#if POLITICALLY_CORRECT
  strcpy_s(commodities[6].name,"Rare Species");
#else 
  strcpy_s(commodities[6].name,"Narcotics ");
#endif 

  commodities[7].baseprice=0x9A;
  commodities[7].gradient=+0x0E;
  commodities[7].basequant=0x38;
  commodities[7].maskbyte=0x03;
  commodities[7].units=0;
  strcpy_s(commodities[7].name,"Computers "); 

  commodities[8].baseprice=0x75;
  commodities[8].gradient=+0x06;
  commodities[8].basequant=0x28;
  commodities[8].maskbyte=0x07;
  commodities[8].units=0;
  strcpy_s(commodities[8].name,"Machinery "); 

  commodities[9].baseprice=0x4E;
  commodities[9].gradient=+0x01;
  commodities[9].basequant=0x11;
  commodities[9].maskbyte=0x1F;
  commodities[9].units=0;
  strcpy_s(commodities[9].name,"Alloys"); 

  commodities[10].baseprice=0x7C;
  commodities[10].gradient=+0x0d;
  commodities[10].basequant=0x1D;
  commodities[10].maskbyte=0x07;
  commodities[10].units=0;
  strcpy_s(commodities[10].name,"Firearms"); 

  commodities[11].baseprice=0xB0;
  commodities[11].gradient=-0x09;
  commodities[11].basequant=0xDC;
  commodities[11].maskbyte=0x3F;
  commodities[11].units=0;
  strcpy_s(commodities[11].name,"Furs "); 

  commodities[12].baseprice=0x20;
  commodities[12].gradient=-0x01;
  commodities[12].basequant=0x35;
  commodities[12].maskbyte=0x03;
  commodities[12].units=0;
  strcpy_s(commodities[12].name,"Minerals"); 

  commodities[13].baseprice=0x61;
  commodities[13].gradient=-0x01;
  commodities[13].basequant=0x42;
  commodities[13].maskbyte=0x07;
  commodities[13].units=1;
  strcpy_s(commodities[13].name,"Gold "); 

  commodities[14].baseprice=0xAB;
  commodities[14].gradient=-0x02;
  commodities[14].basequant=0x37;
  commodities[14].maskbyte=0x1F;
  commodities[14].units=1;
  strcpy_s(commodities[14].name,"Platinum"); 

  commodities[15].baseprice=0x2D;
  commodities[15].gradient=-0x01;
  commodities[15].basequant=0xFA;
  commodities[15].maskbyte=0x0F;
  commodities[15].units=2;
  strcpy_s(commodities[15].name,"Gem-Strones "); 

  commodities[16].baseprice=0x35;
  commodities[16].gradient=+0x0F;
  commodities[16].basequant=0xC0;
  commodities[16].maskbyte=0x07;
  commodities[16].units=0;
  strcpy_s(commodities[16].name,"Alien Items "); 

  for (int i = 0; i < lasttrade+1; i++) { shipshold[i] = 0; }

  cash=0;
} 

/*jm***** * Converts an uint governmentnumber into the appropriate enum value * */
govTypes TextEliteGame::getEnumOfGovernment(uint gov)
{
  switch (gov) {
case 0: return Anarchy;
case 1: return Feudal;
case 2: return MultiGov;
case 3: return Dictatorship;
case 4: return Communist;
case 5: return Confederacy;
case 6: return Democracy;
case 7: return CorporateState;
default: return Anarchy;
  }
}

/*jm****** * Converts an uint economynumber into the appropriate enum value *** */
econTypes TextEliteGame::getEnumOfEconomy(uint econ)
{
  switch(econ) {
 case 0: return RichInd;
 case 1: return AverageInd;
 case 2: return PoorInd;
 case 3: return MainlyInd;
 case 4: return MainlyAgri;
 case 5: return RichAgri;
 case 6: return AverageAgri;
 case 7: return PoorAgri;
 default: return PoorAgri;
  }
} 

/*jm****************************************************************************
/*************************** * original functions *******************************
 */

/*jm************ * functions for random generation (from original code) ******* */

/**- General functions * */

void TextEliteGame::mysrand(unsigned int seed)
{	srand(seed);
lastrand = seed - 1;
}

int TextEliteGame::myrand(void)
{	int r;
if(nativerand) r=rand();
else
{	// As supplied by D McDonnell	from SAS Insititute C
  r = (((((((((((lastrand << 3) - lastrand) << 3)
    + lastrand) << 1) + lastrand) << 4)
    - lastrand) << 1) - lastrand) + 0xe60)
    & 0x7fffffff;
  lastrand = r - 1;	
}
return(r);
}

char TextEliteGame::randbyte(void)	
{ 
  return (char)(myrand()&0xFF);
}

/*jm******************* * help functions (original code) ********************** */

uint TextEliteGame::mymin(uint a,uint b) 
{
  if(a<b) return(a);
  else return(b);
}

/**+ftoi * */
signed int TextEliteGame::ftoi(double value)
{ return ((signed int)floor(value+0.5));
}

void TextEliteGame::tweakseed(seedtype *s)
{ uint16 temp;
temp = ((*s).w0)+((*s).w1)+((*s).w2); /* 2 byte aritmetic  */
(*s).w0 = (*s).w1;
(*s).w1 = (*s).w2;
(*s).w2 = temp;
}

/*jm*************** * string functions (original code) ************************ */
/**-String functions for text interface * */

void TextEliteGame::stripout(char *s,const char c) /* Remove all c's from string s  */
{ size_t i=0,j=0;
while(i<strlen(s))
{ if(s[i]!=c) { s[j]=s[i]; j++;}
i++;
}
s[j]=0;
}

int TextEliteGame::toupper(char c)
{	if((c>='a')&&(c<='z')) return(c+'A'-'a');
return((int)c);
}

int TextEliteGame::tolower(char c)
{	if((c>='A')&&(c<='Z')) return(c+'a'-'A');
return((int)c);
}



markettype TextEliteGame::genmarket(uint fluct, plansys p)
/* Prices and availabilities are influenced by the planet's economy type
(0-7) and a random "fluctuation" byte that was kept within the saved
commander position to keep the market prices constant over gamesaves.
Availabilities must be saved with the game since the player alters them
by buying (and selling(?))

Almost all operations are one byte only and overflow "errors" are
extremely frequent and exploited.

Trade Item prices are held internally in a single byte=true value/4.
The decimal point in prices is introduced only when printing them.
Internally, all prices are integers.
The player's cash is held in four bytes. 
 */

{	markettype market;
unsigned short i;
for(i=0;i<=lasttrade;i++)
{	signed int q; 
signed int product = (p.economy)*(commodities[i].gradient);
signed int changing = fluct & (commodities[i].maskbyte);
q =(commodities[i].basequant) + changing - product;	
q = q&0xFF;
if(q&0x80) {q=0;};/* Clip to positive 8-bit  */

market.quantity[i] = (uint16)(q & 0x3F); /* Mask to 6 bits  */

q =(commodities[i].baseprice) + changing + product;
q = q & 0xFF;
market.price[i] = (uint16) (q*4);
}
market.quantity[AlienItems] = 0; /* Override to force nonavailability  */
return market;
}

/**-Generate system info from seed * */

plansys TextEliteGame::makesystem(seedtype *s)
{	

  char pairs[] = "..lexegezacebiso"// LEXEGEZACEBISO"
    "usesarmaindirea."// USESARMAINDIREA."
    "eratenberalaveti"// ERATENBERALAVETI"
    "edorquanteisrion"; // EDORQUANTEISRION"; /* Dots should be nullprint characters  */

  plansys thissys;
  uint pair1,pair2,pair3,pair4;
  uint16 longnameflag=((*s).w0)&64;

  thissys.x=(((*s).w1)>>8);
  thissys.y=(((*s).w0)>>8);

  thissys.govtype =((((*s).w1)>>3)&7); /* bits 3,4 &5 of w1  */

  thissys.economy =((((*s).w0)>>8)&7); /* bits 8,9 &A of w0  */
  if (thissys.govtype <=1)
  { thissys.economy = ((thissys.economy)|2);
  } 

  thissys.techlev =((((*s).w1)>>8)&3)+((thissys.economy)^7);
  thissys.techlev +=((thissys.govtype)>>1);
  if (((thissys.govtype)&1)==1)	thissys.techlev+=1;
  /* C simulation of 6502's LSR then ADC  */

  thissys.population = 4*(thissys.techlev) + (thissys.economy);
  thissys.population +=(thissys.govtype) + 1;

  thissys.productivity = (((thissys.economy)^7)+3)*((thissys.govtype)+4);
  thissys.productivity *= (thissys.population)*8;

  thissys.radius = 256*(((((*s).w2)>>8)&15)+11) + thissys.x;

  thissys.goatsoupseed.a = (*s).w1 & 0xFF;;
  thissys.goatsoupseed.b = (*s).w1 >>8;
  thissys.goatsoupseed.c = (*s).w2 & 0xFF;
  thissys.goatsoupseed.d = (*s).w2 >> 8;

  pair1=2*((((*s).w2)>>8)&31);tweakseed(s);
  pair2=2*((((*s).w2)>>8)&31);tweakseed(s);
  pair3=2*((((*s).w2)>>8)&31);tweakseed(s);
  pair4=2*((((*s).w2)>>8)&31);	tweakseed(s);
  /* Always four iterations of random number  */

  (thissys.name)[0]=pairs[pair1]; 
  (thissys.name)[1]=pairs[pair1+1];/*jm we changed the planet  */
  (thissys.name)[2]=pairs[pair2]; /* names to lowercase*/
  (thissys.name)[3]=pairs[pair2+1];
  (thissys.name)[4]=pairs[pair3];
  (thissys.name)[5]=pairs[pair3+1];

                                  if(longnameflag) /* bit 6 of ORIGINAL w0 flags a four-pair name  */
  {
    (thissys.name)[6]=pairs[pair4];
    (thissys.name)[7]=pairs[pair4+1];
    (thissys.name)[8]=0;
  }
  else (thissys.name)[6]=0;
  stripout(thissys.name,'.');

  if (thissys.name[0] != 0) 
    thissys.name[0] = toupper(thissys.name[0]); 

  return thissys;
}


/**+Generate galaxy * */


/* Functions for galactic hyperspace  */

uint16 TextEliteGame::rotatel(uint16 x) /* rotate 8 bit number leftwards  */
/* (tried to use chars but too much effort persuading this braindead
language to do bit operations on bytes!)  */
{ uint16 temp = x&128;
return (2*(x&127))+(temp>>7);
} 

uint16 TextEliteGame::twist(uint16 x)
{ return (uint16)((256*rotatel(x>>8))+rotatel(x&255));
} 

void TextEliteGame::nextgalaxy(seedtype *s) /* Apply to base seed; once for galaxy 2*/
{ (*s).w0 = twist((*s).w0);/* twice for galaxy 3, etc.  */
  (*s).w1 = twist((*s).w1);/* Eighth application gives galaxy 1 again */
  (*s).w2 = twist((*s).w2);
}

/* Original game generated from scratch each time info needed  */
void TextEliteGame::buildgalaxy(uint galaxynum)
{	uint syscount,galcount;
  seed.w0=base0; seed.w1=base1; seed.w2=base2; /* Initialise seed for galaxy 1  */
  for(galcount=1;galcount<galaxynum;++galcount) nextgalaxy(&seed);
  /* Put galaxy data into array of structures  */
  for(syscount=0;syscount<galsize;++syscount) galaxy[syscount]=makesystem(&seed);
}

/**-Functions for navigation * */


uint TextEliteGame::distance(plansys a,plansys b)
/* Seperation between two planets (4*sqrt(X*X+Y*Y/4))  */
{	return (uint)ftoi(4*sqrt((float)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)/4)));
}

myboolean TextEliteGame::dogalhyp(char *s) /* Jump to next galaxy  */
/* Preserve planetnum (eg. if leave 7th planet
arrive at 7th planet)  */
{	(void)(&s);/* Discard s  */
galaxynum++;
if(galaxynum==9) {galaxynum=1;}
buildgalaxy(galaxynum);
return true;
}

/* "Goat Soup" planetary description string code - adapted from Christian Pinder's
reverse engineered sources.  */

/* B0 = <planet name>
B1 = <planet name>ian
B2 = <random name>
 */

int TextEliteGame::gen_rnd_number (void)
{	int a,x;
x = (rnd_seed.a  * 2) & 0xFF;
a = x + rnd_seed.c;
if (rnd_seed.a > 127)	a++;
rnd_seed.a = a & 0xFF;
rnd_seed.c = x;

a = a / 256;	/* a = any carry left from above  */
x = rnd_seed.b;
a = (a + x + rnd_seed.d) & 0xFF;
rnd_seed.b = a;
rnd_seed.d = x;
return a;
}

std::string TextEliteGame::goat_soup(const char *source,plansys  * psy)
{
  static struct desc_choice desc_list[] =
  {
    /* 81  */	{"fabled", "notable", "well known", "famous", "noted"},
    /* 82  */	{"very", "mildly", "most", "reasonably", ""},
    /* 83  */	{"ancient", "\x95", "great", "vast", "pink"},
    /* 84  */	{"\x9E \x9D plantations", "mountains", "\x9C", "\x94 forests", "oceans"},
    /* 85  */	{"shyness", "silliness", "mating traditions", "loathing of \x86", "love for \x86"},
    /* 86  */	{"food blenders", "tourists", "poetry", "discos", "\x8E"},
    /* 87  */	{"talking tree", "crab", "bat", "lobst", "\xB2"},
    /* 88  */	{"beset", "plagued", "ravaged", "cursed", "scourged"},
    /* 89  */	{"\x96 civil war", "\x9B \x98 \x99s", "a \x9B disease", "\x96 earthquakes", "\x96 solar activity"},
    /* 8A  */	{"its \x83 \x84", "the \xB1 \x98 \x99","its inhabitants' \x9A \x85", "\xA1", "its \x8D \x8E"},
    /* 8B  */	{"juice", "brandy", "water", "brew", "gargle blasters"},
    /* 8C  */	{"\xB2", "\xB1 \x99", "\xB1 \xB2", "\xB1 \x9B", "\x9B \xB2"},
    /* 8D  */	{"fabulous", "exotic", "hoopy", "unusual", "exciting"},
    /* 8E  */	{"cuisine", "night life", "casinos", "sit coms", " \xA1 "},
    /* 8F  */	{"\xB0", "The planet \xB0", "The world \xB0", "This planet", "This world"},
    /* 90  */	{"n unremarkable", " boring", " dull", " tedious", " revolting"},
    /* 91  */	{"planet", "world", "place", "little planet", "dump"},
    /* 92  */	{"wasp", "moth", "grub", "ant", "\xB2"},
    /* 93  */	{"poet", "arts graduate", "yak", "snail", "slug"},
    /* 94  */	{"tropical", "dense", "rain", "impenetrable", "exuberant"},
    /* 95  */	{"funny", "wierd", "unusual", "strange", "peculiar"},
    /* 96  */	{"frequent", "occasional", "unpredictable", "dreadful", "deadly"},
    /* 97  */	{"\x82 \x81 for \x8A", "\x82 \x81 for \x8A and \x8A", "\x88 by \x89", "\x82 \x81 for \x8A but \x88 by \x89","a\x90 \x91"},
    /* 98  */	{"\x9B", "mountain", "edible", "tree", "spotted"},
    /* 99  */	{"\x9F", "\xA0", "\x87oid", "\x93", "\x92"},
    /* 9A  */	{"ancient", "exceptional", "eccentric", "ingrained", "\x95"},
    /* 9B  */	{"killer", "deadly", "evil", "lethal", "vicious"},
    /* 9C  */	{"parking meters", "dust clouds", "ice bergs", "rock formations", "volcanoes"},
    /* 9D  */	{"plant", "tulip", "banana", "corn", "\xB2weed"},
    /* 9E  */	{"\xB2", "\xB1 \xB2", "\xB1 \x9B", "inhabitant", "\xB1 \xB2"},
    /* 9F  */	{"shrew", "beast", "bison", "snake", "wolf"},
    /* A0  */	{"leopard", "cat", "monkey", "goat", "fish"},
    /* A1  */	{"\x8C \x8B", "\xB1 \x9F \xA2","its \x8D \xA0 \xA2", "\xA3 \xA4", "\x8C \x8B"},
    /* A2  */	{"meat", "cutlet", "steak", "burgers", "soup"},
    /* A3  */	{"ice", "mud", "Zero-G", "vacuum", "\xB1 ultra"},
    /* A4  */	{"hockey", "cricket", "karate", "polo", "tennis"}
  };
  char pairs0[]="ABOUSEITILETSTONLONUTHNO";

  std::string result = ""; /**jm added to get output into a variable instead of using printf  */

  /* must continue into ..  */
  for(;;)
  {	uint8 c=*(source++);
  if(c=='\0')	break;
  if(c<0x80) {
    result += c;
  }
  else
  {	if (c <=0xA4)
  {	int rnd = gen_rnd_number();
  result += goat_soup(desc_list[c-0x81].option[(rnd >= 0x33)+(rnd >= 0x66)+(rnd >= 0x99)+(rnd >= 0xCC)],psy);
  }
  else switch(c)
  { case 0xB0: /* planet name  */
  { int i=1;
  result += psy->name[0];
  while(psy->name[i]!='\0') 
  {
    result += tolower(psy->name[i++]);
  } 
  }	break;
        case 0xB1: /* <planet name>ian  */
          { int i=1;
          result += psy->name[0];
          while(psy->name[i]!='\0')
          {	if((psy->name[i+1]!='\0') || ((psy->name[i]!='E')	&& (psy->name[i]!='I')))
          { 
            result += tolower(psy->name[i]);
          }
          i++;
          }
          result += "ian";
          }	break;
        case 0xB2: /* random name  */
          {	int i;
          int len = gen_rnd_number() & 3;
          for(i=0;i<=len;i++)
          {	int x = gen_rnd_number() & 0x3e;
          if(pairs0[x]!='.') 
          { 
            result += pairs0[x];
          } 
          if(i && (pairs0[x+1]!='.')) 
          {
            result += pairs0[x+1];
          }
          }
          }	break;
        default: return "<bad char in data [%X]>";
  }	/* endswitch  */
  }	/* endelse  */
  }	/* endwhile  */
  return result;
}	/* endfunc  */

/**+end * */

