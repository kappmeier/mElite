/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * parts of code by Allen Sherrod
 * \file	ObjectLoader3ds.h
 * \brief	Declares the object loader 3ds class that loads 3ds objects into a list of meshes.
 */

#ifndef _C3DS_OBJECT_H_
#define _C3DS_OBJECT_H_

#include<stdio.h>
#include<vector>

/**
 * \struct	stChunk
 * \brief	File chunk header
 */
struct stChunk {
  stChunk() : id(0), length(0), bytesRead(0) {}
  unsigned short id; // unsigned added
  size_t length;
  size_t bytesRead;
};

/**
 * \struct	stVertex
 * \brief	A vertex position in 3d space
 */
struct stVertex {
  stVertex() : x(0), y(0), z(0) {}
  float x, y, z;
};

/**
 * \struct	stFace
 * \brief	A triangle face represented by indices of vertices.
 */
struct stFace {
  stFace() : matId(0) { indices[0] = indices[1] = indices[2] = 0; }
  unsigned int indices[3];
  stVertex normal;
  int matId;
};

/**
 * \struct	stFileFace
 * \brief	A triangle face as it is represented in the file. Indices are indices for vertices.
 */
struct stFileFace {
  stFileFace() : vis(0) { indices[0] = indices[1] = indices[2] = 0; }
  unsigned short indices[3], vis;
};

/**
 * \struct	stColor
 * \brief	A RGB color
 */
struct stColor {
  stColor() : r(0), g(0), b(0) {}
  unsigned char r, g, b;
};

/**
 * \struct	stTexCoord
 *
 * \brief	Texture coordinates for a vertex
 */
struct stTexCoord {
  stTexCoord() : tu(0), tv(0) {}
  float tu, tv;
};

/**
 * \struct	stMaterial
 *
 * \brief	Material for a face.
 */
struct stMaterial {
  stMaterial() { name[0] = 0; textureName[0] = 0; }
  char name[256];
  stColor colorDiffuse;
  stColor colorAmbient;
  stColor colorSpecular;
  char textureName[256];
};

/**
 * \struct	stMesh
 * \brief	A mesh. Note that each file may contain more than one mesh.
 */
struct stMesh {
  stMesh() : pFaces(0), pVertices(0), pTexCoords(0),
    totalFaces(0), totalVertices(0), totalTexCoords(0) {}

  char name[256];

  stFace *pFaces;
  stVertex *pVertices;
  stTexCoord *pTexCoords;

  unsigned int totalFaces;
  unsigned int totalVertices;
  unsigned int totalTexCoords;
};

/**
 * \class	ObjectLoader3ds
 * \brief	Loads objects as meshes (i.e. triangle list) from 3ds files.
 * 			
 * A loader that skips through the recoursively defined 3ds files. All
 * unknown chunks are ignored, and only meshes are extracted. The meshes
 * may contain texture coordinates and material information, such as color.
 */
class ObjectLoader3ds {
public:
  ObjectLoader3ds();
  ~ObjectLoader3ds();

  // load a file
  bool LoadModel(const char *fileName);

  // deals with reading chunk headers.
  void ReadSubChunk(FILE *fp, stChunk *pChunk);
  void MoveToNextChunk(FILE *fp, stChunk *pChunk);
  size_t GetNextString(FILE *fp, char *str);
  void ReadChunk(FILE *fp, stChunk *pChunk);

  // read different sections out of the file like vertex
  // positions, tex coords, faces, etc.
  void ReadMaterials(FILE *fp, stChunk *pChunk);
  void ReadDiffuse(FILE *fp, stChunk *pChunk);
  void ReadAmbient(FILE *fp, stChunk *pChunk);
  void ReadSpecular(FILE *fp, stChunk *pChunk);
  void ReadMeshMaterials(FILE *fp, stChunk *pChunk);
  void ReadFaces(FILE *fp, stChunk *pChunk);
  void ReadVertices(FILE *fp, stChunk *pChunk);
  void ReadTexCoords(FILE *fp, stChunk *pChunk);

  void shutdown();

  // List of mesh objects if there are more than 1,
  // list of materials, and counters for each.
  std::vector<stMesh> meshList;
  std::vector<stMaterial> materialList;
  int totalMeshes;
  int totalMaterials;
};

#endif
