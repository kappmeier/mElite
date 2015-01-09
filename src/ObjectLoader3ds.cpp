/**
 * Copyright by Jan-Philipp Kappmeier, Melanie Schmidt
 * parts of code by  Allen Sherrod 
 * \file	ObjectLoader3ds.cpp
 * \brief	Implements the 3ds object loader class.
 */

#include<stdio.h>
#include<math.h>
#include<string.h>
#include"ObjectLoader3ds.h"
#include"3ds_chunks.h"

ObjectLoader3ds::ObjectLoader3ds() : totalMeshes(0), totalMaterials(0) {
  totalMeshes = 0;
}

ObjectLoader3ds::~ObjectLoader3ds() {
  shutdown();
}

bool ObjectLoader3ds::LoadModel(const char *fileName) {
  FILE *fp = NULL;
  stChunk primaryChunk;

  if(!fileName) return false;

  fopen_s(&fp, fileName, "rb");
  if(!fp) return false;

  // The first chunk in the file is called the primary chunk.
  ReadChunk(fp, &primaryChunk);

  // This test will tell us if this is a 3DS file.
  if(primaryChunk.id != PRIMARY_ID) return false;

  // Recursively read the model file.
  ReadSubChunk(fp, &primaryChunk);
  fclose(fp);

  stVertex v1, v2;
  stVertex n;

  // This is used to create triangle normals.If you want your
  // 3DS loader to support bump mapping, adding s and t tangents
  // below will do that for you.
  for(int i = 0; i < totalMeshes; i++)
  {
    for(unsigned int f = 0; f < meshList[i].totalFaces; f++)
    {
      stMesh *pMesh = &meshList[i];
      stFace *pFace = pMesh->pFaces;

      if(!pMesh || !pFace) continue;

      v1.x = pMesh->pVertices[pFace[f].indices[0]].x - pMesh->pVertices[pFace[f].indices[1]].x;
      v1.y = pMesh->pVertices[pFace[f].indices[0]].y - pMesh->pVertices[pFace[f].indices[1]].y;
      v1.z = pMesh->pVertices[pFace[f].indices[0]].z - pMesh->pVertices[pFace[f].indices[1]].z;

      v2.x = pMesh->pVertices[pFace[f].indices[1]].x - pMesh->pVertices[pFace[f].indices[2]].x;
      v2.y = pMesh->pVertices[pFace[f].indices[1]].y - pMesh->pVertices[pFace[f].indices[2]].y;
      v2.z = pMesh->pVertices[pFace[f].indices[1]].z - pMesh->pVertices[pFace[f].indices[2]].z;

      n.x = ((v1.y  * v2.z) - (v1.z  * v2.y));
      n.y = ((v1.z  * v2.x) - (v1.x  * v2.z));
      n.z = ((v1.x  * v2.y) - (v1.y  * v2.x));

      float len = 1 / (float)sqrt((n.x  * n.x + n.y  * n.y + n.z  * n.z));
      n.x *= len; n.y *= len; n.z *= len;

      pFace[f].normal.x = n.x;
      pFace[f].normal.y = n.y;
      pFace[f].normal.z = n.z;
    }
  }
  return true;
}


