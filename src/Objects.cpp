/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	Objects.cpp
 * \brief	Implementation of the singleton objects class.
 */

#include "Objects.h"
#include "LittleHelper.h"
#include "World.h"
#include "ObjectLoader3ds.h"
#include "VectorMath.h"

using namespace std;

/**
 * \brief Static pointer to the singleton instance of this class.
 *
 * Only changes in the function getTextures().
 */
Objects *Objects::m_objects = 0;

/**
 * \brief The public access for the singleton object.
 *
 * If accessed the first time a new instance is created,
 * otherwise the old instance is returned. 
 * \return the singleton instance
 */
Objects *Objects::getObjects ( void ) {
  if( m_objects == 0 )
    m_objects = new Objects;
  return m_objects;
}

/**
 * \brief The constructor initializes space for 10 objects.
 */
Objects::Objects() {
  reserve(10);
}

/**
 * \brief The destructor clears the object list.
 */
Objects::~Objects( void ) {
  objectList.clear();
  m_objects = 0;
}

/**
 * \brief Loads an object from a file to a given position into the object list.
 * \param object number of the object
 * \param filename filename of the object file 
 * \param factor the factor by which the loaded object is stretched
 */
void Objects::loadObject( unsigned int object, string filename, float factor ) {
  // if the size of the vector is not big enough to access object, resize it. therefore create enough
  // space to access objectList[object], that means heighten size to object+1
  if( object >= objectList.size() )
    reserve( object + 1 );

  // free object, if another was assigned
  if( objectList[object].object != -1 ) {
    freeObject( object );
  }

  // load new object. currently, only 3ds is usable. all files are identified by their ending
  // the loader should test if the file contains data in the right type
  if( LittleHelper::stringCmpi( filename.substr(filename.length() - 3, 3), "3ds" ) == 0) {
    loadObject3DS( object, filename, factor );
  }
}

/**
 * \brief Loads a 3d studio object from a given filename.
 * 
 * The model is loaded using the ObjectLoader3ds class and
 * the information is used to create the diplay list. After
 * loading the model the structure which should hold the object
 * propertys is initialized, therefore the view has to be submitted.
 * \param object the object that is to be loaded
 * \param filename the file which contains the mesh data
 */
void Objects::loadObject3DS( unsigned int object, string filename, float factor ) {
  ObjectInfo oi;
  ObjectLoader3ds *model;

  oi.filename = filename;
  oi.object = object;
  model = new ObjectLoader3ds();
  model->LoadModel( filename.c_str() );
  oi.displayList = createDisplayList( model, factor );
  delete model;
  oi.view = Vector3( 0, 0, 1 );
  objectList[object] = oi;
}

/**
 * \brief Frees one object.
 *
 * Reset the object to the initial values, that means the object is set to an
 * object named '1-'. This value should be used to test if the object exists 
 * the display list can't be set to an invalid value and it's not easy to
 * check if the view is invalid. Same for the string.
 * \param object the object that is to be deleted
 */
void Objects::freeObject( unsigned int object ) {
  if( object >= objectList.size() ) // try to access texture that is not in vector
    return;
  if( objectList[object].object == -1 ) // try to free object which is assigned to no object
    return;

  glDeleteLists( objectList[object].displayList, 1 );

  objectList[object].object = -1;
  objectList[object].displayList = 0;
  objectList[object].view = Vector3( 0, 0, 1 );
  objectList[object].filename = "";
}

//! 
/**
 * \brief Reserves space for some objects.
 *
 * Reserves space in order to save objects and spare some time because the 
 * vector does not need to resize itself. The objects are initialized with the *
 * ball object. 
 * \param objectCount maximal count of objects 
 */
void Objects::reserve( unsigned int objectCount ) {
  if( objectCount <= objectList.size() )// new size is not bigger than the old one
    return;

  size_t oldSize = objectList.size();

  objectList.reserve( objectCount );
  objectList.resize ( objectCount );

  // initialize new texture space
  for( size_t i=oldSize ; i < objectList.size(); i++ ) {
    ObjectInfo oi;
    objectList[i] = oi;
  }
}

/**
 * \brief Returns the display list that was created for an object.
 * \param object the object
 */
GLuint Objects::getDisplayList ( unsigned int object ) {
  if( (unsigned)object > objectList.size() )
    return 0;

  return objectList[object].displayList;
}

/**
 * \brief Returns the view assigned to an object. 
 * \param object the object
 */
Vector3 Objects::getObjectView ( unsigned int object ) {
  if( (unsigned)object > objectList.size() )
    return Vector3( 0, 0, 0 );

  return objectList[object].view;
}

//! Creates the display list for an object.
/**
 * Creates a new display list that displays a model. It draws all vertices of
 * a model, calculates the colors and normals and sets texture coordinates.
 * \param model the model, contains mesh data 
 * \param factor a factor that scales the model size
 */
