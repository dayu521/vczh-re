#include "GrammarTree.h"

GrammarTree::GrammarTree()
{
	Expression=0;
}

GrammarTree::GrammarTree(const wchar_t* Input)
{
	Expression=Input;
}

void GrammarTree::GetGrammarTree()
{
	GetA(Data.TreeHead);
}

void GrammarTree::SetErrorMessage(const LytWString ErrorInformation)
{
	ErrorMessage=L"正则表达式出错 : "+ErrorInformation;
}

LytWString GrammarTree::GetErrorMessage()
{
	return ErrorMessage;
}

bool GrammarTree::IsError()
{
	if (ErrorMessage==L"")
		return false;
	else return true;
}

// A = A '|' B
// A = B

TreeNode<CharType>* GrammarTree::GetA(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else
	{
		TreeNode<CharType>* Base=Current;
		if (Expression!=0 && (Expression[0]==L'|' || Expression[0]==L'+' || Expression[0]==L'*' || Expression[0]==L'?' || Expression[0]==L')' || Expression[0]==L']'))
		{
			SetErrorMessage(L"表达式中 "+(LytWString)Expression[0]+L" 出现在错误的位置");
			return 0;
		}
		Current=GetB(Current);
		if (IsError())
			return 0;
		else
		{
			Base=Current;
			while (Expression!=0 && Expression[0]==L'|')
			{
				++Expression;
				if (Expression==0 || Expression[0]==L'\0')
				{
					SetErrorMessage(L"表达式非法结束，| 操作符缺少右操作数");
					return 0;
				}
				else if (Expression[0]==L'|' || Expression[0]==L'?' || Expression[0]==L'*' || Expression[0]==L'+' || Expression[0]==L')' || Expression[0]==L']')
				{
					SetErrorMessage(L"表达式非法结束，| 操作符非法连接右操作数 "+LytWString(Expression[0]));
					return 0;
				}
				--Expression;
				Current=Data.MoveLeft(Base);
				Base=Current;
				Current->Data.Data=Expression[0];
				Current->Data.Type=Operator;
				++Expression;
				Current=Data.AddRight(Current);
				Current=GetB(Current);
			}
			return Base;
		}
	}
}


// B = BC
// B = C

TreeNode<CharType>* GrammarTree::GetB(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else
	{
		TreeNode<CharType>* Base=Current;
		Current=GetC(Current);
		if (IsError())
			return 0;
		else
		{
			Base=Current;
			if (Expression!=0 && (Expression[0]==L'-' || Expression[0]==L'^'))
			{
				SetErrorMessage(L"表达式非法连接 "+LytWString(Expression[0]));
				return 0;
			}
			while (Expression!=0 && Expression[0]!=L'\0' && Expression[0]!=L'|' && Expression[0]!=L'+' && Expression[0]!=L'*' && Expression[0]!=L'?' && Expression[0]!=L'|' && Expression[0]!=L')' && Expression[0]!=']')
			{
				Current=Data.MoveLeft(Base);
				Base=Current;
				Current->Data.Data=L'\0';
				Current->Data.Type=Operator;
				Current=Data.AddRight(Current);
				Current=GetC(Current);
			}
			return Base;
		}
	}
}


// C = C '+'
// C = C '*'
// C = C '?'
// C = '(' A ')'
// C = D

TreeNode<CharType>* GrammarTree::GetC(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else 
	{
		TreeNode<CharType>* Base=Current;
		if (Expression!=0 && Expression[0]==L'(')
		{
			++Expression;
			if (Expression==0 || Expression[0]==L'\0')
			{
				SetErrorMessage(L"表达式非法结束，( 操作符缺少右操作数");
				return 0;
			}
			Current=GetA(Current);
			if (IsError())
				return 0;
			else
			{
				Base=Current;
				if (Expression==0 || Expression[0]!=L')')
				{
					SetErrorMessage(L"表达式非法结束，缺少 )");
					return 0;
				}
				++Expression;
			}
		}
		else
		{
			Current=GetD(Current);
			if (IsError())
				return 0;
			else Base=Current;
		}
		while (Expression!=0 && (Expression[0]==L'+' || Expression[0]==L'*' || Expression[0]==L'?'))
		{
			Current=Data.MoveLeft(Base);
			Base=Current;
			Current->Data.Data=Expression[0];
			Current->Data.Type=Operator;
			++Expression;
		}
		return Base;
	}
}


