/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	OpenGLApplication.cpp
 * \brief	Defines the main class of mElite.
 */
#include "GlobalParameters.h"
#include "OpenGLApplication.h"
#include "World.h"
#include "Textures.h"
#include "Objects.h"
#include "FrameCounter.h"

/**
 * \fn	int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
 * 		int nCmdShow )
 *
 * \brief	WinMain is the main function where the execution begins. An application
 * 			object is created which does the rest of the initialization.
 *
 * \param hInstance Handle for current instance
 * \param hPrevInstance Handle of the last instance
 * \param lpCmdLine Parameter on t he command line
 * \param nCmdShow Status of window
 *
 * \return The Win32 return value
 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  int ret = -1;
  OpenGLApplication  * app = OpenGLApplication::create("OpenGLapldk3hd");			// create application class
  if (app != 0) {
    ret = app->main( hInstance, hPrevInstance, lpCmdLine, nCmdShow );	// execute main program
    delete app;
  } else {																	// error creating application
    MessageBox( HWND_DESKTOP, "Error Creating Application Class", "Error", MB_OK | MB_ICONEXCLAMATION );
  }
  return ret;
}

/**
 * \brief The window procedure handles the window messages for this application.
 * 		  
 * If  an window is being created we get the instance and save it. Otherwise all 
 * messages are passed to the message handler in the class.
 * \param hWnd Handle for the window 
 * \param uMsg The message that is to be handled    
 * \param wParam Additional information
 * \param lParam More additional information
 */
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  LONG userData = GetWindowLong(hWnd, GWL_USERDATA);					// Load The Application Class Context
  if ( userData == 0 ) {	// Application Class Context Not Available
    if ( uMsg == WM_CREATE ) {											// Evaluate Window Message and Creation message received
      // Get Window Structure Pointer Which Contains The Application Class Context
      CREATESTRUCT  * creation = reinterpret_cast<CREATESTRUCT *>( lParam );
      // Get The Application Class Context
      OpenGLApplication  * app = reinterpret_cast<OpenGLApplication *>( creation->lpCreateParams );
      // Store The Application Class Context
      SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast<LONG>(app) );
      app->m_isVisible = true;
      return 0;
    }
  } else {	// Get The Application Class Context
    OpenGLApplication  * app = reinterpret_cast<OpenGLApplication *>( userData );
    return app->message( hWnd, uMsg, wParam, lParam );				// Call Class Message Handler
  }
  return DefWindowProc( hWnd, uMsg, wParam, lParam );					// Pass Unhandled Messages To DefWindowProc
}

/**
 * \fn	OpenGLApplication::OpenGLApplication( const char * className )
 *
 * \brief	Constructs an OpenGLApplication with a given name.
 * 			
 * Initializes the main data structures, such as the world, the textures
 * and the object list.
 *
 * \param	className	Name of the window class that will belong to this application.
 */
OpenGLApplication::OpenGLApplication( const char  * className ) {
  // initialize the variables
  m_className = className;										// Store Class Name
  m_isProgramLooping = true;									// Program Looping Is Set To true
  m_createFullScreen = true;									// Enable FullScreen Mode
  m_isVisible = false;
  m_resizeDraw = false;												// Disable Redraw During Window Resize

  // don' have to be initialized cuz they are initialized if framelimit is set on
  useFrameLimit = false;// soll frame limit benutzt werden?

  // initializing the used objects, create new instances
  fc = new FrameCounter();

  // create global objects
  World::getWorld();
   Textures::getTextures();
  Objects::getObjects();
}

/**
 * Destructor 
 */
OpenGLApplication::~OpenGLApplication( void ) {
  // do some cleanup, delete all created objects
  delete Objects::getObjects();
  delete  Textures::getTextures();
  delete World::getWorld();
  delete fc;
}

/**
 * \brief Disables face culling, lighting and the depth testing
 * 
 * Very useful for 2D  elements, as text or display-elements.
 * It's supposed to use it not so often, as the switching costs time. 
 */
void OpenGLApplication::fastMode() {
  glDisable( GL_CULL_FACE );
  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING );// Licht deaktivieren
  glDisable( GL_LIGHT0 );// Licht0 (Sonne) aktivieren
}

/**
 * \brief Enables face culling, lighting and the depth testing. 
 */
