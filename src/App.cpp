/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	App.cpp
 * \brief	Implementation of the main class of the mElite game. Performs basic drawing and handles all the input. 
 */
#pragma once

#include <fstream>
#include "App.h"
#include "BitmapFont.h"
#include "TextureFont.h"
#include "TextInput.h"
#include "Console.h"
#include "ColorConstants.h"
#include "MouseHandler.h"
#include "Frustum.h"
#include "HudLoadBar.h"
#include "Button.h"
#include "World.h"
#include "PoliceShip.h"
#include "Objects.h"
#include "Shapes.h"
#include "SpaceStation.h"
#include "Star.h"
#include "Planet.h"
#include "TextureLoaderTGA.h"
#include "Textures.h"
#include "PlayerObject.h"
#include "FileAccess.h"
#include "FrameCounter.h"

using namespace std;

/** \brief	Enables the development mode, that allows more features. */
static const bool DEV_MODE = true;

/**
 * \brief a table containing strings and indices. strings can be entered and the index
 * 		  specifies a function that is called.
 */
consoleCommand App::table[] = {
  {"showFPS", 0},
  {"showFrameTime",1},
  {"showSpeed", 2},
  {"showRes", 3},
  {"quit",4},
  {"toggleFullscreen", 5},// is called in the parent class!
  {"help",6},
  {"devmode", 7},
  {"switchColors",8},
  {NULL, -1}
};

/**
 * \fn	OpenGLApplication * OpenGLApplication::create(const char * className)
 * \brief	Create the derived application class and return a pointer to the OpenGLApplication base class.
 * \param	className	Name of the window class.
 * \return	the pointer to the class, or null if it fails.
 */
OpenGLApplication  * OpenGLApplication::create(const char  * className) {
  App * example = new App( className );
  return reinterpret_cast<OpenGLApplication *>(example);
}

/**
 * \fn	App::App( const char * className )
 * \brief	Sets up the class and the variables.
 * 			
 * Initializes the function pointers for the functions called to handle console commands.
 * \param	className	Name of the class.
 */
App::App( const char  * className ) : OpenGLApplication( className ) {
  // create array of function pointer. this can only be done in constructor because it's not static
  fptr[0] = &App::showFrameRate;
  fptr[1] = &App::showFrameTime;
  fptr[2] = &App::showSpeed;
  fptr[3] = &App::showResolution;
  fptr[4] = &App::terminate;
  fptr[5] = &App::toggleFullscreen; // from parent class!
  fptr[6] = &App::showHelp;
  fptr[7] = &App::devMode;
  fptr[8] = &App::switchColors;

  zoomFactor = 1;

  handlingSpeedMultiplyer = 5  * 0.01;
  lightspeed = 0.00815;
  currentScreen = currentSystem;
  m_inputmode = false;
  m_showFrameRate = m_showFrameTime = m_showResolution = true;
  m_showSpeed = false;
  m_switchColors = false;
  //infoOutputLine; // does not need to be initialized, is set to 0 at beginning of every frame

  // debug only vars
  devmode = DEV_MODE; // disable debug modus
  currentSystemInfoNumber = 7; // num for lave ;) right is: World::getWorld()->getCurrentSystem(); //but does not exist! // in debug modus you can switch through the systems in info mode

  // game status
  laserActive = false;
  inStation = false;
  gameOver = false;

  // system info view
  systemInfoViewAngle = -25.0;
  systemInfoViewAngle2 = -10.0;
  zoomFactor = 1.0;

  // initialize the cameras
  cameraSystemView = new CameraObject( ); // has no radius and creates no collision (for now ;) )
  cameraSystemView->setPos( 0, 0, 0 );
  cameraSystemView->setSpeed( lightspeed  * 0.2 );
}

/**
 * \fn	App::~App()
 * \brief	Destructor.
 */
App::~App() {
  //delete texture;
  //delete player;
  delete cameraSystemView;
}

/**
 * \fn	void App::deinitialize()
 * \brief	De-initialises this object and frees any resources it is using.
 */
void App::deinitialize() {
  // delete objects created while initialization
  delete speedBar;
  delete laserHeatBar;

  World::getWorld()->deinitialize();

  delete mouse;
  delete frustum;
  delete con;
  delete ti;
  delete tf2;
  delete tf;
}

/**
 * \fn	void App::draw()
 *
 * \brief	Draws the scene.
 * 			
 * If the player is dead, the end screen is shown except developer mode is active.
 * In this case, the came continues. Mainly the function is a long switch statement
 * for the different game modes. Then appropriate methods are called.
 */
void App::draw() {
  if( devmode )
    gameOver = false;

  if( gameOver ) {
    drawGameOver();
    return;
  }

  // reset info output line counter
  infoOutputLine = 0;

  static int planetToDraw = -1;
  static bool showBox = false;

  switch( currentScreen ) {
case currentSystem:
  {
    getWindow()->showMousePointer( false );
    unsigned int oldHeight = getWindow()->getHeight();
    unsigned int oldWidth = getWindow()->getWidth();
    getWindow()->setHeight( 128 );
    getWindow()->setWidth( 128 );
    getWindow()->reshape();
    firstDraw = true;
    drawCurrentSystem();


    // read the image data for the rear view and load as texture
    unsigned char *image = 0;
    image = new unsigned char[getWindow()->getWidth() * getWindow()->getHeight() * 3];
    glReadPixels( 0, 0, getWindow()->getWidth(), getWindow()->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, image );
    TextureLoader * loader = new TextureLoaderTGA();
    loader->load( image, getWindow()->getWidth(), getWindow()->getHeight(), 3);
    Textures::getTextures()->loadTexture( rearview, loader );
    delete[] image;

    getWindow()->setHeight( oldHeight );
    getWindow()->setWidth( oldWidth );
    getWindow()->reshape();
    firstDraw = false;
    drawCurrentSystem();
  }
  break;
case localSystems:
  {
    getWindow()->showMousePointer( true );
    mouse->setMouseHovering( !showBox ); // enable mouse hovering, this enables highlighting planets
    //if( mouse->getHittedObject() != button || getWindow()->getMouseClick() == false ) // only draw if no button was hit, because status changes!
    drawLocalSystems();
    // check if mouse-click has occured and if that happens, do jump if a planet was hit
    if( getWindow()->getMouseClick() ) {
      if( mouse->getHittedObject() == planet ) {
        // a planet was hit
        // set up an question-box that is painted over the screen --> because of that next time hovering has to be disabled.
        showBox = !showBox;
      }
      if( mouse->getHittedObject() == button ) {
        // should only be executed if a button is pressed
        if( mouse->getHittedObjectInfo() == 0 ) {
          // nothing to do ;)
        } else {
          // perform jump
          World::getWorld()->performJump( planetToDraw );
          player = World::getWorld()->getPlayer();
          // reload textures
          loadSystemTextures();
          getWindow()->showMousePointer( false );
          setScreen( systemInfo );
        }
        mouse->setHittedObject( none );
        planetToDraw=-1;
        showBox = !showBox;
      }
      getWindow()->setMouseClick( false );// click was handled, so delete it!
    }
  }
  break;
case marketplace:
  getWindow()->showMousePointer( true );
  drawMarketplace();
  break;
case systemInfo:
  getWindow()->showMousePointer( false );
  drawSystemInfo();
  break;
case credits:
  getWindow()->showMousePointer( false );
  break;
case help:
  getWindow()->showMousePointer( false );
  drawHelp();
  break;
case helpEnvironment:
  getWindow()->showMousePointer( false );
  drawHelpEnvironment();
  break;
case helpInfo:
  getWindow()->showMousePointer( false );
  drawHelpInfo();
  break;
case helpLocal:
  getWindow()->showMousePointer( false );
  drawHelpLocal();
  break;
case helpMarketplace:
  getWindow()->showMousePointer( false  );
  drawHelpMarketplace();
  break;
case loadProgram:
case loadLevel:
  getWindow()->showMousePointer( false );
  break;
  }

  if( showBox ) {
    // print a box and therefore, enable mouse-hitting
    mouse->setMouseHovering( true );
    mouse->setHittedObject( none );

    // draw a box
    // set up an orthogonal screen for printing text and painting the board
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, getWindow()->getWidth(), 0, getWindow()->getHeight(), -100, 100 ); // full size orthogonal screen
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_TEXTURE_2D );
    // draw a box in gray that represents the box
    glColor3fv( colorLightGrey );
    int writingHeight = tf2->getSize();
    int writingWidth = tf2->getWidth();
    int xCenter = getWindow()->getWidth() * 0.5;
    int yCenter = getWindow()->getHeight() * 0.5;
    int messageWidth = 38  * writingWidth;
    int messageHeight = 8  * writingHeight; // 2  * size over and under text, 2 times space for buttons and one time space under the button
    Shapes::drawRect( xCenter, yCenter, 0, messageWidth, messageHeight );
    // draw yes and no buttons
    int buttonWidth = 90;
    int buttonHeight = 2*writingHeight;

    glColor3fv( colorDarkGrey );
    Shapes::drawRect( xCenter - 2*writingWidth - buttonWidth*0.5, yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5, 0, buttonWidth, buttonHeight);
    Shapes::drawRect( xCenter + 2*writingWidth + buttonWidth*0.5, yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5, 0, buttonWidth, buttonHeight);

    // test if mouse is over a button
    bool overYes = mouse->isMouseOverRectCenter(xCenter - 2*writingWidth - buttonWidth*0.5, getWindow()->getHeight() - (yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5), buttonWidth, buttonHeight);
    bool overNo =mouse->isMouseOverRectCenter(xCenter + 2*writingWidth + buttonWidth*0.5, getWindow()->getHeight() - (yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5), buttonWidth, buttonHeight);

    // print question box
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glColor4fv( colorWhite );
    if( planetToDraw == -1)
      planetToDraw = mouse->getHittedObjectInfo();
    string text = "Do you want to jump to " + World::getWorld()->getSystemName( planetToDraw ) + "?";
    tf2->print( xCenter - messageWidth*0.5 + 2*writingHeight,
      yCenter + messageHeight*0.5 - 3  * writingHeight,
      &text
      );
    // print questions and so on
    if( overYes ) {
      glColor3fv( colorDarkRed );
      tf2->print( xCenter - 2*writingWidth - buttonWidth*0.5 /* now x-center position  */ - 1.5*writingWidth,
        yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5 /* now y center position  */ - 0.5*writingHeight,
        "Yes"
        );
      glColor3fv( colorWhite );
      mouse->setHittedObject( button );
      mouse->setHittedObjectInfo( 1 );
    }
    else
      tf2->print( xCenter - 2*writingWidth - buttonWidth*0.5 /* now x-center position  */ - 1.5*writingWidth,
      yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5 /* now y center position  */ - 0.5*writingHeight,
      "Yes"
      );
    if( overNo ) {
      glColor3fv( colorDarkRed );
      tf2->print( xCenter + 2*writingWidth + buttonWidth*0.5 /* now x-center position  */ - writingWidth,
        yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5 /* now y center position  */ - 0.5*writingHeight,
        "No"
        );
      glColor3fv( colorWhite );
      mouse->setHittedObject( button );
      mouse->setHittedObjectInfo( 0 );
    }
    else
      tf2->print( xCenter + 2*writingWidth + buttonWidth*0.5 /* now x-center position  */ - writingWidth,
      yCenter - messageHeight*0.5 + writingHeight + buttonHeight*0.5 /* now y center position  */ - 0.5*writingHeight,
      "No"
      );
    glDisable( GL_BLEND );

    // restore matrices
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
  }
  glFlush();
}