// D = char
// D = '\' char
// D = '[' E ']'
// D = "[^" E ']'

TreeNode<CharType>* GrammarTree::GetD(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else
	{
		TreeNode<CharType>* Base=Current;
		if (Expression!=0 && Expression[0]==L'\\')
		{
			++Expression;
			if (Expression==0 || Expression[0]==L'\0')
			{
				SetErrorMessage(L"表达式非法结束，\\ 操作符缺少右操作数");
				return 0;
			}
			if (Expression[0]==L'\\' || Expression[0]==L'|' || Expression[0]==L'+' || Expression[0]==L'*' || Expression[0]==L'?' || Expression[0]==L'(' || Expression[0]==L')' || Expression[0]==L'[' || Expression[0]==L']' || Expression[0]==L'^' || Expression[0]==L'-')
			{
				Current->Data.Data=Expression[0];
				Current->Data.Type=Char;
				++Expression;
			}
			else if (Expression[0]==L'r' || Expression[0]==L'n' || Expression[0]==L't')
			{
				Current->Data.Data=Expression[0];
				Current->Data.Type=Transferred;
				++Expression;
			}
			else if (Expression[0]==L'w' || Expression[0]==L'd' || Expression[0]==L'l' || Expression[0]==L'W' || Expression[0]==L'D' || Expression[0]==L'L')
			{
				if (Expression[0]==L'w')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;
	
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'_';
					Current->Right->Data.Type=Char;
	
					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'0';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'9';
					Current->Right->Right->Data.Type=CharGroup;
	
					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'A';
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'Z';
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'a';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'z';
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'd')
				{
					Current->Data.Data=L'-';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'0';
					Current->Left->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'9';
					Current->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'l')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'A';
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'Z';
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'a';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'z';
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'W')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=(wchar_t)0;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'0'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'9'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'A'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'Z'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'_'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'z'+1;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=(wchar_t)65535;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'_'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'a'-1;
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'D')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=(wchar_t)0;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'0'-1;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'9'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=(wchar_t)65535;
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'L')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=(wchar_t)0;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'A'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'Z'+1;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'a'-1;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'z'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=(wchar_t)65535;
					Current->Right->Right->Data.Type=CharGroup;
				}
				++Expression;
			}
			else
			{
				SetErrorMessage(L"表达式非法结束，\\ 操作符非法连接右操作数");
				return 0;
			}
		}
		else if (Expression!=0 && Expression[0]==L'[')
		{
			++Expression;
			if (Expression==0 || Expression[0]==L'\0')
			{
				SetErrorMessage(L"表达式非法结束，[ 操作符缺少右操作数");
				return 0;
			}
			if (Expression!=0 && Expression[0]==L'^')
			{
				Current->Data.Data=Expression[0];
				Current->Data.Type=Operator;
				Current=Data.AddLeft(Current);
				++Expression;
				Current=GetE(Current);
				if (IsError())
					return 0;
				else
				{
					Base->Left=Current;
					Base->Right=0;
				}
			}
			else
			{
				Current=GetE(Current);
				if (IsError())
					return 0;
				else Base=Current;
			}
			if (Expression==0 || Expression[0]!=L']')
			{
				SetErrorMessage(L"表达式非法结束，缺少 ]");
				return 0;
			}
			++Expression;
		}
		else
		{
			Current->Data.Data=Expression[0];
			Current->Data.Type=Char;
			++Expression;
		}
		return Base;
	}
}


// E = EF
// E = F

TreeNode<CharType>* GrammarTree::GetE(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else
	{
		TreeNode<CharType>* Base=Current;
		Current=GetF(Current);
		if (IsError())
			return 0;
		else
		{
			Base=Current;
			if (Expression[0]==L'|' || Expression[0]==L'+' || Expression[0]==L'*' || Expression[0]==L'?')
			{
				SetErrorMessage(L"表达式[]中不可出现 "+LytWString(Expression[0]));
				return 0;
			}
			while (Expression!=0 && Expression[0]!=L'\0' && Expression[0]!=L')' && Expression[0]!=L']')
			{
				Current=Data.MoveLeft(Base);
				Base=Current;
				Current->Data.Data=L'|';
				Current->Data.Type=Operator;
				Current=Data.AddRight(Current);
				Current=GetF(Current);
			}
		}
		return Base;
	}
}


