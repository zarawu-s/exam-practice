
// Kol2019View.h : interface of the CKol2019View class
//

#pragma once
#include "DImage.h"
#define PI 3.1416


class CKol2019View : public CView
{
protected: // create from serialization only
	CKol2019View() noexcept;
	DECLARE_DYNCREATE(CKol2019View)

// Attributes

	DImage* arm1;
	DImage* arm2;
	DImage* leg1;
	DImage* leg2;
	DImage* body;
	DImage* background;

	int position;

public:
	CKol2019Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2019View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Kol2019View.cpp
inline CKol2019Doc* CKol2019View::GetDocument() const
   { return reinterpret_cast<CKol2019Doc*>(m_pDocument); }
#endif

