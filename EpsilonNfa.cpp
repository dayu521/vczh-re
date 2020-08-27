#include "EpsilonNfa.h"

EpsilonNfa::EpsilonNfa()
{
	Start.New();
	End.New();
}

EpsilonNfa::EpsilonNfa(const EpsilonNfa& Object)
{
	Start=Object.Start;
	End=Object.End;
}

void EpsilonNfa::Reverse(TreeNode<CharType>* Temp_TreeNode, CharClassLink& CharLink, Set<int>& CharNumber)
{
	if (Temp_TreeNode)
	{
		if (Temp_TreeNode->Data.Type==Operator)
		{
			if (Temp_TreeNode->Data.Data==L'|')
			{
				Reverse(Temp_TreeNode->Left, CharLink, CharNumber);
				Reverse(Temp_TreeNode->Right, CharLink, CharNumber);
			}
			else if (Temp_TreeNode->Data.Data==L'-')
			{
				CharClass Temp_CharClass;
				Temp_CharClass.Start=Temp_TreeNode->Left->Data.Data;
				Temp_CharClass.End=Temp_TreeNode->Right->Data.Data;
				Temp_CharClass.Type=CharGroup;
				Temp_CharClass.IsCharGroup();
				CharLink.GetCharNumber(Temp_CharClass, CharNumber);
			}
			else if (Temp_TreeNode->Data.Data==L'\\')
			{
				Reverse(Temp_TreeNode->Left, CharLink, CharNumber);
			}
		}
		else if (Temp_TreeNode->Data.Type==Transferred || Temp_TreeNode->Data.Type==Char)
		{
			CharClass Temp_CharClass;
			Temp_CharClass.Start=Temp_TreeNode->Data.Data;
			Temp_CharClass.End=Temp_CharClass.Start;
			Temp_CharClass.Type=Temp_TreeNode->Data.Type;
			CharLink.GetCharNumber(Temp_CharClass, CharNumber);
		}
	}
}

EpsilonNfa EpsilonNfa::GetEpsilonNfa(TreeNode<CharType>* Temp_TreeNode, CharClassLink& CharLink)
{
	if (Temp_TreeNode)
	{
		if (Temp_TreeNode->Data.Type==Operator)
		{
			if (Temp_TreeNode->Data.Data==L'\0')
			{
				EpsilonNfa Result,Left,Right;
				Left=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				Right=GetEpsilonNfa(Temp_TreeNode->Right, CharLink);
				NfaEdge First,Second,Third;
				First.Connect(Result.Start, Left.Start);
				First.Data->Data.Add(0);
				Second.Connect(Left.End, Right.Start);
				Second.Data->Data.Add(0);
				Third.Connect(Right.End, Result.End);
				Third.Data->Data.Add(0);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'|')
			{
				EpsilonNfa Result,Left,Right;
				Left=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				Right=GetEpsilonNfa(Temp_TreeNode->Right, CharLink);
				NfaEdge First,Second,Third,Fourth;
				First.Connect(Result.Start, Left.Start);
				First.Data->Data.Add(0);
				Second.Connect(Result.Start, Right.Start);
				Second.Data->Data.Add(0);
				Third.Connect(Left.End, Result.End);
				Third.Data->Data.Add(0);
				Fourth.Connect(Right.End, Result.End);
				Fourth.Data->Data.Add(0);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'+')
			{
				EpsilonNfa Result,Left,Right;
				Left=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				Right=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				NfaEdge First,Second,Third,Fourth;
				First.Connect(Result.Start, Left.Start);
				First.Data->Data.Add(0);
				Second.Connect(Left.End, Result.End);
				Second.Data->Data.Add(0);
				Third.Connect(Result.End, Right.Start);
				Third.Data->Data.Add(0);
				Fourth.Connect(Right.End, Result.End);
				Fourth.Data->Data.Add(0);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'*')
			{
				EpsilonNfa Result,Left;
				Left=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				Result.End.Data=Result.Start.Data;
				NfaEdge First,Second;
				First.Connect(Result.Start, Left.Start);
				First.Data->Data.Add(0);
				Second.Connect(Left.End, Result.Start);
				Second.Data->Data.Add(0);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'?')
			{
				EpsilonNfa Result,Left;
				Left=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				NfaEdge First,Second,Third;
				First.Connect(Result.Start, Left.Start);
				First.Data->Data.Add(0);
				Second.Connect(Left.End, Result.End);
				Second.Data->Data.Add(0);
				Second.Connect(Result.Start, Result.End);
				Second.Data->Data.Add(0);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'\\')
			{
				EpsilonNfa Result=GetEpsilonNfa(Temp_TreeNode->Left, CharLink);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'-')
			{
				CharClass Temp_CharClass;
				EpsilonNfa Result;
				NfaEdge TempEdge;
				TempEdge.Connect(Result.Start, Result.End);

				Temp_CharClass.Start=Temp_TreeNode->Left->Data.Data;
				Temp_CharClass.End=Temp_TreeNode->Right->Data.Data;
				Temp_CharClass.Type=Temp_TreeNode->Left->Data.Type;
				CharLink.GetCharNumber(Temp_CharClass, TempEdge.Data->Data.Data);
				return Result;
			}
			else if (Temp_TreeNode->Data.Data==L'^')
			{
				EpsilonNfa Result;
				NfaEdge TempEdge;
				TempEdge.Connect(Result.Start, Result.End);

				Node<CharClass*>* Temp_CharLink=CharLink.CharLink.GetHead();
				while(Temp_CharLink)
				{
					TempEdge.Data->Data.Add(Temp_CharLink->Data->Number);
					Temp_CharLink=Temp_CharLink->Next;
				}

				Set<int> SubCharNumber;
				Reverse(Temp_TreeNode->Left, CharLink, SubCharNumber);
				TempEdge.Data->Data.Data.Sub(SubCharNumber);
				return Result;
			}
		}
		else if (Temp_TreeNode->Data.Type==Transferred || Temp_TreeNode->Data.Type==Char)
		{
			CharClass Temp_CharClass;
			EpsilonNfa Result;
			NfaEdge TempEdge;
			TempEdge.Connect(Result.Start, Result.End);

			Temp_CharClass.Start=Temp_TreeNode->Data.Data;
			Temp_CharClass.End=Temp_CharClass.Start;
			Temp_CharClass.Type=Temp_TreeNode->Data.Type;
			CharLink.GetCharNumber(Temp_CharClass, TempEdge.Data->Data.Data);
			return Result;
		}
	}
}

