//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TriFace.h - 04/03/2000 - James Smith
//	3D Trianglular Face structure header
//
// $Id: TriFace.h,v 1.1 2000/06/16 21:59:21 waz Exp $
//

#ifndef _VAPOUR_TRIFACE_
#define _VAPOUR_TRIFACE_

#pragma once

// libVapour includes
#include "TexCoord.h"

struct STriFace
{
	int m_iVertices[3];
	double m_dNormal[3];
	int m_iTextureNumber;
	STexCoord m_sTexCoords[3]; // texture coords
};

#endif // _VAPOUR_TRIFACE_
