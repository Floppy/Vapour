//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Avatar.cpp - 17/06/2000 - James Smith
//	Avatar class implementation
//
// $Id: Avatar.cpp,v 1.13 2000/11/22 00:44:11 waz Exp $
//

#include "stdafx.h"

#include <math.h>

#include "Avatar.h"
#include "AvatarPose.h"
#include "EulerRotation.h"
#include "MathConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
// PUBLIC functions ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Construction/Destruction ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

CAvatar::CAvatar(int iNumVertices, int iNumFaces) : 
   CTriMesh(iNumVertices,iNumFaces),
   m_eStatus(AV_OK),
   m_pBodyParts(NULL),
   m_pDefaultVertices(NULL),
   m_piPartPerFaceMap(NULL),
   m_piPartPerVertexMap(NULL),
   m_bDirtyTranslation(false)
{
   NEWBEGIN
   m_pBodyParts = new SBodyPart[TOTAL_NUMBER_BODYPARTS];
   NEWEND("CAvatar::CAvatar() - body part allocation")
   NEWBEGIN
   m_pDefaultVertices = new SPoint3D[m_iNumVertices];
   NEWEND("CAvatar::CAvatar() - default vertex allocation")
   NEWBEGIN
   m_piPartPerFaceMap = new int[m_iNumFaces];
   NEWEND("CAvatar::CAvatar() - face->part map allocation")
   NEWBEGIN
   m_piPartPerVertexMap = new int[m_iNumVertices];
   NEWEND("CAvatar::CAvatar() - vertex->part map allocation");
	if (m_pBodyParts          == NULL ||
       m_pDefaultVertices    == NULL ||
       m_piPartPerFaceMap    == NULL ||
       m_piPartPerVertexMap  == NULL ) {
      // Set the status to say that we have not managed to allocate memory
		m_eStatus = AV_NOALLOC;
	}
   if (m_pBodyParts) {
      //initialise pointers and joint limits in body parts
	   for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
         m_pBodyParts[i].m_bDirtyAngle = false;
		   m_pBodyParts[i].m_bDirtyShape = false;
		   m_pBodyParts[i].m_iNumVertices = 0;
         m_pBodyParts[i].m_pliVertices = NULL;
		   m_pBodyParts[i].m_piVertices = NULL;
		   m_pBodyParts[i].m_iNumFaces = 0;
         m_pBodyParts[i].m_pliFaces = NULL;
		   m_pBodyParts[i].m_piFaces = NULL;
		   m_pBodyParts[i].m_dMaxX = V_PI;
		   m_pBodyParts[i].m_dMaxY = V_PI;
		   m_pBodyParts[i].m_dMaxZ = V_PI;
		   m_pBodyParts[i].m_dMinX = V_MINUS_PI;
		   m_pBodyParts[i].m_dMinY = V_MINUS_PI;
		   m_pBodyParts[i].m_dMinZ = V_MINUS_PI;
		   m_pBodyParts[i].m_dDampX = 1;
		   m_pBodyParts[i].m_dDampY = 1;
		   m_pBodyParts[i].m_dDampZ = 1;
         m_pBodyParts[i].m_bpChildren[0] = unknown;
         m_pBodyParts[i].m_bpChildren[1] = unknown;
         m_pBodyParts[i].m_bpChildren[2] = unknown;
         m_pBodyParts[i].m_bpParent = unknown;
      }
   }
   return;
} //CAvatar()

CAvatar::~CAvatar() {
   // Delete body parts
	if (m_pBodyParts) {
		for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
			if (m_pBodyParts[i].m_piVertices != NULL) delete [] m_pBodyParts[i].m_piVertices;
			if (m_pBodyParts[i].m_piFaces != NULL) delete [] m_pBodyParts[i].m_piFaces;
		}
		delete [] m_pBodyParts;
	}
   // Dump pose stack
   while (m_oPoseStack.Length() > 0) {
      delete m_oPoseStack.RemoveBack();
   }
   // Delete model data
	if (m_pDefaultVertices)    delete [] m_pDefaultVertices;
	if (m_piPartPerFaceMap)    delete [] m_piPartPerFaceMap;
	if (m_piPartPerVertexMap)  delete [] m_piPartPerVertexMap;
   // Done
   return;
} //~CAvatar()

