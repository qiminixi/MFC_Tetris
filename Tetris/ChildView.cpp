// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Tetris.h"
#include "ChildView.h"
#include "ModeSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LEVEL_MAX 17
#define LEVEL_LINES 1

// CChildView

HANDLE g_hEventControl=NULL;
HANDLE g_hEventSuspend=NULL;
BOOL g_bSuspend=FALSE;//暂停游戏
int g_nLevel=1;//共17级，每级10行
int g_SpeedOfLevels[]={0,1000,900,800,700,600,500,400,300,200,100,
						90,80,70,60,50,40,30};
int g_nSpeed=0;
BOOL g_bGameOver=FALSE;


int g_Score[]={0,1,3,5,8};
CBitmap g_BitmapOfShapes[8];
CBitmap g_BitmapOfModes[2];//0为无限，1为街机
CBitmap g_BitmapOfPreview[19];

UINT Control(LPVOID lParam)
{
	TRACE(_T("Control Function started\n"));
	CChildView* pView=(CChildView*)lParam;
	//return 0;
	while(1)
	{
		::WaitForSingleObject(g_hEventControl,INFINITE);
		for(;;)//一轮游戏
		{
			Sleep(1500);
			pView->SetShape();
			if(g_bGameOver)//街机模式的游戏结束判断
			{
				pView->GameOver();
				g_bGameOver=FALSE;
				break;
			}
			if(pView->TouchBlockDown())//在初始位置不能下落，结束游戏
			{
				pView->GameOver();
				break;
			}
			
			for(;;)
			{
				if(g_bSuspend)//进入暂停，等待继续
				{
					::WaitForSingleObject(g_hEventSuspend,INFINITE);
					g_bSuspend=FALSE;
				}
				pView->m_shape.MoveDown();
				pView->Invalidate();
				Sleep(g_nSpeed);//30-?g_nLevel
				if(pView->TouchBlockDown())
				{
					pView->GetScore();
					break;
				}
			}
		}
	}
	return 1;
}


CChildView::CChildView()
{
	InitBoard();
	g_nSpeed=g_SpeedOfLevels[g_nLevel];
	m_nScore=0;
	m_nLines=0;
	m_bGamerOver=TRUE;
	m_nGameMode=1;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_GAME_START, &CChildView::OnGameStart)
