//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileHANIM.cpp - 04/10/2000 - James Smith
//	H-Anim 1.1 export filter implementation
//
// $Id: AvatarFileHANIM.cpp,v 1.0 2000/10/04 16:27:08 waz Exp $
//

#include "stdafx.h"

#include "AvatarFileHANIM.h"
#include "AvatarFileProxy.h"
#include "ImageFileStore.h"

#include <set>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CImageFileStore g_oImageFileStore;

/////////////////////
// CAvatarFileHANIM

CAvatarFileProxy<CAvatarFileHANIM> g_oAvatarProxyHANIM;

/////////////////////
// Member Functions

static const char* pJointNames[] = 
{
	"HumanoidRoot",
	"sacroiliac",
	"l_hip",
	"l_knee",
	"l_ankle",
	"l_subtalar",
	"l_midtarsal",
	"l_metatarsal",
	"r_hip",
	"r_knee",
	"r_ankle",
	"r_subtalar",
	"r_midtarsal",
	"r_metatarsal",
	"vl5",
	"vl4",
	"vl3",
	"vl2",
	"vl1",
	"vt12",
	"vt11",
	"vt10",
	"vt9",
	"vt8",
	"vt7",
	"vt6",
	"vt5",
	"vt4",
	"vt3",
	"vt2",
	"vt1",
	"vc7",
	"vc6",
	"vc5",
	"vc4",
	"vc3",
	"vc2",
	"vc1",
	"skullbase",
	"l_eyeball_joint",
	"r_eyeball_joint",
	"l_sternoclavicular",
	"l_acromioclavicular",
	"l_shoulder",
	"l_elbow",
	"l_wrist",
	"l_thumb1",
	"l_thumb2",
	"l_thumb3",
	"l_index0",
	"l_index1",
	"l_index2",
	"l_index3",
	"l_middle0",
	"l_middle1",
	"l_middle2",
	"l_middle3",
	"l_ring0",
	"l_ring1",
	"l_ring2",
	"l_ring3",
	"l_pinky0",
	"l_pinky1",
	"l_pinky2",
	"l_pinky3",
	"r_sternoclavicular",
	"r_acromioclavicular",
	"r_shoulder",
	"r_elbow",
	"r_wrist",
	"r_thumb1",
	"r_thumb2",
	"r_thumb3",
	"r_index0",
	"r_index1",
	"r_index2",
	"r_index3",
	"r_middle0",
	"r_middle1",
	"r_middle2",
	"r_middle3",
	"r_ring0",
	"r_ring1",
	"r_ring2",
	"r_ring3",
	"r_pinky0",
	"r_pinky1",
	"r_pinky2",
	"r_pinky3"
};

static const char* pSegmentNames[] = 
{
	"sacrum",
	"pelvis",
	"l_thigh",
	"l_calf",
	"l_hindfoot",
	"l_midproximal",
	"l_middistal",
	"l_forefoot",
	"r_thigh",
	"r_calf",
	"r_hindfoot",
	"r_midproximal",
	"r_middistal",
	"r_forefoot",
	"l5",
	"l4",
	"l3",
	"l2",
	"l1",
	"t12",
	"t11",
	"t10",
	"t9",
	"t8",
	"t7",
	"t6",
	"t5",
	"t4",
	"t3",
	"t2",
	"t1",
	"c7",
	"c6",
	"c5",
	"c4",
	"c3",
	"c2",
	"c1",
	"skull",
	"l_eyeball",
	"r_eyeball",
	"l_clavicle",
	"l_scapula",
	"l_upperarm",
	"l_forearm",
	"l_hand",
	"l_thumb_metacarpal",
	"l_thumb_proximal",
	"l_thumb_distal",
	"l_index_metacarpal",
	"l_index_proximal",
	"l_index_middle",
	"l_index_distal",
	"l_middle_metacarpal",
	"l_middle_proximal",
	"l_middle_middle",
	"l_middle_distal",
	"l_ring_metacarpal",
	"l_ring_proximal",
	"l_ring_middle",
	"l_ring_distal",
	"l_pinky_metacarpal",
	"l_pinky_proximal",
	"l_pinky_middle",
	"l_pinky_distal",
	"r_clavicle",
	"r_scapula",
	"r_upperarm",
	"r_forearm",
	"r_hand",
	"r_thumb_metacarpal",
	"r_thumb_proximal",
	"r_thumb_distal",
	"r_index_metacarpal",
	"r_index_proximal",
	"r_index_middle",
	"r_index_distal",
	"r_middle_metacarpal",
	"r_middle_proximal",
	"r_middle_middle",
	"r_middle_distal",
	"r_ring_metacarpal",
	"r_ring_proximal",
	"r_ring_middle",
	"r_ring_distal",
	"r_pinky_metacarpal",
	"r_pinky_proximal",
	"r_pinky_middle",
	"r_pinky_distal",
   // Site names
   "l_middistal_tip",
   "r_middistal_tip",
   "skull_tip",
   "l_hand_tip",
   "l_hand_tip"
};