/**
 * \fn	void App::drawCurrentSystem()
 * \brief	Displays main screen containing 3d environment.
 * 			
 * Some text as fps and speed is  displayed in orthogonal mode.
 */
void App::drawCurrentSystem() {
  if( !firstDraw ) {
    // send some information lines to the print-on-display vector
    if( m_showFrameTime )
      printInfoLine("Frametime:%3.2f ms", getFrameCounter()->getFrameTimeAverage());
    if( m_showFrameRate )
      printInfoLine("Framerate:%d fps", getFrameCounter()->getFrameRateAverage());
    if( m_showResolution )
      printInfoLine("Screensize: %dx%d", getWindow()->getWidth(), getWindow()->getHeight());
    if( m_showSpeed ) {
      printInfoLine("Speed (abs): %5.4f", player->getSpeed());
      printInfoLine("Speed: %3.2f", (100*player->getSpeed())/lightspeed);// percentage of lightspeed (= normalspeedmax)
    }
    if( player->isRearView() )
      printInfoLine("Rear view is activated.");
  }

  if(firstDraw)
    player->toggleRearView();

  // save projection and create frustrum
  glPushMatrix();
  player->look();

  // draw the skysphere. thus the z-buffer is deactivated, so that the skysphere appears to be in the background
  // fast mode is already activated (at least, it should be ;)
  fastMode();
  glPushMatrix();
  glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
  glTranslatef( player->getPos().x, player->getPos().y, player->getPos().z );// move sphere with viewer
  glRotatef( 90, 1, 0, 0 );
   Textures::getTextures()->bindTexture( background );
  gluSphere( quadObj, 100, 32, 32 );
  glPopMatrix();

  // activate full mode in order to draw the main scene. the z-buffer can now be cleared
  // render main scene
  fullMode();
  glClear( GL_DEPTH_BUFFER_BIT );

  vector<GraphicsObject*> *objects = World::getWorld()->getGraphicObjects();
  for( unsigned int i=0; i < objects->size(); i++ ) {
    CullingLocation res = getFrustum()->isSphereInFrustum( objects->at(i)->getPos(), objects->at(i)->getRadius() );
    if( player->isRearView() )
      res = inside;
    if( res != outside ) {
      objects->at(i)->draw();
      SpaceShip* ship = dynamic_cast<SpaceShip*>( objects->at(i) );
      if( ship ) {
        glDisable( GL_CULL_FACE );
        if( ship->isShooting() ) {
            glDisable( GL_LIGHTING ); // deactivate lighting for the laser
            glDisable( GL_LIGHT0 ); // star is deactivated (is this really necessary?)
            glEnable( GL_BLEND ); // blending enabled, as the laser is transparent
            Textures::getTextures()->bindTexture( laser );
            glColor3fv( colorRed );
            Vector3 shootVector = ship->getPos() - player->getPos();
            shootVector.normalize();

            // new calculation
            Vector3 p = ship->getPos() - player->getPos();  // vector to the ship
            Vector3 v = player->getView();
            v.normalize();

            Vector3 aimFrom = ship->getPos();
            Vector3 aimTo = player->getPos() + player->getView()*20;

            Vector3 sproj = player->getView() * ship->getPos().innerProduct( player->getView() );
            Vector3 v2 = player->getRight();
            v2.normalize();
            Vector3 v3 = ship->getPos() - sproj;
            v3.normalize();

            float cosval = v2.innerProduct( v3 );
            float winkel = acos(cosval) / ANGLE2DEG;

            if(winkel > 90)
            winkel = 180 - winkel;
            Vector3 absbla = player->getUp() * ((90.0-winkel)/90.0) + player->getRight() * (winkel/90.0);
            Vector3 absbla2 = player->getRight() * ((90.0-winkel)/90.0) + player->getUp() * (winkel/90.0);
            absbla.normalize();

            Vector3 aimToLeft = aimTo + absbla * -5.0f;
            Vector3 aimToRight = aimTo + absbla * 5.0f;
            Vector3 aimFromLeft = aimFrom + absbla * -5;
            Vector3 aimFromRight = aimFrom + absbla * 5;

            glEnable( GL_BLEND );
            glBegin( GL_QUADS );
              glTexCoord2f( 0, 1 );
              glVertex3f( aimFromLeft.x, aimFromLeft.y, aimFromLeft.z );
              glTexCoord2f( 0, 0 );
              glVertex3f( aimToLeft.x, aimToLeft.y, aimToLeft.z );
              glTexCoord2f( 1, 0 );
              glVertex3f( aimToRight.x, aimToRight.y, aimToRight.z );
              glTexCoord2f( 1, 1 );
              glVertex3f( aimFromRight.x, aimFromRight.y, aimFromRight.z );
            //if( frustum->isInRightHalf( ship->getPos() ) && frustum->isInUpperHalf( ship->getPos() ) ) {
              Plane aimPlane = Plane(aimToLeft, aimToRight, aimFromRight);
              Vector3 absblaOrtho = aimPlane.getNormal();
              aimToLeft = aimTo + absblaOrtho * -5.0f;
              aimToRight = aimTo + absblaOrtho * 5.0f;
              aimFromLeft = aimFrom + absblaOrtho * -5;
              aimFromRight = aimFrom + absblaOrtho * 5;
            //}
              glTexCoord2f( 0, 1 );
              glVertex3f( aimFromLeft.x, aimFromLeft.y, aimFromLeft.z );
              glTexCoord2f( 0, 0 );
              glVertex3f( aimToLeft.x, aimToLeft.y, aimToLeft.z );
              glTexCoord2f( 1, 0 );
              glVertex3f( aimToRight.x, aimToRight.y, aimToRight.z );
              glTexCoord2f( 1, 1 );
              glVertex3f( aimFromRight.x, aimFromRight.y, aimFromRight.z );
            glEnd();
            glDisable( GL_BLEND );
            glEnable( GL_TEXTURE_2D );
            fullMode();
            glEnable( GL_LIGHTING );
        }
        glEnable( GL_CULL_FACE );
      }
    }
  }
  delete objects;
  if( firstDraw ) {
    player->toggleRearView();
    glPopMatrix();
    return;
  }

  glEnable( GL_BLEND );
  if( laserActive ) {
    glDisable( GL_LIGHTING ); // deactivate lighting,
    glDisable( GL_LIGHT0 ); // and the star
     Textures::getTextures()->bindTexture( laser );
    glColor3fv( colorRed );

    glPushMatrix();
    glBegin( GL_QUADS );
    Vector3 target = player->getPos() + player->getView()*4000;
    Vector3 obenLinks = target + player->getRight()*(-10);
    Vector3 obenRechts = target + player->getRight()*0;
    Vector3 untenLinks = player->getPos() + player->getRight()*(-10) + player->getUp()*(-10);
    Vector3 untenRechts = player->getPos() + player->getRight()*0 + player->getUp()*(-10);
    
    glTexCoord2f( 0, 1 ); // upper left
    glVertex3f( obenLinks.x, obenLinks.y, obenLinks.z );
    glTexCoord2f( 0, 0 ); // lower left
    glVertex3f( untenLinks.x, untenLinks.y, untenLinks.z );
    glTexCoord2f( 1, 0 ); // lower right
    glVertex3f( untenRechts.x, untenRechts.y, untenRechts.z );
    glTexCoord2f( 1, 1 ); // upper right
    glVertex3f( obenRechts.x, obenRechts.y, obenRechts.z );

	obenLinks = target + player->getRight()*(0);
    obenRechts = target + player->getRight()*10;
    untenLinks = player->getPos() + player->getRight()*(0) + player->getUp()*(-10);
    untenRechts = player->getPos() + player->getRight()*10 + player->getUp()*(-10);
    glTexCoord2f( 1, 1 ); // upper left
    glVertex3f( obenLinks.x, obenLinks.y, obenLinks.z );
    glTexCoord2f( 1, 0 ); // lower left
    glVertex3f( untenLinks.x, untenLinks.y, untenLinks.z );
    glTexCoord2f( 0, 0 ); // lower right
    glVertex3f( untenRechts.x, untenRechts.y, untenRechts.z );
	glTexCoord2f( 0, 1 ); // upper right
    glVertex3f( obenRechts.x, obenRechts.y, obenRechts.z );
    glEnd();

	glPopMatrix();
    glEnable( GL_LIGHTING ); // reactivate lighting
    glEnable( GL_LIGHT0 );
  }

  glPopMatrix(); // end of general matrix transformation

  // switch to fast mode in order to draw the display elements
  fastMode();

  // set up an orthogonal screen for printing text and drawing the huds
  setProjectionOrthogonal();
  glEnable( GL_BLEND );							// enable blending (for text and transparent items)
  glColor3fv( colorWhite ); // white color
  printInfoBox( true ); // actually print the lines and the info box

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  // draw the sight
  glColor3f(1,0,0);
  Textures::getTextures()->bindTexture( sight );
  int x = this->getWindow()->getWidth()  * 0.5;
  int y = this->getWindow()->getHeight()  * 0.5;
  int sightSize = 16;
  glBegin( GL_QUADS );
  glTexCoord2f( 0, 1 );
  glVertex3f(-sightSize*2 + x, sightSize + y, 0);
  glTexCoord2f( 0, 0 );
  glVertex3f(-sightSize*2 + x,-sightSize + y, 0);
  glTexCoord2f( 1, 0 );
  glVertex3f( sightSize*2 + x,-sightSize + y, 0);
  glTexCoord2f( 1, 1 );
  glVertex3f( sightSize*2 + x, sightSize + y, 0);
  glEnd();

  // draw missile info
  glColor3fv( colorLightGrey);
  Textures::getTextures()->bindTexture( missile_hud_tex );
  y = 48;
  sightSize = 48;
  for( unsigned int i = 0; i < player->getMissileCount(); i++ ) {
    x = this->getWindow()->getWidth() - 230 - i*30;
    glBegin( GL_QUADS );
    glTexCoord2f( 0, 1 );
    glVertex3f(-sightSize + x, sightSize + y, 0);
    glTexCoord2f( 0, 0 );
    glVertex3f(-sightSize + x,-sightSize + y, 0);
    glTexCoord2f( 1, 0 );
    glVertex3f( sightSize + x,-sightSize + y, 0);
    glTexCoord2f( 1, 1 );
    glVertex3f( sightSize + x, sightSize + y, 0);
    glEnd();
  }

  glDisable( GL_BLEND );
  x = getWindow()->getWidth() - 400;
  y = 64;
  int x2 = getWindow()->getWidth() * 0.5;
   Textures::getTextures()->bindTexture( rearview );
  sightSize = 64;
  int x1 = getWindow()->getWidth() - 440;
  if(x2 > x1)
    x = x1;
  else
    x=x2;
  glBegin( GL_QUADS );
  glTexCoord2f( 0, 1 );
  glVertex3f(-sightSize + x, sightSize + y, 0);
  glTexCoord2f( 0, 0 );
  glVertex3f(-sightSize + x,-sightSize + y, 0);
  glTexCoord2f( 1, 0 );
  glVertex3f( sightSize + x,-sightSize + y, 0);
  glTexCoord2f( 1, 1 );
  glVertex3f( sightSize + x, sightSize + y, 0);
  glEnd();
  glEnable( GL_BLEND );

  // draw speed and laser heat bar
  glColor3fv( colorWhite );
  tf2->print( 10, 76, "Laser heat:" );
  laserHeatBar->draw( 10, 55, 250, 16 );
  glColor3fv( colorWhite );
  tf2->print( 10, 41, "Speed:" );
  speedBar->draw( 10, 20, 250, 16 );
  glDisable( GL_BLEND );

  // draw shield view
  glPushAttrib( GL_ALL_ATTRIB_BITS );
  glDisable( GL_TEXTURE_2D );
  GLfloat colorShieldFront[] = { 1-player->getFrontShieldPowerPercentage(), player->getFrontShieldPowerPercentage(), 0.00f, 1.0f };
  GLfloat colorShieldBack[]= { 1-player->getFrontShieldPowerPercentage(), player->getFrontShieldPowerPercentage(), 0.00f, 1.0f };
  Vector3 shieldPos( getWindow()->getWidth() - 56, 56, 0 );
  Shapes::drawCircleShape( shieldPos, 360, 40, 270, 90, colorShieldFront ); // obere hälfte
  Shapes::drawCircleShape( shieldPos, 360, 40, 90, 270, colorShieldBack );// untere hälfte
  glEnable( GL_BLEND );
  glEnable( GL_TEXTURE_2D );
  glColor3fv( colorWhite );
  tf2->print( shieldPos.x - 3 * tf2->getWidth() - 4, shieldPos.y - tf2->getSize()  * 0.5, "Shield" );
  glDisable( GL_BLEND );
  glDisable( GL_TEXTURE_2D );

  // draw radar
  fastMode();
  glColor3fv( colorWhite );
  glEnable( GL_BLEND );
  glEnable( GL_TEXTURE_2D );
   Textures::getTextures()->bindTexture( radar );
  Vector3 radarPos( getWindow()->getWidth() - 56 - 40 - 56, 56, 0 );
  Shapes::drawCircleTex( radarPos, 360, 40 );
  Shapes::drawCircleTex( radarPos, 360, 40 );

  // draw a red point into the radar
  Vector3 vEye = ( player->getPos() - World::getWorld()->getStation()->getPos() );
  vEye.normalize();
  Vector3 dotPos( 0, 0, 0 );
  dotPos.x = -vEye.innerProduct( player->getView()  * player->getUp() );
  dotPos.y = -vEye.innerProduct( player->getUp() );
  float  z = -vEye.innerProduct( player->getView() );

  glDisable( GL_TEXTURE_2D );
  glDisable( GL_BLEND );
  // the point indicating position has to be 1/2  * radar size
  if ( z < 0 )
    Shapes::drawCircle( radarPos + dotPos*0.5*60, 60, 3, colorDarkRed, colorRed );
  else
    Shapes::drawCircle( radarPos + dotPos*0.5*60, 60, 6, colorDarkRed, colorRed );

  glPopAttrib();

  // restore procections
  restoreProjection();
}

