// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fde/fde_iterator.h"

#include "xfa/fgas/crt/fgas_utils.h"

CFDE_VisualSetIterator::CFDE_VisualSetIterator() : m_dwFilter(0) {}

CFDE_VisualSetIterator::~CFDE_VisualSetIterator() {}

bool CFDE_VisualSetIterator::AttachCanvas(IFDE_CanvasSet* pCanvas) {
  ASSERT(pCanvas);
  m_CanvasStack = std::stack<FDE_CANVASITEM>();

  FDE_CANVASITEM canvas;
  canvas.hCanvas = nullptr;
  canvas.pCanvas = pCanvas;
  canvas.hPos = pCanvas->GetFirstPosition();
  if (!canvas.hPos)
    return false;

  m_CanvasStack.push(canvas);
  return true;
}

bool CFDE_VisualSetIterator::FilterObjects(uint32_t dwObjects) {
  if (m_CanvasStack.empty())
    return false;

  while (m_CanvasStack.size() > 1)
    m_CanvasStack.pop();

  m_dwFilter = dwObjects;

  FDE_CANVASITEM* pCanvas = &m_CanvasStack.top();
  ASSERT(pCanvas && pCanvas->pCanvas);

  pCanvas->hPos = pCanvas->pCanvas->GetFirstPosition();
  return !!pCanvas->hPos;
}

void CFDE_VisualSetIterator::Reset() {
  FilterObjects(m_dwFilter);
}

FDE_TEXTEDITPIECE* CFDE_VisualSetIterator::GetNext(
    IFDE_VisualSet*& pVisualSet,
    FDE_TEXTEDITPIECE** phCanvasObj,
    IFDE_CanvasSet** ppCanvasSet) {
  while (!m_CanvasStack.empty()) {
    FDE_CANVASITEM* pCanvas = &m_CanvasStack.top();
    if (!pCanvas->hPos) {
      if (m_CanvasStack.size() == 1)
        break;

      m_CanvasStack.pop();
      continue;
    }
    do {
      FDE_TEXTEDITPIECE* pObj =
          pCanvas->pCanvas->GetNext(pCanvas->hPos, pVisualSet);
      ASSERT(pObj);

      FDE_VISUALOBJTYPE eType = pVisualSet->GetType();
      if (eType == FDE_VISUALOBJ_Canvas) {
        FDE_CANVASITEM canvas;
        canvas.hCanvas = pObj;
        canvas.pCanvas = static_cast<IFDE_CanvasSet*>(pVisualSet);
        canvas.hPos = canvas.pCanvas->GetFirstPosition();
        m_CanvasStack.push(canvas);
        break;
      }
      uint32_t dwObj = (uint32_t)eType;
      if ((m_dwFilter & dwObj) != 0) {
        if (ppCanvasSet)
          *ppCanvasSet = pCanvas->pCanvas;
        if (phCanvasObj)
          *phCanvasObj = pCanvas->hCanvas;
        return pObj;
      }
    } while (pCanvas->hPos);
  }
  if (ppCanvasSet)
    *ppCanvasSet = nullptr;
  if (phCanvasObj)
    *phCanvasObj = nullptr;

  pVisualSet = nullptr;
  return nullptr;
}