////////////////////
// Store Functions

CAvatarFileHANIM::CAvatarFileHANIM() {
   m_pszBasename = NULL;
} // CAvatarFileHANIM()

float CAvatarFileHANIM::GetFilterVersion() const {
	return GetVersion();
} // GetFilterVersion()

const char* CAvatarFileHANIM::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension()

const char* CAvatarFileHANIM::GetFileTitle() const {
	return GetTitle();
} // GetFileTitle()

bool CAvatarFileHANIM::CanFilterLoadFile() const {
   return CanLoadFile();
} // CanFilterLoadFile();

bool CAvatarFileHANIM::CanFilterLoadStream() const {
   return CanLoadStream();
} // CanFilterLoadStream();

bool CAvatarFileHANIM::CanFilterSaveFile() const {
   return CanSaveFile();
} // CanFilterSaveFile();

bool CAvatarFileHANIM::CanFilterSaveStream() const {
   return CanSaveStream();
} // CanFilterSaveStream();

bool CAvatarFileHANIM::CanFilterLoadBPFile() const {
   return CanLoadBPFile();
} // CanFilterLoadBPFile();

bool CAvatarFileHANIM::CanFilterLoadBPStream() const {
   return CanLoadBPStream();
} // CanFilterLoadBPStream();

////////////////////////
// AvatarFile Functions

