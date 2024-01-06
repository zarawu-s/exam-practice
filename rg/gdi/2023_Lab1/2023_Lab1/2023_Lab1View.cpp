
// 2023_Lab1View.cpp : implementation of the CMy2023Lab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2023_Lab1.h"
#endif

#include "2023_Lab1Doc.h"
#include "2023_Lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535

// CMy2023Lab1View

IMPLEMENT_DYNCREATE(CMy2023Lab1View, CView)

BEGIN_MESSAGE_MAP(CMy2023Lab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2023Lab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2023Lab1View construction/destruction

void CMy2023Lab1View::DrawRegularPolygon(CDC * pDC, int cx, int cy, int r, int n, float rotAngle)
{
	CBrush* old = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
 	float rot = 2 * PI / n;
	POINT* points = new POINT[n];

	for (int i = 0; i < n; i++)
	{
		points[i].x = r * cos(rotAngle) + cx;
		points[i].y = r * sin(rotAngle) + cy;
		rotAngle += rot;
	}

	pDC->Polygon(points, n);
	pDC->SelectObject(old);
}

void CMy2023Lab1View::DrawTriangle(CDC * pDC, POINT p1, POINT p2, POINT p3)
{
	POINT* points = new POINT[3];

	points[0] = p1;
	points[1] = p2;
	points[2] = p3;

	pDC->Polygon(points, 3);
}

void CMy2023Lab1View::DrawTriangleSolid(CDC* pDC, POINT p1, POINT p2, POINT p3, COLORREF color)
{
	CBrush* brush = new CBrush(color);
	CBrush* old = pDC->SelectObject(brush);

	DrawTriangle(pDC, p1, p2, p3);

	pDC->SelectObject(old);
	delete brush;
}

void CMy2023Lab1View::DrawTriangleHatch(CDC* pDC, POINT p1, POINT p2, POINT p3, COLORREF color, COLORREF background, int pattern)
{
	pDC->SetBkColor(background);

	CBrush* brush = new CBrush(pattern, color);
	CBrush* old = pDC->SelectObject(brush);

	DrawTriangle(pDC, p1, p2, p3);

	pDC->SelectObject(old);
	delete brush;
}

void CMy2023Lab1View::DrawQuadrilateral(CDC* pDC, POINT p1, POINT p2, POINT p3, POINT p4, COLORREF color)
{
	CBrush* brush = new CBrush(color);
	CBrush* old = pDC->SelectObject(brush);
	POINT* points = new POINT[4];
	
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
	pDC->Polygon(points, 4);
	
	pDC->SelectObject(old);
	delete brush;
}

void CMy2023Lab1View::DrawGrid(CDC * pDC)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* old = pDC->SelectObject(pen);

	for (int i = 0; i < 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}

	pDC->SelectObject(old);
	delete pen;
}


CMy2023Lab1View::CMy2023Lab1View() noexcept
{
	// TODO: add construction code here

}

CMy2023Lab1View::~CMy2023Lab1View()
{
}

BOOL CMy2023Lab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2023Lab1View drawing

void CMy2023Lab1View::OnDraw(CDC* pDC)
{
	CMy2023Lab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CBrush* background = new CBrush(RGB(228, 228, 228));
	CRect rect(0, 0, 500, 500);

	pDC->FillRect(rect, background);

	CPen* outPen = new CPen(PS_SOLID, 5, RGB(255, 255, 0));
	CPen* old = pDC->SelectObject(outPen);

	DrawTriangleSolid(pDC, { 142,25 }, { 142, 167 }, { 70, 97 }, RGB(150, 0, 220));
	DrawQuadrilateral(pDC, { 142, 100 }, { 214, 169 },  { 214, 240 }, { 142, 167 }, RGB(255, 0, 0));
	DrawTriangleHatch(pDC, { 214, 169 }, { 214, 370 }, {315, 270}, RGB(0,0,255), RGB(255,255, 255), HS_BDIAGONAL);
	DrawQuadrilateral(pDC, { 315, 270 }, { 366,321 }, { 315, 373 }, { 264, 321 }, RGB(32, 192, 32));
	DrawTriangleSolid(pDC, { 264, 321 }, { 160, 424 }, { 370, 424 }, RGB(255, 255, 0));
	DrawTriangleSolid(pDC, { 330, 424 }, { 430, 424 }, { 379,475 }, RGB(255, 192, 0));
	DrawTriangleSolid(pDC, { 351,339 }, { 421,339 }, { 421,267 }, RGB(255, 192, 192));

	CPen* inPen = new CPen(PS_SOLID, 3, RGB(255, 255, 0));
	
	pDC->SelectObject(inPen);
	DrawRegularPolygon(pDC, 112, 97, 15, 6, 0);
	DrawRegularPolygon(pDC, 257, 270, 20, 4, 0);
	DrawRegularPolygon(pDC, 399, 317, 10, 8, 0);
	DrawRegularPolygon(pDC, 378, 444, 10, 7, 0);

	pDC->SelectObject(old);
	delete outPen;
	delete inPen;
	delete background;

	if (hit)
	{
		DrawGrid(pDC);
	}
}


// CMy2023Lab1View printing


void CMy2023Lab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy2023Lab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2023Lab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2023Lab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2023Lab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2023Lab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy2023Lab1View diagnostics

#ifdef _DEBUG
void CMy2023Lab1View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2023Lab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2023Lab1Doc* CMy2023Lab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2023Lab1Doc)));
	return (CMy2023Lab1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2023Lab1View message handlers


void CMy2023Lab1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE) {
		hit = !hit;
		Invalidate();
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
