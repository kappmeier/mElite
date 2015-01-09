/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Wrapper.cpp
 * \brief The class Wrapper provides an interface to communicate with the
 * inner text elite object.
 *
 * Some functions transform the information because
 * the text elite objects saves it in different ways; other functions simply
 * call the corrisponding functions from the TextEliteGame-object.
 */
#include "Wrapper.h"


/** @name Construction
 * Constructor and destructor
 */
//@{
//! Constructor
/**
 * The constructor sets the names of the tradegoods and constructs a text elite object
 */
Wrapper::Wrapper( void (*userErrorFnc)(std::string), void (*progErrorFnc)(std::string) )
{
  vprogErrorFnc = progErrorFnc;
  vuserErrorFnc = userErrorFnc;

  tradegoods[0].name = "Food";
  tradegoods[1].name = "Textiles";
  tradegoods[2].name = "Radioactives";
  tradegoods[5].name = "Luxuries";
  tradegoods[7].name = "Computers";
  tradegoods[8].name = "Machinery";
  tradegoods[9].name = "Alloys";
  tradegoods[10].name = "Firearms";
  tradegoods[11].name = "Furs";
  tradegoods[12].name = "Minerals";
  tradegoods[13].name = "Gold";
  tradegoods[14].name = "Platinum";
  tradegoods[15].name = "Gem-Stones";
  tradegoods[16].name = "Alien Items";
#if POLITICALLY_CORRECT 
  tradegoods[3].name = "Robot Slaves";
  tradegoods[4].name = "Beverages";
  tradegoods[6].name = "Rare Species";
#else
  tradegoods[3].name = "Slaves";
  tradegoods[4].name = "Liquor/Wines";
  tradegoods[6].name = "Narcotics";
#endif

  for (int i = 0; i < numberOfTradegoods; i++)
  {
    tradegoods[i].unit = "t";
  }
  tradegoods[13].unit = "kg";
  tradegoods[14].unit = "kg";
  tradegoods[15].unit = "g";

  txtgame = new TextEliteGame( userErrorFnc, progErrorFnc ); 
};

//! Destructor
Wrapper::~Wrapper(void)
{
  delete(txtgame);
};
//@}

/** @name Get functions
 * The following functions provide a wide range of information about the
 * status of the current game that is saved within the TextEliteGame object.
 */
//@{

//! Get system information
/**
 * Returns information about the system with the given number
 * the structure 'system' from wrappDef.h is used
 */
System Wrapper::getSystemInfo(int number)
{ 
  System zwSystem = txtgame->getSystemInformation(number);

  /* the distance function in textelite divides distances in y-dimension 
  by 2. We decided to divide the y-coordinates of positions by 2 as 
  well to make distances more reasonable to the player*/
  zwSystem.posy = zwSystem.posy/2;
  return zwSystem;
  };

//! Get the current system.
/**
 * Returns the number of the system we are currently in.
 */
int Wrapper::getCurrentSystem()
{
  return txtgame->getCurrentSystem();
};

//! Get systems near to the current system.
/**
 * Returns an int-vector with the numbers of all systems that could
 * be reached if we had the maximal amount of fuel (7.0 LY).
 * We decided that the system info can only be seen for systems within this
 * range; this is somehow analogous to textelite, where you can see the info
 * for all systems if you know their names, but you can only see the names
 * of systems in local range.
 */
std::vector<int> * Wrapper::getLocalSystems()
{
  return txtgame->getLocalSystems();
};

//! Get systems in a special region.
/** 
 * this function returns an int-vector with the numbers of all systems
 * within the given rectangle. Because we decided to divide y-coordinates by
 * 2 for the use in our classes, we have to multiply them by 2 when we
 * contact the textelite.
 */
std::vector<int> * Wrapper::getSystemsInRectangle(int left, int right, int bottom, int top)
{
  /* if the original y-coordinate was odd, multiplying by 2 will not restore
  the original coordinate. Thus we subtract / add 1 to the y-coordinates
  in a way such that the rectangular is maybe greater, but never smaller
  than the meant rectangle.  */
  return txtgame->getSystemsInRectangle(left, right, (bottom-1)*2, (top+1)*2);
};

//! Check if a system is reachable.
/**
 * Returns whether the system 'number' is reachable from the current system
 * by hyperspace with the current amount of fuel
 */
bool Wrapper::systemIsReachable(int number)
{
  return txtgame->systemIsReachable(number);
};

//! Check if a system is generally reachable.
/**
 * Returns whether the system 'number' is reachable from the current
 * system if we have the maximal amount of fuel. The player can only see
 * system information for systems in local range.
 */
bool Wrapper::systemIsInLocalRange(int number)
{
  return txtgame->systemIsInLocalRange(number);
};

//! Get the maximal amount of fuel. 
/**
 * Returns the amount of fuel that fits into the tank
 * The player cannot buy more fuel. Systems that can be reaches with full
 * tank are said to be in 'local range'
 * \sa getLocalSystems()
 */