void ObjectLoader3ds::ReadSubChunk(FILE *fp, stChunk *pChunk)
{
  stMesh mesh;
  stMesh *pMesh = NULL;
  stMaterial *pMat = NULL;
  stMaterial mat;
  char name[256];

  // Keep reading until we have finished reading this chunk.
  while(pChunk->bytesRead < pChunk->length)
  {
    // Read the next chunk.
    stChunk chunk;
    ReadChunk(fp, &chunk);

    // Depending on the chunk will depend on what
    // is being loaded.
    switch(chunk.id)
    {
    case EDIT3DS:
      // Read the next chunk to get the actual data.
      // sobald er das erste mal hier ist, ist das laden fertig. (für ball.3ds)
      ReadSubChunk(fp, &chunk);
      break;

    case NAMED_OBJECT:
      // Add a new mesh object to the list.
      meshList.push_back(mesh);
      totalMeshes++;

      // Load mesh data.
      chunk.bytesRead += GetNextString(fp, name);
      pMesh = &(meshList[totalMeshes - 1]);
      strcpy_s(pMesh->name, name);

      // bis hierher fehlerfrei
      ReadSubChunk(fp, &chunk);
      break;

    case OBJ_MESH:
      // Read the next chunk to get the actual data.
      // bis hierher ok
      ReadSubChunk(fp, &chunk);
      break;

    case MAT_ENTRY:
      // Add a new material to the list then read it.
      materialList.push_back(mat);
      totalMaterials++;

      ReadSubChunk(fp, &chunk);
      break;

    case MAT_NAME:
      // Read materials name and save it.
      chunk.bytesRead += GetNextString(fp, name);
      pMat = &(materialList[totalMaterials - 1]);
      strcpy_s(pMat->name, name);
      break;

    case MAT_TEXFLNM:
      // Read texture filename and save it.
      chunk.bytesRead += GetNextString(fp, name);
      pMat = &(materialList[totalMaterials - 1]);
      strcpy_s(pMat->textureName, name);
      break;

    case MAT_DIFFUSE:
      // Read diffuse colors.
      ReadDiffuse(fp, &chunk);
      break;

    case MAT_AMBIENT:
      // Read diffuse colors.
      ReadAmbient(fp, &chunk);
      break;

    case MAT_SPECULAR:
      // Read diffuse colors.
      ReadSpecular(fp, &chunk);
      break;

    case MAT_TEXMAP:
      // Read the next chunk to get the actual data.
      // bis hier ist alles in ordnung, texturteil scheint auch ok zu sein
      ReadSubChunk(fp, &chunk);
      break;

    case MESH_FACES:
      // Read triangles.
      ReadFaces(fp, &chunk);
      break;

    case MESH_VERTICES:
      // Read vertex points.
      // bis hierher alles korrekt
      ReadVertices(fp, &chunk);
      break;

    case MESH_TEX_VERT:
      // Read texture coords.
      ReadTexCoords(fp, &chunk);
      break;

    case MESH_MATER:
      // Read material indices.
      ReadMeshMaterials(fp, &chunk);
      break;

    default:
      // Skip chunk cuz we don't support
      // whatever is here.
      MoveToNextChunk(fp, &chunk);
      break;
    }

    // Add to the total bytes loaded.
    pChunk->bytesRead += chunk.length;
  }
}


void ObjectLoader3ds::MoveToNextChunk(FILE *fp, stChunk *pChunk)
{
  int buff[50000];
  // Skip a chunks data.
  fread(buff, 1, pChunk->length - pChunk->bytesRead, fp);
}


size_t ObjectLoader3ds::GetNextString(FILE *fp, char *str)
{
  char buff[100] = { 0 };
  int index = 0;

  // Read an entire string.
  fread(buff, 1, 1, fp);
  while(*(buff + index) != 0)
  {
    index++;
    fread(buff + index, 1, 1, fp);
  }

  // Save the string and return the size of the string.
  strcpy(str, buff);
  return strlen(buff) + 1;
}


void ObjectLoader3ds::ReadChunk(FILE *fp, stChunk *pChunk)
{
  if(!pChunk) return;

  // Read chunk header.
  pChunk->bytesRead = fread(&pChunk->id, 1, 2, fp); // lies 2 bytes = 16 bit = unsigned short
  pChunk->bytesRead += fread(&pChunk->length, 1, 4, fp); // lies 4 bytes = 32 bit = unsigned int
}


// warning! in this function it's not checked if the color is _not_ 24 bit rgb-format! the 3ds format
// also allows float? colors.
// sometimes (normally?) two color values are saved: COLOR_24 and LIN_COLOR_24. sometimes (often?) the two
// colors are the same, but not always. in this case the second one is read. it was useful for the tie model ;)
void ObjectLoader3ds::ReadDiffuse(FILE *fp, stChunk *pChunk) {
  stColor color;
  unsigned char header[6];

  // Read a color value.
  pChunk->bytesRead += fread(header, 1, 6, fp); // read header, it's ignored now
  pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

  // Save this color.
  stMaterial *mat = &(materialList[totalMaterials - 1]);
  mat->colorDiffuse.r = color.r;
  mat->colorDiffuse.g = color.g;
  mat->colorDiffuse.b = color.b;

  if(pChunk->length ==24) {
    // if the space in this chunk is 24 read again. it has to be LIN_COLOR_24 as second color information
    pChunk->bytesRead += fread(header, 1, 6, fp); // read header
    pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

    // Save this color.
    stMaterial *mat = &(materialList[totalMaterials - 1]);
    mat->colorDiffuse.r = color.r;
    mat->colorDiffuse.g = color.g;
    mat->colorDiffuse.b = color.b;
  }

  // Skip the rest of the chunk, if there's any
  MoveToNextChunk(fp, pChunk);
}

