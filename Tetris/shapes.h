#pragma once

//��״����˳ʱ�뷽����ת
#define SHAPE_NULL		100
#define SHAPE_I			101	//I��
#define SHAPE_I_1		0	//I��--
#define SHAPE_I_2		1	//I��|
#define SHAPE_J			102	//J��
#define SHAPE_J_1		2	//J��
#define SHAPE_J_2		3	//J��
#define SHAPE_J_3		4	//J��
#define SHAPE_J_4		5	//J��
#define SHAPE_L			103	//L��
#define SHAPE_L_1		6	//L��
#define SHAPE_L_2		7	//L��
#define SHAPE_L_3		8	//L��
#define SHAPE_L_4		9	//L��
#define SHAPE_O			104	//O��
#define SHAPE_O_1		10	//O��
#define SHAPE_S			105	//S��
#define SHAPE_S_1		11	//S��
#define SHAPE_S_2		12	//S��
#define SHAPE_T			106	//T��
#define SHAPE_T_1		13	//T��
#define SHAPE_T_2		14	//T��
#define SHAPE_T_3		15	//T��
#define SHAPE_T_4		16	//T��
#define SHAPE_Z			107	//Z��
#define SHAPE_Z_1		17	//Z��
#define SHAPE_Z_2		18	//Z��

struct SShape
{
	int nType1;
	int nType2;
	int	(*pShapeBoard)[4][4];
	CPoint ptPos;//���½�����
	
	//
	void SetShape(int n);
	void Rotate();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
};
extern int g_Type1FromType2[19];