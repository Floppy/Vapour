//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CGI.h - 12/12/2000 - Warren Moore
//	  CGI utility class
//
// $Id: CGI.h,v 1.1 2000/12/14 14:11:38 warren Exp $
//

#ifndef _VAL_CGI_
#define _VAL_CGI_

#pragma once

//#===--- Defines

#define CGI_MAX_PAIRS					20
#define CGI_MAX_REFERRERS				20

// Request methods
#define CGI_M_UNKNOWN					0x00000000
#define CGI_M_NONE						0x00000001
#define CGI_M_GET							0x00000002
#define CGI_M_POST						0x00000004

#ifndef NULL
#define NULL								0x00000000
#endif // NULL

/////////
// CCGI

class CCGI {
	//#===--- External Functions
public:
	//#===--- Constructor/Destructor
	CCGI();
	~CCGI();

	//#===--- Initialisation
	// Gets environment variables and parses values
	// NOTE: Must be called before values are queried
	void Init();
	// Gets environment variables and parses values, using command line arguments if available
	void Init(int iArgc, char **ppcArgv);

	//#===--- Input Functions
	void AddReferrer(const char *pcName);

	//#===--- Query Functions
	// Returns whether the request method is begin used (OR valid request method defines)
	bool CorrectRequest(unsigned int uMethod) const;
	// Returns true if the passed referrer is valid
	bool ValidReferrer() const;

	// Returns a pointer to the respective value string, NULL if ID not found
	const char *GetValue(const char *pcID) const;

	// Returns true if application called from command line
	bool Debug() const;


	//#===--- Internal Structures
protected:
	// Used to store ID/Value data pair strings
	struct SPair {
		char *m_pcID;
		char *m_pcValue;

		SPair() {
			m_pcID = NULL;
			m_pcValue = NULL;
		}
	};

	//#===--- Internal Functions
protected:
	// Sets the request type
	void GetRequestMethod();
	// Parses the query string environment variable, setting data pairs
	void ParseQuery();
	// Parses the command line arguments, setting data pairs
	void ParseCommandLine(int iArgc, char **ppcArgv);
	// Returns a string containing a two digit hex number to an ASCII character
	char GetChar(char *pcIn);
	// Find %xx hex numbers in the input string and convert them to ASCII characters
	void DecodeHex(char *pcIn);

	//#===--- Internal Data
protected:

	unsigned int m_uRequestType;				// Request method
	SPair m_pcData[CGI_MAX_PAIRS];			// Array of available data pairs
	char *m_pcValidRef[CGI_MAX_REFERRERS];	// List of strings containing valid referrers
	bool m_bDebug;									// Debug operation indicator
};

#endif // _VAL_CGI_