/**
 * \fn	void App::drawGameOver()
 * \brief	If the player lost, print some information and quit the game.
 */
void App::drawGameOver() {
  glDisable( GL_LIGHTING );
  glDisable( GL_LIGHT0 );
  glDisable( GL_BLEND );
  glDisable( GL_TEXTURE_2D );
  bf = new BitmapFont( getWindow()->getDC() );
  bf->buildFont( 20 );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.2f );
  glClear( GL_COLOR_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();					 // reset the view

  glLoadIdentity();					 // reset the view
  glTranslatef( 0.0f, 0.0f, -1.0f );		// move a bit into the screen
  glColor3fv( colorWhite );
  glRasterPos2f(-0.5,0.3);
  bf->print("Thank you for playing our Game!");
  glRasterPos2f(-0.5, 0.15);
  bf->print("mElite");
  glRasterPos2f(-0.5, 0.1);
  bf->print("by Melanie Schmidt & Jan-Philipp Kappmeier");
  glRasterPos2d(-0.5,-0.3);
  bf->print("G a m e   O v e r");
  glRasterPos2d(-0.5,-0.4);
  bf->print("Press any key to exit.");
  glFlush();
  delete bf;
}

/**
 * \fn	void App::drawHelp()
 * \brief	Print some descriptive help text.
 */
void App::drawHelp() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  fastMode();
  setProjectionOrthogonal();

  // print text (fps, frametime, etc.)
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorWhite );// white color

  printInfoLine("elite clone");
  printInfoLine( "copyright 2006 - 2007, Melanie Schmidt & Jan-Philipp Kappmeier");
  printInfoLine( "" );
  printInfoLine( "Global keyboard commands (just press the key):" );
  printInfoLine( " e - show 3D environment in current system." );
  printInfoLine( " i - show system information for current system." );
  printInfoLine( " l - show local systems." );
  printInfoLine( " p - make a screenshot." );
  printInfoLine( "" );
  printInfoLine( "Global console commands (press RETURN, enter command, press RETURN):" );
  printInfoLine( " toggleFullscreen [0|1] - switch to fullscreen or back." );
  printInfoLine( " showFPS [0|1] - show frames per second or not." );
  printInfoLine( " showFrameTime [0|1] - show time per frame in milliseconds or not." );
  printInfoLine( " showSpeed [0|1] - show player speed in absolute values and percent of lightspeed or not." );
  printInfoLine( " showRes [0|1] - show current resolution or not." );
  printInfoLine( " devmode [0|1] - enables developer mode or disables." );
  printInfoLine( " quit - exit the game." );
  printInfoLine( " help - show this text." );
  printInfoLine( "" );
  printInfoLine( "To get further information, there exist different help sites for each screen." );
  printInfoLine( "Just type in one of the following commands at console:" );
  printInfoLine( " help info - gives information about the current system screen." );
  printInfoLine( " help local - gives information about the local systems view." );
  printInfoLine( " help env - gives information about the 3D environment." );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  restoreProjection();
}

/**
 * \fn	void App::drawHelpEnvironment()
 * \brief	Print special and more detailed help text for the 3d environment
 */
void App::drawHelpEnvironment() {
  glClear( GL_COLOR_BUFFER_BIT );
  fastMode();
  setProjectionOrthogonal();

  // print text (fps, frametime, etc.)
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorWhite );// white color

  printInfoLine("elite clone");
  printInfoLine( "copyright 2006 - 2007, Melanie Schmidt & Jan-Philipp Kappmeier");
  printInfoLine( "" );
  printInfoLine( "Keyboard commands for 3D-environment screen (in addition to usual keys):" );
  printInfoLine( " up key    - stick your nose up." );
  printInfoLine( " down key  - stick your nose down." );
  printInfoLine( " left key  - rotate leftwards." );
  printInfoLine( " right key - rotate rightwards." );
  printInfoLine( " space     - shoot laser." );
  printInfoLine( " +         - increase speed." );
  printInfoLine( " -         - decrease speed." );
  printInfoLine( " b         - toggle back and front view." );
  printInfoLine( " m         - shoot a missile." );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  restoreProjection();}

/**
 * \fn	void App::drawHelpInfo()
 * \brief	Print some detailed ifnormation for info screen of the system.
 */
void App::drawHelpInfo() {
  glClear( GL_COLOR_BUFFER_BIT );
  fastMode();
  setProjectionOrthogonal();

  glEnable( GL_BLEND ); // enable blending (for text)
  glColor3fv( colorWhite );

  printInfoLine("elite clone");
  printInfoLine( "copyright 2006 - 2007, Melanie Schmidt & Jan-Philipp Kappmeier");
  printInfoLine( "" );
  printInfoLine( "Keyboard commands for info screen (in addition to usual keys):" );
  printInfoLine( " arrow keys - move view of the system" );
  printInfoLine( " +          - zoom in." );
  printInfoLine( " -          - zoom out." );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  restoreProjection();
}

/**
 * \fn	void App::drawHelpLocal()
 * \brief	Print help text specific to the local screen showing nearby solar systems.
 */
void App::drawHelpLocal() {
  glClear( GL_COLOR_BUFFER_BIT );
  fastMode();
  setProjectionOrthogonal();

  // print text (fps, frametime, etc.)
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorWhite );// white color

  printInfoLine("elite clone");
  printInfoLine( "copyright 2006 - 2007, Melanie Schmidt & Jan-Philipp Kappmeier");
  printInfoLine( "" );
  printInfoLine( "Keyboard commands for local screen (in addition to usual keys):" );
  printInfoLine( "-only mouse input possible-" );
  printInfoLine( "Mouse actions:" );
  printInfoLine( "- go over a system to see information about it (only possible if system is in local range)" );
  printInfoLine( "- click on a system to perform jump (only possible if system is reachable)" );
  printInfoLine( "- confirm jump in messagebox (after clicking on a reachable system)" );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  restoreProjection();
}

/**
 * \fn	void App::drawHelpMarketplace()
 * \brief	Print detailed information for the market screen (within the space station).
 */
void App::drawHelpMarketplace() {
  glClear( GL_COLOR_BUFFER_BIT );
  fastMode();
  setProjectionOrthogonal();

  // print text (fps, frametime, etc.)
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorWhite );// white color

  printInfoLine("elite clone");
  printInfoLine( "copyright 2006 - 2007, Melanie Schmidt & Jan-Philipp Kappmeier");
  printInfoLine( "" );
  printInfoLine( "The marketplace:" );
  printInfoLine( "Here you can buy and sell tradegoods. " );
  printInfoLine( "Keyboard commands for marketplace screen:" );
  printInfoLine( "-only mouse input possible-" );
  printInfoLine( "Mouse actions:" );
  printInfoLine( "- click on tradegood button - mark the good for selling / buying " );
  printInfoLine( "- click on buy / sell button - buys / sells one unit of the market good" );
  printInfoLine( "" );
  printInfoLine( "Note that buttons can be inactive!" );
  printInfoLine( "" );
  printInfoLine( "Rules for the buying:" );
  printInfoLine( "- you can only buy goods that are available" );
  printInfoLine( "- you can only buy goods that fit into your cargo bay (only tonnes count)" );
  printInfoLine( "- you can only buy goods if you have enough money" );
  printInfoLine( "- there is an infinite amount of fuel" );
  printInfoLine( "- you cannot buy more fuel than fits into your tank" );
  printInfoLine( "" );
  printInfoLine( "Rules for selling:" );
  printInfoLine( "- you can only sell goods that you have in your cargo bay" );
  printInfoLine( "- fuel cannot be sold" );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  restoreProjection();
}

/**
 * \fn	void App::drawLocalSystems()
 * \brief	Draw the nearby local systems.
 * 			
 * A circle indicates the radius of the ship regarding its fuel. Reachable
 * systems are yellow, others are white. If mouse is over a system, detailed
 * information is shown.
 */