GLuint Objects::createDisplayList( ObjectLoader3ds * model, float factor ) {
  GLuint list = glGenLists(1);
  glNewList( list, GL_COMPILE ); // New Compiled box Display List
  glPushAttrib( GL_ALL_ATTRIB_BITS );
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  // Loop through all meshes that are apart of the file
  // and render them.
  //glBindTexture( GL_TEXTURE_2D, m_texture );// textures are at present disabled!
  glDisable( GL_TEXTURE_2D );
  for(int i = 0; i < model->totalMeshes; i++) {
    glBegin(GL_TRIANGLES);

    // Draw each triangle of this mesh.
    for(unsigned int f = 0; f < model->meshList[i].totalFaces; f++) {
      // Get pointers to make the below code cleaner.
      stMesh *pMesh = &model->meshList[i];
      stFace *pFace = pMesh->pFaces;
      stTexCoord *pTexCoord = pMesh->pTexCoords;

      // If one or both or NULL, we got a problem.
      if(!pMesh || !pFace) continue;

      // Draw the triangle.
      glNormal3f(pFace[f].normal.x, pFace[f].normal.y,
        pFace[f].normal.z);

      // set the color
      int material = pFace[f].matId;

      stMaterial *pMaterial = &model->materialList[material];

      float color[] = {1,0,0,1};

      glColor3f((float) pMaterial->colorDiffuse.r / 255.0, (float) pMaterial->colorDiffuse.g / 255.0, (float) pMaterial->colorDiffuse.b / 255.0);
      //glMaterialf( GL_FRONT, GLenum pname, TYPE param ) // only for shininess

      //glMaterialf( GL_FRONT, GL_SHININESS, 0.0);
      //color[0] = (float) pMaterial->colorAmbient.r / 255.0;
      //color[1] = (float) pMaterial->colorAmbient.g / 255.0;
      //color[2] = (float) pMaterial->colorAmbient.b / 255.0;
      //glMaterialfv( GL_FRONT, GL_AMBIENT, color );

      //color[0] = (float) pMaterial->colorDiffuse.r / 255.0;
      //color[1] = (float) pMaterial->colorDiffuse.g / 255.0;
      //color[2] = (float) pMaterial->colorDiffuse.b / 255.0;
      //glMaterialfv( GL_FRONT, GL_DIFFUSE, color );

      //color[0] = (float) pMaterial->colorSpecular.r / 255.0;
      //color[1] = (float) pMaterial->colorSpecular.g / 255.0;
      //color[2] = (float) pMaterial->colorSpecular.b / 255.0;

      //color[0] = 1;
      //color[1] = 0;
      //color[2] = 1;

      // set speculor to black, it doesn't work correctly with other values -> why?
      glMaterialfv( GL_FRONT, GL_SPECULAR, color );


      // calculate normal
      Vector3 p1;
      p1.x = pMesh->pVertices[pFace[f].indices[0]].x*factor;
      p1.y = pMesh->pVertices[pFace[f].indices[0]].y*factor;
      p1.z = pMesh->pVertices[pFace[f].indices[0]].z*factor;
      Vector3 p2;
      p2.x = pMesh->pVertices[pFace[f].indices[1]].x*factor;
      p2.y = pMesh->pVertices[pFace[f].indices[1]].y*factor;
      p2.z = pMesh->pVertices[pFace[f].indices[1]].z*factor;
      Vector3 p3;
      p3.x = pMesh->pVertices[pFace[f].indices[2]].x*factor;
      p3.y = pMesh->pVertices[pFace[f].indices[2]].y*factor;
      p3.z = pMesh->pVertices[pFace[f].indices[2]].z*factor;

      Vector3 normale = VectorMath::normal(p1, p2, p3);

      glNormal3f(normale.x, normale.y, normale.z);

      if(pTexCoord) glTexCoord2f(pTexCoord[pFace[f].indices[0]].tu, pTexCoord[pFace[f].indices[0]].tv);
      glVertex3f(pMesh->pVertices[pFace[f].indices[0]].x  * factor,
        pMesh->pVertices[pFace[f].indices[0]].y *factor,
        pMesh->pVertices[pFace[f].indices[0]].z *factor);

      if(pTexCoord) glTexCoord2f(pTexCoord[pFace[f].indices[1]].tu, pTexCoord[pFace[f].indices[1]].tv);
      glVertex3f(pMesh->pVertices[pFace[f].indices[1]].x  * factor,
        pMesh->pVertices[pFace[f].indices[1]].y  * factor,
        pMesh->pVertices[pFace[f].indices[1]].z  * factor);

      if(pTexCoord) glTexCoord2f(pTexCoord[pFace[f].indices[2]].tu, pTexCoord[pFace[f].indices[2]].tv);
      glVertex3f(pMesh->pVertices[pFace[f].indices[2]].x *factor,
        pMesh->pVertices[pFace[f].indices[2]].y *factor,
        pMesh->pVertices[pFace[f].indices[2]].z *factor);
    }
    glEnd();
  }
  glPopAttrib();
  glEndList();
  //listCreated = true;
  return list;
}