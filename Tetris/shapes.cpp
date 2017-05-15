#include "stdafx.h"
#include "shapes.h"
#include "ChildView.h"
int g_Type1FromType2[19]=
{
	101,101,			//I
	102,102,102,102,	//j
	103,103,103,103,	//L
	104,				//O
	105,105,			//S
	106,106,106,106,	//T
	107,107				//Z
};
int g_Shapes[19][4][4]=
{
	{//I1	0
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0}
	},
	{//I2	1
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{1,1,1,1}
	},
	{//J1	2
		{0,0,0,0},
		{0,2,0,0},
		{0,2,0,0},
		{2,2,0,0}
	},
	{//J2	3
		{0,0,0,0},
		{0,0,0,0},
		{2,0,0,0},
		{2,2,2,0}
	},
	{//J3	4
		{0,0,0,0},
		{2,2,0,0},
		{2,0,0,0},
		{2,0,0,0}
	},
	{//J4	5
		{0,0,0,0},
		{0,0,0,0},
		{2,2,2,0},
		{0,0,2,0}
	},
	{//L1	6
		{0,0,0,0},
		{3,0,0,0},
		{3,0,0,0},
		{3,3,0,0}
	},
	{//L2	7
		{0,0,0,0},
		{0,0,0,0},
		{3,3,3,0},
		{3,0,0,0}
	},
	{//L3	8
		{0,0,0,0},
		{3,3,0,0},
		{0,3,0,0},
		{0,3,0,0}
	},
	{//L4	9
		{0,0,0,0},
		{0,0,0,0},
		{0,0,3,0},
		{3,3,3,0}
	},
	{//O1	10
		{0,0,0,0},
		{0,0,0,0},
		{4,4,0,0},
		{4,4,0,0}
	},
	{//S1	11
		{0,0,0,0},
		{0,0,0,0},
		{0,5,5,0},
		{5,5,0,0}
	},
	{//S2	12
		{0,0,0,0},
		{5,0,0,0},
		{5,5,0,0},
		{0,5,0,0}
	},
	{//T1	13
		{0,0,0,0},
		{0,0,0,0},
		{6,6,6,0},
		{0,6,0,0}
	},
	{//T2	14
		{0,0,0,0},
		{0,6,0,0},
		{6,6,0,0},
		{0,6,0,0}
	},
	{//T3	15
		{0,0,0,0},
		{0,0,0,0},
		{0,6,0,0},
		{6,6,6,0}
	},
	{//T4	16
		{0,0,0,0},
		{6,0,0,0},
		{6,6,0,0},
		{6,0,0,0}
	},
	{//Z1	17
		{0,0,0,0},
		{0,0,0,0},
		{7,7,0,0},
		{0,7,7,0}
	},
	{//Z2	18
		{0,0,0,0},
		{0,7,0,0},
		{7,7,0,0},
		{7,0,0,0}
	},
};