void OpenGLApplication::fullMode() {
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glEnable( GL_LIGHTING );// Licht wieder aktivieren
  glEnable( GL_LIGHT0 );// Licht0 (Sonne) aktivieren
}

/**
 * \fn	FrameCounter * OpenGLApplication::getFrameCounter()
 *
 * \brief	Gets frame counter.
 * \return	The frame counter.
 */

FrameCounter * OpenGLApplication::getFrameCounter() {
  return fc;
}

/**
 * \fn	unsigned int OpenGLApplication::getFrameLimit( )
 *
 * \brief	Gets frame limit.
 *
 * \return	The frame limit if active, 0 otherwise.
 */

unsigned int OpenGLApplication::getFrameLimit( ) {
  return this->useFrameLimit ? m_frameLimit : 0;
}

/**
 * \brief Return the window that belongs tho this application instance.
 */
OpenGLWindow * OpenGLApplication::getWindow( void ) {
  return &m_window;
}

/**
 * \brief Enable or disable resizing while changing window size.
 * \param draw the status of disabling or resizing
 */
void OpenGLApplication::resizeDraw( bool draw ) {
  m_resizeDraw = draw;
}

/**
 * \brief This function is called if the application quits by sending the appropriate
 * window message and stopping the main program loop.
 * 
 * The only thing that has 
 * to be done is to delete the window. Other things are done within the
 * destructor.
 */
void OpenGLApplication::terminate() {
  PostMessage( m_window, WM_QUIT, 0, 0 );
  m_isProgramLooping = false;
}

/**
 * \brief Switches between fullscreen and windowed displaymode.
 */
void OpenGLApplication::toggleFullscreen() {
  PostMessage(m_window, WM_TOGGLEFULLSCREEN, 0, 0);					// Send A WM_TOGGLEFULLSCREEN Message
}

/**
 * \brief Routine which really handles the window messages.
 * \param	hWnd handle for the window
 * \param	uMsg the message that is handled
 * \param	wParam some information for the message
 * \param	lParam more information for the message
 */
LRESULT OpenGLApplication::message( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  switch (uMsg) {
  case WM_SYSCOMMAND: // Intercept System Commands
    switch (wParam) {  // Check System Calls
    case SC_SCREENSAVE: // screensaver starting
    case SC_MONITORPOWER: // monitor power save
      return 0; // do not allow that
      break;
    }
    break;
  case WM_CLOSE: // window closing
    terminate(); // terminate
    return 0;
    break;
  case WM_EXITMENULOOP:
  case WM_EXITSIZEMOVE:
    fc->newFrame();
    return 0;
    break;
  case WM_MOVE: // moving. update position
    m_window.setLeft( LOWORD(lParam) ); // x position in lo word
    m_window.setTop( HIWORD(lParam) ); // y position in hi word
    return 0;
    break;
  case WM_PAINT:
    if( m_resizeDraw ) {
      m_window.reshape();
      drawTest();
    }
    break;

  case WM_SIZING:
    {
      RECT * rect = (RECT *)lParam;
      m_window.setWidth( rect->right - rect->left ); // width in lo word
      m_window.setHeight( rect->bottom - rect->top ); // height in hi word
      return TRUE;
    }
    break;

  case WM_SIZE: // resize has occured
    switch (wParam)
    {
    case SIZE_MINIMIZED: // hide if minimized
      m_isVisible = false;
      return 0;
      break;

    case SIZE_MAXIMIZED: // draw and update position
    case SIZE_RESTORED: // if becoming visible again
      m_isVisible = true;
      m_window.setWidth( LOWORD(lParam) );
      m_window.setHeight( HIWORD(lParam) );
      m_window.reshape();
      fc->newFrame();
      return 0;
      break;
    }
    break;

  case WM_KEYDOWN: // key pressed. update buffers.
    m_window.keyDown( (unsigned char)wParam );
    return 0;
    break;
  case WM_KEYUP:
    m_window.keyUp( (unsigned char)wParam );
    return 0;
    break;

  case WM_MOUSEMOVE:
    m_window.setMousePosition( LOWORD (lParam), HIWORD (lParam) );
    break;

  case WM_LBUTTONUP:
    m_window.setMousePosition( LOWORD (lParam), HIWORD (lParam) );
    m_window.setMouseClick( true );
    break;

  case WM_LBUTTONDOWN:
    break;

    // user defined window message to toggle between fullscreen and windowed mode. leads to quit window!
  case WM_TOGGLEFULLSCREEN:
    m_createFullScreen = !m_createFullScreen;
    PostMessage( hWnd, WM_QUIT, 0, 0 );
    break;
  }

  return DefWindowProc( hWnd, uMsg, wParam, lParam ); // let everything else be handled by the default window function
}

