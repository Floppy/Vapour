//#=======---
// Counter
//--------
// Web counter
// Copyright 2000 Vapour Technology Ltd.
// 
// counter.cpp - 06/12/2000 - Warren Moore
//	  Main application
//
// $Id: counter.cpp,v 1.1 2000/12/17 12:50:38 warren Exp $
//

// Common includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "CGI.h"
#include "gd/gd.h"

// Windows includes
#ifdef WIN32
#include <io.h>
#else
// Unix includes
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif // WIN32

// Current version string
#define COUNTER_VERSION			"v1.0"
// Default number of digits to use (can be overidden with digits=<num_of_digits>)
#define COUNTER_DIGITS			6
// Number of time to try locking the count data file
#define COUNTER_LOCK_TRIES		6

// Default string array size
#define STR_SIZE					1024

// Different sets of parameters for Win and Unix builds
// NOTE: You only need to change COUNTER_ROOT for your build type
// NOTE: COUNTER_ZERO and COUNTER_PIC are string formatting defines and don't need to be changed
#ifdef WIN32
#define COUNTER_ROOT				"D:\\Vapour\\Dev\\src\\Vapour\\Counter\\Data\\"
#define COUNTER_ZERO				"%s\\0.gif"
#define COUNTER_PIC				"%s\\%c.gif"			
#else
#define COUNTER_ROOT				"/www/vapourisp/counter/"
#define COUNTER_ZERO				"%s/0.gif"
#define COUNTER_PIC				"%s/%c.gif"		
#endif // WIN32

// List of valid referrers - these are pumped into the CCGI object 
// NOTE: Change these to match your valid referrers. MUST end with an empty string
const char g_pcReferrers[][STR_SIZE] = {
	"http://www.vapourtech.com/",
	"http://www.host.vapourtech.com/",
	"http://www.evilhq.com/",
	"http://www.waz.org.uk/",
	"http://www.elsepth.org.uk/",
	"http://www.w-e-s.co.uk/",
	"http://www.floppy.org.uk/",
	"http://www.smurfette.org.uk/",
	""
};

//#===--- Function Declarations

// Copies the incremented counter string into pcOut, empty if error
void GetCount(const char *pcCountName, char *pcCount);
// Generates the count image from the count string, returns true if all ok
bool CreateImage(const char *pcCount, const char *pcFont, int iDigits);
// Generates an error message or image
void Error(const char *pcText = "");

//#===--- Globals

// The global cgi utility object
CCGI g_oCGI;

//#===--- Function Definitions

int main(int argc, char **argv) {
	// Start 'er up
	g_oCGI.Init(argc, argv);
	// Write banner, if in debug mode
	if (g_oCGI.CorrectRequest(CGI_M_NONE)) {
		printf("Counter %s (%s)\n", COUNTER_VERSION, __DATE__);
		printf("Copyright 2000 Vapour Technology Ltd.\n\n");
	}
	// Add the valid referrers here
	int iCount = 0;
	while (g_pcReferrers[iCount][0] != '\0') {
		g_oCGI.AddReferrer(g_pcReferrers[iCount++]);
	}
	// Check to see we have a valid referrer
	if (!g_oCGI.ValidReferrer()) {
		Error("Invalid referrer");
		return -1;
	}
	// Build the count data file
	char pcCountName[STR_SIZE] = COUNTER_ROOT;
	const char *pcCountID = g_oCGI.GetValue("id");
	if (pcCountID) {
		strcat(pcCountName, pcCountID);
	}
	else {
		Error("No ID specified");
		return -1;
	}
	// Load the data file and returns the count string
	char pcCount[STR_SIZE] = "";
	GetCount(pcCountName, pcCount);
	if (strlen(pcCount) == 0) {
		Error("Unable to retrieve count data");
		return -1;
	}
	// Check the amount of digits
	int iDigits = COUNTER_DIGITS;
	const char *pcDigits = g_oCGI.GetValue("digits");
	if (pcDigits)
		iDigits = atoi(pcDigits);
	// Get the font name
	char pcFontPath[STR_SIZE] = COUNTER_ROOT;
	const char *pcFontName = g_oCGI.GetValue("font");
	if (pcFontName)
		strcat(pcFontPath, pcFontName);
	else
		strcat(pcFontPath, "normal");
	// Write then, create the image
	if (!CreateImage(pcCount, pcFontPath, iDigits)) {
		Error("Unable to create counter image");
		return -1;
	}

	return 0;
}

#ifdef WIN32
void GetCount(const char *pcCountName, char *pcCount) {
	pcCount[0] = '\0';
	// Try to open the file
	int iFile = _open(pcCountName, _O_RDWR);
	if (iFile == -1)
		return;
	// Read in the data
	char pcData[STR_SIZE];
	memset(pcData, 0, STR_SIZE);
	int iLen = _read(iFile, pcData, STR_SIZE);
	// Format out the data
	if (iLen > 0) {
		char pcFormat[STR_SIZE] = "";
		char *pcIn = pcData;
		char *pcOut = pcFormat;
		unsigned int uValue = 0;
		while (*pcIn && ((*pcIn < '1') || (*pcIn > '9')))
			pcIn++;
		while (*pcIn && (*pcIn >= '0') && (*pcIn <= '9')) {
			uValue *= 10;
			uValue += *pcIn - '0';
			*pcOut++ = *pcIn++;
		}
		// Write out the new value
		uValue++;
		sprintf(pcFormat, "%u", uValue);
		if (_lseek(iFile, 0, SEEK_SET) != -1) {
			_write(iFile, pcFormat, strlen(pcFormat));
		}
		// Copy out the string
		strcpy(pcCount, pcFormat);
	}
	// Close the file
	_close(iFile);

	return;
} // GetCount

#else