float Wrapper::getMaxFuel()
{
  return txtgame->getMaxFuel();
}

//! Get actual amount of fuel.
/**
 * Returns the actual amount of fuel
 */
float Wrapper::getFuelAmount()
{
  return txtgame->getFuel();
};

//! Get the number of tradegoods.
/**
 * Returns the number of known tradegoods the information about tradegoos
 * is stored in the wrapperclass and in the TextEliteGame class
 */
int Wrapper::getNumberOfTradegoods()
{
  return numberOfTradegoods;
};

//! Get descriptions for the tradegood.
/**
 * Returns a list with names and units of all known tradegoods.
 * The structure 'Tradegood' from wrappDef.h is used. 
 * The information about tradegoods is stored in the wrapperclass and in the
 * TextEliteGame class.
 * \return A pointer to a vector of Tradegood objects.
 */
std::vector<Tradegood> * Wrapper::getAllTradegoodDescriptions()
{
  std::vector<Tradegood> * list = new std::vector<Tradegood>;
  for (int i = 0; i < numberOfTradegoods; i++) {
    list->push_back(tradegoods[i]);
  }
  return list;
};

//! Get the description to one tradegood.
/** 
 * Returns the name and unit of a certain tradegood.
 * The structure 'Tradegood' from wrappDef.h is used.
 * The information about tradegoods is stored in the wrapperclass and in the 
 * TextEliteGame class.
 * \param number The number of the tradegood
 * \return The tradegood as Tradegood structure
 */
Tradegood Wrapper::getTradegoodDescription(int number)
{
  Tradegood result;
  if (number < 0 || number >= numberOfTradegoods)
    vprogErrorFnc("Error! 'number' not in range!");
  else
    result = tradegoods[number];
  return result;
};

//! Check if a tradegood is weighted in tonnes
/**
 * This function returns whether a certain tradegood has tonnes as unit.
 * this is important because only items in tonnes count for the
 * calculation of the free holdspace.
 * \param number The number of the tradegood
 * \return true, if the tradegood is weighted in tonnes, false elsewise
 */
bool Wrapper::tradegoodIsInTons(int number)
{
  return txtgame->tradegoodIsInTons(number);
}

//! Get information about the current marketplace
/**
 * Gives a list of MarketplaceItems saying how much is available of each
 * of the 17 items and how much it costs (both regarding the market in the
 * current system). The structure 'MarketplaceItem' from wrappDef.h is used.
 * \return A pointer to a vector of MarketplaceItem structures.
 */
std::vector<MarketplaceItem> * Wrapper::getCurrentMarketplace()
{
  return txtgame->getCurrentMarketplace();
};

//! Gets the price for one LY fuel
/**
 * Returns the price for one lightyear of fuel
 * \return The price
 */
float Wrapper::getFuelCost(){
  return (float)txtgame->getFuelCost();
}

//! Gets information about the current cargo.
/**
 * Tells how much we have in our cargo for each of the 17 buyable items.
 * \return A pointer to a vector containing the information
 */
std::vector<int> * Wrapper::getCurrentCargo()
{
  return txtgame->getCurrentCargo();
};

//! Gets the room in cargo
/**
 * Tells how much space is free for storing additional items. Only items 
 * with unit 't' are taken into account, 'kg' and 'g' make no difference
 * \return The free space
 */
unsigned int Wrapper::getFreeHoldspace()
{
  return txtgame->getFreeHoldspace();
};

//! Get the current cash.
/**
 * Returns the amount of available cash
 * \return The cash amount
 */
float Wrapper::getCurrentCash()
{
  /* the value is stored as int in the textelite class and therefor
  multiplied by 10; thus we must divide it by 10.  */
  float fvalue = (float)txtgame->getCurrentCash();
  return (float)(fvalue/10);
};

//! Get the size of the cargo bay.
/**
 * Returns the size of the cargo bay. It can be changed using the cheat functions.
 * \return The current size
 */
unsigned int Wrapper::getCargoBaySize()
{
  return txtgame->getCargoBaySize();
}
//@}

/** @name Actions
 * The following functions implement the actions jump, buy and sell.
 */
//@{
//! Buy fuel.
/**
 * Attempts to buy 'amount' fuel.
 * This can fail because of the following errors:
 * - the player has not enough cash
 * - the tank is too full (maximal 7.0 LY fit into the tank)
 * - amount is not positive
 *
 * If an error occures it is always bought as much as possible.
 * \param amount The amount of fuel that should be bought.
 */
void Wrapper::buyFuel(float amount)
{
  if (amount < 0)
  { 
    vprogErrorFnc("'amount' must not be less than 0! Error occured in getFuelAmount() in Wrapper.cpp");
    return;
  }
  txtgame->buyFuel(amount);
};

//! Jump to another system
/**
 * Attempts to jump to the system with number 'number'.
 * This can fail because of the following errors:
 * - the system is not in local range (you can only jump to those)
 * - player does not have enough fuel
 *
 * In this cases the player stays where he is.
 * \param number The internal number of the destination system.
 */