///////////////////////////////////////////////////////////////////////
// Loading Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Adds the specified number of vertices
// Returns true if addition is successful
// Do not use this too much - it is very expensive.
bool CAvatar::AddVertices(int iNumVertices) {
   bool bResult = false;
   // Store number of vertices
   int iOldNumVertices = m_iNumVertices;
   // and call base class allocator
   bResult = CTriMesh::AddVertices(iNumVertices);
   // If base class addition succeeds
   if (bResult = true) {
      if (iOldNumVertices == 0) {
         // Allocate default vertex storage
         NEWBEGIN
         m_pDefaultVertices = new SPoint3D[m_iNumVertices];
         NEWEND("CAvatar::AddVertices() - default vertex allocation")
         // Allocate part per vertex map storage
         NEWBEGIN
         m_piPartPerVertexMap = new int[m_iNumVertices];
         NEWEND("CAvatar::AddVertices() - vertex->part map allocation");
         // Check to see if allocation has failed
	      if (m_pDefaultVertices   == NULL ||
             m_piPartPerVertexMap == NULL ) {
            // Dispose of any data that may have been allocated
            if (m_pVertices != NULL)           delete [] m_pVertices;
	         if (m_pVertexNormals != NULL)      delete [] m_pVertexNormals;
	         if (m_plFacesPerVertexMap != NULL) delete [] m_plFacesPerVertexMap;
	         if (m_pDefaultVertices != NULL)    delete [] m_pDefaultVertices;
	         if (m_piPartPerVertexMap != NULL)  delete [] m_piPartPerVertexMap;
            m_pVertices = NULL;
            m_pVertexNormals = NULL;
            m_plFacesPerVertexMap = NULL;
            m_pDefaultVertices = NULL;
            m_piPartPerVertexMap = NULL;
            m_iNumVertices = 0;
            bResult = false;
         }
      }
      else {
         // Allocate default vertex storage
         SPoint3D* pDefaultVertices = NULL;
         NEWBEGIN
         pDefaultVertices = new SPoint3D[m_iNumVertices];
         NEWEND("CAvatar::AddVertices() - default vertex reallocation")
         // Allocate part per vertex map storage
         int* piPartPerVertexMap = NULL;
         NEWBEGIN
         piPartPerVertexMap = new int[m_iNumVertices];
         NEWEND("CAvatar::AddVertices() - vertex->part map reallocation");
         // Check to see if allocation has failed
	      if (pDefaultVertices   == NULL ||
             piPartPerVertexMap == NULL ) {
            // Dispose of any data that may have been allocated
	         if (pDefaultVertices != NULL)   delete [] pDefaultVertices;
	         if (piPartPerVertexMap != NULL) delete [] piPartPerVertexMap;
            m_iNumVertices = iOldNumVertices;
            bResult = false;
         }
         else {
            // Allocation successful - copy data
            memcpy(pDefaultVertices,m_pDefaultVertices,iOldNumVertices*sizeof(SPoint3D));
            memcpy(piPartPerVertexMap,m_piPartPerVertexMap,iOldNumVertices*sizeof(int));
            // Delete old memory
            delete [] m_pDefaultVertices;
            delete [] m_piPartPerVertexMap;
            // Store new memory
            m_pDefaultVertices = pDefaultVertices;
            m_piPartPerVertexMap = piPartPerVertexMap;
         }
      }
   }
   return bResult;
} //AddVertices(int iNumVertices)

// Adds the specified number of faces
// Returns true if addition is successful
// Do not use this too much - it is very expensive.
bool CAvatar::AddFaces(int iNumFaces) {
   bool bResult = false;
   // Store number of faces
   int iOldNumFaces = m_iNumFaces;
   // and call base class allocator
   bResult = CTriMesh::AddFaces(iNumFaces);
   // If base class addition succeeds
   if (bResult = true) {
      if (iOldNumFaces == 0) {
         // Allocate part per face map storage
         NEWBEGIN
         m_piPartPerFaceMap = new int[m_iNumFaces];
         NEWEND("CAvatar::AddFaces() - face->part map allocation")
         // Check to see if allocation has failed
	      if (m_piPartPerFaceMap == NULL) {
            // Dispose of any data that may have been allocated
            if (m_pFaces != NULL)           delete [] m_pFaces;
	         if (m_piPartPerFaceMap != NULL) delete [] m_piPartPerFaceMap;
            m_pFaces           = NULL;
            m_piPartPerFaceMap = NULL;
            m_iNumFaces = 0;
            bResult = false;
         }
      }
      else {
         // Allocate part per face map storage
         int* piPartPerFaceMap = NULL;
         NEWBEGIN
         piPartPerFaceMap = new int[m_iNumFaces];
         NEWEND("CAvatar::AddFaces() - face->part map reallocation")
         // Check to see if allocation has failed
	      if (piPartPerFaceMap == NULL) {
            m_iNumFaces = iOldNumFaces;
            bResult = false;
         }
         else {
            // Allocation successful - copy data
            memcpy(piPartPerFaceMap,m_piPartPerFaceMap,iOldNumFaces*sizeof(int));
            // Delete old memory
            delete [] m_piPartPerFaceMap;
            // Store new memory
            m_piPartPerFaceMap = piPartPerFaceMap;
         }
      }
   }
   return bResult;
} //AddFaces(int iNumFaces)