// warning! in this function it's not checked if the color is _not_ 24 bit rgb-format! the 3ds format
// also allows float? colors.
// sometimes (normally?) two color values are saved: COLOR_24 and LIN_COLOR_24. sometimes (often?) the two
// colors are the same, but not always. in this case the second one is read. it was useful for the tie model ;)
void ObjectLoader3ds::ReadAmbient(FILE *fp, stChunk *pChunk) {
  stColor color;
  unsigned char header[6];

  // Read a color value.
  pChunk->bytesRead += fread(header, 1, 6, fp); // read header, it's ignored now
  pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

  // Save this color.
  stMaterial *mat = &(materialList[totalMaterials - 1]);
  mat->colorAmbient.r = color.r;
  mat->colorAmbient.g = color.g;
  mat->colorAmbient.b = color.b;

  if(pChunk->length ==24) {
    // if the space in this chunk is 24 read again. it has to be LIN_COLOR_24 as second color information
    pChunk->bytesRead += fread(header, 1, 6, fp); // read header
    pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

    // Save this color.
    stMaterial *mat = &(materialList[totalMaterials - 1]);
    mat->colorAmbient.r = color.r;
    mat->colorAmbient.g = color.g;
    mat->colorAmbient.b = color.b;
  }

  // Skip the rest of the chunk.
  MoveToNextChunk(fp, pChunk);
}

// warning! in this function it's not checked if the color is _not_ 24 bit rgb-format! the 3ds format
// also allows float? colors.
// sometimes (normally?) two color values are saved: COLOR_24 and LIN_COLOR_24. sometimes (often?) the two
// colors are the same, but not always. in this case the second one is read. it was useful for the tie model ;)
void ObjectLoader3ds::ReadSpecular(FILE *fp, stChunk *pChunk) {
  stColor color;
  unsigned char header[6];

  // Read a color value.
  pChunk->bytesRead += fread(header, 1, 6, fp); // read header, it's ignored now
  pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

  // Save this color.
  stMaterial *mat = &(materialList[totalMaterials - 1]);
  mat->colorSpecular.r = color.r;
  mat->colorSpecular.g = color.g;
  mat->colorSpecular.b = color.b;

  if(pChunk->length ==24) {
    // if the space in this chunk is 24 read again. it has to be LIN_COLOR_24 as second color information
    pChunk->bytesRead += fread(header, 1, 6, fp); // read header
    pChunk->bytesRead += fread(&color, 1, 3, fp); // read colorbytes

    // Save this color.
    stMaterial *mat = &(materialList[totalMaterials - 1]);
    mat->colorSpecular.r = color.r;
    mat->colorSpecular.g = color.g;
    mat->colorSpecular.b = color.b;
  }

  // Skip the rest of the chunk.
  MoveToNextChunk(fp, pChunk);
}


void ObjectLoader3ds::ReadMeshMaterials(FILE *fp, stChunk *pChunk)
{
  char name[256];
  int matId = 0;
  unsigned short totalMatIndices = 0;
  unsigned short *pMatIndices = NULL;

  // Read material name and total.
  pChunk->bytesRead += GetNextString(fp, name);
  pChunk->bytesRead += fread(&totalMatIndices, 1, 2, fp);

  // Allocate the material indices.
  pMatIndices = new unsigned short[totalMatIndices];

  // Read indices.
  pChunk->bytesRead += fread(pMatIndices, 1, totalMatIndices *
    sizeof(unsigned short), fp);

  // Look if this material already exist in the list.
  for(int i = 0; i < totalMaterials; i++)
  {
    if(strcmp(name, materialList[i].name) == 0)
      matId = i;
  }

  // Get the current mesh.
  stMesh *pMesh = &(meshList[totalMeshes - 1]);

  // Add the material id to all faces that uses it.
  for(int i = 0; i < totalMatIndices; i++)
    pMesh->pFaces[pMatIndices[i]].matId = matId;
}


