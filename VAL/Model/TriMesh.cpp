//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TriMesh.cpp - 16/11/2000 - James Smith
//	Triangular mesh class implementation
//
// $Id: TriMesh.cpp,v 1.0 2000/11/21 16:47:15 waz Exp $
//

#include "stdafx.h"

#include "TriMesh.h"
#include "Vector3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
// Construction/Destruction ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

   // Creates an empty TriMesh
CTriMesh::CTriMesh() : 
   CModel<SPoint3D>(),   
   m_iNumFaces(0),
   m_pFaces(NULL),
   m_pVertexNormals(NULL),
   m_plFacesPerVertexMap(NULL)
{
   return;
}

CTriMesh::CTriMesh(int iNumVertices, int iNumFaces) : 
   CModel<SPoint3D>(iNumVertices),
   m_iNumFaces(0),
   m_pFaces(NULL),
   m_pVertexNormals(NULL),
   m_plFacesPerVertexMap(NULL)
{
   // Allocate face storage
   NEWBEGIN
	m_pFaces = new STriFace[iNumFaces];
   NEWEND("CTriMesh::CTriMesh() - face storage allocation")
   // Allocate vertex normal storage
   NEWBEGIN
	m_pVertexNormals = new SPoint3D[m_iNumVertices];
   NEWEND("CTriMesh::CTriMesh() - vertex normal allocation")
   // Allocate face->vertex map storage
   NEWBEGIN
   m_plFacesPerVertexMap = new CDList<int>[m_iNumVertices];
   NEWEND("CTriMesh::CTriMesh() - vertex->face map allocation")
   // Check allocation and store number of faces if appropriate
	if (m_pVertexNormals      != NULL &&
       m_pFaces              != NULL &&
       m_plFacesPerVertexMap != NULL ) {
   	m_iNumFaces = iNumFaces;
   }
   // Done
   return;
} //CTriMesh(int iNumVertices, int iNumFaces)

CTriMesh::~CTriMesh() {
   // Delete materials
   int iNumMaterials = m_lMaterials.Length();
   for (int t=0; t<iNumMaterials; t++) {
      delete m_lMaterials[t];
   }
   // Delete model data
	if (m_pVertexNormals != NULL)      delete [] m_pVertexNormals;
	if (m_plFacesPerVertexMap != NULL) delete [] m_plFacesPerVertexMap;
   if (m_pFaces != NULL)              delete [] m_pFaces;
   // Done
   return;
} //~CTriMesh()

///////////////////////////////////////////////////////////////////////
// Loading Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Adds the specified number of vertices
// Returns true if addition is successful
bool CTriMesh::AddVertices(int iNumVertices) {
   bool bResult = false;
   // Store number of vertices
   int iOldNumVertices = m_iNumVertices;
   // and call base class allocator
   bResult = CModel<SPoint3D>::AddVertices(iNumVertices);
   // If base class addition succeeds
   if (bResult = true) {
      if (iOldNumVertices == 0) {
         // Allocate vertex normal storage
         NEWBEGIN
	      m_pVertexNormals = new SPoint3D[m_iNumVertices];
         NEWEND("CTriMesh::AddVertices() - vertex normal allocation")
         // Allocate face->vertex map storage
         NEWBEGIN
         m_plFacesPerVertexMap = new CDList<int>[m_iNumVertices];
         NEWEND("CTriMesh::AddVertices() - vertex->face map allocation")
         // Check to see if allocation has failed
	      if (m_pVertexNormals      == NULL ||
             m_plFacesPerVertexMap == NULL ) {
            // Dispose of any data that may have been allocated
            if (m_pVertices != NULL)           delete [] m_pVertices;
	         if (m_pVertexNormals != NULL)      delete [] m_pVertexNormals;
	         if (m_plFacesPerVertexMap != NULL) delete [] m_plFacesPerVertexMap;
            m_pVertices = NULL;
            m_pVertexNormals = NULL;
            m_plFacesPerVertexMap = NULL;
            m_iNumVertices = 0;
            bResult = false;
         }
      }
      else {
         // Allocate new vertex normal storage
         SPoint3D* pVertexNormals = NULL;
         NEWBEGIN
	      pVertexNormals = new SPoint3D[m_iNumVertices];
         NEWEND("CTriMesh::AddVertices() - vertex normal reallocation")
         // Allocate face->vertex map storage
         CDList<int>* plFacesPerVertexMap = NULL;
         NEWBEGIN
         m_plFacesPerVertexMap = new CDList<int>[m_iNumVertices];
         NEWEND("CTriMesh::AddVertices() - vertex->face map reallocation")
         // Check to see if allocation has failed
	      if (pVertexNormals      == NULL ||
             plFacesPerVertexMap == NULL ) {
            // Dispose of any data that may have been allocated
	         if (pVertexNormals != NULL)      delete [] pVertexNormals;
	         if (plFacesPerVertexMap != NULL) delete [] plFacesPerVertexMap;
            m_iNumVertices = iOldNumVertices;
            bResult = false;
         }
         else {
            // Allocation successful - copy data
            memcpy(pVertexNormals,m_pVertexNormals,iOldNumVertices*sizeof(SPoint3D));
            memcpy(plFacesPerVertexMap,m_plFacesPerVertexMap,iOldNumVertices*sizeof(CDList<int>));
            // Delete old memory
            delete [] m_pVertexNormals;
            delete [] m_plFacesPerVertexMap;
            // Store new memory
            m_pVertexNormals = pVertexNormals;
            m_plFacesPerVertexMap = plFacesPerVertexMap;
         }
      }
   }
   return bResult;
} //AddVertices(int iNumVertices)

