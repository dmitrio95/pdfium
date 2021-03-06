// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FWL_CFWL_LISTBOX_H_
#define XFA_FWL_CFWL_LISTBOX_H_

#include <memory>
#include <vector>

#include "xfa/fwl/cfwl_edit.h"
#include "xfa/fwl/cfwl_event.h"
#include "xfa/fwl/cfwl_listbox.h"
#include "xfa/fwl/cfwl_listitem.h"
#include "xfa/fwl/cfwl_widget.h"
#include "xfa/fwl/cfwl_widgetproperties.h"

#define FWL_STYLEEXT_LTB_MultiSelection (1L << 0)
#define FWL_STYLEEXT_LTB_LeftAlign (0L << 4)
#define FWL_STYLEEXT_LTB_CenterAlign (1L << 4)
#define FWL_STYLEEXT_LTB_RightAlign (2L << 4)
#define FWL_STYLEEXT_LTB_AlignMask (3L << 4)
#define FWL_STYLEEXT_LTB_ShowScrollBarFocus (1L << 10)
#define FWL_ITEMSTATE_LTB_Selected (1L << 0)
#define FWL_ITEMSTATE_LTB_Focused (1L << 1)

class CFWL_MessageKillFocus;
class CFWL_MessageMouse;
class CFWL_MessageMouseWheel;
class CFX_DIBitmap;

class CFWL_ListBox : public CFWL_Widget {
 public:
  explicit CFWL_ListBox(const CFWL_App* pApp,
                        std::unique_ptr<CFWL_WidgetProperties> properties,
                        CFWL_Widget* pOuter);
  ~CFWL_ListBox() override;

  // CFWL_Widget
  FWL_Type GetClassID() const override;
  void Update() override;
  FWL_WidgetHit HitTest(const CFX_PointF& point) override;
  void DrawWidget(CFX_Graphics* pGraphics, const CFX_Matrix* pMatrix) override;
  void SetThemeProvider(IFWL_ThemeProvider* pThemeProvider) override;
  void OnProcessMessage(CFWL_Message* pMessage) override;
  void OnProcessEvent(CFWL_Event* pEvent) override;
  void OnDrawWidget(CFX_Graphics* pGraphics,
                    const CFX_Matrix* pMatrix) override;

  int32_t CountItems(const CFWL_Widget* pWidget) const;
  CFWL_ListItem* GetItem(const CFWL_Widget* pWidget, int32_t nIndex) const;
  int32_t GetItemIndex(CFWL_Widget* pWidget, CFWL_ListItem* pItem);

  CFWL_ListItem* AddString(const CFX_WideStringC& wsAdd);
  void RemoveAt(int32_t iIndex);
  void DeleteString(CFWL_ListItem* pItem);
  void DeleteAll();

  int32_t CountSelItems();
  CFWL_ListItem* GetSelItem(int32_t nIndexSel);
  int32_t GetSelIndex(int32_t nIndex);
  void SetSelItem(CFWL_ListItem* hItem, bool bSelect);

  FX_FLOAT GetItemHeight() const { return m_fItemHeight; }
  FX_FLOAT CalcItemHeight();

 protected:
  CFWL_ListItem* GetListItem(CFWL_ListItem* hItem, uint32_t dwKeyCode);
  void SetSelection(CFWL_ListItem* hStart, CFWL_ListItem* hEnd, bool bSelected);
  CFWL_ListItem* GetItemAtPoint(const CFX_PointF& point);
  bool ScrollToVisible(CFWL_ListItem* hItem);
  void InitVerticalScrollBar();
  void InitHorizontalScrollBar();
  bool IsShowScrollBar(bool bVert);
  CFWL_ScrollBar* GetVertScrollBar() const { return m_pVertScrollBar.get(); }
  const CFX_RectF& GetRTClient() const { return m_rtClient; }

 private:
  void SetSelectionDirect(CFWL_ListItem* hItem, bool bSelect);
  bool IsMultiSelection() const;
  bool IsItemSelected(CFWL_ListItem* hItem);
  void ClearSelection();
  void SelectAll();
  CFWL_ListItem* GetFocusedItem();
  void SetFocusItem(CFWL_ListItem* hItem);
  void DrawBkground(CFX_Graphics* pGraphics,
                    IFWL_ThemeProvider* pTheme,
                    const CFX_Matrix* pMatrix);
  void DrawItems(CFX_Graphics* pGraphics,
                 IFWL_ThemeProvider* pTheme,
                 const CFX_Matrix* pMatrix);
  void DrawItem(CFX_Graphics* pGraphics,
                IFWL_ThemeProvider* pTheme,
                CFWL_ListItem* hItem,
                int32_t Index,
                const CFX_RectF& rtItem,
                const CFX_Matrix* pMatrix);
  void DrawStatic(CFX_Graphics* pGraphics, IFWL_ThemeProvider* pTheme);
  CFX_SizeF CalcSize(bool bAutoSize);
  void UpdateItemSize(CFWL_ListItem* hItem,
                      CFX_SizeF& size,
                      FX_FLOAT fWidth,
                      FX_FLOAT fHeight,
                      bool bAutoSize) const;
  FX_FLOAT GetMaxTextWidth();
  FX_FLOAT GetScrollWidth();

  void OnFocusChanged(CFWL_Message* pMsg, bool bSet);
  void OnLButtonDown(CFWL_MessageMouse* pMsg);
  void OnLButtonUp(CFWL_MessageMouse* pMsg);
  void OnMouseWheel(CFWL_MessageMouseWheel* pMsg);
  void OnKeyDown(CFWL_MessageKey* pMsg);
  void OnVK(CFWL_ListItem* hItem, bool bShift, bool bCtrl);
  bool OnScroll(CFWL_ScrollBar* pScrollBar,
                CFWL_EventScroll::Code dwCode,
                FX_FLOAT fPos);

  CFX_RectF m_rtClient;
  CFX_RectF m_rtStatic;
  CFX_RectF m_rtConent;
  std::unique_ptr<CFWL_ScrollBar> m_pHorzScrollBar;
  std::unique_ptr<CFWL_ScrollBar> m_pVertScrollBar;
  uint32_t m_dwTTOStyles;
  int32_t m_iTTOAligns;
  CFWL_ListItem* m_hAnchor;
  FX_FLOAT m_fItemHeight;
  FX_FLOAT m_fScorllBarWidth;
  bool m_bLButtonDown;
  IFWL_ThemeProvider* m_pScrollBarTP;
  std::vector<std::unique_ptr<CFWL_ListItem>> m_ItemArray;
};

#endif  // XFA_FWL_CFWL_LISTBOX_H_