void App::drawLocalSystems() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // set hit object to none in case that no object is being hit
  if( mouse->isMouseHoveringActive() )
    mouse->setHittedObject( none );

  // information for gapsize and so on (writing text)
  int gap = 10;
  int size = tf2->getSize();
  int width = tf2->getWidth();
  int lines = 1;

  // set up an orthogonal screen for printing text and painting the board
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0,
    getWindow()->getWidth(),
    0,
    getWindow()->getHeight(),
    -100,
    100
    );
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  // find out maximal/minimal x and y positions
  // set minimal and maximal position in a way that the circle with the maximum-reachable distance is visible
  SystemInfo currentSystem = World::getWorld()->getSystemInfo();
  Position maxPos, minPos;
  // maxfuel = 7
  maxPos.x = currentSystem.pos.x + World::getWorld()->getMaxFuel()*2.5;//World::getWorld()->getPlayerStatus()->fuel  * 2.5;
  minPos.x = currentSystem.pos.x - World::getWorld()->getMaxFuel()*2.5;//World::getWorld()->getPlayerStatus()->fuel  * 2.5;
  if(minPos.x > 500)
    minPos.x = 0;
  maxPos.y = currentSystem.pos.y + World::getWorld()->getMaxFuel()*2.5;//World::getWorld()->getPlayerStatus()->fuel  * 2.5;
  minPos.y = currentSystem.pos.y - World::getWorld()->getMaxFuel()*2.5;//World::getWorld()->getPlayerStatus()->fuel  * 2.5;
  if(minPos.y > 500)
    minPos.y = 0;

  // size of the near environment
  int envWidth = maxPos.x - minPos.x;
  int envHeight = maxPos.y - minPos.y;

  // add some border value
  int border = 30;

  // calculate ratio
  float scaleFactor = min( (float)(getWindow()->getWidth()-border) / (float) envWidth, (float)(getWindow()->getHeight()-border) / (float) envHeight );
  float xOffset = max( border*0.5, (getWindow()->getWidth() - border-scaleFactor*envWidth )*0.5 );
  float yOffset = max( border*0.5, (getWindow()->getHeight()- border-scaleFactor*envHeight)*0.5 );

  // get all systems that fit into the screen!*0.5 --> therefore claculate some additional x and y space used
  float xPlus = max( 0.0f, (float)((float)(getWindow()->getWidth() -border) - ((maxPos.x - minPos.x)  * scaleFactor + xOffset)) / scaleFactor);
  float yPlus = max( 0.0f, (float)((float)(getWindow()->getHeight()-border) - ((maxPos.y - minPos.y)  * scaleFactor + yOffset)) / scaleFactor);
  vector<SystemInfo>  * localSystems = World::getWorld()->getSystems( max( 0.0f, minPos.x - xPlus ),
    maxPos.x + xPlus,
    max( 1.0f, minPos.y - yPlus ),
    maxPos.y + yPlus
    );

  fullMode();
  for( unsigned int i = 0; i < localSystems->size(); i++ ) {
    Star *sun = new Star( (localSystems->at(i).pos.x - minPos.x)  * scaleFactor + xOffset,
      getWindow()->getHeight()+border*0.5-((localSystems->at(i).pos.y - minPos.y)  * scaleFactor + yOffset),
      -20,
      1,
      star
      );
    sun->setLightColor( 1, 1, 0 );
    //sun->setTexture(  Textures::getTextures()->getTexture( star_sun ) );
    //sun->setTexture( World::getWorld()->getSystemStarTexture( localSystems->at(i).systemNumber ) );
    //sun->setTexture( star );
    sun->setAxis( 0, 1, 0 );
    sun->setSpeed( -0.006);
    sun->setRotateAngle( 0 );
    sun->setRotateSpeed( 0 );
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    sun->draw();
    glPopAttrib();
    delete sun;
  }
  fastMode();
  // print the system names
  glEnable( GL_BLEND );							// enable blending (for text), remainds active after resetting projection!
  glColor3fv( colorWhite );

  // position for mouse-position-rectangle
  Position rectPosStart;
  Position rectPosEnd;

  // print text for all planets.
  // 1. calculate positions of an rect lying over text that is to be printed
  // 2. check if mouse position is above, in that case set hit object info
  // 3. if that is the case, switch color
  // 4. print the text
  SystemInfo hoveredSystem;
  hoveredSystem.systemNumber = -1;// intialization to save if a system was hit
  bool alreadyHighlighted = false;
  for( unsigned int i=0; i<localSystems->size(); i++) {
    rectPosStart.x = (localSystems->at(i).pos.x - minPos.x)  * scaleFactor + xOffset; // left border
    rectPosEnd.y = getWindow()->getHeight()+border*0.5-((localSystems->at(i).pos.y - minPos.y)  * scaleFactor + yOffset); // lower border ----> print uses lower border as startposition!
    int paintPosy = rectPosEnd.y;
    // switch positions as the mouse coordinates start with 0 in the upper left corner!
    rectPosEnd.y = getWindow()->getHeight() - rectPosEnd.y;
    rectPosStart.y = rectPosEnd.y - size; // opper border, size is height of writing
    rectPosEnd.x = rectPosStart.x + width  * localSystems->at(i).name.length(); // right border

    if( mouse->isMouseOverRect( rectPosStart.x,
      rectPosEnd.x,
      rectPosStart.y,
      rectPosEnd.y
      )
      && !alreadyHighlighted
      && World::getWorld()->isSystemLocal(localSystems->at(i).systemNumber ) // perform this test in class!
      ) {
        // save hit info
        hoveredSystem = localSystems->at(i);
        mouse->setHittedObject( planet );
        mouse->setHittedObjectInfo( localSystems->at(i).systemNumber );
        // print system info
        if( World::getWorld()->isSystemReachable( localSystems->at(i).systemNumber ) )
          glColor3fv( colorRed );
        else
          glColor3fv( colorYellow ); // this will (maybe) lead to an brighter yellow because planet is drawn twice

        tf2->print( rectPosStart.x,
          paintPosy,
          localSystems->at(i).name.c_str()
          );
        //glColor3fv( colorWhite );
        alreadyHighlighted = true;
    } else
      // if system is local, use yellow color
      if( World::getWorld()->isSystemLocal( localSystems->at(i).systemNumber ) )
        glColor3fv( colorYellow );
      else
        glColor3fv( colorWhite );
    tf2->print( rectPosStart.x,
      paintPosy,
      localSystems->at(i).name.c_str()
      );
  }
  delete localSystems;

  // draw the planet information in an box under the planet (TODO: if the planet is at the bottom, maybe draw on top?
  // draw information only, if planet is local (that means, reachable with full fuel)
  if( hoveredSystem.systemNumber != -1 && World::getWorld()->isSystemLocal( hoveredSystem.systemNumber) ) {
    // set hovered object to none if the object is not reachable with current fuel amount
    if( !World::getWorld()->isSystemReachable( hoveredSystem.systemNumber ) )
      mouse->setHittedObject( none );
    // print 
    int infoPosx = (hoveredSystem.pos.x - minPos.x)  * scaleFactor + xOffset; // left border
    int infoPosy = getWindow()->getHeight()+border*0.5-((hoveredSystem.pos.y - minPos.y)  * scaleFactor + yOffset); // lower border ----> print uses lower border as startposition!

    // create strings for information output
    string government = "Government: " + World::getWorld()->getGovernmentString( hoveredSystem.government );
    string economy = "Economy: " + World::getWorld()->getEconomyString( hoveredSystem.economy );

    //tf2->print( gap, getWindow()->getHeight() - gap - lines++*size, "Position: (%d,%d)", sysinfo.pos.x, sysinfo.pos.y );
    tf2->print( infoPosx, infoPosy - lines++*size, economy.c_str() );
    tf2->print( infoPosx, infoPosy - lines++*size, "Population: %d", hoveredSystem.population );
    tf2->print( infoPosx, infoPosy - lines++*size, "Productivity: %d", hoveredSystem.productivity );
    tf2->print( infoPosx, infoPosy - lines++*size, "Technology level: %d", hoveredSystem.techLevel );
    tf2->print( infoPosx, infoPosy - lines++*size, government.c_str() );
  }

  // print input text
  glColor3fv( colorWhite );
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  // draw dashed cirle that indicates systems in reachable distance
  glDisable( GL_TEXTURE_2D );
  Shapes::drawCircleShapeDashed( (World::getWorld()->getSystemInfo().pos.x - minPos.x)  * scaleFactor + xOffset,
    getWindow()->getHeight()+border*0.5-((World::getWorld()->getSystemInfo().pos.y - minPos.y)  * scaleFactor + yOffset),
    0,
    720,
    World::getWorld()->getPlayerStatus()->fuel  * 2.5  * scaleFactor,
    colorLightGrey 
    );

  glEnable( GL_TEXTURE_2D );

  // restore procections
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

/**
 * \fn	void App::drawMarketplace()
 * \brief	Draws the market place.
 * 			
 *  Basically a table of goods, available amount in the station and on the cargo bay of the
 *  ship. The list is connected to the text elite game, but, however, fuel is visualized as
 *  a 'normal' good on the screen which is not the case for the original game.
 */

