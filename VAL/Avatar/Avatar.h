//=======---
// Avanew
//-------
// Avatar editor and exporter
// Copyright 2000 Vapour Technology Ltd.
//
// Avatar.h - 16/06/2000 - James Smith
//	Avatar class header
//
// $Id: Avatar.h,v 1.2 2000/06/17 10:42:09 waz Exp $
//

#ifndef _VAL_AVATAR_
#define _VAL_AVATAR_

//Standard includes
#include <vector>

// libVapour includes
#include "Defs.h"
#include "Point3D.h"
#include "TriFace.h"
#include "DList.h"

// Avatar includes
#include "BodyPart.h"
#include "AvatarPose.h"

// Avanew includes
#include "Image.h"

// OpenGL includes
#include <gl/gl.h>

#pragma once

// DLL import/export definitions
#ifndef DLL_IMP_EXP
	#ifdef _EXP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllexport)
	#endif
	#ifdef _IMP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllimport)
	#endif
	#ifndef DLL_IMP_EXP
		#define DLL_IMP_EXP
	#endif
#endif

class DLL_IMP_EXP CAvatar {

public:

  	// Enumerated type for status flag
   enum AV_STATUS {
		AV_OK = 0,
		AV_NOALLOC
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
   
   // Enables the loading functions
   void StartLoad(void) {m_bLoading = true;}

   // Sets the sex of the model.
   void SetSex(AV_SEX eSex) {if (m_bLoading) m_eSex = eSex;}

   // Sets the age group of the model.
   void SetAge(AV_AGE eAge) {if (m_bLoading) m_eAge = eAge;}

   // Sets information for a single vertex
   void SetVertex(int iVertex, double dX, double dY, double dZ);

   // Enters data into a particular face
   void SetFace(int iFace, int iVertex0, int iVertex1, int iVertex2, int iTextureIndex, double iVertex0U, double iVertex0V, double iVertex1U, double iVertex1V, double iVertex2U, double iVertex2V);

   // Enters joint centre information into a body part
   void SetCentre(BodyPart bpPart, double dX, double dY, double dZ);

   // Associates a texture with a body part
   void AssociateTexture(BodyPart bpPart, int iTextureIndex);
   
   // Associates a vertex with a body part
   void AssociateVertex(BodyPart bpPart, int iVertexIndex);

   // Associates a face with a body part
   void AssociateFace(BodyPart bpPart, int iFaceIndex);

   // Enters skeleton information into a body part
   void SetupSkeleton(BodyPart bpPart, BodyPart bpParent, BodyPart bpChild0 = unknown, BodyPart bpChild1 = unknown, BodyPart bpChild2 = unknown);

   // Enters joint limit information into a body part
   void SetJointLimit(BodyPart bpPart, double dMaxX, double dMaxY, double dMaxZ, double dMinX, double dMinY, double dMinZ);

   // Fixed body part associations - this needs to be done before shape
   // calculations can be done. It's a bit of a hack at the moment and will be replaced.
   void FixAssociations(void);
   
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

	// Provides direct access to the current vertex positions
   const SPoint3D* Vertices(void) const {return m_pCurrentVertices;}
	
	// Access to the vertex normal array
   const SPoint3D* VertexNormals(void) const {return m_pVertexNormals;}

   // Access to the face array.
   const STriFace* Faces(void) const {return m_pFaces;}

   // Get which part is associated with a face
   int GetFacePart(int iFaceIndex) const {return m_piPartPerFaceMap[iFaceIndex];}

   // Get which part is associated with a vertex
   int GetVertexPart(int iVertexIndex) const {return m_piPartPerVertexMap[iVertexIndex];}

	// The number of vertices in the model.
	int NumVertices(void) const {return m_iNumVertices;}

	// The number of faces in the model.
	int NumFaces(void) const {return m_iNumFaces;}

   ///////////////////////////////////////////////////////////////////////
   // Texture Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // WARNING - TEXTURES ARE ACCESSIBLE FOR MODIFICATION
   //           AT ALL TIMES, NOT JUST DURING LOADING

   // Add a texture to the Avatar
   bool AddTexture(CImage* oTexture);

   // Replace a texture in the Avatar
   bool ReplaceTexture(int iTextureIndex, CImage* oTexture);

   // The number of textures in the model.
	int NumTextures(void) const {return m_lTextures.Length();}

   // Find out which texture maps to the specified body part
   int TextureIndex(BodyPart bpPart) const;

   // Extracts a texture pointer.
   CImage* Texture(int iTextureIndex) const;

   // Extracts a texture pointer. If it is not the last one, bMore is set to true, 
   // otherwise it is set to false.
   CImage* Texture(int iTextureIndex, bool& bMore) const;

   ///////////////////////////////////////////////////////////////////////
   // Info Functions /////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Gets the sex of the model
   AV_SEX Sex(void) const {return m_eSex;}

   // Gets the age group of the model
   AV_AGE Age(void) const {return m_eAge;}

   // Returns the total height of the model
   double Height(void) const {return m_dHeight;}

   // Calculates a bounding box for a body part. 
   // If body part is -1, a bounding box is calculated for the whole avatar.
   void BoundingBox(BodyPart bpBodyPart, SPoint3D& max, SPoint3D& min);

   ///////////////////////////////////////////////////////////////////////
   // Posing Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Zeroes all joint rotations and translations. This moves the model to the default position
   void ResetPose(void);

   // Sets the angle for a particular joint.
	// This new angle will only take effect when UpdateModel() is called.
   void SetJointAngle(BodyPart bpJoint, CAxisRotation& rotNewRotation, bool bLimit = true);

	// Sets a translation for the entire model
	// This new translation will only take effect when UpdateModel() is called.
   void SetRootTranslation(CVector3D& vecNewTranslation);

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
   void IKSetPose(BodyPart bpJoint, CVector3D& vecTarget, BodyPart bpFixed = unknown, bool bTranslation = false, bool bLimit = true);
   
   ///////////////////////////////////////////////////////////////////////
   // Pose Storage Functions /////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Copies all current joint rotations and translations into a new CAvatarPose.
   CAvatarPose ExportPose(void) const;

   // Copies in all the rotations and translations from the passed CAvatarPose.
   // returns true if successful, false if not
   bool ImportPose(CAvatarPose& apNewPose);

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

	// Explicitly calculates normals (no optimisation is done)
	// if bVertex is true, they are calculated for the vertices and faces
   // Otherwise , just for the faces.
	void CalculateNormals(bool bVertex = false);

private:
	
   // Access & Status info
   bool m_bLoading;
	AV_STATUS m_eStatus;

   // Geometry
   int m_iNumVertices;
	SPoint3D* m_pDefaultVertices;
	SPoint3D* m_pCurrentVertices;
	SPoint3D* m_pVertexNormals;

   // Topology
   int m_iNumFaces;
	STriFace* m_pFaces;
   CDList<int>* m_plFacesPerVertexMap;
   int* m_piPartPerFaceMap;
   int* m_piPartPerVertexMap;

   // Texture stuff
   CDList<CImage*> m_lTextures;

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
	friend class CGLAvatar;
	friend class CGLAvatarMorph;
	friend class CGLAvatarPos;

};

#endif //_VAL_AVATAR_
