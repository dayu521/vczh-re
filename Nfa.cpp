#include "Nfa.h"

Nfa::Nfa()
{
}

Nfa::Nfa(EpsilonNfa& EpsilonNfaGraph)
{
	Start.Data=EpsilonNfaGraph.Start.Data;
}

void Nfa::GetAvailableStatus(EpsilonNfa& EpsilonNfaGraph)
{
	AvailableStatus.AddLast()->Data=EpsilonNfaGraph.Start.Data;
	Node<NfaStatusBase*>* TempStatus=NfaStatusBase::AllStatus.GetHead();
	while (TempStatus)
	{
		bool Judge=true;
		Node<NfaEdgeBase*>* TempEdge=TempStatus->Data->InEdges.GetHead();
		while (Judge && TempEdge)
		{
			Node<int>* TempEdgeMatchContent=TempEdge->Data->Data.Data.GetHead();
			while (TempEdgeMatchContent)
			{
				if (TempEdgeMatchContent->Data)
				{
					AvailableStatus.AddLast()->Data=TempStatus->Data;
					Judge=false;
					break;
				}
				TempEdgeMatchContent=TempEdgeMatchContent->Next;
			}
			TempEdge=TempEdge->Next;
		}
		TempStatus=TempStatus->Next;
	}
	
	cout<<"NfaStatus ";
	Node<NfaStatusBase*>* Temp_AvailableStatus=AvailableStatus.GetHead();
	while (Temp_AvailableStatus)
	{
		cout<<Temp_AvailableStatus->Data->Data.StatusNumber<<" ";
		Temp_AvailableStatus=Temp_AvailableStatus->Next;
	}
	cout<<"is AvailableStatus."<<endl<<endl;

}

bool Nfa::IsEpsilonEdge(NfaEdgeBase* Object)
{
	Node<int>* Object_EdgeMatchContent=Object->Data.Data.GetHead();
	while (Object_EdgeMatchContent)
	{
		if (Object_EdgeMatchContent->Data)
			return false;
		Object_EdgeMatchContent=Object_EdgeMatchContent->Next;
	}
	return true;
}

void Nfa::GetEpsilonClosure(NfaStatusBase* Temp_AvailableStatus, Set<NfaStatusBase*>& EpsilonClosure)
{
	if (Temp_AvailableStatus)
	{
		EpsilonClosure.Add(Temp_AvailableStatus);
		while (1)
		{
			Node<NfaStatusBase*>* TempNode_EpsilonClosure=EpsilonClosure.GetHead();
			bool Judge;
			while (TempNode_EpsilonClosure)
			{
				Node<NfaEdgeBase*>* TempEdge=TempNode_EpsilonClosure->Data->OutEdges.GetHead();
				Judge=false;
				while (TempEdge)
				{
					if (IsEpsilonEdge(TempEdge->Data))//不会同时出现Epsilon与非Epsilon的匹配内容
					{
						if (EpsilonClosure.Add(TempEdge->Data->To))
						{
							Judge=true;
							if (TempEdge->Data->To->FinalStatus)
								Temp_AvailableStatus->FinalStatus=true;
						}
					}
					TempEdge=TempEdge->Next;
				}
				TempNode_EpsilonClosure=TempNode_EpsilonClosure->Next;
			}
			if (!Judge) break;
		}
	}
}

void Nfa::InsertEdge()
{
	Node<NfaStatusBase*>* TempNode_AvailableStatus=AvailableStatus.GetHead();
	while (TempNode_AvailableStatus)
	{
		Set<NfaStatusBase*> EpsilonClosure;
		GetEpsilonClosure(TempNode_AvailableStatus->Data, EpsilonClosure);
		Node<NfaStatusBase*>* TempNode_EpsilonClosure=EpsilonClosure.GetHead();
		while (TempNode_EpsilonClosure)
		{
			if (TempNode_EpsilonClosure->Data==TempNode_AvailableStatus->Data)
			{
				TempNode_EpsilonClosure=TempNode_EpsilonClosure->Next;
				continue;
			}
			else
			{
				Node<NfaEdgeBase*>* TempEdge=TempNode_EpsilonClosure->Data->OutEdges.GetHead();
				while (TempEdge)
				{
					if (IsEpsilonEdge(TempEdge->Data)==0)
					{
						NfaEdge NewNfaEdge;
						NewNfaEdge.Connect(TempNode_AvailableStatus->Data, TempEdge->Data->To);
						NewNfaEdge.Data->Data.Data.Copy(TempEdge->Data->Data.Data);
					}
					TempEdge=TempEdge->Next;
				}
			}
			TempNode_EpsilonClosure=TempNode_EpsilonClosure->Next;
		}
		TempNode_AvailableStatus=TempNode_AvailableStatus->Next;
	}
}

void Nfa::RemoveUnnessaryData()
{
	Node<NfaEdgeBase*>* TempEdge=NfaEdgeBase::AllEdge.GetHead();
	while (TempEdge)
	{
		if (IsEpsilonEdge(TempEdge->Data) || AvailableStatus.Exists(TempEdge->Data->From)==0 || AvailableStatus.Exists(TempEdge->Data->To)==0 )
		{
			NfaEdge DeleteEdge(TempEdge->Data);
			DeleteEdge.CutOff();
		}
		TempEdge=TempEdge->Next;
	}
}

void Nfa::GetNfa(EpsilonNfa& EpsilonNfaGraph)
{
	GetAvailableStatus(EpsilonNfaGraph);
	InsertEdge();
	RemoveUnnessaryData();
}

void Nfa::Print()
{
	cout<<"NfaStatus is started from "<<Start.Data->Data.StatusNumber<<endl;
	Node<NfaStatusBase*>* TempNfaStatus=AvailableStatus.GetHead();
	while (TempNfaStatus)
	{
		cout<<"NfaStatus "<<TempNfaStatus->Data->Data.StatusNumber;
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