void App::drawMarketplace() {
  /* clear the screen  */
  glClearColor( 0.0f, 0.0f, 0.0f, 0.2f );
  glClear( GL_COLOR_BUFFER_BIT );

  /* the number of tradegoods is one higher than in the txtelite-game,
  because fuel is treated as a tradegood as well. when contacting
  the txtelite-game, the last tradegood must be done in a special case  */
  size_t numOfTradegoods = mktbuttons.size();

  for (unsigned int i =0; i < numOfTradegoods; i++)
    mktbuttons.at(i)->switchOn();
  buybutton->switchOn();
  sellbutton->switchOn();

  float tabularheight = 0.75;
  float tabular = 0.05;
  float headlines = tabular;
  float buysell = tabular+0.02;
  float infos = buysell+0.06;
  float helplines = tabularheight+infos+0.03;
  float helplineheight = 0.02;


  /* set up a ortho projection for 2D painting  */
  this->setProjectionOrthogonal();

  /* adjust button position to window size  */

  unsigned int wwidth = getWindow()->getWidth();
  unsigned int wheight = getWindow()->getHeight();

  unsigned int startx = wwidth / 4;
  unsigned int jumpy = wheight*tabularheight / (numOfTradegoods+1);

  for (unsigned int i=0; i < numOfTradegoods; i++)
  {	
    mktbuttons.at(i)->setCenter(startx,wheight-(i+1)*jumpy-tabular*wheight);
    mktbuttons.at(i)->setWindowDimensions(wwidth,wheight);
  }

  /* position buy and sell button under the markenplace items  */
  int leftdrift = 0;
  if (mktbuttons.size()>0) 
    leftdrift = (mktbuttons.at(0)->getButtonWidth())/2.0f;
  int rightdrift = (buybutton->getButtonWidth())/2.0f;

  buybutton->setCenter(startx-leftdrift+rightdrift, wheight-(numOfTradegoods+1)*jumpy-buysell*wheight);
  buybutton->setWindowDimensions(wwidth,wheight);

  rightdrift = leftdrift;
  leftdrift = (sellbutton->getButtonWidth())/2.0f;

  sellbutton->setCenter(startx-leftdrift+rightdrift, wheight-(numOfTradegoods+1)*jumpy-buysell*wheight);
  sellbutton->setWindowDimensions(wwidth,wheight);

  /* initialize two vectors for saving which items are buyable and sellable  */

  std::vector<bool> buyable;
  std::vector<bool> sellable;
  buyable.reserve(numOfTradegoods);
  sellable.reserve(numOfTradegoods);

  {
    int freeHoldspace = World::getWorld()->getFreeHoldspace();
    for ( unsigned int i = 0; i < numOfTradegoods-1; i++)
    {
      buyable.push_back( !World::getWorld()->tradegoodIsInTons(i) || (freeHoldspace > 0) );
      sellable.push_back( true );
    }
  }

  /* entries for fuel  */
  buyable.push_back( World::getWorld()->getMaxFuel() != World::getWorld()->getFuelAmount());
  sellable.push_back(false);

  /* print prices and availabilities of actual marketplace  */

  std::vector<MarketplaceItem> * mktplace = World::getWorld()->getCurrentMarketplace();

  glPushAttrib( GL_ALL_ATTRIB_BITS );

  glEnable( GL_TEXTURE_2D );
  glDisable( GL_LIGHTING );
  glEnable( GL_BLEND );

  int currentCash = World::getWorld()->getCurrentCash();

  /* print prices  */
  int downdrift = 0;
  if (mktbuttons.size()>0)
    downdrift = (mktbuttons.at(0)->getButtonInnerHeight())/2.0f;

  rightdrift = 0;

  if (mktbuttons.size()>0) 
    rightdrift = (mktbuttons.at(0)->getButtonWidth())*0.75;

  glColor3fv(colorWhite);
  tf2->print(startx+rightdrift, wheight-headlines*wheight-downdrift, "Price");

  for (unsigned int i=0; i < numOfTradegoods; i++)
  {
    double price =0;
    if (i < numOfTradegoods-1)
    {
      price = mktplace->at(i).price;
    }
    else
    {
      price = World::getWorld()->getFuelCost();
    }

    if (currentCash >= price)
      glColor3fv(colorGreen);
    else
    {
      buyable[i] = false;
      glColor3fv(colorWhite);
    }

    tf2->print(startx+rightdrift, wheight-(i+1)*jumpy-tabular*wheight-downdrift, "%3.1f",price);

  }

  /* print how much is available  */

  rightdrift = 0;

  if (mktbuttons.size()>0) 
    rightdrift = (mktbuttons.at(0)->getButtonWidth())*1.5;

  glColor3fv(colorWhite);
  tf2->print(startx+rightdrift, wheight-headlines*wheight-downdrift, "Market");

  for (unsigned int i=0; i < numOfTradegoods-1; i++)
  {

    if (mktplace->at(i).amount > 0)
      glColor3fv(colorGreen);
    else
    {
      glColor3fv(colorWhite);
      buyable[i] = false;
    }

    std::string sunit = World::getWorld()->getTradegoodDescription(i).unit;
    const char * unit = sunit.c_str();

    tf2->print(startx+rightdrift, wheight-(i+1)*jumpy-tabular*wheight-downdrift, "%d %s",mktplace->at(i).amount, unit);

  }

  /* print how much we have of each tradegood  */

  std::vector<int> * currentCargo = World::getWorld()->getCurrentCargo();
  float fuelamount = World::getWorld()->getFuelAmount();

  rightdrift = 0;

  if (mktbuttons.size()>0) 
    rightdrift = (mktbuttons.at(0)->getButtonWidth())*2.25;

  glColor3fv(colorWhite);
  tf2->print(startx+rightdrift, wheight-headlines*wheight-downdrift, "Cargo");

  for (unsigned int i=0; i < numOfTradegoods; i++)
  {
    if (i < numOfTradegoods-1)
    {
      if (currentCargo->at(i)>0)
        glColor3fv(colorGreen);
      else
      {
        glColor3fv(colorWhite);
        sellable[i] = false;
      }

      std::string sunit = World::getWorld()->getTradegoodDescription(i).unit;
      const char * unit = sunit.c_str();

      tf2->print(startx+rightdrift, wheight-(i+1)*jumpy-tabular*wheight-downdrift, "%d %s",currentCargo->at(i),unit);
    }
    else
    {
      /* print fuel amount here as well, but never green, because it cannot be sold  */
      glColor3fv(colorWhite);
      tf2->print(startx+rightdrift, wheight-(i+1)*jumpy-tabular*wheight-downdrift, "%3.1f %s",fuelamount,"LY");
    }
  }

  /* print current cash, holdspace and fuel  */
  glColor3fv(colorWhite);

  leftdrift = 0;
  if (mktbuttons.size()>0) 
    leftdrift = (mktbuttons.at(0)->getButtonWidth())/2.0f;

  if (fuelamount == World::getWorld()->getMaxFuel())
    tf2->print(startx-leftdrift, wheight-(numOfTradegoods+1)*jumpy-infos*wheight,"Cash: %3.1fFuel: %3.1f (max)HoldSpace: %i",World::getWorld()->getCurrentCash(),fuelamount,World::getWorld()->getFreeHoldspace());
  else
    tf2->print(startx-leftdrift, wheight-(numOfTradegoods+1)*jumpy-infos*wheight,"Cash: %3.1fFuel: %3.1fHoldSpace: %i",World::getWorld()->getCurrentCash(),fuelamount,World::getWorld()->getFreeHoldspace());

  /* print help lines  */
  tf2->print(startx-leftdrift, wheight-helplines*wheight,"- Click on item you want to buy / sell.");
  tf2->print(startx-leftdrift, wheight-helplineheight*wheight-helplines*wheight,"- Click on buy / sell to buy / sell one unit of the chosen tradegood.");

  /* activate buttons if tradegood can be bought / sold  */

  for (unsigned int i=0; i < numOfTradegoods; i++)
  {
    if (!sellable[i] && !buyable[i]) 
      mktbuttons.at(i)->deactivateButton();
    else
      mktbuttons.at(i)->activateButton();
  }

  if (chosenButton > -1 && buyable[chosenButton])
    buybutton->activateButton();
  else
    buybutton->deactivateButton();
  if (chosenButton > -1 && sellable[chosenButton])
    sellbutton->activateButton();
  else
    sellbutton->deactivateButton();

  /* inform buttons about mouse clicks  */
  if (getWindow()->getMouseClick())
  {	 
    ClickableObject::clickAll();
    getWindow()->setMouseClick(false);

    bool noButton = true;
    for (unsigned int i = 0; i < numOfTradegoods; i++)
    {
      if (mktbuttons.at(i)->isClicked())
      {
        noButton = false;
        if ( chosenButton == -1 )
        {
          chosenButton = i;
        }
        else
        {	if ( chosenButton != i )
        {
          mktbuttons.at(chosenButton)->noClick();
          chosenButton = i;
        }	
        }
      }
    }
    if (noButton)
      chosenButton = -1;

  }

  if (buybutton->isClicked())
  {
    if ((unsigned)chosenButton == numOfTradegoods-1) World::getWorld()->buyFuel(1);
    if ((unsigned)chosenButton < numOfTradegoods-1) World::getWorld()->performPurchase(chosenButton,1);
    buybutton->noClick();
  }

  if (sellbutton->isClicked())
  {
    if ((unsigned)chosenButton < numOfTradegoods-1) World::getWorld()->performSale(chosenButton,1);
    sellbutton->noClick();
  }

  /* draw all buttons  */
  ClickableObject::drawAll();

  for (unsigned int i =0; i < numOfTradegoods; i++)
    mktbuttons.at(i)->switchOff();
  buybutton->switchOff();
  sellbutton->switchOff();

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glPopAttrib();

  /* restore matrices  */
  this->restoreProjection();
}

/**
 * \fn	void App::drawSystemInfo()
 * \brief	Draws the info screen of the current system.
 * 			
 * Contains some information in text mode such as population. The system itself is displayed in
 * perspective projection. The system can be rotated using mouse interaction. The circles of
 * the orbits for the planets are drawn.
 */
void App::drawSystemInfo() {
  fullMode();
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // set up perspective projection, after that it should be possible to draw in a rect -50, 50 both, in x and y
  float projectionSize = 1000.0f;
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  static float nearClip = -1;// set to negative, test if zero does not work with float!
  static float clipSize; // size of near clipping plane, calculated in the next block
  if( nearClip <= 0) {
    // folgendes berechnet mit der def. von cosinus die länge der einen seite des frustums bis zur y-achse.
    // es ist gewünscht, daß an den achsen eine breite des frustums von projectionSize vorliegt.
    double val = (M_PI  * 76.5)/ 180.0;
    double len1 = (projectionSize  * (double)0.5) / ( cos(val) );
    // wende satz von pythagoras an um die länge auf der z-achse bis in den koordinaten ursprung des gleichen
    // dreiecks zu berechne
    double len2 = sqrt( (double)(projectionSize*0.5)*(double)(projectionSize*0.5) + len1*len1 );
    nearClip = (float)(len2-projectionSize*0.5);
    // calculate clip size
    clipSize = (nearClip / len2)  * projectionSize*0.5;
  }
  glFrustum( -clipSize*getWindow()->getAspect(), clipSize*getWindow()->getAspect(), -clipSize, clipSize, nearClip, nearClip+projectionSize+1000);
  cameraSystemView->setPos( cameraSystemView->getPos().x, cameraSystemView->getPos().y, (-1*nearClip)-(projectionSize*0.5));
  Vector3 v( 0, 0, 1 );
  cameraSystemView->setView(v);
  cameraSystemView->look();

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  // szene drehen
  glRotatef( systemInfoViewAngle, 1, 0, 0 );
  glRotatef( systemInfoViewAngle2, 0, 0, 1 );


  vector<PlanetInfoGraphical> planets;
  SystemInfo sysinfo;
  if( devmode ) {
    sysinfo = World::getWorld()->getSystemInfo( currentSystemInfoNumber );
    planets = *World::getWorld()->getSystemPlanets( currentSystemInfoNumber );
  } else {
    sysinfo = World::getWorld()->getSystemInfo();
    planets = *World::getWorld()->getSystemPlanets();
  }

  // retrieve size
  float sysSize = 0;
  for( unsigned int i = 0; i < planets.size(); i++) {
    PlanetInfoGraphical planet = planets[i];
    if( planet.distance > sysSize )
      sysSize = planet.distance;
  }
  size_t numberOfPlanets = planets.size();


  float scaleFactor = (projectionSize  * 0.5) / sysSize;

  // this scales the scene to be full-screen-wide (if size is wider than tall)
  //scaleFactor *= getWindow()->getAspect();

  // star
  Star *sun = new Star( 0, 0, 0, 2*zoomFactor*10, star );
  sun->setLightColor( 1, 1, 0);
  sun->setAxis( 0, 1, 0 );
  sun->setSpeed( -0.006);
  sun->setRotateAngle( 0 );
  sun->setRotateSpeed( 0 );
  glPushAttrib( GL_ALL_ATTRIB_BITS );
  sun->draw();
  glPopAttrib();
  delete sun;

  // draw planets
  for( unsigned int i=0; i < planets.size(); i++) {
    PlanetInfoGraphical planet = planets[i];
    Planet *erde2 = new Planet( planet.distance  * scaleFactor  * zoomFactor + zoomFactor*10 , 0, 0, 0.7*zoomFactor *10, planet1+i);
    // WARNING: this is possibly unsave. it only works if all planets in the texture-slot are in order!
    //erde2->setTexture( planet1 + i );
    erde2->setAxis( 0, 1, 0 );
    erde2->setSpeed( planet.spinningSpeed );
    erde2->setRotateAngle( planet.rotateAngle );
    erde2->setRotateSpeed( -1.0  * planet.rotateSpeed  * 100);
    erde2->setRadiusLineDrawing( true );
    //erde2->setRotateSpeed ( erde2->getRotateSpeed() );
    erde2->draw();
    delete erde2;
  }

  // restore projection
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
  glViewport( 0, 0, getWindow()->getWidth(), getWindow()->getHeight() );

  // switch to fast mode and orthogonal screen to print the information text
  fastMode();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0, getWindow()->getWidth(), 0, getWindow()->getHeight(), -1, 1 ); // full size orthogonal screen
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  // print information
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorRed );// red color

  // create strings for information output
  string name = "Name: " + sysinfo.name;
  string government = "Government: " + World::getWorld()->getGovernmentString( sysinfo.government );
  string description = "Description: " + sysinfo.description;
  string economy = "Economy: " + World::getWorld()->getEconomyString( sysinfo.economy );

  printInfoLine( "System Info:" );
  printInfoLine( "" );
  printInfoLine( name.c_str() );
  if( devmode )
    printInfoLine( "Systemnumber: %d", sysinfo.systemNumber );
  printInfoLine( "Number of planets: %d", numberOfPlanets );
  if( devmode )
    printInfoLine( "Position: (%d, %d)", sysinfo.pos.x, sysinfo.pos.y );
  printInfoLine( economy.c_str() );
  printInfoLine( "Population: %d", sysinfo.population );
  printInfoLine( "Productivity: %d", sysinfo.productivity );
  printInfoLine( "Radius: %d", sysinfo.radius );
  printInfoLine( "Technology level: %d", sysinfo.techLevel );
  printInfoLine( government.c_str() );
  printInfoLine( description.c_str() );

  printInfoBox( true );

  // output if inputmode is on
  if( m_inputmode ) {
    this->ti->print();
  }

  glDisable( GL_BLEND );

  // restore procections
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