void CAvatar::SetCentre(BodyPart bpPart, double dX, double dY, double dZ) {
   if (m_bLoading) {
      m_pBodyParts[bpPart].m_pntDefaultCentre.m_dComponents[0] = dX;
      m_pBodyParts[bpPart].m_pntDefaultCentre.m_dComponents[1] = dY;
      m_pBodyParts[bpPart].m_pntDefaultCentre.m_dComponents[2] = dZ;
      // -------------------------
      m_pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[0] = dX;
      m_pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[1] = dY;
      m_pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[2] = dZ;
   }
   return;
} //SetCentre(BodyPart bpPart, double dX, double dY, double dZ)

void CAvatar::AssociateMaterial(BodyPart bpPart, int iMaterialIndex) {
   m_pBodyParts[bpPart].m_iMaterialIndex = iMaterialIndex;
} //AssociateMaterial(BodyPart bpPart, int iMaterialIndex)

void CAvatar::AssociateVertex(BodyPart bpPart, int iVertexIndex) {
   SBodyPart* pPart = &(m_pBodyParts[bpPart]);
   if (pPart->m_pliVertices == NULL) {
      NEWBEGIN
      pPart->m_pliVertices = new CDList<int>;
      NEWEND("CAvatar::AssociateVertex() - m_pliVertices allocation");
      if (pPart->m_pliVertices == NULL) {
         return;
      }
   }
   pPart->m_pliVertices->AddBack(iVertexIndex);
   return;
} //AssociateVertex(BodyPart bpPart, int iVertexIndex)

void CAvatar::AssociateFace(BodyPart bpPart, int iFaceIndex) {
   SBodyPart* pPart = &(m_pBodyParts[bpPart]);
   if (pPart->m_pliFaces == NULL) {
      NEWBEGIN
      pPart->m_pliFaces = new CDList<int>;
      NEWEND("CAvatar::AssociateFace() - m_pliFaces allocation");
      if (pPart->m_pliFaces == NULL) {
         return;
      }
   }
   pPart->m_pliFaces->AddBack(iFaceIndex);
   return;
} //AssociateFace(BodyPart bpPart, int iFaceIndex)

void CAvatar::SetupSkeleton(BodyPart bpPart, BodyPart bpParent, BodyPart bpChild0, BodyPart bpChild1, BodyPart bpChild2) {
   if (m_bLoading) {
      m_pBodyParts[bpPart].m_bpParent = bpParent;
      m_pBodyParts[bpPart].m_bpChildren[0] = bpChild0;
      m_pBodyParts[bpPart].m_bpChildren[1] = bpChild1;
      m_pBodyParts[bpPart].m_bpChildren[2] = bpChild2;
   }
   return;
} //SetupSkeleton(BodyPart bpPart, BodyPart bpParent, BodyPart bpChild0, BodyPart bpChild1, BodyPart bpChild2)

void CAvatar::SetJointLimit(BodyPart bpPart, double dMaxX, double dMaxY, double dMaxZ, double dMinX, double dMinY, double dMinZ) {
   if (m_bLoading) {
      m_pBodyParts[bpPart].m_dMaxX = dMaxX;
      m_pBodyParts[bpPart].m_dMaxY = dMaxY;
      m_pBodyParts[bpPart].m_dMaxZ = dMaxZ;
      m_pBodyParts[bpPart].m_dMinX = dMinX;
      m_pBodyParts[bpPart].m_dMinY = dMinY;
      m_pBodyParts[bpPart].m_dMinZ = dMinZ;
   }
   return;
} //SetJointLimit(BodyPart bpPart, double dMaxX, double dMaxY, double dMaxZ, double dMinX, double dMinY, double dMinZ)

void CAvatar::SetJointDamping(BodyPart bpPart, double dDampX, double dDampY, double dDampZ) {
   if (m_bLoading) {
      m_pBodyParts[bpPart].m_dDampX = dDampX;
      m_pBodyParts[bpPart].m_dDampY = dDampY;
      m_pBodyParts[bpPart].m_dDampZ = dDampZ;
   }
   return;
} //SetJointDamping(BodyPart bpPart, double dDampX, double dDampY, double dDampZ)

void CAvatar::FixVertexAssociations(void) {
   // Fix vertex lists
	for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
      // Convert vertex list
      if (m_pBodyParts[i].m_pliVertices != NULL) {
         m_pBodyParts[i].m_piVertices = m_pBodyParts[i].m_pliVertices->Convert(m_pBodyParts[i].m_iNumVertices);
         delete m_pBodyParts[i].m_pliVertices;
         m_pBodyParts[i].m_pliVertices = NULL;
      }
   }
   // Build part per vertex map
   for (int p=0; p<TOTAL_NUMBER_BODYPARTS; p++) {
      int iNumVertices = m_pBodyParts[p].m_iNumVertices;
      for (int v=0; v<iNumVertices; v++) {
         m_piPartPerVertexMap[m_pBodyParts[p].m_piVertices[v]] = p;
      }
   }
   return;
} //FixVertexAssociations(void)