// Adds the specified number of faces
// Returns true if addition is successful
bool CTriMesh::AddFaces(int iNumFaces) {
   bool bResult = false;
   if (m_iNumFaces = 0) {
      // Allocate face storage
      NEWBEGIN
	   m_pFaces = new STriFace[iNumFaces];
      NEWEND("CTriMesh::AddFaces() - face storage allocation")
      // Check allocation and store number of faces if appropriate
	   if (m_pFaces != NULL) {
   	   m_iNumFaces = iNumFaces;
         bResult = true;
      }
   }
   else {
      iNumFaces += m_iNumFaces;
      // Allocate new face storage
      STriFace* pFaces = NULL;
      NEWBEGIN
	   pFaces = new STriFace[iNumFaces];
      NEWEND("CTriMesh::AddFaces() - face storage allocation")
      // Check face allocation and store info if appropriate
	   if (pFaces != NULL) {
         // Copy vertex data
         memcpy(pFaces,m_pFaces,m_iNumFaces*sizeof(STriFace));
         // Delete old array
         delete [] m_pFaces;
         // Store new array
         m_pFaces = pFaces;
         // Store new face count
         m_iNumFaces = iNumFaces;
         // Success!
         bResult = true;
      }
   }
   return bResult;
} //AddFaces(int iNumFaces)

void CTriMesh::SetFace(int iFace, int iVertex0, int iVertex1, int iVertex2, int iTextureIndex, double iVertex0U, double iVertex0V, double iVertex1U, double iVertex1V, double iVertex2U, double iVertex2V) {
   // Store vertex indices
	m_pFaces[iFace].m_sVertices[0].m_iVertex = iVertex0;
	m_pFaces[iFace].m_sVertices[1].m_iVertex = iVertex1;
	m_pFaces[iFace].m_sVertices[2].m_iVertex = iVertex2;
   // Store texture index
   m_pFaces[iFace].m_iMaterialNumber = iTextureIndex;
   // Store texture coordinates
   m_pFaces[iFace].m_sVertices[0].m_sTexCoord.dU = iVertex0U;
   m_pFaces[iFace].m_sVertices[0].m_sTexCoord.dV = iVertex0V;
   m_pFaces[iFace].m_sVertices[1].m_sTexCoord.dU = iVertex1U;
   m_pFaces[iFace].m_sVertices[1].m_sTexCoord.dV = iVertex1V;
   m_pFaces[iFace].m_sVertices[2].m_sTexCoord.dU = iVertex2U;
   m_pFaces[iFace].m_sVertices[2].m_sTexCoord.dV = iVertex2V;
   // Done
   return;
} //SetFace(int iFace, int iVertex0, int iVertex1, int iVertex2, int iTextureIndex, double iVertex0U, double iVertex0V, double iVertex1U, double iVertex1V, double iVertex2U, double iVertex2V)