/**
 * \fn	bool App::initialize()
 * \brief	Initializing of the application.
 * 			
 * Mainly the OpenGL-System is initialized. Objects, textures are loaded and some needed
 * objects, e.g. for printing, are initialized. This happens everytime the window is
 * created, that means  it happens also if the application toggles between fullscreen
 * and windowed mode.
 * \return	true if it succeeds, false if it fails.
 */
bool App::initialize() {
  // print a loading information. no textures are present so use a bitmap font object
  bf = new BitmapFont( getWindow()->getDC() );
  bf->buildFont( 20 );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.2f );
  glClear( GL_COLOR_BUFFER_BIT );
  glLoadIdentity();					 // reset the view
  glTranslatef( 0.0f, 0.0f, -1.0f );		// move a bit into the screen
  glColor3fv( colorWhite );// white color
  glRasterPos2f(-0.5,0.3);
  bf->print("Progetto per Grafica al Calcolatore");
  glRasterPos2f(-0.5, 0.15);
  bf->print("Jan-Philipp Kappmeier");
  glRasterPos2f(-0.5, 0.1);
  bf->print("Melanie Schmidt");
  glRasterPos2d(-0.5,-0.3);
  bf->print("Loading in progress. Please stand by ...");
  glRasterPos2d(-0.5,-0.4);
  bf->print("Press 'help' at console for possible commands. (To show console press 'Return'.)");
  glFlush();
  getWindow()->swap();

  // OpenGL initialization, loading of memory-resistent textures
  initializeGFX();
  loadAllNeededTextures();

  // create quadric, will be used for skysphere
  quadObj = gluNewQuadric();
  gluQuadricDrawStyle( quadObj, GLU_FILL );
  gluQuadricTexture( quadObj, GL_TRUE );

  // load texturefonts
  tf = new TextureFont( font2 );
  tf->buildFont( 16, 8, 16, 16, 10 );
  tf2 = new TextureFont( font1 ); 
  tf2->buildFont( 16, 8, 16, 12, 8 );

  // create objects
  ti = new TextInput( tf2 );
  con = new Console();
  frustum = new Frustum( getWindow()->getFrustum() );
  mouse = new MouseHandler();
  laserHeatBar = new HudLoadBar( 0.0f, 100.0f, 0.0f );
  laserHeatBar->setSlopePixel( 10 );

  // load game
  loadSystemTextures();
  initializeObjects();
  initMarketplace();

  World::getWorld()->initialize();
  player = World::getWorld()->getPlayer();
  //player->setView( Vector3(0, 0, 1) );
  //player->setPos( 10000,10000,10000 );

  speedBar = new HudLoadBar( player->getMinSpeed(), player->getMaxSpeed(), player->getSpeed() );
  speedBar->setSlopePixel( 10 );

  // delete bitmap font
  delete bf;
  resizeDraw( true );// enable repainting while resizing

  //SpaceShip *ship;
  //ship = World::getWorld()->createNewSpaceShip( ball, 1 );
  //ship->setPos( 10010, 10010, 10050 );
  //ship->setSpeed( 0.00f );

/*  ship = World::getWorld()->createNewSpaceShip( anaconda, 1 );
  ship->setPos( 5, 5, 4150 );
  ship->setSpeed( 0.01f );
  ship = World::getWorld()->createNewSpaceShip( missile, 1 );
  ship->setPos( -5, 0, 4150 );
  ship->setSpeed( 0.01f );
  ship = World::getWorld()->createNewSpaceShip( thargoid, 1 );
  ship->setPos( 0, 5, 4150 );
  ship->setSpeed( 0.01f );*/
  //ship = World::getWorld()->createNewSpaceShip( tieDroid, 1 );
  //ship->setPos( 0, -5, 4150 );
  //ship->setSpeed( 0.01f );

  loader = new TextureLoaderTGA();
  return true;
}

/**
 * \fn	void App::initializeGFX( void )
 * \brief	Sets up the basic OpenGL setting.
 */
void App::initializeGFX( void ) {
  // do all the opengl-initialization
  glShadeModel( GL_SMOOTH );							// activate smooth-shading (Gauraud)
  glClearDepth( 1.0f );									// initialize depth-buffer precision
  glEnable( GL_DEPTH_TEST );							// enable depth buffer. necessary?
  glDepthFunc( GL_LEQUAL );								// quality of depht-testing
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	// perspective calculations with high precision
  //glEnable(GL_CULL_FACE); // enable face culling
  glEnable( GL_TEXTURE_2D ); // enable 2D-texturing
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );

  // inital lights globally
  GLfloat ambient_light[] = { 0.0, 0.0, 0.0, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);// no ambient light, the sun creates it
  GLfloat infinite[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, infinite);// viewer position is infinite
  GLfloat one_side[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, one_side);// only light one side of the objects

  // initialize the actual lights
  GLfloat ambient_light_sun[] = { 0.35f, 0.35f, 0.35f, 1.0};
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light_sun );
  GLfloat specular_light_sun[] = { 0.0f, 0.0f, 0.0f, 1.0};
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular_light_sun );
}

/**
 * \fn	void App::initMarketplace()
 * \brief	Initialises data structures for the marketplace.
 */

void App::initMarketplace() {

  TextureFont * tf3 = new TextureFont( font1 ); 
  tf3->buildFont( 16, 8, 16, 12, 8 );

  int innerBorder = 2;
  int outerBorder = 2;

  int numOfTradegoods = World::getWorld()->getNumberOfTradegoods()+1;

  mktbuttons.reserve(numOfTradegoods+2);

  for (int i=0; i < numOfTradegoods-1; i++)
  {	
    mktbuttons.push_back(new Button(innerBorder,outerBorder,mouse,World::getWorld()->getTradegoodDescription(i).name ,tf3,12));
  }

  mktbuttons.push_back(new Button(innerBorder,outerBorder,mouse,"Fuel",tf3,12));

  buybutton = new Button(innerBorder,outerBorder,mouse,"Buy",tf3,4);
  sellbutton = new Button(innerBorder,outerBorder,mouse,"Sell",tf3,4);

  chosenButton = -1;
}

/**
 * \fn	void App::initializeObjects()
 * \brief	Initializes the 3d objects possible occuring in the world.
 */

void App::initializeObjects() {
  FileAccess::loadObject( adder );
  FileAccess::loadObject( anaconda );
  FileAccess::loadObject( ball );
  FileAccess::loadObject( coriolis );
  FileAccess::loadObject( missile );
  FileAccess::loadObject( thargoid );
  FileAccess::loadObject( tieDroid );
}

/**
 * \fn	void App::update( DWORD deltaTime )
 * \brief	Updates the game status if there is time and nothing else to do.
 * \param	deltaTime	the time in milliseconds that has passed since the last call
 */
void App::update( DWORD deltaTime ) {
  if( gameOver ) {
    // check if any key was pressed
    for( unsigned int i=0; i <256; i++)
      if( getWindow()->keyPressed( i ) )
        this->terminate();
    return;
  }
  // do some update stuff which is generally executed
  getFrameCounter()->update(); // update framecounter to get actual framerate
  mouse->setMousePosition( getWindow()->getMousePosition());
  mouse->setMouseHovering( false );

  // check the console commandos, available for all game modes
  if( getWindow()->keyPressed( 13 ) ) { // go in input mode if 'Return' is pressed
    if( !m_inputmode ) {
      m_inputmode = true;
      ti->reset();
    } else {
      // send command to console and call local member function
      consoleCommand * ptr = table;
      while( ptr->command && ptr->arrayAdress > -1) { // searches until the end is found
        if( Console::getCommand( ti->getText() ) == ptr->command ) {
          (this->*fptr[ptr->arrayAdress])();
        }
        ptr++;
      }
      m_inputmode = false;
    }
    getWindow()->keyUp( 13 );
  }

  if( m_inputmode ) { // text input mode is on, all pressed keys go there
    performConsoleInput();
  } else {// inputmode off, normal commandos and updates. use the different update functison
    // first thing, test if the mode should be switched
    // if that happend, set new screen mode and quit function, now update is performed
    if( getWindow()->keyPressed( 69 ) ) { // test if e was pressed --> show 3d environment
      setScreen( currentSystem ); // show 3d-view of current system
      getWindow()->keyUp( 69 );
      return;
    }
    if( getWindow()->keyPressed( 73 ) ) { // test if i was pressed --> show info mode
      setScreen (systemInfo ); // show system information
      getWindow()->keyUp( 73 );
      return;
    }
    if( getWindow()->keyPressed( 76 ) ) { // test if l was pressed --> show local mode
      setScreen( localSystems ); // local info anzeigen
      getWindow()->keyUp( 76 );
      return;
    }

    if( getWindow()->keyPressed( 80 ) ) { // test if p was pressed --> create screenshot
      // allocate memory
      unsigned char *image = 0;
      image = new unsigned char[getWindow()->getWidth() * getWindow()->getHeight() * 3];

      // read the image data
      glReadPixels( 0, 0, getWindow()->getWidth(), getWindow()->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, image );
      TextureLoader * loader = new TextureLoaderTGA();
      loader->load( image, getWindow()->getWidth(), getWindow()->getHeight(), 3);
      // try to open file
      char space[32];
      ifstream file;
      do {
        sprintf_s( space, "screenshot%d.tga", rand() );
        file.open( space, ios::binary );
      } while( file );
      file.close();
      loader->write( space );

      // load as image
       Textures::getTextures()->loadTexture( rearview, loader );
      delete[] image;
      getWindow()->keyUp( 80 );
      // does not work, however
      //TextureLoaderTGA* tga = reinterpret_cast<TextureLoaderTGA*>(loader);
      //delete tga;
      return;
    }

    if( DEV_MODE && getWindow()->keyPressed( 78 ) ) { // test if m was pressed --> show marketplace
		currentScreen = marketplace; // enter marketplace
		getWindow()->keyUp( 78 );
		return;
    }

    // let handle other input by the appropriate perform function
    switch( currentScreen ) {
case credits:
  break;
case currentSystem:
  performCurrentSystemInput( deltaTime );
  break;
case help:
  break;
case helpEnvironment:
  break;
case helpInfo:
  break;
case helpLocal:
  break;
case loadLevel:
  break;
case loadProgram:
  break;
case localSystems:
  performLocalSystemsInput( deltaTime );
  break;
case systemInfo:
  performSystemInfoInput( deltaTime );
  break;
    }
  }
  // call the propriate update functions for the different screen modes
  switch( currentScreen ) {
case credits:
  break;
case currentSystem:
  updateCurrentSystem( deltaTime );
  break;
case help:
  break;
case helpEnvironment:
  break;
case helpInfo:
  break;
case helpLocal:
  break;
case loadLevel:
  break;
case loadProgram:
  break;
case localSystems:
  updateLocalSystems( deltaTime );
  break;
case systemInfo:
  updateSystemInfo( deltaTime );
  break;
  }
}

/**
 * \fn	void App::stationWasHit()
 *
 * \brief	Activate sall police ships, if the station was hit.
 *
 */

void App::stationWasHit(){
	vector <PoliceShip*> * pships = World::getWorld()->getPoliceShips();
	for (unsigned int i = 0; i < pships->size(); i++)
	{
		pships->at(i)->stationWasHit();
	}
	delete pships;
}

