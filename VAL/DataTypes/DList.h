//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// DList.h - 28/05/2000 - James Smith
//	Doubly Linked list class header
//
// $Id: DList.h,v 1.3 2000/10/06 13:02:16 waz Exp $
//

#ifndef _VAL_LIST_
#define _VAL_LIST_

#pragma once

template<class T>
class CDList {

private:

   // Private data type for the list items
   template<class T> 
   class CDListItem {
   public:
      T m_tData;
      CDListItem<T>* m_pNext;
      CDListItem<T>* m_pPrev;
      CDListItem(const T& tNewData) {m_tData = tNewData;}
   };

   // Item pointers
   CDListItem<T>* m_pFront;
   CDListItem<T>* m_pBack;
   
   // How many items in list?
   int m_iLength;

public:

   // Range Exception - apparently this is a good idea...
   class CDListRangeException {};
   
   // Default Constructor
   CDList();

   // Constructor - precreates iNum elements in the list
   CDList(int iNum);

   // Gives the length of the list
   int Length(void) const {return m_iLength;}

   // Adds an item to the back of the list
   int AddBack(const T& newData);

   // Access to elements
   T& operator[](unsigned int iIndex) const throw(CDListRangeException);

   // Convert to array
   T* Convert(int& iLength);

   // Clear the list
   void Clear(void);

   // Destructor
   virtual ~CDList();

};

template<class T>
CDList<T>::CDList() {
   m_pFront = NULL;
   m_pBack = NULL;
   m_iLength = 0;
   return;
} //CDList()

template<class T>
CDList<T>::CDList(int iNum) {
   NEWBEGIN
   CDListItem<T>* pItems = new CDListItem<T>[iNum];
   NEWEND("CList<T>::CList(int iNum) - memory allocation failed!");
   if (pItems == NULL) {
      m_pFront = NULL;
      m_pBack = NULL;
      m_iLength = 0;
   }
   else {
      for (int i=1; i<iNum; i++) {
         pItems[i].m_pPrev = &(pItems[i-1]);
         pItems[i-1].m_pNext = &(pItems[i]);
      }
      m_pFront = pItems;
      m_pFront->m_Prev = NULL;      
      m_pBack = &pItems[iNum-1];
      m_pBack->m_pNext = NULL;
      m_iLength = iNum;
   }
   return;
} //CDList(int iNum)

template<class T>
int CDList<T>::AddBack(const T& newData) {
   int iRetVal = 0;
   CDListItem<T>* pNewItem;
   NEWBEGIN
   pNewItem = new CDListItem<T>(newData);
   NEWEND("CList<T>::AddBack - new item allocation")
   if (pNewItem != NULL) {
      iRetVal = 1;
      if (m_iLength == 0) m_pFront = pNewItem;
      else m_pBack->m_pNext = pNewItem;
      pNewItem->m_pPrev = m_pBack;
      pNewItem->m_pNext = NULL;
      m_pBack = pNewItem;
      m_iLength++;
   }
   return iRetVal;
} //AddBack(const T& newData)

template<class T> 
T& CDList<T>::operator[](unsigned int iIndex) const throw(CDListRangeException) {
   CDListItem<T>* pItem = m_pFront;
   if (iIndex < m_iLength) {
      for (int i=0; i<iIndex; i++) {
         pItem = pItem->m_pNext;
         ASSERT(pItem);
      }
      return pItem->m_tData;
   }
   else throw CDListRangeException();
} //operator[](unsigned int iIndex)

template<class T> 
T* CDList<T>::Convert(int& iLength) {
   iLength = m_iLength;
   T* pArray = NULL;
   if (iLength != 0) {
      NEWBEGIN
      pArray = new T[iLength];
      NEWEND("CDList<T>::Convert - array allocation");
      CDListItem<T>* pItem = m_pFront;
      for (int i=0; i<iLength; i++) {
         ASSERT(pItem);
         pArray[i] = pItem->m_tData;
         pItem = pItem->m_pNext;
      }
   }
   return pArray;
} //Convert(int& iLength)

template<class T> 
void CDList<T>::Clear(void) {
   while (m_pFront != NULL) {
      CDListItem<T>* pTemp = m_pFront->m_pNext;
      delete m_pFront;
      m_pFront = pTemp;
   }
   m_pFront = NULL;
   m_pBack = NULL;
   m_iLength = 0;
   return;
} //Clear(void)

template<class T> 
CDList<T>::~CDList() {
   while (m_pFront != NULL) {
      CDListItem<T>* pTemp = m_pFront->m_pNext;
      delete m_pFront;
      m_pFront = pTemp;
   }
   return;
} //~CDList()

#endif //_VAL_LIST_