void Wrapper::performJump(int number)
{
  txtgame->performJump(number);
};

//! Buy a tradegood.
/**
 * Attempts to buy 'amount' items of type 'number'.
 * This can fail because of one of the following errors:
 * - amount or number is negative
 * - number does not specify a tradegood
 * - the market does not offer enough of the wished tradegood
 * - the player does not have enough money
 * - the cargo bay is too full
 *
 * In this cases it is always bought as much as possible.
 * \param number The number of the tradegood
 * \param amount The quantity of items that should be bought
 * \return An error string.
 */
std::string Wrapper::performPurchase(int number, int amount)
{ 
  if (amount < 0) {
    vprogErrorFnc("Error! 'amount' is not allowed to be less than 0. Error occured in Wrapper.cpp in the function performPurchase(int number);");
  }
  if (number < 0) {
    vprogErrorFnc("Error! 'number' is not allowed to be less than 0. Error occured in Wrapper.cpp in the function performPurchase(int number);");
  }
  std::string report = txtgame->performPurchase((uint)number, (uint)amount);
  /* * Delete spaces at the end and add a dot * */
  size_t i = report.size()-1;
  while (i >= 0 && report[i] == ' ')
  {
    report.replace(i,1,""); 
    i--; 
  }
  report +=".";
  return report;
};

//! Sell a tradegood.
/**
 * Attempts to sell 'amount' items of type 'number'.
 * This can fail because of the following errors:
 * - amount or number are negative
 * - number does not specify a tradegood
 * - the player does not have enough of this tradegood
 *
 * It is sold as much as possible.
 * \param number The number of the tradegood
 * \param amount The quantity of items that should be bought
 * \return An error string.
 */
std::string Wrapper::performSale(int number, int amount)
{
  if (amount < 0) {
    vprogErrorFnc("Error! 'amount' is not allowed to be less than 0. Error occured in Wrapper.cpp in the function performPurchase(int number);");
  }
  if (number < 0) {
    vprogErrorFnc("Error! 'number' is not allowed to be less than 0. Error occured in Wrapper.cpp in the function performPurchase(int number);");
  } 
  std::string report = txtgame->performSale((uint)number, (uint)amount);
  /* * Delete spaces at the end and add a dot * */
  size_t i = report.size()-1;
  while (i >= 0 && report[i] == ' ')
  {
    report.replace(i,1,""); 
    i--; 
  }
  report +=".";
  return report;
};
//@}

/** @name Control functions (or cheating)
  * The following functions can set values directly without playing after the
  * rules of the game. This can be used to provide additional features (as
  * a cargo bay upgrade) or for cheating.
 */
//@{
//! Sets the amount of cash that we have
/**
 * Because the cash is saved differently inside textelite we have to
 * multiply it with 10 before calling the appropriate function.
 */
void Wrapper::setCash(float amount){
  if (amount < -100000 || amount > 100000){
    vprogErrorFnc("'amount' has to be between -100000 and 100000. Error occured in setCash(float amount) in Wrapper.cpp");
    return;
  }
  float f = amount  * 10;
  signed long i = (signed long) f;
  txtgame->setCash(i);
};

//! Sets the amount of fuel that we have
/**
 * Because the fuel is saved differently inside textelite we have to
 * multiply it with 10 before calling the appropriate function.
 */
void Wrapper::setFuel(float amount){
  if (amount < 0 || amount > 7.0){
    vprogErrorFnc("'amount' has to be between 0 and 7. Error occured in setFuel(float amount) in Wrapper.cpp");
    return;
  }
  float f = amount  * 10;
  signed long i = (signed long) f;
  txtgame->setFuel(i);
};

//! Sets the size of the cargo bay that we have
/** 
 * This can fail if the current cargo would not fit into the new cargo space 
 * In this case the cargo bay does not change. 
 */
void Wrapper::setCargoBay(unsigned int amount){
  txtgame->setCargoBay(amount);
};

//! Tries to add 'amount' fuel to the current amount of fuel.
/** 
 * It failes if the tank would be too full.
 */
void Wrapper::addFuel(float amount)
{
  if (amount < 0 || amount > 7.0){
    vprogErrorFnc("'amount' has to be between 0 and 7. Error occured in adFuel(float amount) in Wrapper.cpp");
    return;
  }
  float newValue = getFuelAmount() + amount;
  if (newValue > 7.0){
    vprogErrorFnc("'amount' does not fit into the tank and was reduced (in addFuel in Wrapper.cpp)");
    newValue = 7.0;
  }
  setFuel(newValue);
};

//! Adds 'amount' cash to the current amount of cash.
/** 
 * Negative values are allowed but not recommended. 
 */
void Wrapper::addCash(float amount)
{
  float newValue = getCurrentCash() + amount;
  setCash(newValue);
};

//! Increases the size of the cargo bay by 'amount'.
void Wrapper::addCargoSpace(unsigned int amount)
{
  unsigned int newValue = getCargoBaySize() + amount;
  txtgame->setCargoBay(newValue);
}
//@}
