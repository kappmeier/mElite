/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	OpenGLApplication.h
 * \brief	Declares the OpenGL application class.
 */
#include "OpenGLWindow.h"
class FrameCounter;

/**
 * \class	OpenGLApplication
 *
 * \brief	A basic class for an OpenGL application.
 * 			
 * Contains basic Windows related functions such as the message handler and important
 * members used for graphic programming, such as screen width. Can toggle between
 * full screen and windowed mode.
 *
 * Supports virtual methods to draw a scene and update the datastructures in each
 * frame.
 *
 * \author	Jan-Philipp Kappmeier
 * \date	27/09/2012
 */

class OpenGLApplication {
public:
  virtual ~OpenGLApplication( void );
  // creation class, must be provided by the base class
  static OpenGLApplication  * create( const char  * class_name );

protected:
  // the constructor is protected, thus only derived class can create instances
  OpenGLApplication( const char  * className );

  // functions that must be provided by the derived class
  virtual bool	initialize() = 0;							// Perform All Initialization
  virtual void	deinitialize() = 0;							// Perform All DeInitialization
  virtual void	update( DWORD milliseconds ) = 0;		// Perform Motion Updates
  virtual void	draw() = 0;									 // Perform All Scene Drawing

  // methods provided by base class
  OpenGLWindow * getWindow( void );
  void fastMode(); // disables some OpenGL features
  void fullMode(); // enables all OpenGL features
  void	terminate();								 // Terminate The Application

  void	toggleFullscreen();									// Toggle Fullscreen / Windowed Mode
  void	resizeDraw( bool enable );	 // Enable Redraw During Window Resize

  void setFrameLimit( unsigned int framesPerSecond );// sets up a frame limit
  unsigned int getFrameLimit( );

  // get the objects provided by this class
  FrameCounter  * getFrameCounter();

private:
  // Windows Main-Function and our mainfunction, Window procedure and our messagehandler
  friend int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
  int main( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
  friend LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  LRESULT	message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  void drawTest();

  // constants
  static const UINT WM_TOGGLEFULLSCREEN = ( WM_USER + 1 );		// application define message for toggling

  // variables
  OpenGLWindow m_window;
  const char  * m_className;
  bool m_isProgramLooping;
  bool m_createFullScreen;
  bool m_isVisible;
  bool m_resizeDraw;

  int m_frameLimit; // number of frames that should be drawed per second
  float frameTime;// time for a frame in milliseconds
  bool useFrameLimit;// soll frame limit benutzt werden?
  FrameCounter *fc; // frame counter, used for accurate timing
};
