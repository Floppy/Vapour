//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// DList.h - 28/05/2000 - James Smith
//	Doubly Linked list class header
//
// $Id: DList.h,v 1.6 2000/11/29 21:20:28 james Exp $
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

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Default Constructor
   CDList();

   // Constructor - precreates iNum elements in the list
   CDList(int iNum);

   ///////////////////////////////////////////////////////////////////////
   // Adding Items ///////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Adds an item to the front of the list - returns true if successful
   bool AddFront(const T& newData);

   // Adds an item to the back of the list - returns true if successful
   bool AddBack(const T& newData);

   ///////////////////////////////////////////////////////////////////////
   // Removing Items ///////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Removes the item at the front of the list
   // Returns the data that was stored.
   T RemoveFront(void);

   // Removes the item at the back of the list
   // Returns the data that was stored.
   T RemoveBack(void);

   // Clear the list
   void Clear(void);

   ///////////////////////////////////////////////////////////////////////
   // Access /////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Gives the length of the list
   int Length(void) const {return m_iLength;}

   // Access to specific elements
   T& operator[](unsigned int iIndex) const;

   // The data in the first element in the list
   T& First(void) const {return m_pFront->m_tData;}

   // The data in the last element in the list
   T& Last(void) const {return m_pBack->m_tData;}

   // Convert to array
   T* Convert(int& iLength);

   // Destructor
   virtual ~CDList();

};

template<class T>
CDList<T>::CDList() :
   m_pFront(NULL),
   m_pBack(NULL),
   m_iLength(0)
{
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
bool CDList<T>::AddFront(const T& newData) {
   bool bReturn = false;
   CDListItem<T>* pNewItem;
   NEWBEGIN
   pNewItem = new CDListItem<T>(newData);
   NEWEND("CList<T>::AddFront - new item allocation")
   if (pNewItem != NULL) {
      bReturn = true;
      if (m_iLength == 0) m_pBack = pNewItem;
      else m_pFront->m_pPrev = pNewItem;
      pNewItem->m_pPrev = NULL;
      pNewItem->m_pNext = m_pFront;
      m_pFront = pNewItem;
      m_iLength++;
   }
   return bReturn;
} //AddFront(const T& newData)

template<class T>
bool CDList<T>::AddBack(const T& newData) {
   bool bReturn = false;
   CDListItem<T>* pNewItem;
   NEWBEGIN
   pNewItem = new CDListItem<T>(newData);
   NEWEND("CList<T>::AddBack - new item allocation")
   if (pNewItem != NULL) {
      bReturn = true;
      if (m_iLength == 0) m_pFront = pNewItem;
      else m_pBack->m_pNext = pNewItem;
      pNewItem->m_pPrev = m_pBack;
      pNewItem->m_pNext = NULL;
      m_pBack = pNewItem;
      m_iLength++;
   }
   return bReturn;
} //AddBack(const T& newData)

template<class T>
T CDList<T>::RemoveFront(void) {
   // If the list is empty, do nothing
   if (m_iLength == 0) return T(0);
   // Otherwise, remove the item from the front of the list
   CDListItem<T>* pOldItem;
   pOldItem = m_pFront;
   m_pFront = m_pFront->m_pNext;
   if (m_pFront != NULL) m_pFront->m_pPrev = NULL;
   m_iLength--;
   // Delete the old item, and we're done!
   T oData(pOldItem->m_tData);
   delete pOldItem;
   return oData;
} //RemoveFront(void)

template<class T>
T CDList<T>::RemoveBack(void) {
   // If the list is empty, do nothing
   if (m_iLength == 0) return T(0);   
   // Otherwise, remove the item from the back of the list
   CDListItem<T>* pOldItem;
   pOldItem = m_pBack;
   m_pBack = m_pBack->m_pPrev;
   if (m_pBack != NULL) m_pBack->m_pNext = NULL;
   m_iLength--;
   // Delete the old item, and we're done!
   T oData(pOldItem->m_tData);
   delete pOldItem;
   return oData;
} //RemoveBack(void)

template<class T> 
T& CDList<T>::operator[](unsigned int iIndex) const {
   CDListItem<T>* pItem = m_pFront;
   if (iIndex < m_iLength) {
      for (int i=0; i<iIndex; i++) {
         pItem = pItem->m_pNext;
         ASSERT(pItem);
      }
      return pItem->m_tData;
   }
   else return m_pBack->m_tData;
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
