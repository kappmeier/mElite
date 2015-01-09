/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	FileAccess.cpp
 * \brief	Implementation of the FileAccess class that allows loading of 3d objects and textures used in the game.
 */
#include "GlobalParameters.h"
#include "FileAccess.h"
#include "Textures.h"
#include "Objects.h"

using namespace std;

static const TextureQuality TEXTURE_QUALITY = medium;// set quality

/**
 * \brief Gives the TextureSlot that is assigned to a given index.
 *
 * It is mainly a translation function to translate from an integer
 * to a TextureSlot.
 * \param index number of the planet slot whose nome is asked for
 */
TextureSlot FileAccess::getPlanetSlot( unsigned int index ) {
  if( index > planetSlotCount-1)
    return TextureSlot();
  return planetSlots[index];
}

/**
 * \brief Returns the filename for a given texture.
 * 
 * The filename is created from the information in the
 * static textureInfos[] array. If a texture is available 
 * in different sizes, an suffix is added to the name.
 * \param texture name of the texture 
 */
string FileAccess::getTextureFile( TextureName texture ) {
  TextureAccess ta = getTextureInfo( texture );
  if( ta.hasQualities ) {
    switch( TEXTURE_QUALITY ) {
case extraLow:
  return ta.name + "_xs." + ta.type;
  break;
case low:
  return ta.name + "_s." + ta.type;
  break;
case medium:
  return ta.name + "_m." + ta.type;
  break;
case high:
  return ta.name + "_l" + ta.type;
  break;
case extraHigh:
  return ta.name + "_xl." + ta.type;
  break;
default: // assume medium
  return ta.name + "_m." + ta.type;
    }
  } else
    return ta.name + "." + ta.type;
}

/**
 * \brief Returns the TextureInfo for a given texture.
 * 
 * Returns are the name, the filetype and the information
 * if the texture exists in multiple qualities. 
 * \param texture the texture name
 */
TextureAccess FileAccess::getTextureInfo( TextureName texture ) {
  return textureInfos[texture];
}

/**
 * Loads a texture which is provided as a TextureName into a TextureSlot.
 * 
 * This is procedure is done because the number of simultaneous loaded textures 
 * should be limited.
 * \param slot the slot in which the texture should be loaded 
 * \param texture the texture
 */
void FileAccess::loadTexture( TextureSlot slot, TextureName texture ) {
  TextureAccess ta = getTextureInfo(texture);
   Textures::getTextures()->loadTexture( slot, getTextureFile( texture ) );
}

/**
 * Returns the filename for an object given as ObjectName. 
 * \param object the given object 
 */
string FileAccess::getObjectFile( ObjectName object ) {
  return getObjectInfo( object ).name;
}

/**
 * Returns the information for a given object. Returned are the filename and 
 * the view direction of the object.
 * \param object the given object 
 */
ObjectAccess FileAccess::getObjectInfo( ObjectName object ) {
  return objectInfos[object];
}

/**
 * Returns the view direction of a given object.
 * \param object the given object 
 */
Vector3 FileAccess::getObjectView( ObjectName object ) {
  return getObjectInfo( object ).view;
}

/**
 * Returns the scale factor of a given object.
 * \param object the given object 
 */
float FileAccess::getObjectFactor( ObjectName object ) {
  return getObjectInfo( object ).factor;
}

/**
 * \brief Loads a 3d object from a file.
 * 
 * In this procedure a file is loaded, the meshs are 
 * extracted and a display list is created.
 * \param object the given object 
 */
void FileAccess::loadObject( ObjectName object ) {
  Objects::getObjects()->loadObject( object, getObjectFile( object ), getObjectFactor( object ) );
}

const TextureSlot FileAccess::planetSlots[] = {
  planet1,
  planet2,
  planet3,
  planet4,
  planet5,
  planet6,
  planet7,
  planet8,
  planet9,
  planet10
};

/**
 * \brief A list of all textures that are needed.
 *
 * The textures have to be in the same order as defined in the enumeration.
 * Buth structures are in nearly alphabetical order. First the textures with only
 * one quality level are loaded.
 */