void CAvatar::FixFaceAssociations(void) {
   // Fix face lists
	for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
      // Convert face list
      if (m_pBodyParts[i].m_pliFaces != NULL) {
         m_pBodyParts[i].m_piFaces = m_pBodyParts[i].m_pliFaces->Convert(m_pBodyParts[i].m_iNumFaces);
         delete m_pBodyParts[i].m_pliFaces;
         m_pBodyParts[i].m_pliFaces = NULL;
      }
   }
   // Build part per face map
   for (int p=0; p<TOTAL_NUMBER_BODYPARTS; p++) {
      int iNumFaces = m_pBodyParts[p].m_iNumFaces;
      for (int f=0; f<iNumFaces; f++) {
         m_piPartPerFaceMap[m_pBodyParts[p].m_piFaces[f]] = p;
      }
   }
   return;
} //FixFaceAssociations(void)

void CAvatar::FinishLoad(int iMissingParts) {
   // Stop loading functions
   m_bLoading = false;
   // Calculate height
   SPoint3D pntMax, pntMin;
   BoundingBox(pntMax,pntMin);
   m_dHeight = pntMax.m_dComponents[1];
   // Store missing sections
   m_iMissingParts = iMissingParts;
   // Calculate normal information
   BuildMaps();
   CalculateNormals(true);
   // done!
   return;
} //FinishLoad(void)

///////////////////////////////////////////////////////////////////////
// Texture Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

int CAvatar::MaterialIndex(BodyPart bpPart) const {
   return m_pBodyParts[bpPart].m_iMaterialIndex;
} //MaterialIndex(BodyPart bpPart)

void CAvatar::BoundingBox(BodyPart bpBodyPart, SPoint3D& max, SPoint3D& min) const {
   // Check we are using a valid body part
   if (bpBodyPart<unknown || bpBodyPart>=TOTAL_NUMBER_BODYPARTS) return;
   // If so, calculate bounding box of all attached faces
   // Initialise results with the current centre position of the joint
   for (int c=0; c<3; c++) {
      max.m_dComponents[c] = m_pBodyParts[bpBodyPart].m_pntCurrentCentre.m_dComponents[c];
      min.m_dComponents[c] = m_pBodyParts[bpBodyPart].m_pntCurrentCentre.m_dComponents[c];
   }
   // Check all attached faces to find the bounding box
   for (int i=0; i<m_pBodyParts[bpBodyPart].m_iNumFaces; i++) {
      STriFace* pCurrentFace = m_pFaces + m_pBodyParts[bpBodyPart].m_piFaces[i];
      for (int v=0; v<3; v++) {
         int iCurrentVertex = pCurrentFace->m_sVertices[v].m_iVertex;
         for (int c=0; c<3; c++) {
            if (m_pVertices[iCurrentVertex].m_dComponents[c] > max.m_dComponents[c])
               max.m_dComponents[c] = m_pVertices[iCurrentVertex].m_dComponents[c];
            else if (m_pVertices[iCurrentVertex].m_dComponents[c] < min.m_dComponents[c])
               min.m_dComponents[c] = m_pVertices[iCurrentVertex].m_dComponents[c];
         }
      }
   }
   // Done
   return;
} //BoundingBox(BodyPart bpBodyPart, SPoint3D& max, SPoint3D& min) const

///////////////////////////////////////////////////////////////////////
// Posing Functions ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::ResetPose(void) {
   CAvatarPose poDefault(TOTAL_NUMBER_BODYPARTS);
   ImportPose(poDefault);
   return;
} //ResetPose(void)

void CAvatar::SetJointAngle(enum BodyPart bpJoint, CAxisRotation& rotNewRotation, bool bLimit, bool bDamp) {
   SBodyPart* pCurrentPart = &(m_pBodyParts[bpJoint]);
   if (bLimit) {
	   UEulerType uType;
	   uType.m_eIdentifier = XYZs;
	   CHomTransform htTemp(rotNewRotation);
	   CEulerRotation rotEulerRotation(htTemp, uType);
      // Damp rotation
      if (bDamp) rotEulerRotation.Damp(pCurrentPart->m_dDampX,pCurrentPart->m_dDampY,pCurrentPart->m_dDampZ);
      // Limit joint rotation
	   rotEulerRotation.Limit(pCurrentPart->m_dMaxX,pCurrentPart->m_dMaxY,pCurrentPart->m_dMaxZ,pCurrentPart->m_dMinX,pCurrentPart->m_dMinY,pCurrentPart->m_dMinZ);
      // Convert back to axis/angle
		CQuaternion quatTemp(rotEulerRotation);
		CAxisRotation rotLimitedRotation(quatTemp);
      // Store
		pCurrentPart->m_rotCurrentRotation = rotLimitedRotation;
   }
   else {
      pCurrentPart->m_rotCurrentRotation = rotNewRotation;
   }
	pCurrentPart->m_bDirtyAngle = true;
   pCurrentPart->m_bDirtyShape = true;
   BodyPart bpParent = pCurrentPart->m_bpParent;
   if (bpParent != unknown) m_pBodyParts[bpParent].m_bDirtyShape = true;
	return;
} //SetJointAngle(BodyPart bpJoint, CAxisRotation& rotNewRotation, bool bLimit, bool bDamp)