FRESULT CAvatarFileHANIM::Save(const char* pszFilename, CAvatar* pAvatar) const {

   if (pAvatar == NULL) return F_NO_DATA_TO_SAVE;
   FRESULT eResult = F_OK;

   // Reset pose
   CAvatarPose poOld = pAvatar->ExportPose();
   //pAvatar->ResetPose();
   //pAvatar->UpdateModel();
   // Work out base filename and store it
   char* pszLocalFilename = strdup(pszFilename);
   char* pszTemp = pszLocalFilename;
   while (strchr(pszTemp,'\\') != NULL) {
      pszTemp = strchr(pszTemp,'\\') + 1;
   }
   int iLength = (strchr(pszTemp,'.') - pszTemp);
   m_pszBasename = new char[iLength+1];
   memset(m_pszBasename,0,iLength+1);
   for (int i=0; i<iLength; i++) m_pszBasename[i] = pszTemp[i];
   free(pszLocalFilename);
   int iBaseNameLength = strlen(m_pszBasename);
   // Setup the export progress dialog
   g_poVAL->SetProgressMax("HAnimSave",3 + 2*pAvatar->NumTextures());
   g_poVAL->StepProgress("HAnimSave");
   // Resize textures
   CString strMessage;
   CImage** pImages = new CImage*[pAvatar->NumTextures()];
   m_pMinima = new int[pAvatar->NumTextures()];
   m_pMaxima = new int[pAvatar->NumTextures()];
   m_iTotalHeight = 0;
   const int iTextureWidth = 256;
   for (int t=0; t<pAvatar->NumTextures(); t++) {
      g_poVAL->StepProgress("HAnimSave");
      strMessage.Format("Resizing texture %d", t);
      g_poVAL->SetProgressText("HAnimSave",strMessage);
      int iWidth = 0;
      int iHeight = 0;
      CImage* pSmallerImage = new CImage(*(pAvatar->Texture(t)));
      pSmallerImage->GetSize(iWidth,iHeight);
      iWidth = iTextureWidth;
      if (iHeight > iTextureWidth/2) iHeight = iTextureWidth/2;
      if (t != pAvatar->TextureIndex(skullbase)) iHeight = iHeight >> 1;
      pSmallerImage->Scale(iWidth,iHeight);
      pSmallerImage->Flip();
      m_pMinima[t] = m_iTotalHeight;
      m_iTotalHeight += iHeight;
      m_pMaxima[t] = m_iTotalHeight - 1;
      pImages[t] = pSmallerImage;
   }
   // Create big image
   CImage imgTexture(IT_RGB,iTextureWidth,m_iTotalHeight);
   int iWidth = 0; 
   int iHeight = 0;
   imgTexture.GetSize(iWidth,iHeight);
   if (iWidth==0 || iHeight==0) {
      ::AfxMessageBox("Could not create merged texture!",MB_ICONERROR|MB_OK);
   }
   else {
      for (t=0; t<pAvatar->NumTextures(); t++) {
         g_poVAL->StepProgress("HAnimSave");
         strMessage.Format("Merging texture %d", t);
         g_poVAL->SetProgressText("HAnimSave",strMessage);
         imgTexture.Paste(*(pImages[t]),0,m_pMinima[t]);
         delete pImages[t];
      }
   }
   delete [] pImages;
   // Save big image
   g_poVAL->StepProgress("HAnimSave");
   g_poVAL->SetProgressText("HAnimSave","Saving JPEG texture");
   CImageFile* pImageFile = g_oImageFileStore.CreateByExtension("jpg");
   char* pszTextureFilename = new char[iBaseNameLength+5];
   strcpy(pszTextureFilename,m_pszBasename);
   strcpy(pszTextureFilename+iBaseNameLength,".jpg");
   imgTexture.Save(pszTextureFilename,pImageFile);
   delete [] pszTextureFilename;
   // Save HANIM file
   g_poVAL->SetProgressText("HAnimSave","Saving VRML file");
	ofstream osOutputStream(pszFilename);
	if (!osOutputStream.bad()) {
	   // Write preamble
	   osOutputStream << "#VRML V2.0 utf8\n";
	   osOutputStream << "\n";
	   // Write out PROTOs
	   osOutputStream << "PROTO Humanoid [\n";
	   osOutputStream << "   field        SFVec3f    bboxCenter       0 0 0\n";
	   osOutputStream << "   field        SFVec3f    bboxSize         -1 -1 -1\n";
	   osOutputStream << "   exposedField SFVec3f    center           0 0 0\n";
	   osOutputStream << "   exposedField MFNode     humanoidBody     [ ]\n";
	   osOutputStream << "   exposedField MFString   info             [ ]\n";
	   osOutputStream << "   exposedField MFNode     joints           [ ]\n";
	   osOutputStream << "   exposedField SFString   name             \"\"\n";
	   osOutputStream << "   exposedField SFRotation rotation         0 0 1 0\n";
	   osOutputStream << "   exposedField SFVec3f    scale            1 1 1\n";
	   osOutputStream << "   exposedField SFRotation scaleOrientation 0 0 1 0\n";
	   osOutputStream << "   exposedField MFNode     segments         [ ]\n";
	   osOutputStream << "   exposedField MFNode     sites            [ ]\n";
	   osOutputStream << "   exposedField SFVec3f    translation      0 0 0\n";
	   osOutputStream << "   exposedField SFString   version          \"1.1\"\n";
	   osOutputStream << "   exposedField MFNode     viewpoints       [ ]\n";
	   osOutputStream << "]\n";
	   osOutputStream << "{\n";
	   osOutputStream << "   Transform {\n";
	   osOutputStream << "      bboxCenter       IS bboxCenter\n";
	   osOutputStream << "      bboxSize         IS bboxSize\n";
	   osOutputStream << "      center           IS center\n";
	   osOutputStream << "      rotation         IS rotation\n";
	   osOutputStream << "      scale            IS scale\n";
	   osOutputStream << "      scaleOrientation IS scaleOrientation\n";
	   osOutputStream << "      translation      IS translation\n";
	   osOutputStream << "      children [\n";
	   osOutputStream << "         Group {\n";
	   osOutputStream << "            children   IS viewpoints\n";
	   osOutputStream << "         }\n";
	   osOutputStream << "         Group {\n";
	   osOutputStream << "            children   IS humanoidBody\n";
	   osOutputStream << "         }\n";
	   osOutputStream << "      ]\n";
	   osOutputStream << "   }\n";
	   osOutputStream << "}\n";
	   osOutputStream << "PROTO Joint [\n";
	   osOutputStream << "   exposedField SFVec3f    center           0 0 0\n";
	   osOutputStream << "   exposedField MFNode     children         [ ]\n";
	   osOutputStream << "   exposedField MFFloat    llimit           [ ]\n";
	   osOutputStream << "   exposedField SFRotation limitOrientation 0 0 1 0\n";
	   osOutputStream << "   exposedField SFString   name             \"\"\n";
	   osOutputStream << "   exposedField SFRotation rotation         0 0 1 0\n";
	   osOutputStream << "   exposedField SFVec3f    scale            1 1 1\n";
	   osOutputStream << "   exposedField SFRotation scaleOrientation 0 0 1 0\n";
	   osOutputStream << "   exposedField MFFloat    stiffness        [ 0 0 0 ]\n";
	   osOutputStream << "   exposedField SFVec3f    translation      0 0 0\n";
	   osOutputStream << "   exposedField MFFloat    ulimit           []\n";
	   osOutputStream << "]\n";
	   osOutputStream << "{\n";
	   osOutputStream << "   Transform {\n";
	   osOutputStream << "      translation      IS translation\n";
	   osOutputStream << "      rotation         IS rotation\n";
	   osOutputStream << "      scale            IS scale\n";
	   osOutputStream << "      scaleOrientation IS scaleOrientation\n";
	   osOutputStream << "      center           IS center\n";
	   osOutputStream << "      children         IS children\n";
	   osOutputStream << "   }\n";
	   osOutputStream << "}\n";
	   osOutputStream << "PROTO Segment [\n";
	   osOutputStream << "   field        SFVec3f    bboxCenter       0 0 0\n";
	   osOutputStream << "   field        SFVec3f    bboxSize         -1 -1 -1\n";
	   osOutputStream << "   exposedField SFVec3f    centerOfMass     0 0 0\n";
	   osOutputStream << "   exposedField MFNode     children         [ ]\n";
	   osOutputStream << "   exposedField SFNode     coord            NULL\n";
	   osOutputStream << "   exposedField MFNode     displacers       [ ]\n";
	   osOutputStream << "   exposedField SFFloat    mass             0\n";
	   osOutputStream << "   exposedField MFFloat    momentsOfInertia [ 0 0 0 0 0 0 0 0 0 ]\n";
	   osOutputStream << "   exposedField SFString   name             \"\"\n";
	   osOutputStream << "   eventIn      MFNode     addChildren\n";
	   osOutputStream << "   eventIn      MFNode     removeChildren\n";
	   osOutputStream << "]\n";
	   osOutputStream << "{\n";
	   osOutputStream << "   Group {\n";
	   osOutputStream << "      children         IS children\n";
	   osOutputStream << "      bboxCenter       IS bboxCenter\n";
	   osOutputStream << "      bboxSize         IS bboxSize\n";
	   osOutputStream << "      addChildren      IS addChildren\n";
	   osOutputStream << "      removeChildren   IS removeChildren\n";
	   osOutputStream << "   }\n";
	   osOutputStream << "}\n";
	   osOutputStream << "\n";
	   // Start writing out Humanoid node
	   osOutputStream << "DEF HUMANOID Humanoid {\n";
	   osOutputStream << "   name \"" << m_pszBasename << "\"\n";
	   osOutputStream << "   humanoidBody [\n";
	   // Write humanoidBody
	   WriteBodyPart(root,pAvatar,osOutputStream);
	   osOutputStream << "   ]\n";
	   // Write joints field
	   osOutputStream << "   joints [\n";
	   WriteUSEStatement(root,pAvatar,osOutputStream);
	   osOutputStream << "   ]\n";
	   // Write segments field
	   osOutputStream << "   segments [\n";
	   WriteUSEStatement(root,pAvatar,osOutputStream,true);
	   osOutputStream << "   ]\n";
		   // Add viewpoints and close up Humanoid node
  	   osOutputStream << "   viewpoints [\n";
	   osOutputStream << "      DEF Front_View Viewpoint {\n";
	   osOutputStream << "         description \"Front View\"\n";
	   osOutputStream << "         position 0 1 3\n";
	   osOutputStream << "      }\n";
	   osOutputStream << "      DEF Side_View Viewpoint {\n";
	   osOutputStream << "         description \"Side View\"\n";
	   osOutputStream << "         position 3 1 0\n";
	   osOutputStream << "         orientation 0 1 0 1.57\n";
	   osOutputStream << "      }\n";
	   osOutputStream << "      DEF Top_View Viewpoint {\n";
	   osOutputStream << "         description \"Top View\"\n";
	   osOutputStream << "         position 0 3 0\n";
	   osOutputStream << "         orientation 1 0 0 -1.57\n";
	   osOutputStream << "      }\n";
	   osOutputStream << "      DEF Inclined_View Viewpoint {\n";
	   osOutputStream << "         description \"Inclined View\"\n";
	   osOutputStream << "         position 2 1 2\n";
	   osOutputStream << "         orientation 0 1 0 .78\n";
	   osOutputStream << "      }\n";
	   osOutputStream << "   ]\n";
	   osOutputStream << "}\n";
      if (osOutputStream.bad()) eResult = F_FILE_ERROR;
	}
	osOutputStream.close();
   // done!
   g_poVAL->StepProgress("HAnimSave");
   delete [] m_pszBasename;
   m_pszBasename = NULL;
   // Restore pose
   delete [] m_pMinima;
   delete [] m_pMaxima;
   pAvatar->ImportPose(poOld);
   pAvatar->UpdateModel();
	return eResult;
} // Save(const char* pszFilename, CAvatar* pAvatar)

