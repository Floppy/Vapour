//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TriMesh.h - 16/11/2000 - James Smith
//	Triangular mesh class declaration
//
// $Id: TriMesh.h,v 1.0 2000/11/21 16:47:17 waz Exp $
//

#ifndef _VAL_TRIMESH_
#define _VAL_TRIMESH_

#pragma once

// VAL includes
#include "Val.h"
#include "Model.h"
#include "Point3D.h"
#include "TriFace.h"
#include "SurfaceMaterial.h"

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

class DLL CTriMesh : public CModel<SPoint3D> {

public:

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates an empty TriMesh
	CTriMesh();

   // Creates a TriMesh with the specified number of vertices and faces.
	CTriMesh(int iNumVertices, int iNumFaces);

   virtual ~CTriMesh();

   ///////////////////////////////////////////////////////////////////////
   // Loading Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // Adds the specified number of vertices
   // Returns true if addition is successful
   // Do not use this too much - it is very expensive.
   virtual bool AddVertices(int iNumVertices);

   // Adds the specified number of faces
   // Returns true if addition is successful
   // Do not use this too much - it is very expensive.
   virtual bool AddFaces(int iNumFaces);

   // Enters data into a particular face
   void SetFace(int iFace, int iVertex0, int iVertex1, int iVertex2, int iTextureIndex, double iVertex0U, double iVertex0V, double iVertex1U, double iVertex1V, double iVertex2U, double iVertex2V);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
	// The number of faces in the model.
	int NumFaces(void) const {return m_iNumFaces;}

   // Access to the face array.
   const STriFace* Faces(void) const {return m_pFaces;}

   // Access to a particular face
   const STriFace* Face(int iFace) const {return (iFace<m_iNumFaces) ? m_pFaces + iFace : NULL;}

	// Access to the vertex normal array
   const SPoint3D* VertexNormals(void) const {return m_pVertexNormals;}

   // Access to a particular vertex normal
   const SPoint3D* VertexNormal(int iVertex) const {return (iVertex<m_iNumVertices) ? m_pVertexNormals + iVertex : NULL;}

   ///////////////////////////////////////////////////////////////////////
   // Material Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // Add a material to the model - returns the index of the new material if successful, -1 otherwise.
   int AddMaterial(void);

   // The number of materials in the model.
	int NumMaterials(void) const {return m_lMaterials.Length();}

   // Extracts a pointer to the chose surface material.
   CSurfaceMaterial* Material(int iMaterialIndex) const {return m_lMaterials[iMaterialIndex];}

   ///////////////////////////////////////////////////////////////////////
   // Info Functions /////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Calculate bounding box of model
   void BoundingBox(SPoint3D& max, SPoint3D& min) const;

   // Get list of faces attached to a certain vertex.
   const CDList<int>* FacesPerVertex(int iVertexIndex) const {return m_plFacesPerVertexMap + iVertexIndex;}

	// Explicitly calculate surface normals.
	// if bVertex is false, normals are calculated for faces only
   // if bVertex is true, vertex normals are also calculated
	void CalculateNormals(bool bVertex = false);

protected:
	
   // Geometry
	SPoint3D* m_pVertexNormals;

   // Topology
   int m_iNumFaces;
	STriFace* m_pFaces;
   CDList<int>* m_plFacesPerVertexMap;

   // Materials
   CDList<CSurfaceMaterial*> m_lMaterials;

   ///////////////////////////////////////////////////////////////////////
   // Post-Load Functions ////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Builds the reverse maps:
   //   m_plFacesPerVertexMap: relates a vertex to it's associated faces
   void BuildMaps(void);

};

#endif //_VAL_TRIMESH_