/**
 * \fn	int OpenGLApplication::main( HINSTANCE hInstance, HINSTANCE hPrevInstance,
 * 		LPSTR lpCmdLine, int nCmdShow )
 *
 * \brief	The main loop for the window (and application)
 *
 * \param	hInstance	 	handle of the instance
 * \param	hPrevInstance	handle of previous instance
 * \param	lpCmdLine	 	array of command-line argument strings
 * \param	nCmdShow	 	number of command-line arguments
 *
 * \return	0 if everything worked fine, -1 else
 */
int OpenGLApplication::main( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
  //WNDCLASSEX wc; // commented for extended classes
  WNDCLASS wc; // we use only default window classes
  ZeroMemory( &wc, sizeof(wc) ); // reset memory
  //wc.cbSize			= sizeof(WNDCLASSEX); // update only for extended classes
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // redraw if resized
  wc.lpfnWndProc = (WNDPROC) WndProc; // the window function
  wc.hInstance = hInstance;
  //wc.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE); // background brush, not needed
  wc.hIcon = LoadIcon( NULL, IDI_WINLOGO ); // load an icon. standard.
  wc.hCursor = LoadCursor( NULL, IDC_ARROW ); // show normal arrow mouse-pointer
  wc.lpszClassName = m_className;

  if( !RegisterClass(&wc) ) { // return, if somehow failed.
    MessageBox( NULL, "Failed to register the Window-Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;
  }

  m_createFullScreen = START_IN_FULLSCREEN;

  while( m_isProgramLooping ) { // loop until WM_QUIT received
    bool result = m_window.create( "mElite - Grafica al Calcolatore Progetto da Melanie und Jan-Philipp", m_createFullScreen, m_className, hInstance, this );
    if ( result ) {
      // window should have been created
      if( initialize() == false ) { // terminate if initialization fails
        terminate();
      } else { // otherwise handle messages
        MSG msg;
        bool isMessagePumpActive = true;
        fc->newFrame();
        while( isMessagePumpActive ) {
          if( PeekMessage( &msg, m_window, 0, 0, PM_REMOVE ) != 0) { // get a message and handle
            if ( msg.message != WM_QUIT ) {
              DispatchMessage( &msg );
            } else {
              isMessagePumpActive = false;
            }
          } else {
            if (m_isVisible == false) {
              WaitMessage();
            } else {
              drawTest();
            }
          }
        }
      }
      deinitialize(); // clean up at the end
      m_window.kill();
    } else { // creation failed.
      MessageBox(HWND_DESKTOP, "Error creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
      m_isProgramLooping = false;
    }
  }

  if( !UnregisterClass( m_className, hInstance ) ) {
    MessageBox( NULL, "Could not unregister Window-Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
  }

  return 0;
}

/**
 * \fn	void OpenGLApplication::setFrameLimit( unsigned int framesPerSecond )
 *
 * \brief	Sets frame limit. If the computer is faster than the frame limit, it is supposed not to render again.
 * \param	framesPerSecond	The frames per second.
 */
void OpenGLApplication::setFrameLimit( unsigned int framesPerSecond ) {
  if(framesPerSecond == 0) // 0 sets frame limiting off
    useFrameLimit = false;
  else {
    useFrameLimit = true;
    m_frameLimit = framesPerSecond;
    this->frameTime = 1000.0 / framesPerSecond;
  }
}

/**
 * \fn	void OpenGLApplication::drawTest()
 * \brief	Draw a simple test scene.
 */
void OpenGLApplication::drawTest() {
  if( useFrameLimit )
    if( fc->getFrameTimeCurrent() < frameTime )
      return;

  update( fc->getFrameTimeCurrent() );
  fc->newFrame();
  draw();
  m_window.swap();
}