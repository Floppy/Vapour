//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileProxy.h - 04/03/2000 - James Smith
//	Avatar import/export speciality store proxy header
//
// $Id: AvatarFileProxy.h,v 1.3 2000/11/21 16:40:39 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILEPROXY_
#define _VAL_AVATARFILEPROXY_

#include <stdio.h>

#include "AvatarFile.h"
#include "AvatarFileStore.h"

//////////////////////////
// External Dependencies

extern CAvatarFileStore g_oAvatarFileStore;

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

////////////////////////
// CAvatarFileProxyBase

class CAvatarFileProxyBase {
public:
	CAvatarFileProxyBase();
	virtual CAvatarFile* CreateObject() const = 0;

	virtual float GetVersion() const = 0;
	virtual const char* GetTitle() const = 0;
	virtual const char* GetExtension() const = 0;
	virtual bool CanLoadFile() const = 0;
	virtual bool CanLoadStream() const = 0;
	virtual bool CanSaveFile() const = 0;
	virtual bool CanSaveStream() const = 0;

   // Less-than operator - use for sorting proxies into alphabetical order.
   virtual bool operator<(const CAvatarFileProxyBase& other) const {
      // Compare titles
      int iTitleResult = strcmp(GetTitle(),other.GetTitle());
      // If different, return title1 < title2
      if (iTitleResult != 0) return iTitleResult < 0 ? true : false;
      // If not, return ext1 < ext2
      else return strcmp(GetExtension(),other.GetExtension()) < 0 ? true : false;
   }
   
};

////////////////////////////
// CAvatarFileProxy Template

template <class T>
class DLL CAvatarFileProxy : public CAvatarFileProxyBase {
public:
	CAvatarFile* CreateObject() const { 
		return new T;
	}
	virtual float GetVersion() const {
		return T::GetVersion();
	}
	virtual const char* GetTitle() const {
		return T::GetTitle();
	}
	virtual const char* GetExtension() const {
		return T::GetExtension();
	}
	virtual bool CanLoadFile() const {
		return T::CanLoadFile();
	}
	virtual bool CanLoadStream() const {
		return T::CanLoadStream();
	}
	virtual bool CanSaveFile() const {
		return T::CanSaveFile();
	}
	virtual bool CanSaveStream() const {
		return T::CanSaveStream();
	}
};

/////////////////////
// Inline Functions

inline CAvatarFileProxyBase::CAvatarFileProxyBase() {
	g_oAvatarFileStore.Register(this);
} // Constructor

#endif // _VAL_AVATARFILEPROXY_
