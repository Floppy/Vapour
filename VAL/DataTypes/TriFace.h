//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TriFace.h - 04/03/2000 - James Smith
//	3D Trianglular Face structure header
//
// $Id: TriFace.h,v 1.2 2000/06/17 10:42:12 waz Exp $
//

#ifndef _VAL_TRIFACE_
#define _VAL_TRIFACE_

#pragma once

// Includes
#include "TexCoord.h"

struct STriFace
{
	int m_iVertices[3];
	double m_dNormal[3];
	int m_iTextureNumber;
	STexCoord m_sTexCoords[3]; // texture coords
};

#endif // _VAL_TRIFACE_