void CAvatarFileHANIM::WriteBodyPart(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream) const {
   const SBodyPart* pCurrentPart = pAvatar->BodyParts() + bpCurrentPart;
   if (bpCurrentPart > r_pinky3) {
   }
   else {
	   using namespace std;
	   // Create the new vertex and face lists
	   vector<int> vNewVertexList;
	   vector<STriFace> vFaceList;
	   // Copy segment's vertex list into new vector
	   int iNumVertices = pCurrentPart->m_iNumVertices;
	   for (int iVertex=0; iVertex<iNumVertices; iVertex++) {
		   vNewVertexList.push_back(pCurrentPart->m_piVertices[iVertex]);
	   }
	   int* pSegmentVertices = pCurrentPart->m_piVertices;
      const SPoint3D *pVertices = pAvatar->Vertices();
	   const STriFace* pFaces = pAvatar->Faces();
	   // For each face
	   int piVertexIndices[3];
	   for (int iFace=0; iFace<pAvatar->NumFaces(); iFace++) {
		   piVertexIndices[0] = -1;
		   piVertexIndices[1] = -1;
		   piVertexIndices[2] = -1;
		   // Calculate offsets of vertices in the segment vertex list.
		   for (int i=0; i<3; i++) {
			   for (int j=0; j<iNumVertices && piVertexIndices[i]==-1; j++) {
				   if (pSegmentVertices[j] == pFaces[iFace].m_sVertices[i].m_iVertex) {
					   piVertexIndices[i] = j;
				   }
			   }
		   }
		   // If the face has one or more vertices in the vertex list
		   if ((piVertexIndices[0] >= 0) || (piVertexIndices[1] >= 0) || (piVertexIndices[2] >= 0)) {
			   // Find missing vertices in the vertex list.
			   for (int i=0; i<3; i++) {
				   for (int j=0; j<vNewVertexList.size() && piVertexIndices[i]==-1; j++) {
					   if (vNewVertexList[j] == pFaces[iFace].m_sVertices[i].m_iVertex) {
						   piVertexIndices[i] = j;
					   }
				   }
			   }
			   // If there are still any missing, add them to the new list and fill in the -1s.
			   for (i=0; i<3; i++) {
				   if (piVertexIndices[i] == -1) {
					   vNewVertexList.push_back(pFaces[iFace].m_sVertices[i].m_iVertex);
					   piVertexIndices[i] = vNewVertexList.size()-1;
				   }
			   }
			   // Add the face to the face list.
			   STriFace sNewFace;
			   sNewFace.m_sVertices[0].m_iVertex = piVertexIndices[0];
			   sNewFace.m_sVertices[1].m_iVertex = piVertexIndices[1];
			   sNewFace.m_sVertices[2].m_iVertex = piVertexIndices[2];
            sNewFace.m_iTextureNumber = pFaces[iFace].m_iTextureNumber;
            sNewFace.m_sVertices[0].m_sTexCoord.dU = pFaces[iFace].m_sVertices[0].m_sTexCoord.dU;
            sNewFace.m_sVertices[0].m_sTexCoord.dV = pFaces[iFace].m_sVertices[0].m_sTexCoord.dV;
            sNewFace.m_sVertices[1].m_sTexCoord.dU = pFaces[iFace].m_sVertices[1].m_sTexCoord.dU;
            sNewFace.m_sVertices[1].m_sTexCoord.dV = pFaces[iFace].m_sVertices[1].m_sTexCoord.dV;
            sNewFace.m_sVertices[2].m_sTexCoord.dU = pFaces[iFace].m_sVertices[2].m_sTexCoord.dU;
            sNewFace.m_sVertices[2].m_sTexCoord.dV = pFaces[iFace].m_sVertices[2].m_sTexCoord.dV;
            vFaceList.push_back(sNewFace);
		   }
	   }	

      // Write output
      osOutputStream << "DEF hanim_" << pJointNames[bpCurrentPart] << " Joint {\n";
	   osOutputStream << "   name \""<< pJointNames[bpCurrentPart] << "\"\n";
	   osOutputStream << "   center "<< pCurrentPart->m_pntDefaultCentre.m_dComponents[0] << " " << pCurrentPart->m_pntDefaultCentre.m_dComponents[1] << " " << pCurrentPart->m_pntDefaultCentre.m_dComponents[2] << "\n";
	   // If we are writing the current pose, output the current joint rotation
		char* pszRotation = pCurrentPart->m_rotCurrentRotation.ToString(2);
		osOutputStream << "   rotation "<< pszRotation << "\n";
		free(pszRotation);
	   osOutputStream << "   children [\n";
	   osOutputStream << "      DEF hanim_" << pSegmentNames[bpCurrentPart] << " Segment {\n";
	   osOutputStream << "         name \""<< pSegmentNames[bpCurrentPart] << "\"\n";
	   osOutputStream << "         children [\n";
	   osOutputStream << "            Shape {\n";
	   osOutputStream << "               appearance Appearance {\n";
	   //osOutputStream << "                  material Material {\n";
	   //osOutputStream << "                     diffuseColor 1 1 1\n";
	   //osOutputStream << "                  }\n";
	   osOutputStream << "                  texture ImageTexture {\n";
	   osOutputStream << "                     url \"" << m_pszBasename << ".jpg\"" << '\n';
	   osOutputStream << "                     repeatS FALSE\n";
	   osOutputStream << "                     repeatT FALSE\n";
	   osOutputStream << "                  }\n";
	   osOutputStream << "               }\n";
	   osOutputStream << "               geometry IndexedFaceSet {\n";
	   osOutputStream << "                  coord Coordinate {\n";
	   osOutputStream << "                     point [\n";
	   // Write vertex list
	   for (int i=0; i<vNewVertexList.size(); i++) {
		   osOutputStream << "                        " << pVertices[vNewVertexList[i]].m_dComponents[0] << ' ' << pVertices[vNewVertexList[i]].m_dComponents[1] << ' ' << pVertices[vNewVertexList[i]].m_dComponents[2] << '\n';
	   }
	   // Write VRML stuff
	   osOutputStream << "                     ]\n";
	   osOutputStream << "                  }\n";
	   osOutputStream << "                  coordIndex [\n";
	   // Write faces list
	   for (i=0; i<vFaceList.size(); i++) {
		   osOutputStream << "                     " << vFaceList[i].m_sVertices[0].m_iVertex << ' ' << vFaceList[i].m_sVertices[1].m_iVertex << ' ' << vFaceList[i].m_sVertices[2].m_iVertex << " -1\n";
	   }
	   // Write VRML stuff
	   osOutputStream << "                  ]\n";
	   osOutputStream << "                  texCoord TextureCoordinate {\n";
	   osOutputStream << "                     point [\n";
	   // Write texture coordinates, translating as we go
	   for (i=0; i<vFaceList.size(); i++) {
         int t = vFaceList[i].m_iTextureNumber;
         // Work out base positions and sizes in pixels
         int iBaseV = m_pMinima[t];
         int iHeight = m_pMaxima[t] - m_pMinima[t]+1;
         osOutputStream << "                     ";
         for (int j=0; j<3; j++) {
            double dTexCoordU = vFaceList[i].m_sVertices[j].m_sTexCoord.dU;
            double dTexCoordV = 1-(iBaseV + ((1-vFaceList[i].m_sVertices[j].m_sTexCoord.dV)*iHeight)) / m_iTotalHeight;
            osOutputStream << dTexCoordU << ' ' << dTexCoordV;
            if (j==2) osOutputStream << '\n';
            else osOutputStream << ", ";
         }
	   }
	   // Write VRML stuff
	   osOutputStream << "                     ]\n";
	   osOutputStream << "                  }\n";
	   osOutputStream << "                  texCoordIndex [\n";
	   // Write texture coordinate index list
	   for (i=0; i<vFaceList.size(); i++) {
		   osOutputStream << "                     " << (i*3) << ' ' << (i*3)+1 << ' ' << (i*3)+2 << " -1\n";
	   }
	   // Finish VRML stuff
	   osOutputStream << "                  ]\n";
	   osOutputStream << "               }\n";
	   osOutputStream << "            }\n";
	   osOutputStream << "         ]\n";
	   osOutputStream << "      } #" << pSegmentNames[bpCurrentPart] << "\n";
	   int iChild = 0;
	   bool bNotDone = true;
	   while (bNotDone && iChild<3) {
		   if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			   WriteBodyPart(pCurrentPart->m_bpChildren[iChild],pAvatar,osOutputStream);
			   iChild++;
		   }
		   else bNotDone = false;
	   }
	   osOutputStream << "   ]\n";
	   osOutputStream << "} #" << pJointNames[bpCurrentPart] << "\n";
   }
	return;
} //WriteBodyPart(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream)

void CAvatarFileHANIM::WriteUSEStatement(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream, bool bSegment) const {
   const SBodyPart* pCurrentPart = pAvatar->BodyParts() + bpCurrentPart;
   if (bpCurrentPart > r_pinky3) {
   }
   else {
	   // Write this body part
	   osOutputStream << "      USE hanim_";
	   if (bSegment) osOutputStream << pSegmentNames[bpCurrentPart];
	   else osOutputStream << pJointNames[bpCurrentPart];
	   osOutputStream << "\n";
	   // Write children
	   int iChild = 0;
	   bool bNotDone = true;
	   while (bNotDone && iChild<3) {
		   if (pCurrentPart->m_bpChildren[iChild] != unknown) {
			   WriteUSEStatement(pCurrentPart->m_bpChildren[iChild],pAvatar,osOutputStream,bSegment);
			   iChild++;
		   }
		   else bNotDone = false;
	   }
   }
	return;
} //WriteUSEStatement(BodyPart bpCurrentPart, CAvatar* pAvatar, ofstream& osOutputStream, bool bSegment)
