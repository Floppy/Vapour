//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderAvatar.cpp - 29/02/2000 - Warren Moore
//	Avatar render object implementation
//
// $Id: RenderAvatar.cpp,v 1.4 2000/10/10 17:52:50 waz Exp $
//

#include "StdAfx.h"
#include "RenderAvatar.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CRenderAvatar

CRenderAvatar::CRenderAvatar(CRenderContext *poContext) : CRenderObject(poContext) {
	m_pAvatar = NULL;

	m_bNewAvatar = false;
	m_bGeneric = true;

	m_uMode = ROM_TEXTURE;

	m_iNumTextures = 0;
} // Constructor

CRenderAvatar::~CRenderAvatar() {
} // Destructor

void CRenderAvatar::SetAvatar(CAvatar *pAvatar, bool bGeneric) {
	m_pAvatar = pAvatar;
	m_bGeneric = bGeneric;

	m_bNewAvatar = true;
} // SetAvatar

void CRenderAvatar::ImportPose(CAvatarPose &oPose) {
	ASSERT(m_pAvatar);
	// Set the pose
	m_pAvatar->ImportPose(oPose);
} // ImportPose

CAvatarPose CRenderAvatar::ExportPose() {
	ASSERT(m_pAvatar);
	// Get the pose
	return m_pAvatar->ExportPose();
} // ExportPose

void CRenderAvatar::Init() {
	// Check the context is active
	if (!m_poContext->Active())
		return;
	// If an avatar is present and is not generic, load it's textures into memory
	if (m_pAvatar && (!m_bGeneric))
		CreateTextures();
	// Avatar sorted
	m_bNewAvatar = false;
} // Init

void CRenderAvatar::Exit() {
	// If the object has loaded textures, delete them
	if (m_iNumTextures)
		DeleteTextures();
} // GLExit

void CRenderAvatar::RenderMode(unsigned int uMode) {
	m_uMode = uMode;
	// If the avatar is generic and texture mode is set, set to flat mode
	if (m_bGeneric && (m_uMode == ROM_TEXTURE)) {
		m_uMode = ROM_FLAT;
	}
} // RenderMode (Set)

void CRenderAvatar::Execute() {
	// Check it's enabled
	if (!m_bEnabled)
		return;
	// Check it's an OpenGL render context
	if (m_uContextMode != RCV_OPENGL)
		return;
	// Move to the object position
	glTranslatef(m_fXPos, m_fYPos, m_fZPos);

	if (m_pAvatar) {
		// If new avatar set, update the textures
		if (m_bNewAvatar) {
			if (m_iNumTextures)
				DeleteTextures();

			if (!m_bGeneric)
				CreateTextures();

			m_bNewAvatar = false;
		}

		// Update the model vertices
		m_pAvatar->UpdateModel(true, true);

		// Render the model accordingly
		switch (m_uMode) {
			case ROM_DEFAULT:
			case ROM_FLAT: RenderFlat(); break;
			case ROM_TEXTURE: RenderTexture(); break;
			case ROM_SELECTION: RenderSelection(); break;
			default: break;
		}
	}
} // Execute

