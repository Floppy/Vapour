//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileSims.cpp - 16/2/2000 - James Smith
//	Sims export filter implementation
//
// $Id: AvatarFileSims.cpp,v 1.5 2000/10/06 13:16:57 waz Exp $
//


#include "stdafx.h"
#include <direct.h>
#include <errno.h>

#include "AvatarFileSims.h"
#include "AvatarFileProxy.h"

#include "MathConstants.h"

#include "ProgressControl.h"
extern CProgressControl g_oProgressControl;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////
// CAvatarFileSims

CAvatarFileProxy<CAvatarFileSims> g_oAvatarProxySims;

/////////////////////
// Member Functions

////////////////////
// Store Functions

CAvatarFileSims::CAvatarFileSims() {
   m_iAge = SIMS_ADULT;
   m_iSex = SIMS_MALE;
   m_iSkin = SIMS_MEDIUM;
   m_iBuild = SIMS_FIT;
   return;
} // CAvatarFileSims()

float CAvatarFileSims::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileSims::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileSims::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileSims::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileSims::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileSims::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileSims::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileSims::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileSims::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFileSims::Save(const char* pszFilename, CAvatar* pAvatar) const {
	if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   
   FRESULT eResult = F_OK;

   // Setup the export progress dialog
	g_poVAL->SetProgressMax("SimsSave", 10);

   // Work out model name and target directory from passed filename
   char* pszInputName = strdup(pszFilename);
   char* pszTemp = pszInputName;
   // Find target directory
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int iDirLength = (pszTemp - pszInputName);
   char* pszTgtDir = new char[iDirLength+1];
   for (int i=0; i<iDirLength; i++) {
      pszTgtDir[i] = pszInputName[i];
   }
   pszTgtDir[iDirLength] = 0;
   // Find model name
   char* pszEnd = strchr(pszTemp,'.');
   int iNameLength = (pszEnd != NULL) ? (pszEnd - pszTemp) : strlen(pszTemp);
   char* pszName = new char[iNameLength+1];
   for (i=0; i<iNameLength; i++) {
      pszName[i] = pszTemp[i];
   }
   pszName[iNameLength] = 0;
   iNameLength = strlen(pszName);
   // Tidy up
   free(pszInputName);

   // Translate information fields into strings
   // Sex
   char cSex;
   switch (m_iSex) {
   case SIMS_FEMALE:
      cSex = 'F';
      break;
   case SIMS_MALE:
   default:
      cSex = 'M';
      break;
   }
   // Skin tone
   char* pszSkin;
   switch (m_iSkin) {
   case SIMS_LIGHT:
      pszSkin = "lgt";
      break;
   case SIMS_DARK:
      pszSkin = "drk";
      break;
   case SIMS_MEDIUM:
   default:
      pszSkin = "med";
      break;
   }
   // Body shape
   char* pszBuild;
   switch (m_iBuild) {
   case SIMS_FAT:
      pszBuild = "Fat";
      break;
   case SIMS_SKINNY:
      pszBuild = "Skn";
      break;
   case SIMS_FIT:
   default:
      pszBuild = "Fit";
      break;
   }
   // Age group
   char cAge;
   switch (m_iAge) {
   case SIMS_CHILD:
      cAge = 'C';
      pszBuild = "Chd";
      break;
   case SIMS_ADULT:
   default:
      cAge = 'A';
      break;
   }   

   // Reset pose
   CAvatarPose poOld = pAvatar->ExportPose();
   pAvatar->ResetPose();
   pAvatar->UpdateModel();
   
   // Calculate scale factor
   const double dScale = ((m_iAge == SIMS_CHILD) ? 4 : 5.25) / pAvatar->Height();

   // Store a few texture numbers for later use
   int iFaceTexNum = pAvatar->TextureIndex(skullbase);
   int iRHandTexNum = pAvatar->TextureIndex(r_wrist);
   int iLHandTexNum = pAvatar->TextureIndex(l_wrist);
   int iTorsoTexNum = pAvatar->TextureIndex(vl5);

   const STriFace* pFaces = pAvatar->Faces();
   SPoint3D pntMax, pntMin;

   ////////////////////////////////////////////////////////////////////
   // CRANIUM /////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////

   // Build Cranium Names
   char* pszMeshName = new char[8+iNameLength]; //C999MA_name
   pszMeshName[0] = 'C';
   strcpy(pszMeshName+1,"999");
   pszMeshName[4] = cSex;
   pszMeshName[5] = cAge;
   pszMeshName[6] = '_';
   strcpy(pszMeshName+7,pszName);
   char* pszSkinName = new char[18+strlen(pszMeshName)]; //xskin-C999MA_name-HEAD-HEADB
   strcpy(pszSkinName,"xskin-");
   strcpy(pszSkinName+6,pszMeshName);
   strcpy(pszSkinName+6+strlen(pszMeshName),"-HEAD-HEADB");
   char* pszTextureName = new char[11+iNameLength]; //C999MAlgt_name
   strcpy(pszTextureName,pszMeshName);
   strcpy(pszTextureName+6,pszSkin);
   pszTextureName[9] = '_';
   strcpy(pszTextureName+10,pszName);

   // Save cranium info file
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving head info");
   int iTempLength = strlen(pszMeshName);
   char* pszTempFilename = new char[iDirLength+iTempLength+10];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"cmx3\\");
   // Create output directory
   int iResult = _mkdir(pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create cmx3 directory!\n");
      eResult = F_DIR_ERROR;
   }
   strcpy(pszTempFilename+iDirLength+5,pszMeshName);
   strcpy(pszTempFilename+iDirLength+5+iTempLength,".cmx");
   ofstream osCraniumCMXStream(pszTempFilename);
   delete [] pszTempFilename;
   if (!osCraniumCMXStream.fail()) {
      // Write file
      osCraniumCMXStream << "// Character File. Copyright 1997, Maxis Inc." << endl;
      osCraniumCMXStream << "version 300" << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream << "1" << endl;
      osCraniumCMXStream << pszMeshName << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream << "1" << endl;
      osCraniumCMXStream << "HEAD" << endl;
      osCraniumCMXStream << pszSkinName << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream << "0" << endl;
      osCraniumCMXStream.close();
   }
   else eResult = F_FILE_ERROR;

   // Save cranium texture
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Converting head texture");
   CImage imgCranium(*(pAvatar->Texture(iFaceTexNum)));
   imgCranium.Scale(128,128);
   imgCranium.Flip();
   imgCranium.Convert(IT_PALETTE);
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving head texture");
   CImageFile* pFilter = g_oImageFileStore.CreateByExtension("bmp");
   iTempLength = strlen(pszTextureName);
   pszTempFilename = new char[iDirLength+iTempLength+11];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"skins\\");
   // Create output directory
   iResult = _mkdir(pszTempFilename);
   if ((iResult == -1) && (errno != EEXIST)) {
      TRACE("Couldn't create skins directory!\n");
      eResult = F_DIR_ERROR;
   }
   strcpy(pszTempFilename+iDirLength+6,pszTextureName);
   strcpy(pszTempFilename+iDirLength+6+iTempLength,".bmp");
   imgCranium.Save(pszTempFilename,pFilter);
   delete pFilter;
   delete [] pszTempFilename;

   // Save cranium skin
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving head skin");
   iTempLength = strlen(pszSkinName);
   pszTempFilename = new char[iDirLength+iTempLength+10];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"cmx3\\");
   strcpy(pszTempFilename+iDirLength+5,pszSkinName);
   strcpy(pszTempFilename+iDirLength+5+iTempLength,".skn");
   ofstream osCraniumSKNStream(pszTempFilename);
   delete [] pszTempFilename;
   if (!osCraniumSKNStream.fail()) {
      // Create head submesh
	   // Create the new face list
      int iNumFaces = pAvatar->NumFaces();
      int iNumVertices = pAvatar->NumVertices();
	   std::vector<const STriFace*> vFaceList;
      // Create new face list
      for (int i=0; i<iNumFaces; i++) {
         if (pFaces[i].m_iTextureNumber == iFaceTexNum) vFaceList.push_back(&(pFaces[i]));
      }
      iNumFaces = vFaceList.size();
      iNumVertices = iNumFaces * 3;
      // Write info to file
      osCraniumSKNStream << pszSkinName << endl;
      osCraniumSKNStream << 'x' << endl;
      // Write skeleton
      osCraniumSKNStream << '1' << endl;
      osCraniumSKNStream << "HEAD" << endl;
      // Write face info
      osCraniumSKNStream << iNumFaces << endl;
      for (i=0; i<iNumFaces; i++) {
         osCraniumSKNStream << (i*3)+2 << ' ';
         osCraniumSKNStream << (i*3)+1 << ' ';
         osCraniumSKNStream << (i*3)+0 << endl;
      }
      // Write bone info
      osCraniumSKNStream << '1' << endl;
      osCraniumSKNStream << "0 0 " << iNumVertices << " -1 0" << endl;
      // Write texture coordinates
      osCraniumSKNStream << iNumVertices << endl;
      for (i=0; i<iNumFaces; i++) {
         double dU0 = vFaceList[i]->m_sVertices[0].m_sTexCoord.dU;
         double dU1 = vFaceList[i]->m_sVertices[1].m_sTexCoord.dU;
         double dU2 = vFaceList[i]->m_sVertices[2].m_sTexCoord.dU;
         double dV0 = 1-vFaceList[i]->m_sVertices[0].m_sTexCoord.dV;
         double dV1 = 1-vFaceList[i]->m_sVertices[1].m_sTexCoord.dV;
         double dV2 = 1-vFaceList[i]->m_sVertices[2].m_sTexCoord.dV;
         osCraniumSKNStream << dU0 << ' ' << dV0 << endl;
         osCraniumSKNStream << dU1 << ' ' << dV1 << endl;
         osCraniumSKNStream << dU2 << ' ' << dV2 << endl;
      }
      // Write some stuff we don't understand yet...
      osCraniumSKNStream << '0' << endl;
      // Write vertex and normal data
      pAvatar->BoundingBox(skullbase,pntMax,pntMin);
      CVector3D vecMax(pntMax);
      CVector3D vecMin(pntMin);
      CVector3D vecCentre = (vecMax + vecMin) / 2;
      double dX, dY, dZ;
      vecCentre.ToDouble(dX, dY, dZ);
      dY = (dY + pntMin.m_dComponents[1]) / 2;
      dZ = (3*dZ + pntMin.m_dComponents[2]) / 4;
      const SPoint3D* pVertices = pAvatar->Vertices();
      const SPoint3D* pNormals = pAvatar->VertexNormals();
      osCraniumSKNStream << iNumVertices << endl;
      for (i=0; i<iNumFaces; i++) {
         for (int v=0; v<3; v++) {
            int iVertexIndex = vFaceList[i]->m_sVertices[v].m_iVertex;
            osCraniumSKNStream << (pVertices[iVertexIndex].m_dComponents[1]-dY)*dScale << ' ';
            osCraniumSKNStream << (pVertices[iVertexIndex].m_dComponents[0]-dX)*dScale << ' ';
            osCraniumSKNStream << (pVertices[iVertexIndex].m_dComponents[2]-dZ)*dScale << ' ';
            osCraniumSKNStream << pNormals[iVertexIndex].m_dComponents[1] << ' ';
            osCraniumSKNStream << pNormals[iVertexIndex].m_dComponents[0] << ' ';
            osCraniumSKNStream << pNormals[iVertexIndex].m_dComponents[2] << endl;
         }
      }
      osCraniumSKNStream.close();
   }
   else eResult = F_ERROR;

   ////////////////////////////////////////////////////////////////////
   // BODY ////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////

   // Build Body Names
   delete [] pszMeshName;
   delete [] pszSkinName;
   delete [] pszTextureName;
   pszMeshName = new char[11+iNameLength]; //B999MAFit_name
   pszMeshName[0] = 'B';
   strcpy(pszMeshName+1,"999");
   pszMeshName[4] = cSex; //(m_iAge == SIMS_CHILD) ? 'U' : cSex;
   pszMeshName[5] = cAge;
   strcpy(pszMeshName+6,pszBuild);
   pszMeshName[9] = '_';
   strcpy(pszMeshName+10,pszName);
   pszSkinName = new char[20+strlen(pszMeshName)]; //xskin-B999MAFit_name-PELVIS-BODY
   strcpy(pszSkinName,"xskin-");
   strcpy(pszSkinName+6,pszMeshName);
   strcpy(pszSkinName+6+strlen(pszMeshName),"-PELVIS-BODY");
   pszTextureName = new char[14+iNameLength]; //"B999MAFitlgt_name
   strcpy(pszTextureName,pszMeshName);
   //pszTextureName[4] = cSex;
   strcpy(pszTextureName+9,pszSkin);
   pszTextureName[12] = '_';
   strcpy(pszTextureName+13,pszName);

   // Save body info file
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving body info");
   iTempLength = strlen(pszMeshName);
   pszTempFilename = new char[iDirLength+iTempLength+10];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"cmx3\\");
   strcpy(pszTempFilename+iDirLength+5,pszMeshName);
   strcpy(pszTempFilename+iDirLength+5+iTempLength,".cmx");
   ofstream osBodyCMXStream(pszTempFilename);
   delete [] pszTempFilename;
   if (!osBodyCMXStream.fail()) {
      // Write file
      osBodyCMXStream << "// Character File. Copyright 1997, Maxis Inc." << endl;
      osBodyCMXStream << "version 300" << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream << "1" << endl;
      osBodyCMXStream << pszMeshName << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream << "1" << endl;
      osBodyCMXStream << "PELVIS" << endl;
      osBodyCMXStream << pszSkinName << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream << "0" << endl;
      osBodyCMXStream.close();
   }
   else eResult = F_ERROR;

   // Save body texture
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Creating body texture");
   // Resize textures
   CImage** pImages = new CImage*[pAvatar->NumTextures()];
   int* pMinima = new int[pAvatar->NumTextures()];
   int* pMaxima = new int[pAvatar->NumTextures()];
   int iTotalHeight = 0;
   const int iTextureWidth = 256;
   const SBodyPart* pBodyParts = pAvatar->BodyParts();
   for (int t=0; t<pAvatar->NumTextures(); t++) {
      if ((t != iFaceTexNum) && (t != iRHandTexNum) && (t != iLHandTexNum)) {
         int iWidth = 0;
         int iHeight = 0;
         CImage* pSmallerImage = new CImage(*(pAvatar->Texture(t)));
         pSmallerImage->GetSize(iWidth,iHeight);
         iWidth = iTextureWidth;
         if (t == iTorsoTexNum) iHeight = iTextureWidth;
         else if (iHeight > iTextureWidth/4) iHeight = iTextureWidth/4;
         pSmallerImage->Scale(iWidth,iHeight);
         pSmallerImage->Flip();
         pMinima[t] = iTotalHeight;
         iTotalHeight += iHeight;
         pMaxima[t] = iTotalHeight - 1;
         pImages[t] = pSmallerImage;
      }
      else {
         pMinima[t] = iTotalHeight;
         iTotalHeight += 0;
         pMaxima[t] = iTotalHeight;
         pImages[t] = NULL;
      }
   }
   // Create big image
   CImage imgTexture(IT_RGB,iTextureWidth,iTotalHeight);
   int iWidth = 0; 
   int iHeight = 0;
   imgTexture.GetSize(iWidth,iHeight);
   if (iWidth==0 || iHeight==0) {
      return F_OUT_OF_MEMORY;
   }
   else {
      for (t=0; t<pAvatar->NumTextures(); t++) {
         if (pImages[t] != NULL) {
            imgTexture.Paste(*(pImages[t]),0,pMinima[t]);
            delete pImages[t];
         }
      }
   }
   delete [] pImages;
   const int iTextureHeight = 256;
   imgTexture.Scale(iTextureWidth,iTextureHeight);
   // Save big image
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Converting body texture");
   imgTexture.Convert(IT_PALETTE);
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving body texture");
   pFilter = g_oImageFileStore.CreateByExtension("bmp");
   iTempLength = strlen(pszTextureName);
   pszTempFilename = new char[iDirLength+iTempLength+11];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"skins\\");
   strcpy(pszTempFilename+iDirLength+6,pszTextureName);
   strcpy(pszTempFilename+iDirLength+6+iTempLength,".bmp");
   imgTexture.Save(pszTempFilename,pFilter);
   delete pFilter;
   delete [] pszTempFilename;

   // Pose model
   CVector3D vecY(0,1,0);
   // Point arms vaguely upwards...
   pAvatar->SetJointAngle(l_shoulder,CAxisRotation(1,0,0,-V_PI),false);
   pAvatar->SetJointAngle(r_shoulder,CAxisRotation(1,0,0,V_PI),false);
   pAvatar->UpdateModel();
   // ...and then align exactly vertical
   pAvatar->AlignBodyPart(l_elbow,vecY,false);
   pAvatar->AlignBodyPart(r_elbow,vecY,false);
   // Legs togethers and pointing upwards
   pAvatar->SetJointAngle(l_hip,CAxisRotation(0,0,1,V_PI),false);
   pAvatar->SetJointAngle(r_hip,CAxisRotation(0,0,1,-V_PI),false);
   pAvatar->UpdateModel();
   // Align forearms
   pAvatar->AlignBodyPart(l_wrist,vecY,false);
   pAvatar->AlignBodyPart(r_wrist,vecY,false);
   pAvatar->UpdateModel();

   // Save body skin
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "Saving body skin");
   iTempLength = strlen(pszSkinName);
   pszTempFilename = new char[iDirLength+iTempLength+10];
   strcpy(pszTempFilename,pszTgtDir);
   strcpy(pszTempFilename+iDirLength,"cmx3\\");
   strcpy(pszTempFilename+iDirLength+5,pszSkinName);
   strcpy(pszTempFilename+iDirLength+5+iTempLength,".skn");
   ofstream osBodySKNStream(pszTempFilename);
   delete [] pszTempFilename;
   if (!osBodySKNStream.fail()) {

      // Write file info
      osBodySKNStream << pszSkinName << endl;
      osBodySKNStream << pszTextureName << endl;

      // Write skeleton
      const int iNumBones = 15;
      osBodySKNStream << iNumBones << endl;
      osBodySKNStream << "PELVIS" << endl;
      osBodySKNStream << "L_LEG" << endl;
      osBodySKNStream << "L_LEG1" << endl;
      osBodySKNStream << "L_FOOT" << endl;
      osBodySKNStream << "R_LEG" << endl;
      osBodySKNStream << "R_LEG1" << endl;
      osBodySKNStream << "R_FOOT" << endl;
      osBodySKNStream << "SPINE" << endl;
      osBodySKNStream << "SPINE1" << endl;
      osBodySKNStream << "SPINE2" << endl;
      osBodySKNStream << "NECK" << endl;
      osBodySKNStream << "L_ARM1" << endl;
      osBodySKNStream << "L_ARM2" << endl;
      osBodySKNStream << "R_ARM1" << endl;
      osBodySKNStream << "R_ARM2" << endl;
   
	   // Create body submesh
      BodyPart bpParts[] = {
         sacroiliac,
         l_hip,
         l_knee,
         l_ankle,
         r_hip,
         r_knee,
         r_ankle,
         vl5,
         vt12,
         vt4,
         vc7,
         l_shoulder,
         l_elbow,
         r_shoulder,
         r_elbow,
         skirt
      };
      // Count faces
      int iNumFaces = 0;
      for (int i=0; i<iNumBones+1; i++) {
         iNumFaces += pBodyParts[bpParts[i]].m_iNumFaces;
         // Merge vt12 and vt8
         if (bpParts[i] == vt12) iNumFaces += pBodyParts[vt8].m_iNumFaces;
      }
      // Copy and translate face info
      const double dTextureScale = (double)iTextureHeight / (double)iTotalHeight;
      STriFace* pSubMesh;
      NEWBEGIN
      pSubMesh = new STriFace[iNumFaces];
      NEWEND("CAvatarFileSims::Save() - body submesh creation failed!")
      BodyPart* pFaceToPartMap;
      NEWBEGIN
      pFaceToPartMap = new BodyPart[iNumFaces];
      NEWEND("CAvatarFileSims::Save() - face map creation failed!");
      int iFace = 0;
      for (i=0; i<iNumBones+1; i++) {
         BodyPart bpPart = bpParts[i];
         int iPartFaces = pBodyParts[bpPart].m_iNumFaces;
         for (int f=0; f<iPartFaces; f++) {
            pFaceToPartMap[iFace] = bpPart;
            const STriFace* pFace = &pFaces[pBodyParts[bpPart].m_piFaces[f]];
            // Copy vertex indices
            for (int v=0; v<3; v++) pSubMesh[iFace].m_sVertices[v].m_iVertex = pFace->m_sVertices[v].m_iVertex;
            // Copy texture number and work out texture base positions and sizes
            int t = pSubMesh[iFace].m_iTextureNumber = pFace->m_iTextureNumber;
            double dBaseV = pMinima[t] * dTextureScale;
            double dHeight = (pMaxima[t] - pMinima[t]+1) * dTextureScale;
            // Copy and translate texture coordinates
            for (v=0; v<3; v++) {
                double dTexCoordV = 1-pFace->m_sVertices[v].m_sTexCoord.dV;
               dTexCoordV *= dHeight;
               dTexCoordV += dBaseV;
               dTexCoordV /= (double)iTextureHeight;
               pSubMesh[iFace].m_sVertices[v].m_sTexCoord.dU = pFace->m_sVertices[v].m_sTexCoord.dU ;
               pSubMesh[iFace].m_sVertices[v].m_sTexCoord.dV = dTexCoordV ;
            }
            iFace++;
         }
         if (bpPart == vt12) {
            int iPartFaces = pBodyParts[vt4].m_iNumFaces;
            for (int f=0; f<iPartFaces; f++) {
               pFaceToPartMap[iFace] = bpPart;
               const STriFace* pFace = &pFaces[pBodyParts[vt4].m_piFaces[f]];
               // Copy vertex indices
               for (int v=0; v<3; v++) pSubMesh[iFace].m_sVertices[v].m_iVertex = pFace->m_sVertices[v].m_iVertex;
               // Copy texture number and work out texture base positions and sizes
               int t = pSubMesh[iFace].m_iTextureNumber = pFace->m_iTextureNumber;
               double dBaseV = pMinima[t] * dTextureScale;
               double dHeight = (pMaxima[t] - pMinima[t]+1) * dTextureScale;
               // Copy and translate texture coordinates
               for (v=0; v<3; v++) {
                  double dTexCoordV = 1-pFace->m_sVertices[v].m_sTexCoord.dV;
                  dTexCoordV *= dHeight;
                  dTexCoordV += dBaseV;
                  dTexCoordV /= (double)iTextureHeight;
                  pSubMesh[iFace].m_sVertices[v].m_sTexCoord.dU = pFace->m_sVertices[v].m_sTexCoord.dU ;
                  pSubMesh[iFace].m_sVertices[v].m_sTexCoord.dV = dTexCoordV ;
               }
               iFace++;
            }
         }
      }
      int iNumVertices = iNumFaces * 3;

      // Create new vertex list and map
      int* pVertexList = new int[iNumVertices];
      int* pVertexMap = new int[iNumVertices];
      int* pVertexCounts = new int[iNumBones];
      int iCurrentVertex = 0;
      int iLastBoneCount = 0;
      for (i=0; i<iNumBones; i++) {
         BodyPart bpPart = bpParts[i];
         for (int f=0; f<iNumFaces; f++) {
            for (int v=0; v<3; v++) {
               BodyPart bpVertexPart = (BodyPart)(pAvatar->GetVertexPart(pSubMesh[f].m_sVertices[v].m_iVertex));
               if (bpVertexPart == bpPart) {
                  pVertexList[iCurrentVertex] = pSubMesh[f].m_sVertices[v].m_iVertex;
                  pVertexMap[(f*3)+v] = iCurrentVertex++;
               }
               else if ((bpPart == vt12) && (bpVertexPart == vt8)){
                  pVertexList[iCurrentVertex] = pSubMesh[f].m_sVertices[v].m_iVertex;
                  pVertexMap[(f*3)+v] = iCurrentVertex++;
               }
               else if ((bpPart == r_elbow) && (bpVertexPart == r_wrist)) {
                  pVertexList[iCurrentVertex] = pSubMesh[f].m_sVertices[v].m_iVertex;
                  pVertexMap[(f*3)+v] = iCurrentVertex++;
               }
               else if ((bpPart == l_elbow) && (bpVertexPart == l_wrist)) {
                  pVertexList[iCurrentVertex] = pSubMesh[f].m_sVertices[v].m_iVertex;
                  pVertexMap[(f*3)+v] = iCurrentVertex++;
               }
               else if ((bpPart == vc7) && (bpVertexPart == skullbase)) {
                  pVertexList[iCurrentVertex] = pSubMesh[f].m_sVertices[v].m_iVertex;
                  pVertexMap[(f*3)+v] = iCurrentVertex++;
               }
            }
         }
         pVertexCounts[i] = iCurrentVertex - iLastBoneCount;
         iLastBoneCount = iCurrentVertex;
      }
      
      // Write faces
      osBodySKNStream << iNumFaces << endl;
      for (i=0; i<iNumFaces; i++) {         
         osBodySKNStream << pVertexMap[(i*3)+2] << ' ';
         osBodySKNStream << pVertexMap[(i*3)+1] << ' ';
         osBodySKNStream << pVertexMap[(i*3)+0] << endl;
      }

      // Write bone info
      osBodySKNStream << iNumBones << endl;
      int iStart = 0;
      for (i=0; i<iNumBones; i++) {         
         osBodySKNStream << "0 " << iStart << ' ' << pVertexCounts[i] << " -1 0" << endl;
         iStart += pVertexCounts[i];
      }

      // Write texture coordinates
      osBodySKNStream << iNumVertices << endl;
      STexCoord* pTexCoords = new STexCoord[iNumVertices];
      for (i=0; i<iNumFaces; i++) {
         const STriFace* pFace = &pSubMesh[i];
         for (int v=0; v<3; v++) {
            pTexCoords[pVertexMap[(i*3)+v]].dU = pFace->m_sVertices[v].m_sTexCoord.dU;
            pTexCoords[pVertexMap[(i*3)+v]].dV = pFace->m_sVertices[v].m_sTexCoord.dV;
         }
      }
      for (i=0; i<iNumVertices; i++) {
         osBodySKNStream << pTexCoords[i].dU << ' ' << pTexCoords[i].dV << endl;
      }
      delete [] pTexCoords;

      // Write some stuff we don't understand yet...
      osBodySKNStream << '0' << endl;

      // Write vertex and normal data
      const SPoint3D* pVertices = pAvatar->Vertices();
      const SPoint3D* pNormals = pAvatar->VertexNormals();
      osBodySKNStream << iNumVertices << endl;
      for (i=0; i<iNumVertices; i++) {
         int iVertexIndex = pVertexList[i];
         BodyPart bpPart = (BodyPart)pAvatar->GetVertexPart(iVertexIndex);
         if (bpPart == l_wrist) bpPart = l_elbow;
         else if (bpPart == r_wrist) bpPart = r_elbow;
         else if (bpPart == skullbase) bpPart = vc7;
         else if (bpPart == vt8) bpPart = vt12;
         double dX = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[0];
         double dY = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[1];
         // Correct leg positioning
         if (bpPart == l_hip || bpPart == r_hip) {
            pAvatar->BoundingBox(sacroiliac,pntMax,pntMin);
            dY = pntMin.m_dComponents[1];
         }
         double dZ = pBodyParts[bpPart].m_pntCurrentCentre.m_dComponents[2];
         // Correct neck positioning
         if (bpPart == vc7) {
            dZ = (dZ + pBodyParts[vl5].m_pntCurrentCentre.m_dComponents[2]) / 2;
         }
         // Correct body positioning
         if (bpPart == vt4) {
            pAvatar->BoundingBox(vc7,pntMax,pntMin);
            double dMinNeck = pntMin.m_dComponents[1];
            pAvatar->BoundingBox(vt4,pntMax,pntMin);
            double dMaxBody = pntMax.m_dComponents[1];
            dY -= (dMinNeck - dMaxBody) / 2;
         }
         if (bpPart == vt12) {
            pAvatar->BoundingBox(vc7,pntMax,pntMin);
            double dMinNeck = pntMin.m_dComponents[1];
            pAvatar->BoundingBox(vt4,pntMax,pntMin);
            double dMaxBody = pntMax.m_dComponents[1];
            dY -= (dMinNeck - dMaxBody) / 4;
         }
         // Correct chest positioning
         double dVerticalStretch = 1;
         double dHorizontalStretch = 0.8;
         if ((bpPart == l_shoulder) || (bpPart == l_elbow) || (bpPart == r_shoulder) || (bpPart == r_elbow)) {
            dHorizontalStretch = 1;
         }
         else if ((bpPart == l_knee) || (bpPart == l_ankle) || (bpPart == r_knee) || (bpPart == r_ankle)) {            
            dHorizontalStretch = 0.9;
         }
         // Try to hide the neck gap
         if (bpPart == vc7) dVerticalStretch = 2;
         // Extend forearms to meet hands
         if ((bpPart == l_elbow) || (bpPart == r_elbow)) dVerticalStretch = 1.2;
         // Write data
         osBodySKNStream << (pVertices[iVertexIndex].m_dComponents[1]-dY)*dScale*dVerticalStretch << ' ';
         osBodySKNStream << (pVertices[iVertexIndex].m_dComponents[0]-dX)*dScale*dHorizontalStretch << ' ';
         osBodySKNStream << (pVertices[iVertexIndex].m_dComponents[2]-dZ)*dScale*dHorizontalStretch << ' ';
         osBodySKNStream << pNormals[iVertexIndex].m_dComponents[1] << ' ';
         osBodySKNStream << pNormals[iVertexIndex].m_dComponents[0] << ' ';
         osBodySKNStream << pNormals[iVertexIndex].m_dComponents[2] << endl;
      }      

      // Tidy up allocated memory
      delete [] pVertexCounts;
      delete [] pVertexList;
      delete [] pVertexMap;
      delete [] pFaceToPartMap;
      delete [] pSubMesh;

      // Close stream
      osBodySKNStream.close();
   }
   else eResult = F_ERROR;

   // Done!
   g_poVAL->StepProgress("SimsSave");
   g_poVAL->SetProgressText("SimsSave", "");
   // Tidy up memory
   delete [] pMinima;
   delete [] pMaxima;
   delete [] pszTgtDir;
   delete [] pszName;
   delete [] pszMeshName;
   delete [] pszSkinName;
   delete [] pszTextureName;
   // Restore old pose
   pAvatar->ImportPose(poOld);
   pAvatar->UpdateModel();
   return eResult;

} // Save(const char* pszFilename, CAvatar* pAvatar)

void CAvatarFileSims::SetOption(int iOption, int iData) const {
   switch (iOption) {
   case SIMS_AGE:
      m_iAge = iData;
      break;
   case SIMS_SEX:
      m_iSex = iData;
      break;
   case SIMS_SKINTONE:
      m_iSkin = iData;
      break;
   case SIMS_BUILD:
      m_iBuild = iData;
      break;
   default:
      break;
   }
   return;
} //SetOption(int iOption, int iData;)