CAxisRotation CAvatar::GetJointAngle(BodyPart bpJoint) const {
   SBodyPart* pCurrentPart = &(m_pBodyParts[bpJoint]);
	return (pCurrentPart->m_rotCurrentRotation);
} //GetJointAngle(BodyPart bpJoint)

void CAvatar::SetRootTranslation(CVector3D& vecNewTranslation) {
	m_vecRootTranslation = vecNewTranslation;
   m_bDirtyTranslation = true;
	return;
} //SetRootTranslation(CVector3D& vecNewTranslation)

CVector3D CAvatar::GetRootTranslation() const {
	return m_vecRootTranslation;
} //GetRootTranslation()

void CAvatar::TargetBodyPart(enum BodyPart bpJoint, CVector3D& vecTarget, bool bLimit) {
   BodyPart bpParent = m_pBodyParts[bpJoint].m_bpParent;
   // Calculate vectors
   CVector3D vecParent(m_pBodyParts[bpParent].m_pntCurrentCentre);
   CVector3D vecDCurrent(CVector3D(m_pBodyParts[bpJoint].m_pntCurrentCentre) - vecParent);
   CVector3D vecDTarget(vecTarget - vecParent);
   // Calculate rotation
   CVector3D vecAxis = vecDTarget.Cross(vecDCurrent).Normalise();
   double dAngle = acos(vecDCurrent.Dot(vecDTarget)/(vecDCurrent.Length()*vecDTarget.Length()));
   CAxisRotation rotRotation(vecAxis,dAngle);
   // Merge rotation with current rotation
   rotRotation = rotRotation.MergeInside(m_pBodyParts[bpParent].m_rotCurrentRotation);
   SetJointAngle(bpParent, rotRotation, bLimit);
   return;
} //TargetBodyPart(enum BodyPart bpJoint, CVector3D& vecTarget, bool bLimit)

void CAvatar::AlignBodyPart(enum BodyPart bpJoint, CVector3D& vecTarget, bool bLimit) {
   BodyPart bpParent = m_pBodyParts[bpJoint].m_bpParent;
   CVector3D vecNewTarget = vecTarget + m_pBodyParts[bpParent].m_pntCurrentCentre;
   TargetBodyPart(bpJoint,vecNewTarget,bLimit);
   return;
} //AlignBodyPart(enum BodyPart bpJoint, CVector3D& vecTarget, bool bLimit)

void CAvatar::IKSetPose(BodyPart bpJoint, CVector3D& vecTarget, BodyPart bpFixed, bool bTranslation, bool bLimit, bool bDamp) {
   BodyPart bpCurrent = m_pBodyParts[bpJoint].m_bpParent;   
   CVector3D vecCurrentPos(m_pBodyParts[bpJoint].m_pntCurrentCentre);
   while ((vecCurrentPos != vecTarget) && (bpCurrent != bpFixed)) {
      CVector3D vecCurrentJoint(m_pBodyParts[bpCurrent].m_pntCurrentCentre);
      // Calculate vectors
      CVector3D vecToTarget(vecTarget - vecCurrentJoint);
      CVector3D vecToCurrent(vecCurrentPos - vecCurrentJoint);
      // Calculate rotation
      CVector3D vecAxis = vecToCurrent.Cross(vecToTarget).Normalise();
      double dAngle = acos(vecToCurrent.Dot(vecToTarget)/(vecToCurrent.Length()*vecToTarget.Length()));
      CAxisRotation rotRotation(vecAxis,dAngle);
      // Store joint angle
      rotRotation = rotRotation.MergeInside(m_pBodyParts[bpCurrent].m_rotCurrentRotation);
      SetJointAngle(bpCurrent, rotRotation, bLimit, bDamp);
      // Update Skeleton positions
      CHomTransform htTransform(m_vecRootTranslation);
      UpdateSkeleton(root,htTransform,false);
      // Update working fields
      vecCurrentPos = m_pBodyParts[bpJoint].m_pntCurrentCentre;
      bpCurrent = m_pBodyParts[bpCurrent].m_bpParent;
   }
   // Perform translation of the root if allowed and necessary
   if ((bTranslation) && (bpCurrent == root)) {
      CVector3D vecResidual = (vecTarget - vecCurrentPos);
      SetRootTranslation(vecResidual);
   }
   return;
} //IKSetPose(BodyPart bpJoint, CVector3D& vecTarget, BodyPart bpFixed, bool bTranslation, bool bLimit, bool bDamp)