/**
 * \fn	void App::updateCurrentSystem( DWORD deltaTime )
 * \brief	Updates the position of the player and removes object, that have been destroyed
 * 			in the previous frame.
 * 			
 * Checks if collisions occured, both between ships and the player and between lasers and
 * ships.
 * \param	deltaTime	passed time in milliseconds
 */
void App::updateCurrentSystem( DWORD deltaTime ) {
  // update the game environment in 3d-system-view
  // move camera and adjust frustum
  player->update( deltaTime );
  frustum->setFrustumInformation( getWindow()->getFrustum() );
  frustum->update( player );

  // delete all objects that hav hit each other
  World::getWorld()->deleteCrashedShips();
  World::getWorld()->deleteDestroyedShips();

  // test if on the way to station
  SpaceStation *coriolis = World::getWorld()->getStation();
  if( coriolis->test( player->getPos() ) ) {
    setScreen( marketplace ); // enter marketplace
    return;
  }

  // get all the objects from the world and check if an object was hit or if the player collided with
  // an object. the objects have to be deleted after performing the tests
  vector<GraphicsObject*> *objects = World::getWorld()->getGraphicObjects();
  for( unsigned int i=0; i < objects->size(); i++ ) {// check if we hit another space ship
    Vector3 v = objects->at(i)->getPos() - player->getPos();
    float distance = v.length();
    float shieldDist = objects->at(i)->getRadius() + player->getRadius();
    if( distance <shieldDist) {
      printInfoLine( "You hit an object");
      gameOver = true;
    }
  }
  delete objects;

  vector<SpaceShip*> *ships = World::getWorld()->getSpaceShips();
  for( unsigned i=0; i < ships->size(); i++ ) {
    ships->at(i)->update( deltaTime );
    if( ships->at(i)->isShooting() ) {
      //printInfoLine( "You were hit by laser." );
      player->receiveHitFront( deltaTime );
      if( player->getFrontShieldPower() <= 0 )
        //printInfoLine( "You are dead!" );
        gameOver = true;
    }
  }

  if( laserActive ) { // check if laser is active and we hit an object
    player->setShoot( true );
    laserHeatBar->setCurrent( player->getLaserHeat() );

	SpaceStation * myStation = World::getWorld()->getStation();

    for( unsigned int i=0; i < ships->size()+1; i++ ) {
      // check if we hit the the ships by laser
      // ships not in frustum can't be hit by laser
	  
	  // support for laser hit test with station
	  Vector3 myPos;
	  float myRadius;
	  if (i < ships->size())
	  {
		  myPos = ships->at(i)->getPos();
		  myRadius = ships->at(i)->getRadius();
	  }
	  else
	  {
		  myPos = myStation->getPos();
		  myRadius = myStation->getRadius();
	  }

      CullingLocation res = getFrustum()->isSphereInFrustum( myPos, myRadius );
      if( res != outside ) {// ships can't be hit if they are outside the frustum
        // (not optimized) calculation of the ray-sphere intersection between the players laser and space ships
        Vector3 v = player->getView();      // the current view direction 
        Vector3 c = myPos;//ships->at(i)->getPos(); // the center of sphere (object position)
        Vector3 p = player->getPos();       // the ray initial point
        Vector3 u = c-p;
        float scalar = v.innerProduct( u );
        scalar = scalar / v.length();
        Vector3 projectionPosition = p + v  * scalar;
        Vector3 distance = projectionPosition - c;
        float distanceToRay = distance.length();
		if( distanceToRay < myRadius ) { //ships->at(i)->getRadius() ) {
          // check if projected point is behind or in front of eye point
          // pfact is the factor used in the line equation p + pfact*v = projectionPosition.
          // it is the same for all three elements of the vector, but it can't be calculated if the entry is zero.
          // it can not happen that all three elements of v are zero because v is an orthonormal vector
          float pfact;
          if(v.x != 0)
            pfact = (projectionPosition.x - p.x) / v.x;
          else if(v.y != 0)
            pfact = (projectionPosition.y - p.y) / v.y;
          else if(v.z != 0)
            pfact = (projectionPosition.z - p.z) / v.z;
          if( pfact > 0 ) {
            printInfoLine( "Object hit by laser." );
			if (i < ships->size())
				ships->at(i)->receiveHitFront( deltaTime );
			else
				this->stationWasHit();
          }
          // missing: calculate hit-distance (for accurate drawing of laser ---> if it hit's an object, 
          // the laser should only be painted 'til it reaches this object
          // also it's possible to hit more than one object
        } 
      }
    }
  } else {// no laser is used
    player->setShoot(false);
    laserHeatBar->setCurrent( player->getLaserHeat() );
  }
  delete ships;
}

void App::updateSystemInfo( DWORD deltaTime ) {
}

void App::updateLocalSystems( DWORD deltaTime ) {
}

/**
 * \fn	void App::performConsoleInput()
 * \brief	Updates the console text, if a key has been pressed.
 */
void App::performConsoleInput() {
  // checks for pressed keys. as there is no checking for capital letters, this checking is manually done
  if( getWindow()->keyPressed( VK_BACK ) ) {
    ti->back();
    getWindow()->keyUp( VK_BACK );
  }
  for( int i = 32; i < 128; i++) {
    if( getWindow()->keyPressed( i ) ) {
      // check for capital etc
      if( i > 64 && i < 92 && !getWindow()->keyPressed( 16 ) ) { // 16 = VK_CAPITAL<-- does not work!
        ti->newLetter( i + 32);
        getWindow()->keyUp(i);
      } else {
        ti->newLetter( i );
        getWindow()->keyUp( i );
      }
    }
  }
}

/**
 * \fn	void App::performCurrentSystemInput( DWORD deltaTime )
 * \brief	Checks, if keys have been pressed and react, if necessary.
 * \param	deltaTime	time passed in milliseconds
 */
void App::performCurrentSystemInput( DWORD deltaTime ) {
  // rear view
  if( getWindow()->keyPressed( 66 ) ) { // test if b was pressed --> rear view
    player->toggleRearView();
    getWindow()->keyUp( 66 );
  }
  if( getWindow()->keyPressed( 77 ) ) { // test if m was pressed --> shoot a missile if possible
    if( player->getMissileCount() > 0 && player->getSpeed() > 0 ) {
      SpaceShip *m = World::getWorld()->createNewSpaceShip( missile, 0.1 );
      m->setMaxSpeed( player->getMaxSpeed() + player->getMaxSpeed()*0.3 );
      m->setMinSpeed( 0 );
      m->setSpeed( player->getSpeed() + player->getMaxSpeed()*0.3 );
      m->setView( player->getView() );
      m->setPos( player->getPos() + player->getView()  * ( player->getRadius() * 10  + 1)  );
      player->useMissile();
    }
    getWindow()->keyUp( 77 );
  }

  // laser
  if( getWindow()->keyPressed( 32 ) ) { // test if space was pressed
    if( !player->isOverHeated() )
      laserActive=true;
    else
      laserActive=false;
  } else {
    laserActive=false;
  }

  // enable frame limiter. frame limit is 100 FPS
  if( getWindow()->keyPressed( 70 ) ) { // test if f was pressed
    if( getFrameLimit() == 0 )
      setFrameLimit( 100 );
    else
      setFrameLimit( 0 );
    getWindow()->keyUp( 70 );
  }

  // control movement
  if( getWindow()->keyPressed( VK_UP ) ) { // test if up was pressed
    player->pitch( deltaTime  * handlingSpeedMultiplyer );
  }
  if( getWindow()->keyPressed( VK_DOWN ) ) { // test if down was pressed
    player->pitch( -(float)deltaTime  * handlingSpeedMultiplyer );
  }
  if( getWindow()->keyPressed( VK_RIGHT ) ) { // test if right was pressed
    player->roll( deltaTime  * handlingSpeedMultiplyer );
  }
  if( getWindow()->keyPressed( VK_LEFT ) ) { // test if left was pressed
    player->roll( -(float)deltaTime  * handlingSpeedMultiplyer );
  }

  // speed
  if( getWindow()->keyPressed( 187 ) ) { // test if plus was pressed --> accelerate
    player->accelerate( deltaTime  * handlingSpeedMultiplyer );
    speedBar->setCurrent( player->getSpeed() );
    //getWindow()->keyUp( 187 );
  }
  if( getWindow()->keyPressed( 189 ) ) { // test if minus was pressed --> decrease speed
    player->accelerate( -(float)deltaTime  * handlingSpeedMultiplyer );
    speedBar->setCurrent( player->getSpeed() );
    //getWindow()->keyUp( 189 );
  }

  // development movement: use wasd control for moving in x and y direction
  if( getWindow()->keyPressed( 87 ) ) { // testen if w was pressed
    player->moveUp( deltaTime  * handlingSpeedMultiplyer  * 100);
  }
  if( getWindow()->keyPressed( 65 ) ) { // test if a was pressed
    player->moveLeft( deltaTime  * handlingSpeedMultiplyer  * 100);
  }
  if( getWindow()->keyPressed( 83 ) ) { // test if s was pressed
    player->moveDown( deltaTime  * handlingSpeedMultiplyer  * 100);
  }
  if( getWindow()->keyPressed( 68 ) ) { // test if d was pressed
    player->moveRight( deltaTime  * handlingSpeedMultiplyer  * 100);
  }
}

void App::performLocalSystemsInput( DWORD deltaTime ) {

}

/**
 * \fn	void App::performSystemInfoInput( DWORD deltaTime )
 * \brief	Checks for pressed keys in info screen.
 * \param	deltaTime	passed time in milliseconds
 */
void App::performSystemInfoInput( DWORD deltaTime ) {
  // switch system in debug mode
  if( getWindow()->keyPressed( 66 ) ) { // testen if b was pressed
    currentSystemInfoNumber = currentSystemInfoNumber++%256;
    getWindow()->keyUp( 66 );
    // load new textures for the new system
    loadSystemTextures();
  }

  // change viewing angle
  if( getWindow()->keyPressed( VK_UP ) ) { // // test if up was pressed
    systemInfoViewAngle++;
    getWindow()->keyUp( VK_UP );
  }
  if( getWindow()->keyPressed( VK_DOWN ) ) { // test if down was pressed
    systemInfoViewAngle--;
    getWindow()->keyUp( VK_DOWN );
  }
  if( getWindow()->keyPressed( VK_RIGHT ) ) { // test if right was pressed
    systemInfoViewAngle2++;
    getWindow()->keyUp( VK_RIGHT );
  }
  if( getWindow()->keyPressed( VK_LEFT ) ) { // test if left was pressed
    systemInfoViewAngle2--;
    getWindow()->keyUp( VK_LEFT );
  }

  // move camera
  // speed
  if( getWindow()->keyPressed( 187 ) ) { // // test if plus was pressed
    zoomFactor += 0.02;
    getWindow()->keyUp( 187 );
  }
  if( getWindow()->keyPressed( 189 ) ) { // // test if minus was pressed
    zoomFactor -= 0.02;
    getWindow()->keyUp( 189 );
  }

  // development movement: use wasd control for moving in x and y direction
  if( getWindow()->keyPressed( 87 ) ) { // test if w was pressed
    cameraSystemView->moveDown( deltaTime  * handlingSpeedMultiplyer  * 2);
  }
  if( getWindow()->keyPressed( 65 ) ) { // test if a was pressed
    cameraSystemView->moveRight( deltaTime  * handlingSpeedMultiplyer  * 2);
  }
  if( getWindow()->keyPressed( 83 ) ) { // test if s was pressed
    cameraSystemView->moveUp( deltaTime  * handlingSpeedMultiplyer  * 2);
  }
  if( getWindow()->keyPressed( 68 ) ) { // test if d was pressed
    cameraSystemView->moveLeft( deltaTime  * handlingSpeedMultiplyer  * 2);
  }
}

