//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Model.h - 15/11/2000 - James Smith
//	Object model base class declaration
//
// $Id: Model.h,v 1.0 2000/11/21 16:47:16 waz Exp $
//

#ifndef _VAL_MODEL_
#define _VAL_MODEL_

#pragma once

// VAL includes
#include "Val.h"

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

// CModel: a templated base class for a model. 
// The class T is the type of the vertices/control points for the model

template<class T>
class DLL CModel {

public:

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates an empty Model
	CModel();

   // Creates a Model with the specified number of vertices
	CModel(int iNumVertices);

   virtual ~CModel();

   ///////////////////////////////////////////////////////////////////////
   // Loading Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Adds the specified number of vertices
   // Returns true if addition is successful
   // Do not use this too much - it is very expensive.
   virtual bool AddVertices(int iNumVertices);

   // Sets information for a single vertex
   void SetVertex(int iVertex, const T& oVertex) {if (iVertex<m_iNumVertices) m_pVertices[iVertex]=oVertex;}

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
	// Provides direct access to the vertex array
   const T* Vertices(void) const {return m_pVertices;}
	
	// The number of vertices in the model.
	int NumVertices(void) const {return m_iNumVertices;}

   // Access to a particular vertex
   const T* Vertex(int iVertex) const {return (iVertex<m_iNumVertices) ? m_pVertices + iVertex : NULL;}

protected:
	
   ///////////////////////////////////////////////////////////////////////
   // Data ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Geometry
   int m_iNumVertices;
	T* m_pVertices;

};

// Creates a Model with 0 vertices
template<class T>
CModel<T>::CModel() :
   m_iNumVertices(0),
   m_pVertices(NULL)
{
   return;
} //CModel()

// Creates a Model with the specified number of vertices
template<class T>
CModel<T>::CModel(int iNumVertices) {
   // Initialise number of vertices
   m_iNumVertices = 0;
   // Allocate vertex storage
   m_pVertices = NULL;
   NEWBEGIN
   m_pVertices = new T[iNumVertices];
   NEWEND("CModel::CModel - vertex allocation")
   // Check vertex allocation and store number of vertices if appropriate
   if (m_pVertices != NULL) m_iNumVertices = iNumVertices;
   // Done
   return;
} //CModel(int iNumVertices)

template<class T>
CModel<T>::~CModel() {
   if (m_pVertices != NULL) delete [] m_pVertices;
   return;
} //~CModel()

// Adds the specified number of vertices
// Returns true if addition is successful
template<class T>
bool CModel<T>::AddVertices(int iNumVertices) {
   bool bResult = false;
   if (m_iNumVertices == 0) {
      // Allocate vertex storage
      NEWBEGIN
      m_pVertices = new T[iNumVertices];
      NEWEND("CModel::AddVertices - vertex allocation")
      // Check vertex allocation and store number of vertices if appropriate
      if (m_pVertices != NULL) {
         // Store vertex count
         m_iNumVertices = iNumVertices;
         // Success!
         bResult = true;
      }
   }
   else {
      iNumVertices += m_iNumVertices;
      // Allocate new vertex storage
      T* pVertices = NULL;
      NEWBEGIN
      pVertices = new T[iNumVertices];
      NEWEND("CModel::AddVertices - vertex reallocation")
      // Check vertex allocation and store info if appropriate
      if (pVertices  != NULL) {
         // Copy vertex data
         memcpy(pVertices,m_pVertices,m_iNumVertices*sizeof(T));
         // Delete old array
         delete [] m_pVertices;
         // Store new array
         m_pVertices = pVertices;
         // Store new vertex count
         m_iNumVertices = iNumVertices;
         // Success!
         bResult = true;
      }
   }
   return bResult;
} //AddVertices(int iNumVertices)

#endif //_VAL_MODEL_