///////////////////////////////////////////////////////////////////////
// Mutilation Functions ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::Chop(BodyPart bpJoint) {
   // Use specific position chop function to do work
   Chop(bpJoint,m_pBodyParts[bpJoint].m_pntCurrentCentre);
   // Done
   return;
} //Chop(BodyPart bpJoint)

void CAvatar::Chop(BodyPart bpJoint, const CVector3D& vecPosition) {
   SBodyPart* pPart = &(m_pBodyParts[bpJoint]);
   // Set vertices
   int iNumVertices = pPart->m_iNumVertices;
   for (int v=0; v<iNumVertices; v++) {
      vecPosition.ToPoint3D(m_pVertices[pPart->m_piVertices[v]]);
   }
   // Chop children
   for (int c=0; c<3; c++) {
      if (pPart->m_bpChildren[c] != unknown) Chop(pPart->m_bpChildren[c],vecPosition);
   }
   // Done
   return;
} //Chop(BodyPart bpJoint, const CVector3D& vecPosition)

///////////////////////////////////////////////////////////////////////
// Pose Storage Functions /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

CAvatarPose CAvatar::ExportPose(void) const {
   CAvatarPose pose(TOTAL_NUMBER_BODYPARTS);
   pose.SetRootTranslation(m_vecRootTranslation);
   for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
      // Copy old rotation into array
      pose.SetJointRotation(i,m_pBodyParts[i].m_rotCurrentRotation);
   }
   return pose;
} //ExportPose(void)

bool CAvatar::ImportPose(CAvatarPose& apNewPose) {
   if (apNewPose.m_iNumJoints != TOTAL_NUMBER_BODYPARTS) return false;
   m_vecRootTranslation = apNewPose.GetRootTranslation();
   for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
      // Set Joint angle
      SetJointAngle((BodyPart)i,apNewPose.GetJointRotation(i),false);
   }
   return true;
} //ImportPose(CAvatarPose& apNewPose)

bool CAvatar::ImportPosePart(BodyPart bpJoint, CAvatarPose& apNewPose) {
   if (apNewPose.m_iNumJoints != TOTAL_NUMBER_BODYPARTS)
		return false;
	// Params
	bool bReturn = true;
	SBodyPart sPart = m_pBodyParts[bpJoint];
	// Loop through children
	for (int i=0; i<3; i++) {
		if (sPart.m_bpChildren[i] > 0)
			bReturn = ImportPosePart(sPart.m_bpChildren[i], apNewPose);
	}
	// Copy the joint data
	SetJointAngle(bpJoint, apNewPose.GetJointRotation(bpJoint), false, false);
	return bReturn;
} //ImportPosePart(BodyPart bpJoint, CAvatarPose& apNewPose)

bool CAvatar::PushPose(void) {
   bool bResult = false;
   CAvatarPose* pPose = NULL;
   NEWBEGIN
   pPose = new CAvatarPose(TOTAL_NUMBER_BODYPARTS);
   NEWEND("CAvatar::PushPose - pose allocation")
   if (pPose != NULL) {
      // Copy information into pose object
      pPose->SetRootTranslation(m_vecRootTranslation);
      for (int i=0; i<TOTAL_NUMBER_BODYPARTS; i++) {
         // Set Joint angle
         pPose->SetJointRotation(i,m_pBodyParts[i].m_rotCurrentRotation);
      }
      // Store pointer at front of pose stack
      bResult = m_oPoseStack.AddFront(pPose);
      // If the pose wasn't stored, dump it and return false
      if (!bResult) {
         delete pPose;
      }
   }
   return bResult;
} //PushPose(void)

bool CAvatar::PopPose(void) {
   bool bResult = false;
   if (m_oPoseStack.Length() > 0) {
      CAvatarPose* pPose = m_oPoseStack.First();
      bResult = ImportPose(*pPose);      
      if (bResult) {
         m_oPoseStack.RemoveFront();
         delete pPose;
      }
   }
   return bResult;
} //PopPose(void)

///////////////////////////////////////////////////////////////////////
// Post-Load Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::UnTranslateDefaultModel(void) {
   if (m_bLoading) {
      double dX = -m_pBodyParts[root].m_pntDefaultCentre.m_dComponents[0];
      CVector3D vecTranslation(dX,0,0);
      TranslateDefault(root,vecTranslation);
   }
   m_bDirtyTranslation = true;
   return;
} //UnTranslateDefaultModel(void)