const TextureAccess FileAccess::textureInfos[] = {
  TextureAccess("./textures/stars_sun_korona", false,"bmp"),
  TextureAccess("./textures/bar_green_red",false,"bmp"),
  TextureAccess("./textures/radar", false,"bmp"),
  TextureAccess("./textures/sight1",false,"bmp"),
  TextureAccess("./textures/font2", false,"bmp"),// font 1 does not exist ;)
  TextureAccess("./textures/font2", false,"bmp"),
  TextureAccess("./textures/font3", false,"bmp"),
  TextureAccess("./textures/laser1",false,"tga"), 
  TextureAccess("./textures/missile", false,"tga"), 
  TextureAccess("./textures/dirt",false,"bmp"),
  TextureAccess("./textures/metall",false,"bmp"),
  // now we start with the planets and stuff in different qualities
  TextureAccess("./textures/planets_01",true, "bmp"),
  TextureAccess("./textures/planets_02",true, "bmp"),
  TextureAccess("./textures/planets_03",true, "bmp"),
  TextureAccess("./textures/planets_04",true, "bmp"),
  TextureAccess("./textures/planets_05",true, "bmp"),
  TextureAccess("./textures/planets_06",true, "bmp"),
  TextureAccess("./textures/planets_07",true, "bmp"),
  TextureAccess("./textures/planets_08",true, "bmp"),
  TextureAccess("./textures/planets_09",true, "bmp"),
  TextureAccess("./textures/planets_red",true, "bmp"),
  TextureAccess("./textures/planets_ss_earth", true, "bmp"),
  TextureAccess("./textures/planets_ss_moon",true, "bmp"),
  TextureAccess("./textures/planets_ss_jupiter", true, "bmp"),
  TextureAccess("./textures/planets_ss_mars",true, "bmp"),
  TextureAccess("./textures/planets_ss_mercury", true, "bmp"),
  TextureAccess("./textures/planets_ss_neptune", true, "bmp"),
  TextureAccess("./textures/planets_ss_pluto", true, "bmp"),
  TextureAccess("./textures/planets_ss_saturn",true, "bmp"),
  TextureAccess("./textures/planets_ss_uranus",true, "bmp"),
  TextureAccess("./textures/planets_ss_venus", true, "bmp"),
  TextureAccess("./textures/sun001",true, "bmp"),
  TextureAccess("./textures/sun002",true, "bmp"),
  TextureAccess("./textures/sun003",true, "bmp"),
  TextureAccess("./textures/sun004",true, "bmp"),
  TextureAccess("./textures/sun005",true, "bmp"),
  TextureAccess("./textures/sun006",true, "bmp"),
  TextureAccess("./textures/sun010",true, "bmp"),
  TextureAccess("./textures/sun011",true, "bmp"),
  TextureAccess("./textures/sun012",true, "bmp"),
  TextureAccess("./textures/sun013",true, "bmp"),
  TextureAccess("./textures/sun014",true, "bmp"),
  TextureAccess("./textures/stars_sun", true, "bmp"),
  TextureAccess("./textures/universe_galactic_center",true, "bmp"),
  TextureAccess("./textures/universe_cirrus_8_micron",true, "bmp"),
  TextureAccess("./textures/universe_iras-100um",true, "bmp"),
  TextureAccess("./textures/universe_iras",true, "bmp"),
};

/**
 * \brief A list of all 3d objects that are contained in the game.
 *
 * The files have to be ordered in the same way as in the object enumeration.
 */
const ObjectAccess FileAccess::objectInfos[] = {
  ObjectAccess( "./objects/myAdder.3ds",Vector3( 0, 0, 1 ), 0.02 ),
  ObjectAccess( "./objects/myAnaconda.3ds", Vector3( 0, 0, 1 ), 0.01 ),
  ObjectAccess( "./objects/ball.3ds",Vector3( 0, 0, 1 ), 1 ),
  ObjectAccess( "./objects/myCoriolis.3ds", Vector3( 0, 0, 1 ), 0.01 ),
  ObjectAccess( "./objects/myMissile.3ds",Vector3( 0, 0, 1 ), 0.01 ),
  ObjectAccess( "./objects/myThargoid.3ds", Vector3( 0, 0, 1 ), 0.05 ),
  ObjectAccess( "./objects/tieDroid.3ds",Vector3( 0,-1, 0 ), 0.01 )
};
