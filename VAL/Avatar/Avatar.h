//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Avatar.h - 16/06/2000 - James Smith
//	Avatar class header
//
// $Id: Avatar.h,v 1.13 2000/11/29 21:16:02 james Exp $
//

#ifndef _VAL_AVATAR_
#define _VAL_AVATAR_

#pragma once

//Standard includes
#include <vector>

// libVapour includes
#include "Val.h"
#include "Point3D.h"
#include "TriFace.h"
#include "DList.h"

// Avatar includes
#include "BodyPart.h"
#include "AvatarPose.h"

// VAL includes
#include "TriMesh.h"

// OpenGL includes
#include <gl/gl.h>

// DLL import/export definitions
#ifndef DLL
	#ifdef VAL_DLL_EXPORT
		#define DLL __declspec(dllexport)
	#endif
	#ifdef VAL_DLL_IMPORT
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

class DLL CAvatar : public CTriMesh {

public:

  	// Enumerated type for status flag
   enum AV_STATUS {
		AV_OK = 0,
		AV_NOALLOC,
      AV_BADDATA
	};

   enum AV_SEX {
      SEX_UNKNOWN = 0,
      SEX_MALE,
      SEX_FEMALE
   };

   enum AV_AGE {
      AGE_UNKNOWN = 0,
      AGE_ADULT,
      AGE_TEEN,
      AGE_CHILD
   };
   
   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates an Avatar with the specified number of vertices and faces.
	CAvatar(int iNumVertices, int iNumFaces);

   virtual ~CAvatar();

   ///////////////////////////////////////////////////////////////////////
   // Loading Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
	// Sets the error status of the avatar
	void SetStatus(AV_STATUS eStatus) {m_eStatus = eStatus;}

   // Adds the specified number of vertices
   // Returns true if addition is successful
   // Do not use this too much - it is very expensive.
   virtual bool AddVertices(int iNumVertices);

   // Adds the specified number of faces
   // Returns true if addition is successful
   // Do not use this too much - it is very expensive.
   virtual bool AddFaces(int iNumFaces);

   // Enables the loading functions
   void StartLoad(void) {m_bLoading = true;}

   // Sets the sex of the model.
   void SetSex(AV_SEX eSex) {if (m_bLoading) m_eSex = eSex;}

   // Sets the age group of the model.
   void SetAge(AV_AGE eAge) {if (m_bLoading) m_eAge = eAge;}

   // Enters joint centre information into a body part
   void SetCentre(BodyPart bpPart, double dX, double dY, double dZ);

   // Associates a material with a body part
   void AssociateMaterial(BodyPart bpPart, int iMaterialIndex);
   
   // Associates a vertex with a body part
   void AssociateVertex(BodyPart bpPart, int iVertexIndex);

   // Associates a face with a body part
   void AssociateFace(BodyPart bpPart, int iFaceIndex);

   // Enters skeleton information into a body part
   void SetupSkeleton(BodyPart bpPart, BodyPart bpParent, BodyPart bpChild0 = unknown, BodyPart bpChild1 = unknown, BodyPart bpChild2 = unknown);

   // Enters joint limit information into a body part
   void SetJointLimit(BodyPart bpPart, double dMaxX, double dMaxY, double dMaxZ, double dMinX, double dMinY, double dMinZ);

   // Enters joint damping information into a body part
   void SetJointDamping(BodyPart bpPart, double dDampX, double dDampY, double dDampZ);

   // Fixed body part vertex associations - this needs to be done before shape
   // calculations can be done. It's a bit of a hack at the moment and will be replaced.
   void FixVertexAssociations(void);
   
   // Fixed body part face associations - this needs to be done before shape
   // calculations can be done. It's a bit of a hack at the moment and will be replaced.
   void FixFaceAssociations(void);

