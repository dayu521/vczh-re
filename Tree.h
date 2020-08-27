#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "LytWString.h"

using namespace std;

template<typename _Type>
class TreeNode
{
public:
	_Type Data;
	TreeNode* Left;
	TreeNode* Right;
	TreeNode* Up;

	TreeNode()
	{
		Left=0;
		Right=0;
		Up=0;
	}

	~TreeNode()
	{
		if (Left) delete Left;
		if (Right) delete Right;
	}

	LytWString ToString(LytWString Prefix , LytWString(*Conv)(_Type&))
	{
		if(this)
		{
			LytWString Result=Prefix+Conv(Data);
			if(Left||Right)
			{
				Result=Result+L"{\r\n";
				if(Left)
                    Result=Result+Left->ToString(Prefix+L"  ",Conv);    //之前是Tos
				if(Right)
                    Result=Result+Right->ToString(Prefix+L"  ",Conv);   //之前是Tos
				return Result+Prefix+L"}\r\n";
			}
			else return Result+L"\r\n";
		}
		else return L"";
	}
};

template <typename _Type>
class Tree
{
public:
	TreeNode<_Type>* TreeHead;

	Tree()
	{
		TreeHead=new TreeNode<_Type>;
	}

	~Tree()
	{
		if (TreeHead) delete TreeHead;
		TreeHead=0;
	}

	TreeNode<_Type>* AddUp()
	{
		TreeNode<_Type>* Temp=new TreeNode<_Type>;
		if (TreeHead!=0)
		{
			TreeHead->Up=Temp;
			Temp->Up=0;
			Temp->Left=TreeHead;
			Temp->Right=0;
			Temp->Left->Up=Temp;
		}
		else
		{
			Temp->Up=0;
			Temp->Left=0;
			Temp->Right=0;
		}
		TreeHead=Temp;
		return Temp;
	}

	TreeNode<_Type>* AddLeft(TreeNode<_Type>* X)
	{
		if (X!=0 && X->Left==0)
		{
			TreeNode<_Type>* Temp=new TreeNode<_Type>;
			Temp->Up=X;
			X->Left=Temp;
			Temp->Left=0;
			Temp->Right=0;
			Temp->Up->Left=Temp;
			return Temp;
		}
		else return 0;
	}

	TreeNode<_Type>* AddRight(TreeNode<_Type>* X)
	{
		if (X!=0 && X->Right==0)
		{
			TreeNode<_Type>* Temp=new TreeNode<_Type>;
			Temp->Up=X;
			X->Right=Temp;
			Temp->Left=0;
			Temp->Right=0;
			Temp->Up->Right=Temp;
			return Temp;
		}
		else return 0;
	}

	TreeNode<_Type>* MoveLeft(TreeNode<_Type>* X)
	{
		if (X!=0)
		{
			TreeNode<_Type>* Temp=new TreeNode<_Type>;
			if (X==TreeHead)
				TreeHead=Temp;
			if (X->Up!=0)
			{
				Temp->Up=X->Up;
				Temp->Up->Right=Temp;
				Temp->Left=X;
				Temp->Right=0;
				X->Up=Temp;
			}
			else
			{
				Temp->Up=0;
				Temp->Left=X;
				Temp->Right=0;
				X->Up=Temp;
			}
			return Temp;
		}
		else return 0;
	}

	TreeNode<_Type>* MoveRight(TreeNode<_Type>* X)
	{
		if (X!=0)
		{
			TreeNode<_Type>* Temp=new TreeNode<_Type>;
			if (X==TreeHead)
				TreeHead=Temp;
			if (X->Up!=0)
			{
				Temp->Up=X->Up;
				Temp->Up->Left=Temp;
				Temp->Left=X;
				Temp->Right=0;
				X->Up=Temp;
			}
			else
			{
				Temp->Up=0;
				Temp->Left=0;
				Temp->Right=X;
				X->Up=Temp;
			}
			return Temp;
		}
		else return 0;
	}

	TreeNode<_Type>* KeepLeft(TreeNode<_Type>* X)
	{
		if (X->Left)
		{
			TreeNode<_Type>* Keep=X->Left;
			if (X->Right)
				delete X->Right;
			X->Right=0;
			if (X==TreeHead)
			{
				TreeHead=Keep;
				Keep->Up=0;
			}
			else
			{
				X->Left->Up=X->Up;
				if (X->Up->Left==X)
					X->Up->Left=X->Left;
				else X->Up->Right=X->Left;
			}
			delete X;
			X=0;
			return Keep;
		}
		return 0;
	}

	TreeNode<_Type>* KeepRight(TreeNode<_Type>* X)
	{
		if (X->Right)
		{
			TreeNode<_Type>* Keep=X->Right;
			if (X->Left)
				delete X->Left;

			if (X==TreeHead)
			{
				TreeHead=Keep;
				Keep->Up=0;
			}
			else
			{
				X->Right->Up=X->Up;
				if (X->Up->Left==X)
					X->Up->Left=X->Right;
				else X->Up->Right=X->Right;
			}
			delete X;
			X=0;
			return Keep;
		}
		return 0;
	}
};

#endif