//	ON_COMMAND(ID_32773, &CChildView::On32773)
ON_COMMAND(ID_GAME_SELECT_MODE, &CChildView::OnGameSelectMode)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectClient;
	GetClientRect(rectClient);
	CBitmap membmp;
	membmp.CreateCompatibleBitmap(&dc,rectClient.Width(),rectClient.Height());
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CBitmap* pOldmembmp=memDC.SelectObject(&membmp);
	
	CBrush brBg(RGB(87,87,168));
	memDC.FillRect(rectClient,&brBg);
	
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmapBmp=bmpDC.SelectObject(&g_BitmapOfModes[0]);
	memDC.SetStretchBltMode(COLORONCOLOR);
	memDC.SetBkMode(TRANSPARENT);
	
	//左边的状态显示
	CPoint ptTopleftL(MARGIN_H,MARGIN_V);
	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(CRect(ptTopleftL.x,ptTopleftL.y,ptTopleftL.x+LR_WIDTH,ptTopleftL.y+HEIGHT*SIDE_LENGTH));
	
	//模式
	if(m_nGameMode==1)
		memDC.StretchBlt(ptTopleftL.x,ptTopleftL.y+100,LR_WIDTH,36,&bmpDC,0,0,100,60,SRCCOPY);
	else
	{
		bmpDC.SelectObject(&g_BitmapOfModes[1]);
		memDC.StretchBlt(ptTopleftL.x,ptTopleftL.y+100,LR_WIDTH,36,&bmpDC,0,0,100,60,SRCCOPY);
	}
	//------------------------------
	
	//等级
	CString str;
	str.Format(_T("等级：%d"),g_nLevel);
	memDC.TextOutW(ptTopleftL.x,ptTopleftL.y+150,str);
	
	//行数
	str.Format(_T("行数：%d"),m_nLines);
	memDC.TextOutW(ptTopleftL.x,ptTopleftL.y+200,str);
	
	//分数
	m_rectScore=CRect(ptTopleftL.x,ptTopleftL.y+275,ptTopleftL.x+LR_WIDTH,ptTopleftL.y+312);
	memDC.Rectangle(m_rectScore);
	if(m_nScore<10)
		str.Format(_T("000%d"),m_nScore);
	else if(m_nScore<100)
		str.Format(_T("00%d"),m_nScore);
	else if(m_nScore<999)
		str.Format(_T("0%d"),m_nScore);
	else
		str.Format(_T("%d"),m_nScore);
	
	CFont font;
	font.CreatePointFont(180,_T("黑体"));
	CFont* pOldFont=memDC.SelectObject(&font);
	memDC.TextOutW(ptTopleftL.x+6,ptTopleftL.y+280,str);
	//-----------------------------
	//------------------------------------------
	
	//中间
	CPoint ptTopleftM(MARGIN_H+LR_WIDTH,MARGIN_V);
	m_ptTopleftM=ptTopleftM;
	CBrush brBgM(RGB(0,0,0));
	CBrush* pOldBrush=memDC.SelectObject(&brBgM);
	memDC.Rectangle(CRect(ptTopleftM,CSize(WIDTH*SIDE_LENGTH,HEIGHT*SIDE_LENGTH)));
	
	CPen pen(PS_SOLID,1,RGB(30,30,30));
	CPen* pOldPen=memDC.SelectObject(&pen);
	for(int i=0;i<=WIDTH;i++)
	{
		memDC.MoveTo(ptTopleftM.x+i*SIDE_LENGTH,ptTopleftM.y);
		memDC.LineTo(ptTopleftM.x+i*SIDE_LENGTH,ptTopleftM.y+HEIGHT*SIDE_LENGTH);
	}
	for(int i=0;i<=HEIGHT;i++)
	{
		memDC.MoveTo(ptTopleftM.x,ptTopleftM.y+i*SIDE_LENGTH);
		memDC.LineTo(ptTopleftM.x+WIDTH*SIDE_LENGTH,ptTopleftM.y+i*SIDE_LENGTH);
	}
	
	CPoint point;
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			if(m_Board[i+3][j])
			{
				point.x=ptTopleftM.x+j*SIDE_LENGTH;
				point.y=ptTopleftM.y+i*SIDE_LENGTH;
				bmpDC.SelectObject(&g_BitmapOfShapes[m_Board[i+3][j]]);
				memDC.StretchBlt(point.x,point.y,SIDE_LENGTH,SIDE_LENGTH,&bmpDC,0,0,100,100,SRCCOPY);
			}
		}
	
	}
	DrawShape(&memDC,&bmpDC);
	//--------------------------------------------------------------------
	
	//右边
	memDC.SelectStockObject(BLACK_PEN);
	memDC.SelectStockObject(NULL_BRUSH);
	CPoint ptTopleftR(ptTopleftM.x+WIDTH*SIDE_LENGTH,ptTopleftM.y);
	memDC.Rectangle(CRect(ptTopleftR,CSize(LR_WIDTH,HEIGHT*SIDE_LENGTH)));
	//if(!m_bGamerOver)
	{
		CRect rect(ptTopleftR.x+10,ptTopleftR.y+30,ptTopleftR.x+50,ptTopleftR.y+70);
		CPen pen(PS_SOLID,1,RGB(208,149,208));
		memDC.SelectObject(&pen);
		memDC.Rectangle(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1);
		memDC.Rectangle(rect.left-4,rect.top-4,rect.right+4,rect.bottom+4);
		bmpDC.SelectObject(g_BitmapOfPreview[m_PreviewShapes[0]]);
		memDC.StretchBlt(rect.left,rect.top,40,40,&bmpDC,0,0,50,50,SRCCOPY);
		
		rect.left+=2;
		rect.top+=35+20;
		bmpDC.SelectObject(g_BitmapOfPreview[m_PreviewShapes[1]]);
		memDC.StretchBlt(rect.left,rect.top,35,35,&bmpDC,0,0,50,50,SRCCOPY);
		
		rect.top+=35+20;
		bmpDC.SelectObject(g_BitmapOfPreview[m_PreviewShapes[2]]);
		memDC.StretchBlt(rect.left,rect.top,35,35,&bmpDC,0,0,50,50,SRCCOPY);
		
		rect.top+=35+20;
		bmpDC.SelectObject(g_BitmapOfPreview[m_PreviewShapes[3]]);
		memDC.StretchBlt(rect.left,rect.top,35,35,&bmpDC,0,0,50,50,SRCCOPY);
	}
	//---------------------------------------------------------------------
	
	dc.BitBlt(0,0,rectClient.Width(),rectClient.Height(),&memDC,0,0,SRCCOPY);
	
	memDC.SelectObject(pOldmembmp);
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldFont);
	bmpDC.SelectObject(pOldBitmapBmp);
	// Do not call CWnd::OnPaint() for painting messages
}
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	static int f=1;
	if(f)
	{
		f=0;
		g_hEventControl=CreateEvent(NULL,FALSE,FALSE,NULL);
		g_hEventSuspend=CreateEvent(NULL,FALSE,FALSE,NULL);
		AfxBeginThread(Control,this);
		ChangeWindowSize();
		srand((unsigned int)time(NULL));
		for(int i=0;i<NUM_SHAPES;i++)
		{
			m_PreviewShapes[i]=int((double)rand()/RAND_MAX*SHAPE_Z_2);
		}
		for(int i=0;i<7;i++)
			g_BitmapOfShapes[i+1].LoadBitmapW(IDB_BITMAP_I+i);
		g_BitmapOfModes[0].LoadBitmapW(IDB_BITMAP_MODE_UNLIMITED);
		g_BitmapOfModes[1].LoadBitmapW(IDB_BITMAP_MODE_ARCADE);
		for(int i=0;i<19;i++)
		{
			g_BitmapOfPreview[i].LoadBitmapW(IDB_BITMAP_PREVIEW_I1+i);
		}
	}
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}
void CChildView::DrawShape(CDC* pmemDC,CDC* pbmpDC)
{
	//CBrush brDia(RGB(0,128,128));
	if(m_shape.nType1==SHAPE_NULL)
		return;
	int c=m_shape.ptPos.x;
	int r=m_shape.ptPos.y;
	CPoint point;
	for(int i=3;i>=0;i--)
	{
		if((r-3+i)<3) continue;
		for(int j=0;j<4;j++)
		{
			if((*m_shape.pShapeBoard)[i][j]==0)
				continue;
			point.x=m_ptTopleftM.x+(c+j)*SIDE_LENGTH;
			point.y=m_ptTopleftM.y+(r-3+i-3)*SIDE_LENGTH;
			pbmpDC->SelectObject(&g_BitmapOfShapes[m_shape.nType1-100]);
			pmemDC->StretchBlt(point.x,point.y,SIDE_LENGTH,SIDE_LENGTH,pbmpDC,0,0,100,100,SRCCOPY);
		}
	}
	
}
void CChildView::ChangeWindowSize()
{
	int cx=MARGIN_H*2+LR_WIDTH*2+SIDE_LENGTH*WIDTH+20;
	int cy=MARGIN_V*2+SIDE_LENGTH*HEIGHT+60;
	AfxGetApp()->m_pMainWnd->MoveWindow(300,200,cx,cy);
}
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_UP:
		m_shape.Rotate();
		Invalidate();
		break;
	case VK_DOWN:
		FreeFall();
		//m_shape.MoveDown();
		Invalidate();
		break;
	case VK_LEFT:
		if(!TouchBlockLeft())
			m_shape.MoveLeft();
		Invalidate();
		break;
	case VK_RIGHT:
		if(!TouchBlockRight())
			m_shape.MoveRight();
		Invalidate();
		break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CChildView::FreeFall()            
{
	if(m_shape.nType1==SHAPE_NULL)
		return;
	if(m_shape.ptPos.y<3)
		return;
	int j;
	for(j=m_shape.ptPos.y;j<=HEIGHT+2;j++)
	{
		if(TouchBlockDown())
			break;
		m_shape.ptPos.y++;
	}
	TRACE("FreeFall: j=%d\n",m_shape.ptPos.y);
	//m_shape.ptPos.y=j;//如果没有break，说明是空的，直接掉到底部
	//GetScore();
	//SetShape();
	//g_bFallDown=TRUE;
	Invalidate();
}