void ObjectLoader3ds::ReadFaces(FILE *fp, stChunk *pChunk)
{
  unsigned int totalFaces = 0;
  // totalVertices is an integer, 4 bytes have to be loaded. therefore initialized to 0
  stFileFace *pFaces;

  // Read number of faces.
  pChunk->bytesRead += fread(&totalFaces, 1, 2, fp);

  // Read faces.
  pFaces = new stFileFace[totalFaces];
  pChunk->bytesRead += fread(pFaces, 1, totalFaces *
    sizeof(stFileFace), fp);

  // Get current mesh and a pointer to its faces.
  stMesh *pMesh = &(meshList[totalMeshes - 1]);
  pMesh->pFaces = new stFace[totalFaces];
  pMesh->totalFaces = totalFaces;

  // Loop through and copy the face data.
  for(unsigned int i = 0; i < totalFaces; i++)
  {
    pMesh->pFaces[i].indices[0] = pFaces[i].indices[0];
    pMesh->pFaces[i].indices[1] = pFaces[i].indices[1];
    pMesh->pFaces[i].indices[2] = pFaces[i].indices[2];
  }

  // Delete temp memory.
  delete[] pFaces;

  // Read the next chunk.
  ReadSubChunk(fp, pChunk);
}

/**
 * \fn	void ObjectLoader3ds::ReadVertices(FILE *fp, stChunk *pChunk)
 *
 * \brief	Reads the list of vertices.
 * \param [in,out]	fp	  	pointer to the file
 * \param [in,out]	pChunk	pointer to the current chunk
 */

void ObjectLoader3ds::ReadVertices(FILE *fp, stChunk *pChunk)
{
  unsigned int totalVertices = 0;
  // totalVertices is an integer, 4 bytes have to be loaded. therefore initialized with 0
  stVertex *pVertices;

  // Read number of vertices.

  pChunk->bytesRead += fread(&totalVertices, 1, 2, fp);

  // Load vertex points.
  pVertices = new stVertex[totalVertices];
  pChunk->bytesRead += fread(pVertices, 1, totalVertices *
    sizeof(stVertex), fp);

  // Get the current mesh and save the data to it.
  stMesh *pMesh = &(meshList[totalMeshes - 1]);
  pMesh->pVertices = pVertices;
  pMesh->totalVertices = totalVertices;

  // Skip any data left in this chunk.
  MoveToNextChunk(fp, pChunk);
}

/**
 * \fn	void ObjectLoader3ds::ReadTexCoords(FILE *fp, stChunk *pChunk)
 *
 * \brief	Reads texture coordinates from a file. Everything else but textures is skipped.
 * \param [in,out]	fp	  	pointer to the file
 * \param [in,out]	pChunk	pointer to the current chunk
 */
void ObjectLoader3ds::ReadTexCoords(FILE *fp, stChunk *pChunk)
{
  unsigned int totalTexCoords = 0;
  // totalTexCoords is an integer, 4 bytes have to be loaded. therefore initialized to 0
  stTexCoord *pTexCoords;

  // read total.
  pChunk->bytesRead += fread(&totalTexCoords, 1, 2, fp);

  // read all the tex coords.
  pTexCoords = new stTexCoord[totalTexCoords];
  pChunk->bytesRead += fread(pTexCoords, 1, totalTexCoords *
    sizeof(stTexCoord), fp);

  // get the current mesh and save the data to it.
  stMesh *pMesh = &(meshList[totalMeshes - 1]);
  pMesh->pTexCoords = pTexCoords;

  // skip anything that is left.
  MoveToNextChunk(fp, pChunk);
}

/**
 * \fn	void ObjectLoader3ds::shutdown()
 * \brief	releases all created objects after all meshes have been loaded and processed.
 */
void ObjectLoader3ds::shutdown() {
  // Loop through and make sure all data is released.
  for(int i = 0; i < totalMeshes; i++)
  {
    if(meshList[i].pFaces)
    {
      delete[] meshList[i].pFaces;
      meshList[i].pFaces = NULL;
    }

    if(meshList[i].pVertices)
    {
      delete[] meshList[i].pVertices;
      meshList[i].pVertices = NULL;
    }

    if(meshList[i].pTexCoords)
    {
      delete[] meshList[i].pTexCoords;
      meshList[i].pTexCoords = NULL;
    }

    meshList[i].totalFaces = 0;
    meshList[i].totalVertices = 0;
    meshList[i].totalTexCoords = 0;
  }

  meshList.erase( meshList.begin(), meshList.end());

  materialList.erase( materialList.begin(), materialList.end());

  totalMeshes = 0;
  totalMaterials = 0;
}
