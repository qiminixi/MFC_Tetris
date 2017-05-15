// ChildView.h : interface of the CChildView class
//
#include "shapes.h"

#pragma once

#define SIDE_LENGTH		18		//格子边长
#define LR_WIDTH		60		//两边的长度(3*SIDE_LENGTH)
#define NUM_SHAPES		4		//显示的接下来的形状数
#define MARGIN_H		10		//水平方向上的留空
#define MARGIN_V		10		//竖直方向上的留空
#define HEIGHT			20
#define WIDTH			10

// CChildView window
class CChildView : public CWnd
{
public:
	void ChangeWindowSize();
	void DrawShape(CDC* pmemDC,CDC* pbmpDC);
	void FreeFall();
	void SetShape();
	BOOL TouchBlockDown();//判断下移是否接触到方块（下一行的指定位置是否有方块）
	BOOL TouchBlockLeft();//判断左移是否接触到方块
	BOOL TouchBlockRight();//判断右移是否接触到方块
	void UpdateBoard();//将方块加入盘中
	void GetScore();//消掉一行，增加分值
	void GameOver();
	void InitBoard();//初始化
public:
	CRect m_rectMode;
	CRect m_rectScore;
	//CRect m_rectTime;
	CRect m_rectShapes[NUM_SHAPES];//接下来的下来的形状
	CPoint m_ptTopleftM;//TopleftL,TopleftR
	
	int m_Board[HEIGHT+3][WIDTH];//多加两行用于判断第0行处是否接触到方块
	int m_PreviewShapes[NUM_SHAPES];
	SShape m_shape;
	int m_nScore;//得分
	int m_nLines;//行数
	
	BOOL m_bGamerOver;
	int m_nGameMode;//1为无限，2为街机
	//int m_nLevel;
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnGameStart();
public:
//	afx_msg void On32773();
public:
	afx_msg void OnGameSelectMode();
};

