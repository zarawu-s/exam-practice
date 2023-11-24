
// Kol2019View.cpp : implementation of the CKol2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2019.h"
#endif

#include "Kol2019Doc.h"
#include "Kol2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2019View

IMPLEMENT_DYNCREATE(CKol2019View, CView)

BEGIN_MESSAGE_MAP(CKol2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKol2019View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CKol2019View construction/destruction

CKol2019View::CKol2019View() noexcept
{
	// TODO: add construction code here

	arm1 = new DImage();
	arm1->Load((CString)"arm1.png");

	arm2 = new DImage();
	arm2->Load((CString)"arm2.png");

	leg1 = new DImage();
	leg1->Load((CString)"leg1.png");

	leg2 = new DImage();
	leg2->Load((CString)"leg2.png");

	body = new DImage();
	body->Load((CString)"body.png");

	background = new DImage();
	background->Load((CString)"background.jpg");
}

CKol2019View::~CKol2019View()
{
	delete arm1;
	delete arm2;
	delete leg1;
	delete leg2;
	delete body;
	delete background;
}

BOOL CKol2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2019View drawing

void CKol2019View::OnDraw(CDC* pDC)
{
	CKol2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	memDC->SelectObject(bitmap);

	int prevMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM global, old;
	memDC->GetWorldTransform(&global);
	background->Draw(memDC, CRect(0, 0, background->Width(), background->Height()), rect);

	memDC->GetWorldTransform(&old);

	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prevMode);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete bitmap;
	memDC->DeleteDC();
	delete memDC;

}

// CKol2019View printing


void CKol2019View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKol2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKol2019View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKol2019View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKol2019View diagnostics

#ifdef _DEBUG
void CKol2019View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2019Doc* CKol2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2019Doc)));
	return (CKol2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2019View message handlers