void GetCount(const char *pcCountName, char *pcCount) {
	// Try to open the file
	int iFile = open(pcCountName, O_CREAT | O_RDWR);
	if (iFile == -1)
		return ;
	// Lock the file
	int iCount = 0;
	while ((flock(iFile, LOCK_EX) == -1) && (iCount++ < COUNTER_LOCK_TRIES)) {}
	if (iCount == COUNTER_LOCK_TRIES) {
		close(iFile);
		return;
	}
	// Read in the data
	char pcData[STR_SIZE];
	memset(pcData, 0, STR_SIZE);
	int iLen = read(iFile, pcData, STR_SIZE);
	// Format out the data
	if (iLen > 0) {
		char pcFormat[STR_SIZE] = "";
		char *pcIn = pcData;
		char *pcOut = pcFormat;
		unsigned int uValue = 0;
		while (*pcIn && ((*pcIn < '1') || (*pcIn > '9')))
			pcIn++;
		while (*pcIn && (*pcIn >= '0') && (*pcIn <= '9')) {
			uValue *= 10;
			uValue += *pcIn - '0';
			*pcOut++ = *pcIn++;
		}
		// Write out the new value
		uValue++;
		sprintf(pcFormat, "%u", uValue);
		if (lseek(iFile, 0, SEEK_SET) != -1) {
			write(iFile, pcFormat, strlen(pcFormat));
		}
		// Copy out the string
		strcpy(pcCount, pcFormat);
	}
	// Unlock and close the file
	flock(iFile, LOCK_UN);
	close(iFile);

	return;
} // GetCount
#endif // WIN32

// Generates the count image from the count string, returns true if all ok
bool CreateImage(const char *pcCount, const char *pcFont, int iDigits) {
	// Set the spare value 
	int iCountLen = strlen(pcCount);
	if (iDigits < iCountLen)
		iDigits = iCountLen;
	int iSpare = 0;
	if (iDigits > iCountLen)
		iSpare = iDigits - iCountLen;
	// Loop vars
	char pcDigitName[STR_SIZE] = "";
	gdImagePtr pImg = NULL;
	FILE *pPic = NULL;
	// Load the zero digit image
	sprintf(pcDigitName, COUNTER_ZERO, pcFont);
	pPic = fopen(pcDigitName, "rb+");
	if (pPic == NULL)
		return false;
	pImg = gdImageCreateFromGif(pPic);
	fclose(pPic);
	if (pImg == NULL)
		return false;
	// Create the output image
	gdImagePtr pOutImg = NULL;
	pOutImg = gdImageCreate(iDigits * pImg->sx, pImg->sy);
	if (pOutImg == NULL) {
		gdImageDestroy(pImg);
		return false;
	}
	// Loop through each leading zero
	int iPos = 0;
	while (iSpare-- > 0) {
		// paste in the image
		gdImageCopy(pOutImg, pImg, iPos, 0, 0, 0, pImg->sx, pImg->sy);
		iPos += pImg->sx;
	}
	// Delete the zero image
	gdImageDestroy(pImg);
	// Loop through each counter character
	const char *pcPos = pcCount;
	while (*pcPos != '\0') {
		sprintf(pcDigitName, COUNTER_PIC, pcFont, *pcPos);
		// Load the image
		pPic = fopen(pcDigitName, "rb+");
		if (pPic == NULL) {
			gdImageDestroy(pOutImg);
			return false;
		}
		pImg = gdImageCreateFromGif(pPic);
		if (pImg == NULL) {
			gdImageDestroy(pOutImg);
			return false;
		}
		// Paste in the image
		gdImageCopy(pOutImg, pImg, iPos, 0, 0, 0, pImg->sx, pImg->sy);
		iPos += pImg->sx;
		// Delete the image
		gdImageDestroy(pImg);
		// Update the position counter
		pcPos++;
	}
	// Write out the output image
	if (g_oCGI.Debug()) {
		char pcGIFName[STR_SIZE] = COUNTER_ROOT;
		strcat(pcGIFName, "counter.gif");
		FILE *pGIFFile = NULL;
		pGIFFile = fopen(pcGIFName, "wb");
		if (pGIFFile) {
			gdImageGif(pOutImg, pGIFFile);
			fclose(pGIFFile);
		}
	}
	else {
		gdImageInterlace(pOutImg, 1);
#ifdef WIN32
		_setmode(_fileno(stdout), _O_BINARY);
#endif
		printf("Content-type: image/gif\n\n");
		gdImageGif(pOutImg, stdout);
	}
	// Destroy the image
	gdImageDestroy(pOutImg);

	return true;
} // CreateImage

// Prints the error string if in debug, otherwise generates an error image
void Error(const char *pcText) {
	if (g_oCGI.Debug()) {
		printf("Error: %s\n\n", pcText);
		printf("Usage: id=<counter_id> [font=<font_name>] [digits=<num_of_digits>]\n");
		printf("Note: counter.gif will be placed in %s\n", COUNTER_ROOT);
	}
	else {
		gdImagePtr pOutImg = NULL;
		pOutImg = gdImageCreate(20, 20);
		if (pOutImg) {
			int iBlack = gdImageColorAllocate(pOutImg, 0, 0, 0);
			int iRed = gdImageColorAllocate(pOutImg, 255, 0, 0);
			gdImageFill(pOutImg, 0, 0, iBlack);
			gdImageLine(pOutImg, 0, 0, 19, 19, iRed);
			gdImageLine(pOutImg, 19, 0, 0, 19, iRed);
			gdImageInterlace(pOutImg, 1);
#ifdef WIN32
			_setmode(_fileno(stdout), _O_BINARY);
#endif
			printf("Content-type: image/gif\n\n");
			gdImageGif(pOutImg, stdout);
		}
	}
} // Error