// F = char '-' char
// F = char
// F = '\' char

TreeNode<CharType>* GrammarTree::GetF(TreeNode<CharType>* Current)
{
	if (IsError())
		return 0;
	else
	{
		TreeNode<CharType>* Base=Current;
		if (Expression[0]==L'\\')
		{
			++Expression;
			if (Expression==0 || Expression[0]==L'\0')
			{
				SetErrorMessage(L"表达式非法结束，\\ 操作符缺少右操作数");
				return 0;
			}
			if (Expression[0]==L'\\' || Expression[0]==L'+' || Expression[0]==L'*' || Expression[0]==L'?' || Expression[0]==L'(' || Expression[0]==L')' || Expression[0]==L'[' || Expression[0]==L']' || Expression[0]==L'^' || Expression[0]==L'-')
			{
				Current->Data.Data=Expression[0];
				Current->Data.Type=Char;
				++Expression;
			}
			else if (Expression[0]==L'r' || Expression[0]==L'n' || Expression[0]==L't')
			{
				Current->Data.Data=Expression[0];
				Current->Data.Type=Transferred;
				++Expression;
			}
			else if (Expression[0]==L'w' || Expression[0]==L'd' || Expression[0]==L'l' || Expression[0]==L'W' || Expression[0]==L'D' || Expression[0]==L'L')
			{
				if (Expression[0]==L'w')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;
	
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'_';
					Current->Right->Data.Type=Char;
	
					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'0';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'9';
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'A';
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'Z';
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'a';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'z';
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'd')
				{
					Current->Data.Data=L'-';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'0';
					Current->Left->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'9';
					Current->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'l')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'A';
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'Z';
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'a';
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'z';
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'W')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=(wchar_t)0;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'0'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'9'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'A'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'Z'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'_'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'z'+1;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=(wchar_t)65535;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'_'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'a'-1;
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'D')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=(wchar_t)0;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'0'-1;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'9'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=(wchar_t)65535;
					Current->Right->Right->Data.Type=CharGroup;
				}
				else if (Expression[0]==L'L')
				{
					Current->Data.Data=L'|';
					Current->Data.Type=Operator;

					Data.AddLeft(Current);
					Current->Left->Data.Data=L'|';
					Current->Left->Data.Type=Operator;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=(wchar_t)0;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=L'A'-1;
					Current->Right->Right->Data.Type=CharGroup;

					Current=Current->Left;
					Data.AddLeft(Current);
					Current->Left->Data.Data=L'-';
					Current->Left->Data.Type=Operator;
					Data.AddLeft(Current->Left);
					Current->Left->Left->Data.Data=L'Z'+1;
					Current->Left->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Left);
					Current->Left->Right->Data.Data=L'a'-1;
					Current->Left->Right->Data.Type=CharGroup;
					Data.AddRight(Current);
					Current->Right->Data.Data=L'-';
					Current->Right->Data.Type=Operator;
					Data.AddLeft(Current->Right);
					Current->Right->Left->Data.Data=L'z'+1;
					Current->Right->Left->Data.Type=CharGroup;
					Data.AddRight(Current->Right);
					Current->Right->Right->Data.Data=(wchar_t)65535;
					Current->Right->Right->Data.Type=CharGroup;
				}
				++Expression;
			}
			else
			{
				SetErrorMessage(L"表达式非法结束，\\ 操作符非法连接右操作数");
				return 0;
			}
		}
		else
		{
			Current->Data.Data=Expression[0];
			Current->Data.Type=Char;
			++Expression;
			if (Expression!=0 && Expression[0]==L'-')///////////////////////////////////////
			{
				++Expression;
				if (Expression==0 || Expression[0]==L'\0')
				{
					SetErrorMessage(L"表达式非法结束，- 操作符缺少右操作数");
					return 0;
				}
				Current->Data.Type=CharGroup;
				--Expression;
				Current=Data.MoveLeft(Current);
				Base=Current;
				Current->Data.Data=Expression[0];
				Current->Data.Type=Operator;
				++Expression;
				if (Expression[0]<=Current->Left->Data.Data)
				{
					SetErrorMessage(L"表达式非法结束，- 中左操作数不小于右操作数");
					return 0;
				}
				Current=Data.AddRight(Current);
				Current->Data.Data=Expression[0];
				Current->Data.Type=CharGroup;
				++Expression;
			}
		}
		return Base;
	}
}
