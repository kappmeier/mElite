/**
/* Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	OpenGLWindow.h
 * \brief	Declares class representing an OpenGL window in Microsoft Windows
 */

#include "Structures.h"
#include <Windows.h>

/**
 * \class	OpenGLWindow
 *
 * \brief	A basic window with additional members.
 * 			
 * Supports the use of OpenGL by providing easy access to aspect ratios, viewport and
 * automatically setting a perspective view.
 *
 * \author	Jan-Philipp Kappmeier
 * \date	27/09/2012
 */
class OpenGLWindow {
public:
  OpenGLWindow( void );
  ~OpenGLWindow( void );

  bool changeScreenResolution();
  bool create( const char * windowTitle, bool fullscreen, const char  * className, HINSTANCE hInstance, LPVOID lpParam );

  bool isFullscreen( void );

  float getAspect( void );
  unsigned int getHeight( void );
  unsigned int getWidth( void );
  unsigned int getLeft( void );
  unsigned int getTop( void );
  FrustumInformation getFrustum( void );

  // key input
  void keyDown( unsigned char i ) { keys[i] = true; }
  bool keyPressed(unsigned char i ) { return keys[i]; }
  void keyUp( unsigned char i) { keys[i] = false; }
  // mouse input
  void setMousePosition( int x, int y );
  Position getMousePosition();
  void setMouseClick( bool click );
  bool getMouseClick( void );
  void showMousePointer( bool mouseVisible );

  void kill( void );
  void reshape( void );

  void setBits( int bits ) { this->m_bpp = bits; }
  void setLeft( unsigned int left );
  void setTop( unsigned int top );
  void setHeight( unsigned int height );
  void setWidth( unsigned int width );

  void swap( void ) { SwapBuffers( m_hDC ); }

  HDC getDC( void ) { return m_hDC; }

  // Class conversion operator for window handle. with this, OpenGLWindow can be used as HWND
  operator HWND() { return m_hWnd; }

private:
  bool	keys[256]; // array for keyboard routinges
  Position m_mousePos; // holds information about mouse position
  bool m_mouseClick; // checks if a (left) mouse click has occured
  bool	active; // window active flag set to TRUE by default
  HWND	m_hWnd;
  HDC		m_hDC;
  HGLRC	m_hRC;
  bool m_fullscreen; // fullscreen flag set to fullscreen mode by default
  int m_bpp; // bits per pixel (color)
  int m_width;
  int m_height;
  int m_screenWidth;
  int m_screenHeight;
  int m_left; // window x position
  int m_top; // window y position
  FrustumInformation m_frustum; // contains information about the frustum
  bool m_mouseVisible;
};