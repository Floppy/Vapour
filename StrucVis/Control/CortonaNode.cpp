//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaNode.cpp
// 10/03/2002 - Warren Moore
//
// $Id: CortonaNode.cpp,v 1.1 2002/03/21 14:32:07 vap-warren Exp $

#include "stdafx.h"
#include "CortonaBase.h"

#include "CortonaNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CCortonaNode

CCortonaNode::CCortonaNode(INodeObject *pNode) : m_pNode(pNode) {
}

CCortonaNode::~CCortonaNode() {
}

void CCortonaNode::Release() {
   // If we have a node, set it free
   if (m_pNode)
      m_pNode->Release();
}

