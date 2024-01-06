
// 2023_Lab1View.h : interface of the CMy2023Lab1View class
//

#pragma once


class CMy2023Lab1View : public CView
{

private:
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawTriangle(CDC* pDC, POINT p1, POINT p2, POINT p3);
	void DrawTriangleSolid(CDC* pDC, POINT p1, POINT p2, POINT p3, COLORREF color);
	void DrawTriangleHatch(CDC* pDC, POINT p1, POINT p2, POINT p3, COLORREF color, COLORREF background, int pattern);
	void DrawQuadrilateral(CDC* pDC, POINT p1, POINT p2, POINT p3, POINT p4, COLORREF color);
	void DrawGrid(CDC* pDC);

protected: // create from serialization only
	bool hit;
	CMy2023Lab1View() noexcept;
	DECLARE_DYNCREATE(CMy2023Lab1View)

// Attributes
public:
	CMy2023Lab1Doc* GetDocument() const;

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
	virtual ~CMy2023Lab1View();
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
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in 2023_Lab1View.cpp
inline CMy2023Lab1Doc* CMy2023Lab1View::GetDocument() const
   { return reinterpret_cast<CMy2023Lab1Doc*>(m_pDocument); }
#endif