void CAvatar::AlignDefaultCentres(BodyPart bpStart, CVector3D vecTarget) {
   if (m_bLoading) {
      BodyPart bpParent = m_pBodyParts[bpStart].m_bpParent;
      CVector3D vecCentre(m_pBodyParts[bpParent].m_pntDefaultCentre);
      CVector3D vecSegment(CVector3D(m_pBodyParts[bpStart].m_pntDefaultCentre) - vecCentre);
      CVector3D vecAxis = vecSegment.Cross(vecTarget).Normalise();
      double dAngle = acos(vecSegment.Dot(vecTarget)/(vecSegment.Length()*vecTarget.Length()));
      CAxisRotation rotRotation(vecAxis,dAngle);
      CHomTransform htTransform(rotRotation,m_pBodyParts[bpParent].m_pntDefaultCentre);
      RotateDefaultCentres(bpStart,htTransform);
      SetJointAngle(bpParent,CAxisRotation(vecAxis,-dAngle),false);
   }
   return;
} //AlignDefaultCentres(BodyPart bpParent, BodyPart bpStart, CVector3D vecTarget)

void CAvatar::CalculateDefaultVertices(void) {
   if (m_bLoading) {
      CHomTransform htTransform;
      UnRigidTransform(root,htTransform);
   }
   return;
} //CalculateDefaultVertices(void)

///////////////////////////////////////////////////////////////////////
// Merging Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

int CAvatar::Merge(const CAvatar* pAvatar) {
   if (m_iNumVertices != pAvatar->m_iNumVertices) {
      // We can't currently cope with avatars that have a different number of vertices
      return 0;
   }
   if (m_iMissingParts == 0) {
      // Nothing to do!
      return 0;
   }
   int iRetVal = 0;

   // enable loading functions, as we're basically laoding info from another avatar
   StartLoad();

   // Merge information
   if (Sex() != pAvatar->Sex()) SetSex(SEX_UNKNOWN);
   if (Age() != pAvatar->Age()) SetAge(AGE_UNKNOWN);
   
   // Merge vertices
   if (m_iMissingParts & BS_HEAD) {
   }
   if (m_iMissingParts & BS_TORSO) {
   }
   if (m_iMissingParts & BS_LEGS) {
   }

   // Merge faces (for skirt)
   if (m_iMissingParts & BS_HEAD) {
   }
   if (m_iMissingParts & BS_TORSO) {
   }
   if (m_iMissingParts & BS_LEGS) {
   }

   // Merge materials
   if (m_iMissingParts & BS_HEAD) {
      int iThisMaterial = MaterialIndex(skullbase);
      int iThatMaterial = pAvatar->MaterialIndex(skullbase);
      CImage* pNewSkullImage = new CImage(*(pAvatar->Material(iThatMaterial)->Texture()));
      Material(iThisMaterial)->SetTexture(pNewSkullImage);
   }
   if (m_iMissingParts & BS_TORSO) {
   }
   if (m_iMissingParts & BS_LEGS) {
   }

   // Close off loading funcs and finish up
   FinishLoad();

   return iRetVal;
} //Merge(const CAvatar* pAvatar)

///////////////////////////////////////////////////////////////////////
// Animation Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::UpdateModel(bool bRecalculateNormals, bool bVertexNormals) {
   // Create starting transform
   CHomTransform transform(m_vecRootTranslation);
   // Update joint positions
   UpdateSkeleton(root,transform,m_bDirtyTranslation);
   // Update shape
   UpdateShape(root,transform,m_bDirtyTranslation);
   // Reset translation's dirty flag
   m_bDirtyTranslation = false;
   // Recalculate normals
   CalculateNormals(true);
	return;
} //UpdateModel(bool bRecalculateNormals)

void CAvatar::UpdateSkeleton(enum BodyPart bpBodyPart, CHomTransform& htTransform, bool bDirty) {
   SBodyPart* pJoint = m_pBodyParts + bpBodyPart;
   if (bDirty) {
      // Transform joint centre
      pJoint->m_pntCurrentCentre = htTransform * pJoint->m_pntDefaultCentre;
   }
   bDirty |= pJoint->m_bDirtyAngle;
   if (bDirty) {
		CHomTransform temp(pJoint->m_rotCurrentRotation, pJoint->m_pntDefaultCentre);
		pJoint->m_htLowerTransform = htTransform * temp;
   }
   BodyPart* bpChildren = pJoint->m_bpChildren;
   if (bpChildren[0] != unknown) UpdateSkeleton(bpChildren[0],pJoint->m_htLowerTransform,bDirty);
   if (bpChildren[1] != unknown) UpdateSkeleton(bpChildren[1],pJoint->m_htLowerTransform,bDirty);
   if (bpChildren[2] != unknown) UpdateSkeleton(bpChildren[2],pJoint->m_htLowerTransform,bDirty);
   pJoint->m_bDirtyAngle = false;
   return;
} //UpdateSkeleton(enum BodyPart bpBodyPart, CHomTransform& htTransform, bool bDirty)