void CChildView::SetShape()
{
	m_shape.SetShape(m_PreviewShapes[1]);
	TRACE(_T("Shapes: 0=%d,1=%d,2=%d,3=%d\n"),m_shape.nType2,
		m_PreviewShapes[0],m_PreviewShapes[1],m_PreviewShapes[2]);
	for(int i=0;i<NUM_SHAPES-1;i++)
		m_PreviewShapes[i]=m_PreviewShapes[i+1];
	m_PreviewShapes[NUM_SHAPES-1]=int((double)rand()/RAND_MAX*SHAPE_Z_2);
	Invalidate();
}
BOOL CChildView::TouchBlockDown()
{
	int r=m_shape.ptPos.y;
	int c=m_shape.ptPos.x;
	if(r==HEIGHT+2)//到达底端
		return TRUE;
	r++;//移动到下一行
	BOOL b=FALSE;
	switch(m_shape.nType1)
	{
	case SHAPE_NULL:
		return FALSE;
	case SHAPE_I:
		if(m_shape.nType2==SHAPE_I_1)
		{
			b=m_Board[r][c];
		}
		else
		{
			b=m_Board[r][c]||m_Board[r][c+1]||m_Board[r][c+2]||m_Board[r][c+3];
		}
		break;
	case SHAPE_J:
		if(m_shape.nType2==SHAPE_J_1)
		{
			b=m_Board[r][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_J_2)
		{
			b=m_Board[r][c]||m_Board[r][c+1]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_J_3)
		{
			b=m_Board[r][c]||m_Board[r-2][c+1];
		}
		else if(m_shape.nType2==SHAPE_J_4)
		{
			b=m_Board[r-1][c]||m_Board[r-1][c+1]||m_Board[r][c+2];
		}
		break;
	case SHAPE_L:
		if(m_shape.nType2==SHAPE_L_1)
		{
			b=m_Board[r][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_L_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_L_3)
		{
			b=m_Board[r-2][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_L_4)
		{
			b=m_Board[r][c]||m_Board[r][c+1]||m_Board[r][c+2];
		}
		break;
	case SHAPE_O:
		b=m_Board[r][c]||m_Board[r][c+1];
		break;
	case SHAPE_S:
		if(m_shape.nType2==SHAPE_S_1)
		{
			b=m_Board[r][c]||m_Board[r][c+1]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_S_2)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1];
		}
		break;
	case SHAPE_T:
		if(m_shape.nType2==SHAPE_T_1)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_T_2)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_T_3)
		{
			b=m_Board[r][c]||m_Board[r][c+1]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_T_4)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1];
		}
		break;
	case SHAPE_Z:
		if(m_shape.nType2==SHAPE_Z_1)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_Z_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1];
		}
		break;
	}
	return b;
}
BOOL CChildView::TouchBlockLeft()
{
	int r=m_shape.ptPos.y;
	int c=m_shape.ptPos.x;
	if(c==0)//到达最左边
		return TRUE;
	if(r<3)//允许形状没有显示的时候移动
		return FALSE;
	c--;//指向左边下一个位置
	BOOL b=FALSE;
	switch(m_shape.nType1)
	{
	case SHAPE_NULL:
		return FALSE;
	case SHAPE_I:
		if(m_shape.nType2==SHAPE_I_1)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c]||m_Board[r-3][c];
		}
		else
		{
			b=m_Board[r][c];
		}
		break;
	case SHAPE_J:
		if(m_shape.nType2==SHAPE_J_1)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1]||m_Board[r-2][c+1];
		}
		else if(m_shape.nType2==SHAPE_J_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c];
		}
		else if(m_shape.nType2==SHAPE_J_3)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c];
		}
		else if(m_shape.nType2==SHAPE_J_4)
		{
			b=m_Board[r-1][c]||m_Board[r][c+2];
		}
		break;
	case SHAPE_L:
		if(m_shape.nType2==SHAPE_L_1)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c];
		}
		else if(m_shape.nType2==SHAPE_L_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c];
		}
		else if(m_shape.nType2==SHAPE_L_3)
		{
			b=m_Board[r-2][c]||m_Board[r-1][c+1]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_L_4)
		{
			b=m_Board[r][c]||m_Board[r-1][c+2];
		}
		break;
	case SHAPE_O:
		b=m_Board[r][c]||m_Board[r-1][c];
		break;
	case SHAPE_S:
		if(m_shape.nType2==SHAPE_S_1)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1];
		}
		else if(m_shape.nType2==SHAPE_S_2)
		{
			b=m_Board[r-2][c]||m_Board[r-1][c]||m_Board[r][c+1];
		}
		break;
	case SHAPE_T:
		if(m_shape.nType2==SHAPE_T_1)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_T_2)
		{
			b=m_Board[r-1][c]||m_Board[r-2][c+1]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_T_3)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1];
		}
		else if(m_shape.nType2==SHAPE_T_4)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c];
		}
		break;
	case SHAPE_Z:
		if(m_shape.nType2==SHAPE_Z_1)
		{
			b=m_Board[r-1][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_Z_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c+1];
		}
		break;
	}
	return b;
}
BOOL CChildView::TouchBlockRight()
{
	int r=m_shape.ptPos.y;
	int c=m_shape.ptPos.x;
	if(c==WIDTH-1)//到达最左端
		return TRUE;
	if(r<3)//允许形状没有显示的时候移动
		return FALSE;
	c++;//指向右边下一个位置位置
	BOOL b=FALSE;
	switch(m_shape.nType1)
	{
	case SHAPE_NULL:
		return FALSE;
	case SHAPE_I:
		if(m_shape.nType2==SHAPE_I_1)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c]||m_Board[r-3][c];
		}
		else
		{
			b=m_Board[r][c];
		}
		break;
	case SHAPE_J:
		if(m_shape.nType2==SHAPE_J_1)
		{
			b=m_Board[r][c+1]||m_Board[r-1][c+1]||m_Board[r-2][c+1];
		}
		else if(m_shape.nType2==SHAPE_J_2)
		{
			b=m_Board[r-1][c]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_J_3)
		{
			b=m_Board[r][c]||m_Board[r-1][c]||m_Board[r-2][c+1];
		}
		else if(m_shape.nType2==SHAPE_J_4)
		{
			b=m_Board[r-1][c+2]||m_Board[r][c+2];
		}
		break;
	case SHAPE_L:
		if(m_shape.nType2==SHAPE_L_1)
		{
			b=m_Board[r-2][c]||m_Board[r-1][c]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_L_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_L_3)
		{
			b=m_Board[r-2][c+1]||m_Board[r-1][c+1]||m_Board[r][c+1];
		}
		else if(m_shape.nType2==SHAPE_L_4)
		{
			b=m_Board[r][c+2]||m_Board[r-1][c+2];
		}
		break;
	case SHAPE_O:
		b=m_Board[r][c+1]||m_Board[r-1][c+1];
		break;
	case SHAPE_S:
		if(m_shape.nType2==SHAPE_S_1)
		{
			b=m_Board[r][c+1]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_S_2)
		{
			b=m_Board[r-2][c]||m_Board[r-1][c+1]||m_Board[r][c+1];
		}
		break;
	case SHAPE_T:
		if(m_shape.nType2==SHAPE_T_1)
		{
			b=m_Board[r][c+1]||m_Board[r-1][c+2];
		}
		else if(m_shape.nType2==SHAPE_T_2)
		{
			b=m_Board[r][c+1]||m_Board[r-1][c+1]||m_Board[r-2][c+1];
		}
		else if(m_shape.nType2==SHAPE_T_3)
		{
			b=m_Board[r-1][c+1]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_T_4)
		{
			b=m_Board[r-2][c]||m_Board[r-1][c+1]||m_Board[r][c];
		}
		break;
	case SHAPE_Z:
		if(m_shape.nType2==SHAPE_Z_1)
		{
			b=m_Board[r-1][c+1]||m_Board[r][c+2];
		}
		else if(m_shape.nType2==SHAPE_Z_2)
		{
			b=m_Board[r][c]||m_Board[r-1][c+1]||m_Board[r-2][c+1];
		}
		break;
	}
	return b;
}
void CChildView::GetScore()
{
	if(m_shape.nType1==SHAPE_NULL)
		return;
	
	int r=m_shape.ptPos.y;
	int c=m_shape.ptPos.x;
	//将图形合并到Board中
	for(int i=3;i>=0;i--)
	{
		for(int j=0;j<4;j++)
		{
			if((*m_shape.pShapeBoard)[i][j]>0)
				m_Board[r+i-3][c+j]=(*m_shape.pShapeBoard)[i][j];
		}
	}
	m_shape.nType1=SHAPE_NULL;
		
	for(int i=0;i<HEIGHT+3;i++)
	{
		for(int j=0;j<WIDTH;j++)
			TRACE(_T("%-3d"),m_Board[i][j]);
		TRACE(_T("\n"));
	}
	//---------------------------------

	//计算分值
	int n=0;
	for(int i=3;i<HEIGHT+3;i++)
	{
		BOOL b=TRUE;//这一行是否全为1
		for(int j=0;j<WIDTH;j++)
		{
			if(m_Board[i][j]==0)
			{
				b=FALSE;
				break;
			}
		}
		if(!b&&n!=0)//如果这一行不全为1，n不为的0的话，“连行”统计结束，给个分值
		{
			m_nScore+=g_Score[n];
			n=0;
		}
		else if(b)
		{
			for(int j=0;j<WIDTH;j++)//上面的内容下移
			{
				for(int k=i;k>0;k--)
				{
					m_Board[k][j]=m_Board[k-1][j];
				}
			}
			n++;//计数增加
			m_nLines++;//行数增加
		}
	}
	if(n!=0)
		m_nScore+=g_Score[n];
	
	TRACE(_T("---------------------\n"));
	for(int i=0;i<HEIGHT+3;i++)
	{
		for(int j=0;j<WIDTH;j++)
			TRACE(_T("%-3d"),m_Board[i][j]);
		TRACE(_T("\n"));
	}
	//---------------------------------
	
	if(m_nGameMode==2&&m_nLines>=LEVEL_LINES)//等级增加
	{
		g_nLevel++;//等级增加
		if(m_nGameMode==2)//街机模式下的游戏结束判断
		{
			if(g_nLevel>LEVEL_MAX)
			{
				g_nLevel-=1;
				g_bGameOver=TRUE;
			}
		}
		g_nSpeed=g_SpeedOfLevels[g_nLevel];
		m_nLines=m_nLines%LEVEL_LINES;
	}
	
}

