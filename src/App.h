/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	App.h
 * \brief	Declares the application class for the mElite game.
 */
#pragma once

// global includes
#include "OpenGLApplication.h"// base class
#include <vector>
#include <string>

// Forward declarations of classes
class TextureLoader;
class CameraObject;
class PlayerObject;
class BitmapFont;
class TextureFont;
class TextInput;
class Console;
class MouseHandler;
class Frustum;
class HudLoadBar;
class Button;

/**
 * \class	App
 *
 * \brief	The main class of mElite, which is an extension of the general OpenGLApplication class.
 *
 * Contains as members all elements that are contained in the game, such as world objects and 
 * and GUI objects. Has functions to show and update them. 
 *
 * \author	Jan-Philipp Kappmeier, Melanie Schmidt
 * \date	06/12/2006
 */
class App : OpenGLApplication {
private:
  GLuint g_dynamicTextureID;

private:
  static consoleCommand table[];
public:
  bool initialize( void );// perform initialization, executed when toggling to fullscreen
  void deinitialize( void );// perform all deinitialization
  void update( DWORD milliseconds );						// perform motion updates and get user inputs
  void draw( void ); // draw the scene
private:
  // initialization
  void initializeObjects( void );// loads and initializes all used game models
  void initMarketplace( void ); // initializes the marketplace
  void initializeGFX( void ); // initializes graphics
  // some private draw functions
  void drawCurrentSystem();
  void drawHelp();
  void drawHelpEnvironment();
  void drawHelpInfo();
  void drawHelpLocal();
  void drawHelpMarketplace();
  void drawLocalSystems();
  void drawMarketplace();
  void drawSystemInfo();
  void drawGameOver();
  // some private update functions
  void stationWasHit();
  void updateCurrentSystem( DWORD deltaTime );
  void updateLocalSystems( DWORD deltaTime );
  void updateSystemInfo( DWORD deltaTime );
  // some private input handling functions
  void performConsoleInput(); // handles input for the console
  void performCurrentSystemInput( DWORD deltaTime );// handles input for current system screen
  void performLocalSystemsInput( DWORD deltaTime ); // handles input for local systems screen
  void performSystemInfoInput( DWORD deltaTime ); // handles input for system info screen
public:
  GLUquadricObj *getQuadricObject() { return quadObj; }
private:
  bool gameOver;
  bool firstDraw;
  TextureLoader * loader;
private:
  // constructors and destructors
  friend class OpenGLApplication;
  App( const char  * className );						// Class Constructor
  ~App();

  // the screen that is to be drawn
  Screens currentScreen;
  void setScreen( Screens screen );
private:
  bool inStation;

public:
  // general initialization
  void loadAllNeededTextures();
  void loadSystemTextures();

  // objects used for games
  MouseHandler *mouse;// gives a mouse object used for mouse-handling
  PlayerObject *player;
  CameraObject *cameraSystemView;// the camera that sees the system in system information screen
  BitmapFont * bf;// automatic generated bitmap font, used for startup screen
  TextureFont * tf;// texture font 1
  TextureFont * tf2;// texture font 2
  TextInput *ti; // input class, used for giving commands
  Console *con;// console
  Frustum *frustum;// frustum, used for culling of hidden objects

public:
  // some functions to get the objects
  Frustum * getFrustum( void );
private:
  // the quadric object that is used for the skysphere
  GLUquadricObj *quadObj;

  // internal status
  bool m_inputmode;
  GLfloat lightspeed;
  bool devmode;
  float handlingSpeedMultiplyer; // allows faster movement of the camera

  // game status
  bool laserActive;

  // information output
public:
  void printInfoLine( const char *text, ... ); // print info line
private:
  void printInfoBox( bool clear );
  std::vector<std::string> infoBoxLines;
  bool m_showFrameRate;
  bool m_showFrameTime;
  bool m_showSpeed;
  bool m_showResolution;
  unsigned int infoOutputLine;
  bool m_switchColors;

  // debug status vars
  int currentSystemInfoNumber;

  // system info status
  GLfloat systemInfoViewAngle;
  GLfloat systemInfoViewAngle2;
  GLfloat zoomFactor;

  // functions to change status with console
  void showFrameRate();
  void showFrameTime();
  void showSpeed();
  void showResolution();
  void showHelp();
  void devMode();
  void switchColors();

  // function pointer array for use with console commands
  void (App::*fptr[10])( void );

private:
  // help functions
  void setProjectionOrthogonal( void );
  void restoreProjection( void );

  // stuff for the hud and information elements
  HudLoadBar * speedBar;
  HudLoadBar * laserHeatBar;

  // buttons for marketplace screen
  std::vector<Button*> mktbuttons; // buttons that represent the tradegoods
  Button * buybutton;// button for buying
  Button * sellbutton; // button for selling
  int chosenButton;// saves the chosen button to avoid that two buttons are in status clicked
};