void CRenderAvatar::RenderFlat() {
	// Set the shade model
	glShadeModel(GL_SMOOTH);
	// Create the material params
	GLfloat fAmbient[] = { ROAV_AMBIENT, ROAV_AMBIENT, ROAV_AMBIENT, 1.0F };
	GLfloat fDiffuseWhite[] = { ROAV_DIFFUSE, ROAV_DIFFUSE, ROAV_DIFFUSE, 1.0F };
	GLfloat fDiffuseBlue[] = { ROAV_DIFFUSE_SEL, ROAV_DIFFUSE_SEL, ROAV_DIFFUSE, 1.0F };
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	// Enable lighting
	glEnable(GL_LIGHTING);
	// Avatar vars
	SPoint3D *pNormal = NULL;
	SPoint3D *pVertex = NULL;
	// Loop vars
	int iFaces = 0, iCurrentFace = 0;
	const SBodyPart *pPart = m_pAvatar->BodyParts();
	// Render the flat shaded avatar
	for (register int iPartNum = 0; iPartNum < TOTAL_NUMBER_BODYPARTS; iPartNum++) {
		// Set the material
		glMaterialfv(GL_FRONT, GL_AMBIENT, fAmbient);
		if ((m_uSelection > 0) && (iPartNum == m_uSelection - 1))
			glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiffuseBlue);
		else
			glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiffuseWhite);
		// Process the face list for the body part
		iFaces = pPart[iPartNum].m_iNumFaces;
		for (register int iCount = 0; iCount < iFaces; iCount++) {
			// Get the current face
			iCurrentFace = pPart[iPartNum].m_piFaces[iCount];
			// Calculate the point
			int iV0 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[0].m_iVertex;
			int iV1 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[1].m_iVertex;
			int iV2 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[2].m_iVertex;
			pNormal = m_pAvatar->m_pVertexNormals;
			pVertex = m_pAvatar->m_pCurrentVertices;
			// Draw the triangle
			glBegin(GL_TRIANGLES);
				glNormal3f(pNormal[iV0].m_dComponents[0],
									 pNormal[iV0].m_dComponents[1],
									 pNormal[iV0].m_dComponents[2]);
				glVertex3f(pVertex[iV0].m_dComponents[0],
									 pVertex[iV0].m_dComponents[1],
									 pVertex[iV0].m_dComponents[2]);

				glNormal3f(pNormal[iV1].m_dComponents[0],
									 pNormal[iV1].m_dComponents[1],
									 pNormal[iV1].m_dComponents[2]);
				glVertex3f(pVertex[iV1].m_dComponents[0],
									 pVertex[iV1].m_dComponents[1],
									 pVertex[iV1].m_dComponents[2]);

				glNormal3f(pNormal[iV2].m_dComponents[0],
									 pNormal[iV2].m_dComponents[1],
									 pNormal[iV2].m_dComponents[2]);
				glVertex3f(pVertex[iV2].m_dComponents[0],
									 pVertex[iV2].m_dComponents[1],
									 pVertex[iV2].m_dComponents[2]);
			glEnd();
		}
	}
	// Disable lighting
	glDisable(GL_LIGHTING);
	// Disable backface culling
	glDisable(GL_CULL_FACE);
} // RenderFlat

void CRenderAvatar::RenderTexture() {
	// Set the shade model
	glShadeModel(GL_SMOOTH);
	// Create the material params
	GLfloat fAmbient[] = { ROAV_AMBIENT, ROAV_AMBIENT, ROAV_AMBIENT, 1.0F };
	GLfloat fDiffuseWhite[] = { ROAV_DIFFUSE, ROAV_DIFFUSE, ROAV_DIFFUSE, 1.0F };
	GLfloat fDiffuseBlue[] = { ROAV_DIFFUSE_SEL, ROAV_DIFFUSE_SEL, ROAV_DIFFUSE, 1.0F };
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	// Enable lighting
	glEnable(GL_LIGHTING);
	// Set the texture modulation
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Avatar vars
	SFaceVertex *sTexCoords = NULL;
	SPoint3D *pNormal = NULL;
	SPoint3D *pVertex = NULL;
	// Loop vars
	int iFaces = 0, iCurrentFace = 0;
	int iTextureNum = -1;
	const SBodyPart *pPart = m_pAvatar->BodyParts();
	// Render the textured avatar
	for (register int iPartNum = 0; iPartNum < TOTAL_NUMBER_BODYPARTS; iPartNum++) {
		// Set the material
		glMaterialfv(GL_FRONT, GL_AMBIENT, fAmbient);
		if ((m_uSelection > 0) && (iPartNum == m_uSelection - 1))
			glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiffuseBlue);
		else
			glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiffuseWhite);
		// Process the face list for the body part
		iFaces = pPart[iPartNum].m_iNumFaces;
		for (register int iCount = 0; iCount < iFaces; iCount++) {
			// Get the current face
			iCurrentFace = pPart[iPartNum].m_piFaces[iCount];
			// Calculate the point
			int iV0 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[0].m_iVertex;
			int iV1 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[1].m_iVertex;
			int iV2 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[2].m_iVertex;
			pNormal = m_pAvatar->m_pVertexNormals;
			pVertex = m_pAvatar->m_pCurrentVertices;
			// Find the texture
			iTextureNum = m_pAvatar->m_pFaces[iCurrentFace].m_iTextureNumber;
			ASSERT(m_iTexture[iTextureNum] != -1);
			// Set the texture
			m_poContext->UseTexture(m_iTexture[iTextureNum]);
			sTexCoords = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices;
			// Draw the triangle
			glBegin(GL_TRIANGLES);
				glNormal3f(pNormal[iV0].m_dComponents[0],
									 pNormal[iV0].m_dComponents[1],
									 pNormal[iV0].m_dComponents[2]);
				glTexCoord2f(sTexCoords[0].m_sTexCoord.dU,
										 sTexCoords[0].m_sTexCoord.dV);
				glVertex3f(pVertex[iV0].m_dComponents[0],
									 pVertex[iV0].m_dComponents[1],
									 pVertex[iV0].m_dComponents[2]);

				glNormal3f(pNormal[iV1].m_dComponents[0],
									 pNormal[iV1].m_dComponents[1],
									 pNormal[iV1].m_dComponents[2]);
				glTexCoord2f(sTexCoords[1].m_sTexCoord.dU,
										 sTexCoords[1].m_sTexCoord.dV);
				glVertex3f(pVertex[iV1].m_dComponents[0],
									 pVertex[iV1].m_dComponents[1],
									 pVertex[iV1].m_dComponents[2]);

				glNormal3f(pNormal[iV2].m_dComponents[0],
									 pNormal[iV2].m_dComponents[1],
									 pNormal[iV2].m_dComponents[2]);
				glTexCoord2f(sTexCoords[2].m_sTexCoord.dU,
										 sTexCoords[2].m_sTexCoord.dV);
				glVertex3f(pVertex[iV2].m_dComponents[0],
									 pVertex[iV2].m_dComponents[1],
									 pVertex[iV2].m_dComponents[2]);
			glEnd();
		}
	}
	// Disable texture mapping
	glDisable(GL_TEXTURE_2D);
	// Disable lighting
	glDisable(GL_LIGHTING);
	// Disable backface culling
	glDisable(GL_CULL_FACE);
} // RenderTexture