void CChildView::GameOver()
{
	MessageBox(_T("游戏结束！"));
}
void CChildView::OnGameStart()
{
	InitBoard();
	SetEvent(g_hEventControl);
	
}
void CChildView::InitBoard()
{
	for(int i=0;i<=HEIGHT+2;i++)
		for(int j=0;j<WIDTH;j++)
			m_Board[i][j]=0;
}
void CChildView::OnGameSelectMode()
{
	CModeSelectDlg dlg;
	dlg.m_nSelect=m_nGameMode;
	dlg.m_nLevelUnlimited=g_nLevel;
	dlg.m_nLevelArcade=g_nLevel;
	g_bSuspend=TRUE;//控制线程进入等待
	if(dlg.DoModal()==IDOK)
	{
		if(m_nGameMode==dlg.m_nSelect)//如果没有更改参数的话就不重新开始游戏
		{
			if(m_nGameMode==1&&g_nLevel==dlg.m_nLevelUnlimited)
				goto END;
			if(m_nGameMode==2&&g_nLevel==dlg.m_nLevelArcade)
				goto END;
		}
		

		m_nGameMode=dlg.m_nSelect;
		if(m_nGameMode==1)
			g_nLevel=dlg.m_nLevelUnlimited;
		else if(m_nGameMode==2)
			g_nLevel=dlg.m_nLevelArcade;
		g_nSpeed=g_SpeedOfLevels[g_nLevel];
		TRACE(_T("OnGameSelectMode: Has changed!\n"));
		InitBoard();
		Invalidate();
	}
END:
	SetEvent(g_hEventSuspend);
}
