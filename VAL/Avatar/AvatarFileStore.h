//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileStore.h - 16/03/2000 - James Smith
//	Avatar import/export filter speciality store header
//
// $Id: AvatarFileStore.h,v 1.3 2000/11/21 16:40:13 waz Exp $
//

#pragma once

#ifndef _VAL_AVATARFILESTORE_
#define _VAL_AVATARFILESTORE_

#include <stdio.h>
#include <vector>

#include "AvatarFile.h"

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

// Predeclare CAvatarFileProxyBase - do not include here
class CAvatarFileProxyBase;

// Vector and Iterator Typedef
typedef std::vector<const CAvatarFileProxyBase*> AvatarFileVector;
typedef std::vector<const CAvatarFileProxyBase*>::iterator AvatarFileIterator;

// Predicate function for comparing filter names. Returns true if pFilterOne < pFilterTwo.
bool CompareFilters(const CAvatarFileProxyBase* pFilterOne ,const CAvatarFileProxyBase* pFilterTwo);

////////////////////
// CAvatarFileStore

class DLL CAvatarFileStore {

private:
	void Init();

	AvatarFileVector* m_pAvatarFiles;

   static const double m_dMaxFilterVersion;

   CAvatarFileStore* m_pParentStore;

public:
	CAvatarFileStore() {m_pParentStore=NULL;};
	~CAvatarFileStore();

	void Register(const CAvatarFileProxyBase* pProxy);

   void Override(CAvatarFileStore &oAvatarFileStore);

	bool CheckForExtension(const char* pszExtension);
	CAvatarFile* CreateByExtension(const char* pszExtension);	

	int GetCount() const;
	const CAvatarFileProxyBase* GetAt(int i) const;

};

#endif // _VAL_AVATARFILESTORE_
