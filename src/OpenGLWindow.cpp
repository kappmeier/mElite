/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	OpenGLWindow.cpp
 * \brief	Defines a special Windows window that supports OpenGL.
 */ 
#include "GlobalGL.h"
#include "OpenGLWindow.h"

/**
 * \fn	OpenGLWindow::OpenGLWindow( void )
 * \brief	Default constructor. Initializes the necessary member variables.
 * 			
 * All keys are set to not pressed, color depth is 24 bits, full screen is
 * deactivated.
 *
 */
OpenGLWindow::OpenGLWindow( void ) {
  // needed handles are initialized to zero
  m_hWnd = NULL;
  m_hDC = NULL;
  m_hRC = NULL;

  // window position and size
  m_fullscreen = false;
  active = true;
  m_left = 0;												// window x Position
  m_top = 0;												// window y Position
  m_width = 800;
  m_height = 600;
  m_screenWidth = 1920;
  m_screenHeight = 1080;

  // frustum
  m_frustum.farDist = 5000.0;
  m_frustum.nearDist = 1.0;

  m_bpp = 32; // 32 bit farbtiefe anfordern
  //m_BitsPerPixel	= 16;												// Bits Per Pixel

  // input: characters and mouse
  for(unsigned char i=0;i<255;i++)
    keys[i] = false;
  keys[255] = false;
  m_mouseClick = false;
  m_mouseVisible = false;
}

/**
 * Destructor.
 */
OpenGLWindow::~OpenGLWindow( void ) {

}

/**
 * \brief change the screen resolution
 */
bool OpenGLWindow::changeScreenResolution() {
  DEVMODE dmScreenSettings; // Device Mode
  ZeroMemory( &dmScreenSettings, sizeof( dmScreenSettings ) ); 

  dmScreenSettings.dmSize = sizeof( dmScreenSettings );
  dmScreenSettings.dmPelsWidth = getWidth(); // the new screen width
  dmScreenSettings.dmPelsHeight = getHeight(); // the new screen height
  dmScreenSettings.dmBitsPerPel = m_bpp; // the bits per pixel
  dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

  if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN )!= DISP_CHANGE_SUCCESSFUL ) {
    return false;
  }
  return true;
}

/**
 * Creates an OpenGL window.
 * \param windowTitle the title of the window (in windowed mode)
 * \param fullscreen should fullscreen mode be used
 * \param className the name of the window class
 * \param hInstance the handle of the instance
 * \param lpParam some parameter
 */