   // Disables the loading functions, and amongst other things calculates normals
   // If any body sections are not complete, they should be passed as parameters here
   void FinishLoad(int iMissingParts = 0);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
	// Gets the current error status of the avatar
	AV_STATUS Status(void) const {return m_eStatus;}

	// Provides direct access to the bodypart array
   const SBodyPart* BodyParts(void) const {return m_pBodyParts;}

   // Get which part is associated with a face
   int GetFacePart(int iFaceIndex) const {return m_piPartPerFaceMap[iFaceIndex];}

   // Get which part is associated with a vertex
   int GetVertexPart(int iVertexIndex) const {return m_piPartPerVertexMap[iVertexIndex];}

   ///////////////////////////////////////////////////////////////////////
   // Texture Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // Find out which material maps to the specified body part
   int MaterialIndex(BodyPart bpPart) const;

   ///////////////////////////////////////////////////////////////////////
   // Info Functions /////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Gets the sex of the model
   AV_SEX Sex(void) const {return m_eSex;}

   // Gets the age group of the model
   AV_AGE Age(void) const {return m_eAge;}

   // Returns the total height of the model
   double Height(void) const {return m_dHeight;}

   // Calculates a bounding box for the entire model
   void BoundingBox(SPoint3D& max, SPoint3D& min) const {CTriMesh::BoundingBox(max,min);}

   // Calculates a bounding box for a body part. 
   // If body part is -1, a bounding box is calculated for the whole avatar.
   void BoundingBox(BodyPart bpBodyPart, SPoint3D& max, SPoint3D& min) const;

   ///////////////////////////////////////////////////////////////////////
   // Posing Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Zeroes all joint rotations and translations. This moves the model to the default position
   void ResetPose(void);

   // Sets the angle for a particular joint.
	// This new angle will only take effect when UpdateModel() is called.   
   void SetJointAngle(BodyPart bpJoint, CAxisRotation& rotNewRotation, bool bLimit = true, bool bDamp = true);

	// Gets the angle for a particular joint
	CAxisRotation GetJointAngle(BodyPart bpJoint) const;

	// Sets a translation for the entire model
	// This new translation will only take effect when UpdateModel() is called.
   void SetRootTranslation(CVector3D& vecNewTranslation);

	// Gets the translation of the entire model
	CVector3D GetRootTranslation() const;

   // Sets the angle for the joint so that it points as close as possible
	// towards the target position.
   void TargetBodyPart(BodyPart bpJoint, CVector3D& vecTarget, bool bLimit = true);

	// Sets the angle for the joint so that it points as close as possible
	// in the direction of the target vector.
   void AlignBodyPart(BodyPart bpJoint, CVector3D& vecTarget, bool bLimit = true);

   // Uses an IK system to position bpJoint as close as possible to vecTarget.
   // The sksleton will be moved as far up as bpFixed, which will not move.
   // bTranslation specifies whether the root joint of the model is allowed to translate (if used).
   // bLimit specifies whether joint limit information will be used to constrain the motion.
   // bDamp specified whether joint damping factors are used during joint limitation.
   void IKSetPose(BodyPart bpJoint, CVector3D& vecTarget, BodyPart bpFixed = unknown, bool bTranslation = false, bool bLimit = true, bool bDamp = true);
   
   ///////////////////////////////////////////////////////////////////////
   // Mutilation Functions ///////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Sets all vertices attached to bpJoint and below to be at the position of bpJoint,
   // with the effect of chopping the body off at that joint.
   void Chop(BodyPart bpJoint);

   // Sets all vertices attached to bpJoint and below to be at vecPosition,
   // with the effect of chopping the body off at that joint.
   void Chop(BodyPart bpJoint, const CVector3D& vecPosition);

   ///////////////////////////////////////////////////////////////////////
   // Pose Storage Functions /////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Copies all current joint rotations and translations into a new CAvatarPose.
   CAvatarPose ExportPose(void) const;

