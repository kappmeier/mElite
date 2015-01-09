/**
 * Copyright Jan-Philipp Kappmeier, Melanie Schmidt
 * \file	3ds_chunks.h
 * \brief	Defines a list of possible elements (chunks) in 3ds files.
 */

#ifndef _3DS_CHUNKS_H
#define _3DS_CHUNKS_H

#define PRIMARY_ID 0x4D4D
#define EDIT3DS 0x3D3D // 15677
#define KEYF3DS 0xB000
#define VERSION 0x0002
#define MESH_VERSION 0x3D3E // 15678
#define KFVERSION 0x0005
#define COLOR_F 0x0010
#define COLOR_24 0x0011 // the first of two color parts (e.g. for diffuse)
#define LIN_COLOR_24 0x0012 // the second of two color parts( e.g. for diffuse)
#define LIN_COLOR_F 0x0013
#define INT_PERCENTAGE 0x0030
#define FLOAT_PERC 0x0031
#define MASTER_SCALE 0x0100 // 256
#define IMAGE_FILE 0x1100
#define AMBIENT_LIGHT 0x2100
#define NAMED_OBJECT 0x4000 // 16384
#define OBJ_MESH 0x4100 // 16640
#define MESH_VERTICES 0x4110 // 16656
#define VERTEX_FLAGS 0x4111
#define MESH_FACES 0x4120
#define MESH_MATER 0x4130
#define MESH_TEX_VERT 0x4140
#define MESH_XFMATRIX 0x4160
#define MESH_COLOR_IND 0x4165
#define MESH_TEX_INFO 0x4170
#define HIRARCHY 0x4F00
#define MAT_ENTRY 0xAFFF // 45055
#define MAT_NAME 0xA000 // 40960
#define MAT_AMBIENT 0xA010 // 40976
#define MAT_DIFFUSE 0xA020 // 40992
#define MAT_SPECULAR 0xA030 // 41008
#define MAT_SHININESS 0xA040 // 41024
#define MAT_FALLOFF 0xA052
#define MAT_EMISSIVE 0xA080
#define MAT_SHADING 0xA100 // 41216
#define MAT_TEXMAP 0xA200 // 41472
#define MAT_TEXFLNM 0xA300
#define OBJECT_LIGHT 0x4600
#define OBJECT_CAMERA 0x4700
#define ANIM_HEADER 0xB00A
#define ANIM_OBJ 0xB002
#define ANIM_NAME 0xB010
#define ANIM_POS 0xB020
#define ANIM_ROT 0xB021
#define ANIM_SCALE 0xB022

#endif