void CAvatar::UpdateShape(enum BodyPart joint, CHomTransform& htTransform, bool bDirty) {
	SBodyPart* pCurrentPart = m_pBodyParts + joint;
	bDirty |= pCurrentPart->m_bDirtyShape;
	if (bDirty) {
		int iCurrentPoint = 0;
		int iNumPoints = pCurrentPart->m_iNumVertices;
		while (iCurrentPoint<iNumPoints) {
			int iCurrentVertex = pCurrentPart->m_piVertices[iCurrentPoint];
			m_pVertices[iCurrentVertex] = pCurrentPart->m_htLowerTransform * m_pDefaultVertices[iCurrentVertex];
			iCurrentPoint++;
		}
	}
	int iChild = 0;
	bool bNotDone = true;
	while (bNotDone && iChild<3) {
		if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			UpdateShape(pCurrentPart->m_bpChildren[iChild],pCurrentPart->m_htLowerTransform,bDirty);
			iChild++;
		}
		else bNotDone = false;
	}
	pCurrentPart->m_bDirtyShape = false;
	return;
} //UpdateShape(enum BodyPart joint, CHomTransform& htTransform, bool bDirty)

///////////////////////////////////////////////////////////////////////
// PRIVATE functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Post-Load Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::BuildMaps(void) {
   // Call base class function
   CTriMesh::BuildMaps();
   // Done
   return;
} //BuildMaps(void)

///////////////////////////////////////////////////////////////////////
// UnPose Functions ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CAvatar::RotateDefaultCentres(BodyPart bpBodyPart, CHomTransform& htTransform) {
	SBodyPart* pCurrentPart = m_pBodyParts + bpBodyPart;
   pCurrentPart->m_pntDefaultCentre = htTransform * pCurrentPart->m_pntDefaultCentre;
	int iChild = 0;
	bool bNotDone = true;
	while (bNotDone && iChild<3) {
		if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			RotateDefaultCentres(pCurrentPart->m_bpChildren[iChild],htTransform);
			iChild++;
		}
		else bNotDone = false;
	}
   return;
} //RotateDefaultCentre(BodyPart bpBodyPart, CHomTransform& htTransform)

void CAvatar::UnRigidTransform(BodyPart joint, CHomTransform& htTransform) {
	SBodyPart* pCurrentPart = m_pBodyParts + joint;
   CAxisRotation invRotation(pCurrentPart->m_rotCurrentRotation.GetAxis(),-pCurrentPart->m_rotCurrentRotation.GetAngle());
   CHomTransform temp(invRotation, pCurrentPart->m_pntDefaultCentre);
	pCurrentPart->m_htLowerTransform =  temp * htTransform;
	int iCurrentPoint = 0;
	int iNumPoints = pCurrentPart->m_iNumVertices;
	while (iCurrentPoint<iNumPoints) {
		int iCurrentVertex = pCurrentPart->m_piVertices[iCurrentPoint];
		m_pDefaultVertices[iCurrentVertex] = pCurrentPart->m_htLowerTransform * m_pVertices[iCurrentVertex];
		iCurrentPoint++;
	}
	int iChild = 0;
	bool bNotDone = true;
	while (bNotDone && iChild<3) {
		if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			UnRigidTransform(pCurrentPart->m_bpChildren[iChild],pCurrentPart->m_htLowerTransform);
			iChild++;
		}
		else bNotDone = false;
	}
	pCurrentPart->m_bDirtyAngle = true;
	return;
} //UnRigidTransform(BodyPart joint, CHomTransform& htTransform)

void CAvatar::TranslateDefault(BodyPart joint, CVector3D& vecTranslation) {
	SBodyPart* pCurrentPart = m_pBodyParts + joint;
   CVector3D vecNewPoint(vecTranslation + pCurrentPart->m_pntDefaultCentre);
   vecNewPoint.ToPoint3D(pCurrentPart->m_pntDefaultCentre);
	int iCurrentPoint = 0;
	int iNumPoints = pCurrentPart->m_iNumVertices;
	while (iCurrentPoint<iNumPoints) {
		int iCurrentVertex = pCurrentPart->m_piVertices[iCurrentPoint];
		vecNewPoint = vecTranslation + m_pDefaultVertices[iCurrentVertex];
      vecNewPoint.ToPoint3D(m_pDefaultVertices[iCurrentVertex]);
		iCurrentPoint++;
	}
	int iChild = 0;
	bool bNotDone = true;
	while (bNotDone && iChild<3) {
		if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			TranslateDefault(pCurrentPart->m_bpChildren[iChild],vecTranslation);
			iChild++;
		}
		else bNotDone = false;
	}
} //TranslateDefault(CVector3D& vecTranslation)

///////////////////////////////////////////////////////////////////////
// Animation Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