void CRenderAvatar::RenderSelection() {
	// Set the shade model
	glShadeModel(GL_FLAT);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	// Render the segmented avatar
	int iFaces = 0, iCurrentFace = 0;
	const SBodyPart *pPart = m_pAvatar->BodyParts();
	for (register int iPartNum = 0; iPartNum < TOTAL_NUMBER_BODYPARTS; iPartNum++) {
		// Set the material
		glColor3ub(iPartNum + 1, 0, 0);
		// Process the face list for the body part
		iFaces = pPart[iPartNum].m_iNumFaces;
		for (register int iCount = 0; iCount < iFaces; iCount++) {
			// Get the current face
			iCurrentFace = pPart[iPartNum].m_piFaces[iCount];
			// Calculate the point
			int iV0 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[0].m_iVertex;
			int iV1 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[1].m_iVertex;
			int iV2 = m_pAvatar->m_pFaces[iCurrentFace].m_sVertices[2].m_iVertex;
			// Draw the triangle
			glBegin(GL_TRIANGLES);
				glVertex3f(m_pAvatar->m_pCurrentVertices[iV0].m_dComponents[0],
									 m_pAvatar->m_pCurrentVertices[iV0].m_dComponents[1],
									 m_pAvatar->m_pCurrentVertices[iV0].m_dComponents[2]);

				glVertex3f(m_pAvatar->m_pCurrentVertices[iV1].m_dComponents[0],
									 m_pAvatar->m_pCurrentVertices[iV1].m_dComponents[1],
									 m_pAvatar->m_pCurrentVertices[iV1].m_dComponents[2]);

				glVertex3f(m_pAvatar->m_pCurrentVertices[iV2].m_dComponents[0],
									 m_pAvatar->m_pCurrentVertices[iV2].m_dComponents[1],
									 m_pAvatar->m_pCurrentVertices[iV2].m_dComponents[2]);
			glEnd();
		}
	}
	// Disable backface culling
	glDisable(GL_CULL_FACE);
} // RenderSelection

void CRenderAvatar::CreateTextures() {
	if (m_pAvatar) {
		const int iNumTextures = m_pAvatar->NumTextures();
		// For each texture in the model
		for (int i = 0; i < iNumTextures; i++) {
			CImage *pImg = m_pAvatar->Texture(i);
			m_iTexture[i] = m_poContext->ImportTexture(*pImg);
			if (m_iTexture[i] >= 0)
				m_iNumTextures++;
		}
		ASSERT(m_iNumTextures == iNumTextures);
	}
} // CreateTextures

void CRenderAvatar::DeleteTextures() {
	for (int i = 0; i < m_iNumTextures; i++) {
		m_poContext->DeleteTexture(m_iTexture[i]);
		m_iTexture[i] = -1;
	}
	m_iNumTextures = 0;
} // DeleteTextures
