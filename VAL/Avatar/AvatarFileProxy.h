//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileProxy.h - 04/03/2000 - James Smith
//	Avatar import/export speciality store proxy header
//
// $Id: AvatarFileProxy.h,v 1.2 2000/06/17 10:42:11 waz Exp $
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
#ifndef DLL_IMP_EXP
	#ifdef _EXP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllexport)
	#endif
	#ifdef _IMP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllimport)
	#endif
	#ifndef DLL_IMP_EXP
		#define DLL_IMP_EXP
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
};

////////////////////////////
// CAvatarFileProxy Template

template <class T>
class DLL_IMP_EXP CAvatarFileProxy : public CAvatarFileProxyBase {
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