void SShape::SetShape(int n)
{
	nType2=n;
	nType1=g_Type1FromType2[n];
	pShapeBoard=&g_Shapes[n];
	ptPos.y=2;//初始位置2
	ptPos.x=2;
}
void SShape::Rotate()
{
	switch(nType1)
	{
	case SHAPE_NULL:
		return;
	case SHAPE_I:
		if(nType2==SHAPE_I_1)
		{
			nType2=SHAPE_I_2;
			pShapeBoard=&g_Shapes[SHAPE_I_2];
			if(ptPos.x==0)
				;//保持为0
			else if(ptPos.x>WIDTH-4)
				ptPos.x=WIDTH-4;
			else
				ptPos.x-=1;	
		}
		else
		{
			nType2=SHAPE_I_1;
			pShapeBoard=&g_Shapes[SHAPE_I_1];
			ptPos.x+=1;
		}
		break;
	case SHAPE_J:
		if(nType2==SHAPE_J_1)
		{
			nType2=SHAPE_J_2;
			pShapeBoard=&g_Shapes[SHAPE_J_2];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		else if(nType2==SHAPE_J_2)
		{
			nType2=SHAPE_J_3;
			pShapeBoard=&g_Shapes[SHAPE_J_3];
			ptPos.x+=1;
			
		}
		else if(nType2==SHAPE_J_3)
		{
			nType2=SHAPE_J_4;
			pShapeBoard=&g_Shapes[SHAPE_J_4];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		else if(nType2==SHAPE_J_4)
		{
			nType2=SHAPE_J_1;
			pShapeBoard=&g_Shapes[SHAPE_J_1];
			ptPos.x+=1;
		}
		break;
	case SHAPE_L:
		if(nType2==SHAPE_L_1)
		{
			nType2=SHAPE_L_2;
			pShapeBoard=&g_Shapes[SHAPE_L_2];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		else if(nType2==SHAPE_L_2)
		{
			nType2=SHAPE_L_3;
			pShapeBoard=&g_Shapes[SHAPE_L_3];
			ptPos.x+=1;
		}
		else if(nType2==SHAPE_L_3)
		{
			nType2=SHAPE_L_4;
			pShapeBoard=&g_Shapes[SHAPE_L_4];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		else if(nType2==SHAPE_L_4)
		{
			nType2=SHAPE_L_1;
			pShapeBoard=&g_Shapes[SHAPE_L_1];
			ptPos.x+=1;
		}
		break;
	case SHAPE_O:
		break;
	case SHAPE_S:
		if(nType2==SHAPE_S_1)
		{
			nType2=SHAPE_S_2;
			pShapeBoard=&g_Shapes[SHAPE_S_2];
			ptPos.x+=1;
		}
		else if(nType2==SHAPE_S_2)
		{
			nType2=SHAPE_S_1;
			pShapeBoard=&g_Shapes[SHAPE_S_1];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		break;
	case SHAPE_T:
		if(nType2==SHAPE_T_1)
		{
			nType2=SHAPE_T_2;
			pShapeBoard=&g_Shapes[SHAPE_T_2];
			ptPos.x+=1;
		}
		else if(nType2==SHAPE_T_2)
		{
			nType2=SHAPE_T_3;
			pShapeBoard=&g_Shapes[SHAPE_T_3];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		else if(nType2==SHAPE_T_3)
		{
			nType2=SHAPE_T_4;
			pShapeBoard=&g_Shapes[SHAPE_T_4];
			ptPos.x+=1;
		}
		else if(nType2==SHAPE_T_4)
		{
			nType2=SHAPE_T_1;
			pShapeBoard=&g_Shapes[SHAPE_T_1];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		break;
	case SHAPE_Z:
		if(nType2==SHAPE_Z_1)
		{
			nType2=SHAPE_Z_2;
			pShapeBoard=&g_Shapes[SHAPE_Z_2];
			ptPos.x+=1;
		}
		else if(nType2==SHAPE_Z_2)
		{
			nType2=SHAPE_Z_1;
			pShapeBoard=&g_Shapes[SHAPE_Z_1];
			if(ptPos.x!=0)//为0的话保持为0
				ptPos.x-=1;
		}
		break;
	}
}

void SShape::MoveLeft()
{
	if(nType1==SHAPE_NULL)
		return;
	ptPos.x--;
	if(ptPos.x<0)
		ptPos.x=0;
}
void SShape::MoveRight()
{
	if(nType1==SHAPE_NULL)
		return;
	ptPos.x++;
	if( nType2==SHAPE_J_1||nType2==SHAPE_J_3||
		nType2==SHAPE_L_1||nType2==SHAPE_L_3||
		nType2==SHAPE_O_1||nType2==SHAPE_S_2||
		nType2==SHAPE_T_2||nType2==SHAPE_T_4||
		nType2==SHAPE_Z_2 )
	{
		if(ptPos.x>WIDTH-2)
			ptPos.x=WIDTH-2;
	}
	else if( nType2==SHAPE_J_2||nType2==SHAPE_J_4||
		nType2==SHAPE_L_2||nType2==SHAPE_L_4||
		nType2==SHAPE_S_1||nType2==SHAPE_T_1||
		nType2==SHAPE_T_3||nType2==SHAPE_Z_1 )
	{
		if(ptPos.x>WIDTH-3)
			ptPos.x=WIDTH-3;
	}
	else if(nType2==SHAPE_I_1)
	{
		if(ptPos.x>WIDTH-1)
			ptPos.x=WIDTH-1;
	}
	else
	{
		if(ptPos.x>WIDTH-4)
			ptPos.x=WIDTH-4;
	}
}
void SShape::MoveDown()
{
	if(nType1==SHAPE_NULL)
		return;
	ptPos.y++;
	if(ptPos.y>HEIGHT+2)
		ptPos.y=HEIGHT+2;
}