///////////////////////////////////////////////////////////////////////
// Material Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Add a material to the model - returns the index of the new material if successful, -1 otherwise.
int CTriMesh::AddMaterial(void) {
   int iResult = -1;
   CSurfaceMaterial* pNewMaterial = NULL;
   NEWBEGIN
   pNewMaterial = new CSurfaceMaterial;
   NEWEND("CTriMesh::AddMaterial - material allocation failed")
   if (pNewMaterial != NULL) {
      if (m_lMaterials.AddBack(pNewMaterial)) iResult = m_lMaterials.Length()-1;
      else delete pNewMaterial;
   }
   return iResult;
}

///////////////////////////////////////////////////////////////////////
// Info Functions /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CTriMesh::BoundingBox(SPoint3D& max, SPoint3D& min) const {
   if (m_iNumVertices > 0) {
      for (int c=0; c<3; c++) {
         max.m_dComponents[c] = m_pVertices[0].m_dComponents[c];
         min.m_dComponents[c] = m_pVertices[0].m_dComponents[c];
      }
      for (int i=0; i<m_iNumVertices; i++) {
         for (c=0; c<3; c++) {
            if (m_pVertices[i].m_dComponents[c] > max.m_dComponents[c])
               max.m_dComponents[c] = m_pVertices[i].m_dComponents[c];
            else if (m_pVertices[i].m_dComponents[c] < min.m_dComponents[c])
               min.m_dComponents[c] = m_pVertices[i].m_dComponents[c];
         }
      }
   }
   else {
      for (int c=0; c<3; c++) {
         max.m_dComponents[c] = 0;
         min.m_dComponents[c] = 0;
      }
   }
   return;
} //BoundingBox(SPoint3D& max, SPoint3D& min) const

void CTriMesh::CalculateNormals(bool bVertex) {
   // Calculate face normals
   for (int f=0; f<m_iNumFaces; f++) {
      CVector3D vectorOne(CVector3D(m_pVertices[m_pFaces[f].m_sVertices[1].m_iVertex]) - CVector3D(m_pVertices[m_pFaces[f].m_sVertices[0].m_iVertex]));
      CVector3D vectorTwo(CVector3D(m_pVertices[m_pFaces[f].m_sVertices[2].m_iVertex]) - CVector3D(m_pVertices[m_pFaces[f].m_sVertices[0].m_iVertex]));
      CVector3D vecNormal((vectorOne.Cross(vectorTwo)).Normalise());
      vecNormal.ToDouble(m_pFaces[f].m_dNormal[0],m_pFaces[f].m_dNormal[1],m_pFaces[f].m_dNormal[2]);
   }
   // Calculate vertex normals if necessary
   if (bVertex) {
      for (int v=0; v<m_iNumVertices; v++) {
         int iNumAdjoiningFaces = m_plFacesPerVertexMap[v].Length();
         CVector3D vecNormal;
         for (f=0; f<iNumAdjoiningFaces; f++) {
            vecNormal += CVector3D(m_pFaces[m_plFacesPerVertexMap[v][f]].m_dNormal[0], m_pFaces[m_plFacesPerVertexMap[v][f]].m_dNormal[1], m_pFaces[m_plFacesPerVertexMap[v][f]].m_dNormal[2]);
         }
         vecNormal = vecNormal.Normalise();
         vecNormal.ToDouble(m_pVertexNormals[v].m_dComponents[0], m_pVertexNormals[v].m_dComponents[1], m_pVertexNormals[v].m_dComponents[2]);
      }
   }
	return;
} //CalculateNormals(bool bVertex)

///////////////////////////////////////////////////////////////////////
// Post-Load Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CTriMesh::BuildMaps(void) {
   // Build faces per vertex map
   for (int f=0; f<m_iNumFaces; f++) {
      m_plFacesPerVertexMap[m_pFaces[f].m_sVertices[0].m_iVertex].AddBack(f);
      m_plFacesPerVertexMap[m_pFaces[f].m_sVertices[1].m_iVertex].AddBack(f);
      m_plFacesPerVertexMap[m_pFaces[f].m_sVertices[2].m_iVertex].AddBack(f);
   }
   return;
} //BuildMaps(void)