   // Copies in all the rotations and translations from the passed CAvatarPose.
   // returns true if successful, false if not
   bool ImportPose(CAvatarPose& apNewPose);

   // Copies in some of the rotations and translations from the passed CAvatarPose.
   // returns true if successful, false if not
   bool ImportPosePart(BodyPart bpJoint, CAvatarPose& apNewPose);

   // Pushes a copy of the current pose onto the pose stack. This can be
   // restored with PopPose(). Returns true if successful, false otherwise.
   bool PushPose(void);

   // Pops off the pose at the top of the pose stack, and puts it back into 
   // the avatar. If this function fails, the pose stays on top of the stack. 
   // It is not discarded. Returns true if successful, false otherwise.
   bool PopPose(void);

   ///////////////////////////////////////////////////////////////////////
   // Post-Load Functions ////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Translates the X coordinates of the default model so that the root is on the Y-Z plane
   void UnTranslateDefaultModel(void);

   // Aligns the joint centre of bpStart with vecTarget, centred on it's parent.
   // All child joints have their default centres updated.
   void AlignDefaultCentres(BodyPart bpStart, CVector3D vecTarget);

	// Calculates the default vertices, based upon the current vertices, 
   // current rotations and default centres.
   void CalculateDefaultVertices(void);

   ///////////////////////////////////////////////////////////////////////
   // Merging Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // If this avatar is partially complete, this function merges it with information from pOtherAvatar
   // iSections uses the definitions in BodyPart.h for body sections: BSHEAD, BSTORSO and BSLEGS
   // Everything is copied, so pOtherAvatar can be safely deleted or modified after this.
   // Returns 1 if successful
   int Merge(const CAvatar* pOtherAvatar);

   ///////////////////////////////////////////////////////////////////////
   // Animation Functions ////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Updates the vertex and joint positions of the model.
	// Normals will be recalculated if bRecalculateNormals is true.
   // Face normals only will be calculated if bVertexNormals is false, 
   // vertex normals also if it is true.
   void UpdateModel(bool bRecalculateNormals = true, bool bVertexNormals = true);

   // Updates all joint positions below the starting point (bpBodyPart)
   // htTransform is applied to all joints
   void UpdateSkeleton(enum BodyPart bpBodyPart, CHomTransform& htTransform, bool bDirty);

   // Updates shape based on joint positions
   void UpdateShape(enum BodyPart joint, CHomTransform& htTransform, bool bDirty);

private:
	
   // Access & Status info
   bool m_bLoading;
	AV_STATUS m_eStatus;

   // Geometry
	SPoint3D* m_pDefaultVertices;

   // Topology
   int* m_piPartPerFaceMap;
   int* m_piPartPerVertexMap;

   // Animation structure
   SBodyPart* m_pBodyParts;

   // Root Animation
   bool m_bDirtyTranslation;
   CVector3D m_vecRootTranslation;

   // Avatar info
   AV_SEX m_eSex;
   AV_AGE m_eAge;
   double m_dHeight;
   int m_iMissingParts;

   // Pose storage
   CDList<CAvatarPose*> m_oPoseStack;

   ///////////////////////////////////////////////////////////////////////
   // Post-Load Functions ////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Builds the reverse maps:
   // m_plFacesPerVertexMap: relates a vertex to it's associated faces
   // m_piPartPerFaceMap: relates a face to a part
   void BuildMaps(void);

   ///////////////////////////////////////////////////////////////////////
   // UnPose Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Rotates the default centres below this point.
   void RotateDefaultCentres(BodyPart bpBodyPart, CHomTransform& htTransform);

   // UnTransforms a body part and it's children (sets default vertices from current data)
   void UnRigidTransform(BodyPart joint, CHomTransform& htTransform);

   // Translates the default positions of the joint and it's children
   void TranslateDefault(BodyPart joint, CVector3D& vecTranslation);

   //#===--- Friend Classes
	friend class CRenderAvatar;

};

#endif //_VAL_AVATAR_