// ChildView.h : interface of the CChildView class
//
#include "shapes.h"

#pragma once

#define SIDE_LENGTH		18		//���ӱ߳�
#define LR_WIDTH		60		//���ߵĳ���(3*SIDE_LENGTH)
#define NUM_SHAPES		4		//��ʾ�Ľ���������״��
#define MARGIN_H		10		//ˮƽ�����ϵ�����
#define MARGIN_V		10		//��ֱ�����ϵ�����
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
	BOOL TouchBlockDown();//�ж������Ƿ�Ӵ������飨��һ�е�ָ��λ���Ƿ��з��飩
	BOOL TouchBlockLeft();//�ж������Ƿ�Ӵ�������
	BOOL TouchBlockRight();//�ж������Ƿ�Ӵ�������
	void UpdateBoard();//�������������
	void GetScore();//����һ�У����ӷ�ֵ
	void GameOver();
	void InitBoard();//��ʼ��
public:
	CRect m_rectMode;
	CRect m_rectScore;
	//CRect m_rectTime;
	CRect m_rectShapes[NUM_SHAPES];//����������������״
	CPoint m_ptTopleftM;//TopleftL,TopleftR
	
	int m_Board[HEIGHT+3][WIDTH];//������������жϵ�0�д��Ƿ�Ӵ�������
	int m_PreviewShapes[NUM_SHAPES];
	SShape m_shape;
	int m_nScore;//�÷�
	int m_nLines;//����
	
	BOOL m_bGamerOver;
	int m_nGameMode;//1Ϊ���ޣ�2Ϊ�ֻ�
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

