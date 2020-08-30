#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "LytWString.h"
#include "Link.h"
#include "OrderedLink.h"
#include "Tree.h"
#include "Set.h"
#include "GrammarTree.h"
#include "CharClass.h"
#include "StatusEdge.h"
#include "EpsilonNfa.h"
#include "Nfa.h"
#include "Dfa.h"
#include "Matrix.h"
#include "DfaMatrix.h"
#include "Regex.h"

using namespace std;

int main(int argc , char* argv[])
{
//	setlocale(LC_ALL,"chs");
    setlocale(LC_ALL,"zh_CN.UTF-8");

	while(true)
	{
		LytWString Expression;
        wcout<<L"请输入正则表达式(exit结束) : ";
		wcin>>Expression;
		if(Expression==L"exit")
		{
			break;
		}
		Regex ExpressionRegex(Expression);
		if(ExpressionRegex.IsError())
		{
			wcout<<ExpressionRegex.GetErrorMessage()<<endl;
		}
		else while(true)
		{
			LytWString Content;
			wcout<<L"请输入所要匹配的字符串(exit结束) : ";
			wcin>>Content;
			if(Content==L"exit")
			{
				break;
			}
			Regex::MatchResult Result=ExpressionRegex.Match(Content.Buffer());
			if (ExpressionRegex.IsError()==0)
			{
				int len1=wcslen(Content.Buffer())-wcslen(Result.Start);
				int len2=Result.Length;
				int len3=wcslen(Content.Buffer())-len1-len2;
				wcout<<Content.Sub(0,len1)<<L"【"<<Content.Sub(len1,len2)<<L"】"<<Content.Sub(len1+len2,len3)<<endl;
			}
			else
			{
				wcout<<ExpressionRegex.GetErrorMessage()<<endl;
			}
		}
		wcout<<endl;
	}
    return 0;
}
