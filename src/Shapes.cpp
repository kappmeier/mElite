/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Shapes.cpp
 * \brief	Implementations of some methods that can draw shapes of 2 dimensional objects
 */
#include "GlobalGL.h"
#include "Shapes.h"

Shapes::Shapes(void) {
}

Shapes::~Shapes(void) {
}

void Shapes::drawCircle( Vector3 pos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface ) {
  drawCircle( pos.x, pos.y, pos.z, n, r, colorCenter, colorSurface );
}

void Shapes::drawCircle( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface ) {
  int i;
  float alphaPi = ( ( 360.0/n ) / 180.0)  * M_PI;
  if(n <3 )
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glBegin( GL_TRIANGLE_FAN );
  glColor3fv(colorCenter);
  glVertex3f( xPos + r/3, yPos + r/3, 0 );
  glColor3fv(colorSurface);
  for(i=0; i <= n; i++)
    glVertex3f(xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos);
  glEnd();
}

void Shapes::drawCircleTex( Vector3 pos, GLint n, GLfloat r ) {
  drawCircleTex( pos.x, pos.y, pos.z, n, r );
}

void Shapes::drawCircleTex( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r ) {
  int i;
  float alphaPi = ( ( 360.0/n ) / 180.0)  * M_PI;
  if(n <3 )
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glBegin( GL_TRIANGLE_FAN );
  glTexCoord2f( 0.5, 0.5 );
  glVertex3f( xPos, yPos, 0 );
  for(i=0; i <= n; i++) {
    // texcoords generated as distances +- 0.5 from center
    glTexCoord2f( 0.5 + cos(i  * alphaPi + M_PI_2)  * 0.5, 0.5 + sin(i  * alphaPi + M_PI_2)  * 0.5 );
    glVertex3f( xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos );
  }
  glEnd();
}

void Shapes::drawCircleTex( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat *colorCenter, const GLfloat *colorSurface ) {
  int i;
  float alphaPi = ( ( 360.0/n ) / 180.0)  * M_PI;
  if(n <3 )
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glBegin( GL_TRIANGLE_FAN );
  glColor3fv( colorCenter );
  glTexCoord2f( 0.5, 0.5 );
  glVertex3f( xPos, yPos, 0 );
  glColor3fv( colorSurface );
  for(i=0; i <= n; i++) {
    // texcoords generated as distances +- 0.5 from center
    glTexCoord2f(0.5 + cos( i  * alphaPi + M_PI_2 )  * 0.5, 0.5 + sin( i  * alphaPi + M_PI_2 )  * 0.5 );
    glVertex3f( xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos );
  }
  glEnd();
}

void Shapes::drawCircle( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat *colorCenter, const GLfloat *colorSurface ) {
  int i;
  float alphaPi = ( 360.0/n )  * ANGLE2DEG;
  if(n <3 )
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glBegin( GL_TRIANGLE_FAN );
  glColor3fv(colorCenter);
  glVertex3f( xPos + r/3, yPos + r/3, 0 );
  glColor3fv(colorSurface);
  for(i=0; i <= n; i++)
    glVertex3f(xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos);
  glEnd();
}

void Shapes::drawCircleShape( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat * color ) {
  int i;
  float alphaPi = ( 360.0/n )  * ANGLE2DEG;

  if(n < 3)
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glLineWidth( 2 );
  glColor3fv( color );
  glBegin( GL_LINE_LOOP );
  for(i=0; i <= n; i++)
    glVertex3f(xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos);
  glEnd();
}

void Shapes::drawCircleShape( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat * color ) {
  int i;
  float segmentStart2 = segmentStart - floor(segmentStart/360.0f)*360.0; 
  float segmentEnd2 = segmentEnd - floor(segmentEnd/360.0f)*360.0; 
  float alphaPi;
  if( segmentEnd2 < segmentStart2 )
    alphaPi = ((360.0 - segmentStart2 + segmentEnd2 )/n)  * ANGLE2DEG;
  else
    alphaPi = ((segmentEnd2 - segmentStart2)/n)  * ANGLE2DEG;
  //float alphaPi = ( 360.0/n )  * ANGLE2DEG;

  if(n < 3)
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glLineWidth( 2 );
  glColor3fv( color );
  glBegin( GL_LINE_STRIP );
  for(i=0; i <= n; i++)
    glVertex3f(xPos + cos(i  * alphaPi + M_PI_2 + segmentStart2*ANGLE2DEG)*r, yPos + sin(i  * alphaPi + M_PI_2 + segmentStart2*ANGLE2DEG)*r, zPos);
  glEnd();
}

void Shapes::drawCircleShape( Vector3 pos, GLint n, GLfloat r, float segmentStart, float segmentEnd, const GLfloat * color ) {
  drawCircleShape( (GLfloat)pos.x, (GLfloat)pos.y, (GLfloat)pos.z, n, r, segmentStart, segmentEnd, color );
}


void Shapes::drawCircleShapeDashed( GLfloat xPos, GLfloat yPos, GLfloat zPos, GLint n, GLfloat r, const GLfloat * color ) {
  int i;
  float alphaPi = ( 360.0/n )  * ANGLE2DEG;

  if(n < 3)
    printf("\nFehler! Mindestens 3 Ecken!.\n\n");
  glLineWidth( 2 );
  glColor3fv( color );
  glBegin( GL_LINES );
  for(i=0; i <= n; i+=2)
    glVertex3f(xPos + cos(i  * alphaPi + M_PI_2)*r, yPos + sin(i  * alphaPi + M_PI_2)*r, zPos);
  glEnd();
}

void Shapes::drawRect( GLfloat xCenterPos, GLfloat yCenterPos, GLfloat zPos, GLfloat width, GLfloat height ) {
  glBegin( GL_QUADS );
  glVertex3f( xCenterPos - width*0.5, yCenterPos + height*0.5, zPos );
  glVertex3f( xCenterPos - width*0.5, yCenterPos - height*0.5, zPos );
  glVertex3f( xCenterPos + width*0.5, yCenterPos - height*0.5, zPos );
  glVertex3f( xCenterPos + width*0.5, yCenterPos + height*0.5, zPos );
  glEnd();
}
