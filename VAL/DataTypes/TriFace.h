//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TriFace.h - 04/03/2000 - James Smith
//	3D Trianglular Face structure header
//
// $Id: TriFace.h,v 1.4 2000/11/21 16:41:29 waz Exp $
//

#ifndef _VAL_TRIFACE_
#define _VAL_TRIFACE_

#pragma once

// Includes
#include "DList.h"
#include "TexCoord.h"
#include <vector>

struct SFaceVertex {
   int m_iVertex;
	STexCoord m_sTexCoord;
};

struct STriFace {
	SFaceVertex m_sVertices[3];
	double m_dNormal[3];
	int m_iMaterialNumber;
};

typedef std::vector<SFaceVertex> CTriStrip;

#endif // _VAL_TRIFACE_