void EpsilonNfa::RemoveUnnessaryData()
{
	Node<NfaStatusBase*>* TempStatus=NfaStatusBase::AllStatus.GetHead();
	while (TempStatus)
	{
		if ((!TempStatus->Data) || (!TempStatus->Data->InEdges.GetHead() && !TempStatus->Data->OutEdges.GetHead()))
		{
			Node<NfaStatusBase*>* TempStatusNext=TempStatus->Next;
			delete TempStatus->Data;
			TempStatus->Data->AllStatus.Delete(TempStatus);
			TempStatus=TempStatusNext;
		}
		else TempStatus=TempStatus->Next;
	}

	Node<NfaEdgeBase*>* TempEdge=NfaEdgeBase::AllEdge.GetHead();
	while (TempEdge)
	{
		if ((!TempEdge->Data) || (!TempEdge->Data->From || !TempEdge->Data->To))
		{
			Node<NfaEdgeBase*>* TempEdgeNext=TempEdge->Next;
			delete TempEdge->Data;
			TempEdge->Data->AllEdge.Delete(TempEdge);
			TempEdge=TempEdgeNext;
		}
		else TempEdge=TempEdge->Next;
	}
}

void EpsilonNfa::Print()
{
	cout<<"EpsilonNfaStatus is started from "<<Start.Data->Data.StatusNumber<<endl;
	Node<NfaStatusBase*>* TempNfaStatus=Start.Data->AllStatus.GetHead();
	while (TempNfaStatus)
	{
		cout<<"EpsilonNfaStatus "<<TempNfaStatus->Data->Data.StatusNumber;
		if (TempNfaStatus->Data->FinalStatus)
			cout<<"(final)";
		cout<<":";
		Node<NfaEdgeBase*>* TempNfaEdge=TempNfaStatus->Data->OutEdges.GetHead();
		while (TempNfaEdge)
		{
			Node<int>* TempNfaEdgeMatchContent=TempNfaEdge->Data->Data.Data.GetHead();
			if (TempNfaEdgeMatchContent)
			{
				cout<<" [ ";
				while(TempNfaEdgeMatchContent)
				{
					cout<<TempNfaEdgeMatchContent->Data<<" ";
					TempNfaEdgeMatchContent=TempNfaEdgeMatchContent->Next;
				}
				cout<<"] -> ";
			}
			cout<<TempNfaEdge->Data->To->Data.StatusNumber<<" |";
			TempNfaEdge=TempNfaEdge->Next;
		}
		cout<<endl;
		TempNfaStatus=TempNfaStatus->Next;
	}
}
