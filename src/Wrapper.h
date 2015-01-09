/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Wrapper.h
 * \brief   A wrapper class for the TextElite core of the game.
 */
#pragma once

/** * Include definitions for communication with textelite and graphic classes  */
#include "wrappDef.h"
/** * Include TextElite ******************************************************* */
#include "txtelite.h"

/** * Include help structures ************************************************* */
#include <vector>
#include <string>

//! Provides an interface to communicate with the inner text elite object. 
/** 
 * \nosubgrouping
 * Some functions transform the information because
 * the text elite objects saves it in different ways; other functions simply
 * call the corrisponding functions from the TextEliteGame-object.
 */
class Wrapper
{
public:
  /*********************** * functions: constructor and destructor ************** */
  Wrapper(void (*userErrorFnc)(std::string), void (*progErrorFnc)(std::string) );
  ~Wrapper(void);

  /*************** * functions: control functions (or cheating) ***************** */
  void setCash(float amount);
  void setFuel(float amount);
  void setCargoBay(unsigned int amount);
  void addCash(float amount);
  void addFuel(float amount);
  void addCargoSpace(unsigned int amount);

  /************************ * functions: get functions ************************** */
  System getSystemInfo(int number);
  int getCurrentSystem();
  std::vector<int> * getLocalSystems();
  std::vector<int> * getSystemsInRectangle(int left, int right, int bottom, int top);
  bool systemIsReachable(int number);
  bool systemIsInLocalRange(int number);

  float getMaxFuel();
  float getFuelAmount();

  int getNumberOfTradegoods();
  std::vector<Tradegood> * getAllTradegoodDescriptions();
  Tradegood getTradegoodDescription(int);
  bool tradegoodIsInTons(int);
  std::vector<MarketplaceItem> * getCurrentMarketplace();
  float getFuelCost();
  std::vector<int> * getCurrentCargo();
  unsigned int getFreeHoldspace();
  float getCurrentCash();
  unsigned int getCargoBaySize();

  /*********************** * functions: actions ********************************* */
  void buyFuel(float amount);
  void performJump(int number);
  std::string performPurchase(int number, int amount);
  std::string performSale(int number, int amount);

  /**************************** * variables ************************************* */
private:
  /******************* * object of the textEliteGame **************************** */
  TextEliteGame *txtgame;

  /********* * pointers to error functions to report occuring errors ************ */
  void (*vuserErrorFnc)(std::string);
  void (*vprogErrorFnc)(std::string);

  /***************************** * help variables ******************************* */
  static const int numberOfTradegoods = 17;
  Tradegood tradegoods[numberOfTradegoods]; 
};

