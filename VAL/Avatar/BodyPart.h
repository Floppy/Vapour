//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// BodyPart.h - 17/06/2000 - James Smith
//	Body Part information structures and types
//
// $Id: BodyPart.h,v 1.6 2000/11/30 11:26:29 warren Exp $
//

#ifndef _VAL_BODYPART_
#define _VAL_BODYPART_

// libVapour includes
#include "Point3D.h"
#include "HomTransform.h"
#include "AxisRotation.h"
#include "DList.h"

#pragma once

// The total number of body parts
#define TOTAL_NUMBER_BODYPARTS 95

// encoding for the different body sections (larger than parts)
#define BS_NONE  0x00
#define BS_HEAD  0x01
#define BS_TORSO 0x02
#define BS_LEGS  0x04
#define BS_ALL   0x07

// An enumerated type that encodes the different body parts
// These generally correspond to the H-Anim 1.1 recommendations
// These are joint names - segments are indexed by their parent joint
enum BodyPart {
	unknown = -1,
	root = 0,
	sacroiliac = 1,
	l_hip = 2,
	l_knee = 3,
	l_ankle = 4,
	l_subtalar = 5,
	l_midtarsal = 6,
	l_metatarsal = 7,
	r_hip = 8,
	r_knee = 9,
	r_ankle = 10,
	r_subtalar = 11,
	r_midtarsal = 12,
	r_metatarsal = 13,
	vl5 = 14,
	vl4 = 15,
	vl3 = 16,
	vl2 = 17,
	vl1 = 18,
	vt12 = 19,
	vt11 = 20,
	vt10 = 21,
	vt9 = 22,
	vt8 = 23,
	vt7 = 24,
	vt6 = 25,
	vt5 = 26,
	vt4 = 27,
	vt3 = 28,
	vt2 = 29,
	vt1 = 30,
	vc7 = 31,
	vc6 = 32,
	vc5 = 33,
	vc4 = 34,
	vc3 = 35,
	vc2 = 36,
	vc1 = 37,
	skullbase = 38,
	l_eyeball_joint = 39,
	r_eyeball_joint = 40,
	l_sternoclavicular = 41,
	l_acromioclavicular = 42,
	l_shoulder = 43,
	l_elbow = 44,
	l_wrist = 45,
	l_thumb1 = 46,
	l_thumb2 = 47,
	l_thumb3 = 48,
	l_index0 = 49,
	l_index1 = 50,
	l_index2 = 51,
	l_index3 = 52,
	l_middle0 = 53,
	l_middle1 = 54,
	l_middle2 = 55,
	l_middle3 = 56,
	l_ring0 = 57,
	l_ring1 = 58,
	l_ring2 = 59,
	l_ring3 = 60,
	l_pinky0 = 61,
	l_pinky1 = 62,
	l_pinky2 = 63,
	l_pinky3 = 64,
	r_sternoclavicular = 65,
	r_acromioclavicular = 66,
	r_shoulder = 67,
	r_elbow = 68,
	r_wrist = 69,
	r_thumb1 = 70,
	r_thumb2 = 71,
	r_thumb3 = 72,
	r_index0 = 73,
	r_index1 = 74,
	r_index2 = 75,
	r_index3 = 76,
	r_middle0 = 77,
	r_middle1 = 78,
	r_middle2 = 79,
	r_middle3 = 80,
	r_ring0 = 81,
	r_ring1 = 82,
	r_ring2 = 83,
	r_ring3 = 84,
	r_pinky0 = 85,
	r_pinky1 = 86,
	r_pinky2 = 87,
	r_pinky3 = 88,
   // Tips - used for alignment only
   l_foot_tip = 89,
   r_foot_tip = 90,
   skull_tip = 91,
   l_hand_tip = 92,
   r_hand_tip = 93,
   // Skirt - don't like this, it seems a bit of a hack.
   skirt = 94
};

//////////////////////////////////////////////////////////////////
// SBodyPart
//////////////////////////////////////////////////////////////////
struct SBodyPart {

	// The joint centre in the default pose
	SPoint3D m_pntDefaultCentre;
	// The joint centre in the current pose
	SPoint3D m_pntCurrentCentre;

   // The current rotation of the joint
	CAxisRotation m_rotCurrentRotation;

   // A flag to say whether the angle has changed recently
	bool m_bDirtyAngle;
   // A flag to say whether the shape has changed recently
	bool m_bDirtyShape;

	// The number of vertices that are mapped to the body part
	int m_iNumVertices;
	// A list of vertices that are mapped to the body part
	CDList<int>* m_pliVertices; // Temporary use during load
	int* m_piVertices;

	// The number of faces that are mapped to the body part
	int m_iNumFaces;
	// A list of faces that are mapped to the body part
	CDList<int>* m_pliFaces; // Temporary use during load
   int* m_piFaces;

   // Material information
   int m_iMaterialIndex;

   // A set of rotation limits
	double m_dMaxX;
	double m_dMaxY;
	double m_dMaxZ;
	double m_dMinX;
	double m_dMinY;
	double m_dMinZ;

   // The set of rotation damping factors
	double m_dDampX;
	double m_dDampY;
	double m_dDampZ;

	// The parent body part
	BodyPart m_bpParent;
	// A maximum of 3 child body parts
	BodyPart m_bpChildren[3];	

   // The transform below this point in the skeleton.
	CHomTransform m_htLowerTransform;

   // A static array of joint name strings
   static const char* Names[];

	// Constructor
	SBodyPart();

	// Destructor
	~SBodyPart();

};

#endif //_VAL_BODYPART_