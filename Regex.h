#ifndef REGEX_H
#define REGEX_H

#include <iostream>
#include <string.h>
#include <math.h>
#include <iomanip> 
#include "LytWString.h"
#include "GrammarTree.h"
#include "EpsilonNfa.h"
#include "Nfa.h"
#include "Dfa.h"
#include "DfaMatrix.h"

using namespace std;

class Regex
{
public:
	class MatchResult
	{
	public:
		const wchar_t* Start; //成功匹配的起始位置，0为失败
		int Length;

		MatchResult()
		{
			Start=0;
			Length=0;
		}

		MatchResult(const MatchResult& Object)
		{
			Start=Object.Start;
			Length=Object.Length;
		}

		~MatchResult()
		{
		}
	};

	LytWString ErrorMessage;
	DfaMatrix DfaGraphMatrix;
	CharClassLink CharLink;

	Regex(LytWString Expression)
	{
		GrammarTree ExpressionGrammarTree(Expression.Buffer());
		ExpressionGrammarTree.GetGrammarTree();
		ErrorMessage=ExpressionGrammarTree.GetErrorMessage();

		if (ExpressionGrammarTree.IsError()==0)
		{
			CharLink.SetCharNumber(ExpressionGrammarTree);
			cout<<endl;
			CharLink.Print();
	
			//ε-NFA
			EpsilonNfa EpsilonNfaGraph;
			EpsilonNfaGraph=EpsilonNfaGraph.GetEpsilonNfa(ExpressionGrammarTree.Data.TreeHead, CharLink);
			EpsilonNfaGraph.End.Data->FinalStatus=1;
			EpsilonNfaGraph.RemoveUnnessaryData();
			cout<<endl;
			EpsilonNfaGraph.Print();
	
			//NFA
			Nfa NfaGraph(EpsilonNfaGraph);
			NfaGraph.GetNfa(EpsilonNfaGraph);
			cout<<endl;
			NfaGraph.Print();
	
			//DFA
			Dfa DfaGraph(NfaGraph);
			DfaGraph.GetDfa(NfaGraph);
			cout<<endl;
			DfaGraph.Print();

			//DFA矩阵
			DfaGraphMatrix.New(DfaStatusNumber::StaticStatusNumber, CharLink.Size);
			DfaGraphMatrix.GetDfaMatrix();
			cout<<endl;
			DfaGraphMatrix.Print();

			//内存管理：释放图
			NfaStatusBase::ReleaseStatus();
			NfaEdgeBase::ReleaseEdge();
			DfaStatusBase::ReleaseStatus();
			DfaEdgeBase::ReleaseEdge();
		}
	}

	bool IsError()
	{
		if (ErrorMessage==L"")
			return false;
		else return true;
	}

	void SetErrorMessage(const LytWString ErrorInformation)
	{
		ErrorMessage=ErrorInformation;
	}

	LytWString GetErrorMessage()
	{
		return ErrorMessage;
	}

	int RunDfa(const wchar_t* Input)
	{
		int LastFinalLength=-1;
		if (IsError()==0)
		{
			int Status=1;
			const wchar_t* Initializtion=Input;
			while (Status!=0)
			{
				if (DfaGraphMatrix.Final[Status])
					LastFinalLength=Input-Initializtion;

				if (Input[0])
				{
					CharType Temp;
					if (Input[0]==L'\\' && Input[1]==L'\\')
					{
						Temp.Type=Transferred;
						Input=Input+2;
					}
					else Temp.Type=Char;
					Temp.Data=*Input++;
	
					int CharNumber=CharLink.GetCharNumber(Temp);
					if (CharNumber<=0)
						break;
					Status=DfaGraphMatrix.Data(Status, CharNumber);
				}
				else break;
			}
			return LastFinalLength;
		}
		else return LastFinalLength;
	}

	MatchResult Match(const wchar_t* Input)
	{
		MatchResult Result;
		if (IsError()==0)
		{
			const wchar_t* InputBuffer=Input;
			while (*InputBuffer)
			{
				int TempLength=RunDfa(InputBuffer);
				if (TempLength!=-1)
				{
					Result.Start=InputBuffer;
					Result.Length=TempLength;
					return Result;
				}
				++InputBuffer;
			}
			SetErrorMessage(L"匹配失败");
		}
		else return Result;
	}
};

#endif
