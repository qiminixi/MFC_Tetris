#pragma once

//形状，按顺时针方向旋转
#define SHAPE_NULL		100
#define SHAPE_I			101	//I形
#define SHAPE_I_1		0	//I形--
#define SHAPE_I_2		1	//I形|
#define SHAPE_J			102	//J形
#define SHAPE_J_1		2	//J形
#define SHAPE_J_2		3	//J形
#define SHAPE_J_3		4	//J形
#define SHAPE_J_4		5	//J形
#define SHAPE_L			103	//L形
#define SHAPE_L_1		6	//L形
#define SHAPE_L_2		7	//L形
#define SHAPE_L_3		8	//L形
#define SHAPE_L_4		9	//L形
#define SHAPE_O			104	//O形
#define SHAPE_O_1		10	//O形
#define SHAPE_S			105	//S形
#define SHAPE_S_1		11	//S形
#define SHAPE_S_2		12	//S形
#define SHAPE_T			106	//T形
#define SHAPE_T_1		13	//T形
#define SHAPE_T_2		14	//T形
#define SHAPE_T_3		15	//T形
#define SHAPE_T_4		16	//T形
#define SHAPE_Z			107	//Z形
#define SHAPE_Z_1		17	//Z形
#define SHAPE_Z_2		18	//Z形

struct SShape
{
	int nType1;
	int nType2;
	int	(*pShapeBoard)[4][4];
	CPoint ptPos;//左下角坐标
	
	//
	void SetShape(int n);
	void Rotate();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
};
extern int g_Type1FromType2[19];