bool OpenGLWindow::create( const char * windowTitle, bool fullscreen, const char  * className, HINSTANCE hInstance, LPVOID lpParam ) {
  GLuint PixelFormat;
  DWORD dwExStyle;
  DWORD dwStyle;

  m_fullscreen = fullscreen;// Übernehmen der Vollbild-Einstellung

  if(m_fullscreen) {
    if( !changeScreenResolution() ) { // try to change resolution, switch to window mode or quit if failed
      if( MessageBox( NULL, "The requested fullscreen mode is not supported by\nyour video card. Use windowed mode instead?", "Question", MB_YESNO | MB_ICONEXCLAMATION ) == IDYES ) {
        m_fullscreen = false;
      } else {
        MessageBox( NULL, "Program will now close.", "ERROR", MB_OK | MB_ICONSTOP );
        return false;
      }
    }
  }

  RECT windowRect = { getLeft(), getTop(), getLeft() + getWidth(), getTop() + getHeight() };

  if( fullscreen ) {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
    dwStyle = WS_POPUP;
    ShowCursor( FALSE );
  } else {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx( &windowRect, dwStyle, false, dwExStyle );
    // move window top left corner to visible area
    if (windowRect.left < 0) {
      windowRect.right -= windowRect.left;
      windowRect.left = 0;
    }
    if (windowRect.top < 0) {
      windowRect.bottom -= windowRect.top;
      windowRect.top = 0;
    }
  }

  // finally create the OpenGL Window
  if ( !(m_hWnd=CreateWindowEx(	dwExStyle, // style
    className,							// window class
    windowTitle, 								 // title
    dwStyle |							// style
    WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    windowRect.left, windowRect.top, // left, top
    windowRect.right - windowRect.left,	// height
    windowRect.bottom - windowRect.top,	// width
    HWND_DESKTOP,								 // desktop as parent
    NULL,								// no menu
    hInstance,							 // the instance
    lpParam)))								 // application class context
  {
    kill();
    MessageBox( NULL, "Window creation error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return FALSE;
  }

  // the propertys our window should have
  static PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory( &pfd, sizeof( pfd ) ); // fill with zeros
  pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR ); // size of Pixelformat Descriptors
  pfd.nVersion = 1; // necessary
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // use double buffering
  pfd.iPixelType = PFD_TYPE_RGBA; // RGBA colors
  pfd.cColorBits = m_bpp; // color depth
  pfd.cDepthBits = 24;// depth buffer size
  pfd.iLayerType = PFD_MAIN_PLANE;

  if( !(m_hDC=GetDC( m_hWnd )) ) { // quit if failed
    kill();
    MessageBox( NULL, "Can't create a GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;
  }
  if( !(PixelFormat = ChoosePixelFormat( m_hDC, &pfd )) ) {
    kill();
    MessageBox( NULL, "Can't find a suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return false;
  }
  if( !SetPixelFormat( m_hDC, PixelFormat, &pfd ) ) {
    kill();
    MessageBox( NULL, "Can't set the PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return FALSE;
  }
  if( !(m_hRC = wglCreateContext( m_hDC )) ) {
    kill();
    MessageBox( NULL, "Can't create a GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return FALSE;
  }
  if( !wglMakeCurrent( m_hDC, m_hRC ) ) {
    kill();
    MessageBox( m_hWnd, "Can't activate the GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
    return FALSE;
  }

  // everything fine. finally show window
  ShowWindow( m_hWnd, SW_SHOW );
  SetFocus( m_hWnd );
  reshape();
  return true;
}

//! Gets the aspect ratio of the window.
/**
 * \fn	float OpenGLWindow::getAspect( void )
 * \brief	The aspect ratio is the ratio between height and width of the screen.
 * \return	The aspect ratio.
 */
float OpenGLWindow::getAspect( void ) {
  return m_frustum.aspect;
}

/**
 * \fn	unsigned int OpenGLWindow::getHeight( void )
 * \brief	Gets the height of the window.
 * \return	The height of the window.
 */
unsigned int OpenGLWindow::getHeight( void ) {
  if( m_fullscreen )
    return m_screenHeight;
  else
    return m_height;
}

/**
 * \fn	unsigned int OpenGLWindow::getWidth( void )
 * \brief	Gets the width of the window.
 * \return	The width of the window.
 */
unsigned int OpenGLWindow::getWidth( void ) {
  if( m_fullscreen )
    return m_screenWidth;
  else
    return m_width;
}

/**
 * \fn	unsigned int OpenGLWindow::getLeft( void )
 * \brief	Gets the coordinate of the left corners. 0 if full screen.
 * \return	The left most coordinate.
 */
unsigned int OpenGLWindow::getLeft( void ) {
  if( m_fullscreen )
    return 0;
  else
    return m_left;
}

/**
 * \fn	unsigned int OpenGLWindow::getTop( void )
 * \brief	Gets the coordinate of the top corners. 0 if full screen
 * \return	The top most coordinate.
 */
unsigned int OpenGLWindow::getTop( void ) {
  if( m_fullscreen )
    return 0;
  else
    return m_top;
}

/**
 * \brief Returns the current frustum data which is updated on resize.
 * \return the data of the current frustum
 */
FrustumInformation OpenGLWindow::getFrustum( void ) {
  return m_frustum;
}

/**
 * \brief Returns true if the window is in fullscreen mode and false else. 
 */
bool OpenGLWindow::isFullscreen( void ) {
  return m_fullscreen;
}

//! Closes the window, release all ressources.
/**
 * Return to desktop mode, if full screen was activated.
 */
void OpenGLWindow::kill( void ) {
  if( m_fullscreen ) {
    ChangeDisplaySettings( NULL, 0 );
    ShowCursor( TRUE );
  }

  if( m_hRC ) {
    if( !wglMakeCurrent( NULL, NULL ) )
      MessageBox( NULL, "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    if( !wglDeleteContext( m_hRC ) )
      MessageBox( NULL, "Release rendering context failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    m_hRC = NULL;
  }
  if( m_hDC && !ReleaseDC( m_hWnd, m_hDC ) ) {
    MessageBox( NULL, "Release device context failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    m_hDC = NULL;
  }
  if( m_hWnd && !DestroyWindow( m_hWnd ) )	{
    MessageBox( NULL, "Could not release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    m_hWnd = NULL;
  }
}

//! Update sthe member variables and the aspect ratio.
/**
 * Resets the perspective projection. 
 */
GLvoid OpenGLWindow::reshape( ) {
  GLsizei width = getWidth();
  GLsizei height = getHeight();
  if( height == 0 ) // avoid division by zero
    height = 1;

  // Calculate correct perspective and aspect ratio of the window
  m_frustum.aspect = (float)width / (float)height;
  glViewport( 0, 0, width, height );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( m_frustum.fov, m_frustum.aspect, m_frustum.nearDist, m_frustum.farDist );

  // do _not_ forget transforming 45 degree angle to degree for use with tangens function!
  m_frustum.nearHeight = 2 * tan( m_frustum.fov * ANGLE2DEG * 0.5 ) * m_frustum.nearDist;
  m_frustum.farHeight  = 2 * tan( m_frustum.fov * ANGLE2DEG * 0.5 ) * m_frustum.farDist;
  m_frustum.nearWidth  = m_frustum.nearHeight * m_frustum.aspect;
  m_frustum.farWidth   = m_frustum.farHeight  * m_frustum.aspect;

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

/**
 * /brief Sets the new left position for the window, this is not used if the window 
 * is in fullscreen mode.
 * \param left the new left position of the window 
 */
void OpenGLWindow::setLeft( unsigned int left ) {
  m_left = left;
}

/**
 * /brief Sets the new top position for the window, this is not used if the window is
 * in fullscreen mode. 
 * \param top new top position of the window 
 */
void OpenGLWindow::setTop( unsigned int top ) {
  m_top = top;
}

/**
 * /brief Sets the new height for the window depending whether the window is in
 * fullscreen mode or not.
 * \param height new height of the window
 */
void OpenGLWindow::setHeight( unsigned int height ) {
  if ( m_fullscreen )
    m_screenHeight = height;
  else
    m_height = height;
}

/**
 * /brief Sets the new height for the window depending whether the window is in
 * fullscreen mode or not.
 * \param width new width of the window
 */
void OpenGLWindow::setWidth( unsigned int width ) {
  if ( m_fullscreen )
    m_screenWidth = width;
  else
    m_width = width;
}

/**
 * \fn	void OpenGLWindow::setMousePosition( int x, int y )
 * \brief	Sets a new mouse position.
 * \param	x	The x coordinate.
 * \param	y	The y coordinate.
 */
void OpenGLWindow::setMousePosition( int x, int y ) {
  m_mousePos.x = x;
  m_mousePos.y = y;
}

/**
 * \fn	Position OpenGLWindow::getMousePosition()
 * \brief	Gets the mouse position.
 * \return	The mouse position.
 */
Position OpenGLWindow::getMousePosition() {
  return m_mousePos;
}

/**
 * \fn	void OpenGLWindow::setMouseClick( bool click )
 * \brief	Sets, if a mouse click occured.
 * \param	click	true if click occured.
 */
void OpenGLWindow::setMouseClick( bool click ) {
  m_mouseClick = click;
}

/**
 * \fn	bool OpenGLWindow::getMouseClick( void )
 * \brief	Gets, if a mouse click occured.
 * \return	true if click occured
 */

bool OpenGLWindow::getMouseClick( void ) {
  return m_mouseClick;
}

/**
 * \fn	void OpenGLWindow::showMousePointer( bool mouseVisible )
 * \brief	Shows and hides the mouse pointer, but hiding takes place
 * 			only in full screen mode.
 * \param	mouseVisible	if the mouse should be visible
 */
void OpenGLWindow::showMousePointer( bool mouseVisible ) {
  m_mouseVisible = mouseVisible;
  if( m_fullscreen )
    ShowCursor( mouseVisible );
}