/**
 * \fn	void App::loadSystemTextures()
 * \brief	Loads textures for the current system.
 * 			
 * Called whenever a hyperjump has been performed. In developer mode,
 * detailed information on the loaded files is given, otherwise only a percent information.
 */

void App::loadSystemTextures() {
   Textures::getTextures()->changeColors( m_switchColors );
  glClear( GL_COLOR_BUFFER_BIT );
  // switch to fast mode in order to draw the display elements
  fastMode();

  // set up an orthogonal screen for printing help text
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0, getWindow()->getWidth(), 0, getWindow()->getHeight(), -1, 1 ); // full size orthogonal screen
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  // print text
  glEnable( GL_BLEND );							// enable blending (for text)
  glColor3fv( colorWhite );// white color

  if( devmode ) {
    vector<PlanetInfoGraphical> *planets = World::getWorld()->getSystemPlanets( currentSystemInfoNumber );
    printInfoLine( "Loading new System ..." );
    printInfoLine( "" );
    printInfoLine( "Textures to load: %d ", planets->size() + 1 ); // +1 for sun
    printInfoLine( "" );
    for( unsigned int i=0; i < planets->size(); i++ ) {
      glClear( GL_COLOR_BUFFER_BIT );
      string text = "Loading texture from file " + FileAccess::getTextureFile(planets->at(i).texture);
      printInfoLine( text.c_str() );
      printInfoBox( false );
      glFlush();
      getWindow()->swap();
      FileAccess::loadTexture( FileAccess::getPlanetSlot(i), planets->at(i).texture );
    }
    glClear( GL_COLOR_BUFFER_BIT );

    string text = "Loading texture from file " + FileAccess::getTextureFile( World::getWorld()->getSystemStarTexture( currentSystemInfoNumber ) );
    printInfoLine( text.c_str() );
    printInfoBox( false );
    glFlush();
    getWindow()->swap();
    FileAccess::loadTexture( star, World::getWorld()->getSystemStarTexture( currentSystemInfoNumber ) );

  } else {
    vector<PlanetInfoGraphical> *planets = World::getWorld()->getSystemPlanets();
    for( unsigned int i=0; i < planets->size(); i++ ) {
      glClear( GL_COLOR_BUFFER_BIT );
      printInfoLine( "Loading new System ..." );
      printInfoLine( "" );
      printInfoLine( "%7.2f percent done.", (100 * ((float)(i) / ((float)planets->size()+1.0)) ) ); //+1 for sun
      printInfoBox( true );
      glFlush();
      getWindow()->swap();
      FileAccess::loadTexture( FileAccess::getPlanetSlot(i), planets->at(i).texture );
    }
    glClear( GL_COLOR_BUFFER_BIT );
    printInfoLine( "Loading new System ..." );
    printInfoLine( "" );
    printInfoLine( "%7.2f percent done.", (100 * ((float)(planets->size()) / ((float)planets->size()+1.0)) ) ); //+1 for sun
    printInfoBox( true );
    glFlush();
    getWindow()->swap();
    FileAccess::loadTexture( star, World::getWorld()->getSystemStarTexture() );

    printInfoLine( "Loading new System ..." );
    printInfoLine( "" );
    printInfoLine( "%7.2f percent done.", 100.0f );
  }

  // print for last time, clear this time
  printInfoBox( true );
  glFlush();
  getWindow()->swap();

  glDisable( GL_BLEND );

  // restore procections
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

/**
 * \fn	void App::loadAllNeededTextures()
 * \brief	Loads all needed textures into the textureslots, exclusive planets.
 * 			
 * Planets are loaded in loadSystemTextures. If special color mode is active, the colors are swapped.
 */
void App::loadAllNeededTextures() {
  Textures::getTextures()->reserve( 32 );
  Textures::getTextures()->changeColors( m_switchColors );
  FileAccess::loadTexture( corona, sun_corona );
  FileAccess::loadTexture( bar, bar_green_red );
  FileAccess::loadTexture( radar, radar_tex );
  FileAccess::loadTexture( sight, decal_sight );
  FileAccess::loadTexture( font1, font_2 );
  FileAccess::loadTexture( font2, font_3 );
  FileAccess::loadTexture( laser, laser1 );
  FileAccess::loadTexture( background, universe_1 );
  FileAccess::loadTexture( missile_hud_tex, missile_hud );
  FileAccess::loadTexture( rearview, missile_hud );
}

/**
 * \fn	void App::setScreen( Screens screen )
 * \brief	Sets a screen, activates mouse pointer.
 * \param	screen	the screen.
 */
void App::setScreen( Screens screen ) {
  if( screen == marketplace )
    inStation = true;
  if( screen == currentSystem && inStation ) {
    player->setPos( World::getWorld()->getStation()->getOutPosition() );
    player->setView( World::getWorld()->getStation()->getOutVector() ); // do stuff that we start outside the coriolis station
    player->setSpeed( player->getMaxSpeed()  * 0.5 );
    speedBar->setCurrent( player->getSpeed() );
    inStation = false;
  }

  switch( screen ) {
    case currentSystem:
    case help:
    case helpEnvironment:
    case helpInfo:
    case helpLocal:
    case helpMarketplace:
    case loadLevel:
    case systemInfo:
      getWindow()->showMousePointer( false );
      break;
    case marketplace:
    case localSystems:
      getWindow()->showMousePointer( true );
      break;
    default:
      getWindow()->showMousePointer( false );
  }

  currentScreen = screen;
}

/**
 * \fn	void App::printInfoLine( const char*inputstring, ... )
 * \brief	Prints a text.
 * \param	inputstring	The string to be printed.
 */
void App::printInfoLine( const char*inputstring, ... ) {
  // copied from the texture font class. variable argument list are old C-style and have to be replaced.
  char text[256];				// create space for text
  va_list ap;					// pointer to list of arguments

  if( inputstring == NULL )// quit, if there is no texst
    return;

  va_start( ap, inputstring );					// parse the string for variables
  vsprintf_s( text, inputstring, ap );		// convert symbols to actual numbers
  va_end( ap );						// store results in text

  // now the string is placed in text and send to the vector
  string textString( text );
  this->infoBoxLines.push_back(textString);
}

/**
 * \fn	void App::printInfoBox( bool clear )
 * \brief	Displays a box containing information, centered to the screen.
 * \param	clear	true to clear.
 */

void App::printInfoBox( bool clear ) {
  static const int gap = 10;
  static const int size = tf2->getSize();
  for( unsigned int i=0; i<infoBoxLines.size(); i++)
    tf2->print(gap, getWindow()->getHeight() - gap - ++infoOutputLine*size, &infoBoxLines.at(i));
  if( clear )
    infoBoxLines.clear();
  infoOutputLine = 0;
}

/**
 * \fn	void App::showFrameRate()
 * \brief	Function that is called, if the user typed in the command to display frame rate.
 */

void App::showFrameRate() {
  char opt = Console::getSingleCharacterOption( string(ti->getText()) );
  if( opt == '1' )
    m_showFrameRate = true;
  else if( opt == '0' )
    m_showFrameRate = false;
  else
    ; // error
}

/**
 * \fn	void App::showFrameTime()
 * \brief	Sets the variable to show the frame time.
 * 			
 * Called when the according command has been typed into the console.
 */

void App::showFrameTime() {
  char opt = Console::getSingleCharacterOption(ti->getText() );
  if( opt == '1' )
    m_showFrameTime = true;
  else if( opt == '0' )
    m_showFrameTime = false;
  else
    ; // error
}

/**
 * \fn	void App::showSpeed()
 * \brief	Sets the variable to show the speed.
 * 			
 * Called when the according command has been typed into the console.
 */

void App::showSpeed() {
  char opt = Console::getSingleCharacterOption(ti->getText() );
  if( opt == '1' )
    m_showSpeed = true;
  else if( opt == '0' )
    m_showSpeed = false;
  else
    ; // error
}

/**
 * \fn	void App::showResolution()
 * \brief	Turns on and of displaying of the resolution.
 *
 * Supposed to be called, if the according command is typed into the console by the user.
 */

void App::showResolution() {
  char opt = Console::getSingleCharacterOption(ti->getText() );
  if( opt == '1' )
    m_showResolution = true;
  else if( opt == '0' )
    m_showResolution = false;
  else
    ; // error occured
}

/**
 * \fn	void App::showHelp()
 * \brief	Displays the help screen.
 * 			
 * Called when the command has been typed by the user.
 */

void App::showHelp() {
  string option = Console::getOption( ti->getText() );
  if( option == "" ) {
    getWindow()->showMousePointer( false );
    setScreen( help );
  }
  if( option == "local" ) {
    getWindow()->showMousePointer( false );
    setScreen( helpLocal );
  }
  if( option == "info" ) {
    getWindow()->showMousePointer( false );
    setScreen( helpInfo );
  }
  if( option == "env" ) {
    getWindow()->showMousePointer( false );
    setScreen( helpEnvironment );
  }
  if( option == "mkt" ) {
    getWindow()->showMousePointer( false );
    setScreen( helpMarketplace );
  }
}

/**
 * \fn	void App::devMode()
 * \brief	Turns on and off development mode, if the command has been typed into console.
 * 			
 * If normal mode is on, the speed is maximized to light speed, as in dev mode
 * the speed may be larger.
 */

void App::devMode() {
  char opt = Console::getSingleCharacterOption(ti->getText() );
  if( opt == '1' ) {
    player->setMaxSpeed( lightspeed + lightspeed  * 99 );
    player->setMinSpeed( -lightspeed*0.5 - lightspeed  * 99 );
    devmode = true;
  } else if( opt == '0' ) {
    player->setMaxSpeed( lightspeed );
    if( player->getSpeed() > lightspeed )
      player->setSpeed( lightspeed );
    player->setMinSpeed( -lightspeed*0.5 );
    if( player->getSpeed() < -lightspeed*0.5 )
      player->setSpeed( -lightspeed*0.5 );
    devmode = true;
    devmode = false;
  } else
    ; // an error occured
}

/**
 * \fn	void App::switchColors()
 * \brief	Switches colors (and reloads textures) if the command has been
 * 			typed in console.
 */

void App::switchColors() {
  char opt = Console::getSingleCharacterOption(ti->getText() );
  if( opt == '1' ) {
    m_switchColors = true;
    loadAllNeededTextures();
    loadSystemTextures();
  } else if( opt == '0' ) {
    m_switchColors = false;
    loadAllNeededTextures();
    loadSystemTextures();
  } else
    ; // an error occured
}

/**
 * \fn	Frustum * App::getFrustum( void )
 * \brief	Gets the frustum.
 * \return	The frustum.
 */

Frustum * App::getFrustum( void ){
  return frustum;
}

/**
 * \fn	void App::setProjectionOrthogonal( void )
 * \brief	Sets the current projection to orthogonal mode.
 * 			
 * Used to print on screen. Uses OpenGL PushMatrix command.
 */

void App::setProjectionOrthogonal( void ) {
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0, getWindow()->getWidth(), 0, getWindow()->getHeight(), -1, 1 ); // full size orthogonal screen
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
}

/**
 * \fn	void App::restoreProjection( void )
 * \brief	Resets projection to perspective projection, after it has been set to
 * 			orthogonal projection before.
 * 			
 * Uses OpenGL PopMatrix.
 */

void App::restoreProjection( void ) {
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}
